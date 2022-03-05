#pragma once

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#include "Common.h"
#include "Config.h"
#include "WLM.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

class Application {
public:
    Application();
    ~Application();
    void start();

private:
    void startUnsafe();
    bool isRunning();
    void setup();
    void setupGUI();
    void handleCore();
    void showBuffer();
    void renderGUI();
    static bool timePassed(TimePoint& timePoint, uint64_t duration);
    static void glfwErrorCallback(int error, const char* description);

private:
    Config                      mConfig;
    WLM                         mWLM;
    GLFWwindow *                mWindowGLFW             = nullptr;
    GLuint                      mOpenGLTexture          = 0;
    TimePoint                   mBufferUpdateTimePoint;
    bool                        mRestartRender          = false;
    RenderMode                  mNewRenderMode          = RenderMode::Fast;
};