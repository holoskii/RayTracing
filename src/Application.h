#pragma once

#include "Common.h"
#include "Config.h"
#include "Scene.h"
#include "WLM.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

class Application {
public:
    Application();
    ~Application();
    void start();
    void benchmark();

private:
    void startUnsafe();
    void setup();
    void setupGUI();
    void handleCore();
    void showBuffer();
    void renderGUI();
    void shutdown();
    bool isRunning();
    void restartRender();
    static bool timePassed(TimePoint& timePoint, uint64_t duration);
    static void glfwErrorCallback(int error, const char* description);

private:
    /// GUI
    GLFWwindow *                mWindowGLFW             = nullptr;
    GLuint                      mOpenGLTexture          = 0;

    /// Object managing render
    Config                      mConfig;
    Scene                       mScene;
    WLM                         mWLM;

    /// Render
    RenderMode                  mNewRenderMode          = RenderMode::Fast;
    TimePoint                   mBufferUpdateTimePoint;
    TimePoint                   mRenderRestartTimePoint;
    bool                        mRestartRender          = false;
    bool                        mShutdown               = false;
};