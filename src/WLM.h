#pragma once

#include "Config.h"
#include "Common.h"
#include "RenderBasics.h"
#include "RenderCore.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

class RenderCore;

/// WorkLoad Manager
class WLM {
public:
    WLM(Config& config, Scene& scene);
    ~WLM();

    bool isRenderRunning();
    Pixel * getImageBuffer();
    void startRender();
    void restartRender();
    void benchmarkRender();


private:
    enum class Status { Idle, Fill, Render, Shutdown };
    enum class Job { Idle, StartRender, RestartRender, Shutdown };

    void workerEntryPoint(uint64_t threadId);
    void WLMEntryPoint();

    void startNewState(Status newStatus);
    void managerNotifyWorkers();
    void workerNotifyManager();
    void managerWait();
    void setNextJob(Job nextJob);
    void workerWait(uint64_t & threadLoopIndex);

private:
    RenderCore                  mCore;
    Config &                    mConfig;
    Scene&                      mScene;
    std::vector<std::thread>    mThreads;
    std::thread                 mWLMThread;
    Pixel *                     mImageBuffer;
    std::atomic<Status>         mStatus;
    uint64_t                    mLoopIndex      = 0;
    std::atomic<uint64_t>       mTileIndex      = 0;
    std::atomic<uint64_t>       mWorkersReady;
    std::atomic<uint64_t>       mLineIndex;
    std::vector<Tile>           mTiles;
    std::mutex                  mWorkerMutex;
    std::mutex                  mManagerMutex;
    std::condition_variable     mCVManager;
    std::condition_variable     mCVWorker;
    bool                        mRenderRunning;

    std::mutex                  mMutexWLM;
    std::condition_variable     mCVWLM;
    Job                         mNextJob;
};