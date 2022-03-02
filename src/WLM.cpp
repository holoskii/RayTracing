#include "WLM.h"

#include <cstring>

WLM::WLM(Config& config) :
mConfig(config),
mCore(config),
mWorkersReady(mConfig.threadsCount),
mStatus(Status::Fill),
mLineIndex(0)
{
    mImageBuffer = new pixel[mConfig.renderWidth * mConfig.renderHeight];
    mCore.setBuffer(mImageBuffer);
    for(int i = 0; i < mConfig.threadsCount; i++) {
        mThreads.emplace_back(&WLM::workerEntryPoint, this, i);
    }
    startNewState(Status::Fill);
}

WLM::~WLM() {
    startNewState(Status::Shutdown);
    for(auto& thread : mThreads)
        thread.join();
    delete[] mImageBuffer;
}

bool WLM::isRenderRunning() {
    return mStatus == Status::Render;
}

pixel * WLM::getImageBuffer() {
    return mImageBuffer;
}

void WLM::startRender() {
    startNewState(Status::Render);
}

void WLM::restartRender() {
    // this code should execute in new thread to avoid blocking main thread
    startNewState(Status::Fill);
    startNewState(Status::Render);
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
                std::memset(mImageBuffer + line * mConfig.renderWidth, 0, mConfig.renderWidth * sizeof(pixel));
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