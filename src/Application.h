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
    ~Application() = default;
    void start();
    void benchmark();

private:
    void startUnsafe();

    /// Separates code into logic
    void setup();
    void setupGUI();
    void handleCore();
    void showBuffer();
    void renderGUI();
    void shutdown();

    bool isRunning();
    void restartRender();

    /// Utility functions
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
    bool                        mRestartRender          = false;
    RenderMode                  mNewRenderMode          = RenderMode::Simple;
    TimePoint                   mBufferUpdateTimePoint;
    TimePoint                   mRenderRestartTimePoint;

    /// ImGUI limits (sliders, etc)
    float       objPosMin       = -50;
    float       objPosMax       = 50;
    float       fovMinRad       = degToRad(15);
    float       fovMaxRad       = degToRad(150);
};