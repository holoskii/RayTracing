#pragma once

#include "Config.h"

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>

class Timer {
public:
#if ENABLE_TIMERS
    explicit Timer(const std::string&& name)
        : mName(name)
        , mStart(std::chrono::high_resolution_clock::now()) {}

        ~Timer() {
        stop();
    }

    void start() {
        mStart = std::chrono::high_resolution_clock::now();
    }

    /// Return time in us
    uint64_t stopSilent() {

        if(!mRunning)
            return 0;

        mRunning = false;
        auto end = std::chrono::high_resolution_clock::now();
        uint64_t timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - mStart).count();
        return timeTaken;
    }

    /// Return time in us and print time in ms
    uint64_t stop() {

        if(!mRunning)
            return 0;

        uint64_t timeTaken = stopSilent();
        std::cout << (mName.length() > 0 ? mName : "") << " \t" << std::fixed << std::setprecision(3)
                  << std::setw(8) << timeTaken * 1e-3f  << " ms" << '\n';
        return timeTaken;
    }

private:
    bool mRunning = true;
    const std::string mName;
    std::chrono::high_resolution_clock::time_point mStart;
#else
    Timer(const std::string& name) {}
    void stop() {}
#endif
};