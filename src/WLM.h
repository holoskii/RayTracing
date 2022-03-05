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
    explicit WLM(Config& config);
    ~WLM();

    bool isRenderRunning();
    pixel * getImageBuffer();
    void startRender();
    void restartRender();

private:
    enum class Status { Idle, Fill, Render, Shutdown };

    void workerEntryPoint(uint64_t threadId);
    void startNewState(Status newStatus);
    void managerNotifyWorkers();
    void workerNotifyManager();
    void managerWait();
    void workerWait(uint64_t & threadLoopIndex);

private:
    RenderCore                  mCore;
    Config &                    mConfig;
    std::vector<std::thread>    mThreads;
    pixel *                     mImageBuffer;
    std::atomic<Status>         mStatus;
    uint64_t                    mLoopIndex      = 0;
    std::atomic<uint64_t>       mWorkersReady;
    std::atomic<uint64_t>       mLineIndex;
    std::atomic<uint64_t>       mTileIndex      = 0;
    std::vector<Tile>           mTiles;
    std::mutex                  mWorkerMutex;
    std::mutex                  mManagerMutex;
    std::condition_variable     mCVManager;
    std::condition_variable     mCVWorker;
    bool                        mRenderRunning;
};