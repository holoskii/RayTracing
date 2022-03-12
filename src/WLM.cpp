#include "WLM.h"

#include "Timer.h"
#include <cstring>

WLM::WLM(Config& config, Scene& scene)
    : mConfig(config)
    , mScene(scene)
    , mCore(config, scene)
    , mWorkersReady(mConfig.threadsCount)
    , mStatus(Status::Fill)
    , mLineIndex(0) {
    mImageBuffer = new Pixel[mConfig.renderWidth * mConfig.renderHeight];
    mCore.setBuffer(mImageBuffer);
    for(int i = 0; i < mConfig.threadsCount; i++) {
        mThreads.emplace_back(&WLM::workerEntryPoint, this, i);
    }

    mWLMThread = std::thread(&WLM::WLMEntryPoint, this);
}

void WLM::WLMEntryPoint() {
    startNewState(Status::Fill);
    while(mStatus != Status::Shutdown) {
        {
            std::unique_lock<std::mutex> lk(mMutexWLM);
            mCVWLM.wait(lk);
        }
        switch (mNextJob) {
            case Job::StartRender: {
                startNewState(Status::Render);
                break;
            }
            case Job::RestartRender: {
                mRenderRunning = false;
                startNewState(Status::Fill);
                startNewState(Status::Render);
                break;
            }
            case Job::Shutdown: {
                startNewState(Status::Shutdown);
                break;
            }
            default: break;
        }
    }
}

WLM::~WLM() {
    setNextJob(Job::Shutdown);
    mWLMThread.join();
    for(auto& thread : mThreads)
        thread.join();
    delete[] mImageBuffer;
}

bool WLM::isRenderRunning() {
    return mStatus == Status::Render;
}

Pixel * WLM::getImageBuffer() {
    return mImageBuffer;
}

void WLM::startRender() {
    setNextJob(Job::StartRender);
}

void WLM::restartRender() {
    setNextJob(Job::RestartRender);
}

void WLM::benchmarkRender() {
    std::cout << "Benchmark warming up\n";
    // Warmup for 500ms
    auto start = std::chrono::high_resolution_clock::now();
    while(true) {
        startNewState(Status::Render);
        managerWait();
        startNewState(Status::Fill);
        auto end = std::chrono::high_resolution_clock::now();
        int64_t res = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(res > 500) {
            break;
        }
    }
    std::cout << "Benchmark started\n";
    // Collect data
    uint64_t min = UINT64_MAX, max = 0;
    uint64_t iterations = 0;
    uint64_t timeSumQuadratic = 0;
    start = std::chrono::high_resolution_clock::now();
    while(true) {
        uint64_t res = 0;
        {
            Timer t("Benchmark");
            startNewState(Status::Render);
            managerWait();
            res = t.stopSilent();
        }
        timeSumQuadratic += res * res;
        iterations++;
        min = res < min ? res : min;
        max = res > max ? res : max;
        startNewState(Status::Fill);
        auto end = std::chrono::high_resolution_clock::now();
        int64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(duration > 2500) {
            break;
        }
    }
    float avgTime = sqrtf(1e-6f * timeSumQuadratic / iterations);
    std::cout << "RMS: " << std::fixed << std::setprecision(3) << avgTime << "ms" << std::endl;
    std::cout << "Min: " << std::fixed << std::setprecision(3) << min * 1e-3f << "ms" << std::endl;
    std::cout << "Max: " << std::fixed << std::setprecision(3) << max * 1e-3f << "ms" << std::endl;
}

void WLM::workerEntryPoint(uint64_t threadId) {
    uint64_t threadLoopIndex = 1;
    while(mStatus != Status::Shutdown) {
        workerWait(threadLoopIndex);
        if(mStatus == Status::Fill) {
            while (mStatus == Status::Fill) {
                uint64_t line = mLineIndex++;
                if(line >= mConfig.renderHeight) {
                    break;
                }
                std::memset(mImageBuffer + line * mConfig.renderWidth, 0, mConfig.renderWidth * sizeof(Pixel));
            }
        }
        else if(mStatus == Status::Render) {
            while (mStatus == Status::Render) {
                uint64_t index = mTileIndex++;
                if(index >= mTiles.size()) {
                    break;
                }
                mCore.renderTile(mTiles.at(index), mRenderRunning);
            }
        }
        workerNotifyManager();
    }
}

void WLM::setNextJob(Job nextJob) {
    mNextJob = nextJob;
    {
        std::lock_guard<std::mutex> lk(mMutexWLM);
        mCVWLM.notify_one();
    }
}

void WLM::startNewState(Status newStatus) {
    if(newStatus == mStatus) {
        return;
    }

    managerWait();
    mStatus = newStatus;
    mRenderRunning = mStatus == Status::Render;
    if(newStatus == Status::Fill) {
        mLineIndex.store(0);
    }
    else if(newStatus == Status::Render) {
        mTileIndex.store(0);
        mTiles = Tile::getSpiralTiles(mConfig);
    }

    managerNotifyWorkers();
}

void WLM::managerNotifyWorkers() {
    mWorkersReady = 0;
    {
        std::lock_guard<std::mutex> lk(mManagerMutex);
        mLoopIndex++;
        mCVManager.notify_all();
    }
}

void WLM::workerNotifyManager() {
    {
        std::unique_lock<std::mutex> lk(mWorkerMutex);
        mWorkersReady++;
        mCVWorker.notify_one();
    }
}

void WLM::managerWait() {
    std::unique_lock<std::mutex> lk(mWorkerMutex);
    mCVWorker.wait(lk, [&]{ return mWorkersReady == mConfig.threadsCount; } );
}

void WLM::workerWait(uint64_t & threadmLoopIndex) {
    {
        std::unique_lock<std::mutex> lk(mManagerMutex);
        mCVManager.wait(lk, [&]{ return threadmLoopIndex == this->mLoopIndex; });
    }
    threadmLoopIndex++;
}