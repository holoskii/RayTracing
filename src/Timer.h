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

    void stop() {

        if(!mRunning)
            return;

        mRunning = false;
        auto end = std::chrono::high_resolution_clock::now();
        long time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStart).count();

        std::cout << (mName.length() > 0 ? mName : "") << " \t" << std::fixed << std::setprecision(3) << std::setw(8) << time_taken * 1e-6  << " ms" << '\n';
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