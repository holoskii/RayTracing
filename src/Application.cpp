#include "Application.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#include "Timer.h"
#include <iostream>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


Application::Application()
    : mConfig()
    , mScene(mConfig)
    , mWLM(mConfig, mScene) {}

void Application::start() {
    try {
        startUnsafe();
    }
    catch (const std::exception& exp) {
        std::cout << "Application exception caught: " << exp.what() << '\n';
    }
}

void Application::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mWindowGLFW);
    glfwTerminate();
}

void Application::benchmark() {
    mWLM.benchmarkRender();
}

void Application::startUnsafe() {
    setup();

    while(isRunning()) {
        setupGUI();
        handleCore();
        showBuffer();
        renderGUI();
    }

    shutdown();
}

bool Application::isRunning() {
    return !glfwWindowShouldClose(mWindowGLFW);
}

void Application::setup() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        throw std::runtime_error("!glfwInit()");
    }

    // GL 3.0 + GLSL 130
    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    mWindowGLFW = glfwCreateWindow(mConfig.windowWidth, mConfig.windowHeight,
                                   "ThesisWindow", nullptr, nullptr);
    if (mWindowGLFW == nullptr) {
        throw std::runtime_error("glfwCreateWindow == null");
    }
    glfwMakeContextCurrent(mWindowGLFW);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mWindowGLFW, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    ImFont* font = io.Fonts->AddFontFromFileTTF(mConfig.fontPath, mConfig.fontSize);
    font->Scale = mConfig.fontScale;
    io.Fonts->AddFontDefault(font->ConfigData);

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    glGenTextures(1, &mOpenGLTexture);
    glBindTexture(GL_TEXTURE_2D, mOpenGLTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mConfig.renderWidth, mConfig.renderHeight,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, mWLM.getImageBuffer());

    mNewRenderMode = mConfig.renderMode;
};

void Application::setupGUI() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    {
        /// Frame buffer is displayed here
        ImGui::Begin("Image");
        {
            ImGui::BeginChild("GameRender");
            ImVec2 windowSize = ImGui::GetWindowSize();
            float windowAR = windowSize.x / windowSize.y;
            float targetAR = (float)mConfig.renderWidth / (float)mConfig.renderHeight;
            ImVec2 size = (targetAR > windowAR) ?
                          ImVec2{ windowSize.x, windowSize.x / targetAR } :
                          ImVec2{ windowSize.y * targetAR, windowSize.y };
            ImGui::Image(reinterpret_cast<ImTextureID>(mOpenGLTexture), size, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();
    }

    bool valuesChanged = false;

    /// Controls happen from this panel
    /// Draw all the GUI, restart render if any value changed
    {
        ImGui::Begin("Configuration");

        if (ImGui::TreeNode("Configuration")) {
            uint64_t selectedInt = (uint64_t)mConfig.renderMode;
            for (uint64_t i = 1; i < getRenderModes().size(); i++) {
                if (ImGui::Selectable(getRenderModes()[i].data(), selectedInt == i)) {
                    if (selectedInt != i) {
                        restartRender();
                        mNewRenderMode = (RenderMode) i;
                    }
                }
            }
            ImGui::TreePop();
            ImGui::Spacing();
        }

        if (ImGui::TreeNode("Entities")) {
            uint32_t i = 1;
            std::stringstream ss;
            ImGui::Text("Objects");
            for (auto &obj: mScene.mObjects) {
                if (ImGui::TreeNode( obj->mName.c_str())) {
                    ss.str("");
                    ss << obj->mName << " x";
                    valuesChanged |= ImGui::DragScalar(ss.str().c_str(), ImGuiDataType_Float, &obj->mPos.x,
                                             0.005f, &objPosMin, &objPosMin, "%f");
                    ss.str("");
                    ss << obj->mName << " y";
                    valuesChanged |= ImGui::DragScalar(ss.str().c_str(), ImGuiDataType_Float, &obj->mPos.y,
                                             0.005f, &objPosMin, &objPosMin, "%f");
                    ss.str("");
                    ss << obj->mName << " z";
                    valuesChanged |= ImGui::DragScalar(ss.str().c_str(), ImGuiDataType_Float, &obj->mPos.z,
                                             0.005f, &objPosMin, &objPosMin, "%f");
                    i++;
                    ss.str("");
                    ImGui::TreePop();
                }
            }

            ImGui::Text("Light sources");
            for (auto &obj: mScene.mLights) {
                if (ImGui::TreeNode( obj->mName.c_str())) {
                    ss.str("");
                    ss << obj->mName << " x";
                    valuesChanged |= ImGui::DragScalar(ss.str().c_str(), ImGuiDataType_Float, &obj->mPos.x,
                                                       0.005f, &objPosMin, &objPosMin, "%f");
                    ss.str("");
                    ss << obj->mName << " y";
                    valuesChanged |= ImGui::DragScalar(ss.str().c_str(), ImGuiDataType_Float, &obj->mPos.y,
                                                       0.005f, &objPosMin, &objPosMin, "%f");
                    ss.str("");
                    ss << obj->mName << " z";
                    valuesChanged |= ImGui::DragScalar(ss.str().c_str(), ImGuiDataType_Float, &obj->mPos.z,
                                                       0.005f, &objPosMin, &objPosMin, "%f");
                    i++;
                    ss.str("");
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
            ImGui::Spacing();
        }

        if (ImGui::TreeNode("Camera")) {
            vec3 &camPos = mScene.mCamera.mPos;
            {
                valuesChanged |= ImGui::DragScalar(" Cam x", ImGuiDataType_Float, &camPos.x, 0.005f, &objPosMin, &objPosMin, "%f");
                valuesChanged |= ImGui::DragScalar(" Cam y", ImGuiDataType_Float, &camPos.y, 0.005f, &objPosMin, &objPosMin, "%f");
                valuesChanged |= ImGui::DragScalar(" Cam z", ImGuiDataType_Float, &camPos.z, 0.005f, &objPosMin, &objPosMin, "%f");
                valuesChanged |= ImGui::DragScalar(" FOV", ImGuiDataType_Float, &mScene.mCamera.mFov, 0.005f, &fovMinRad, &fovMaxRad,"%f");
                ImGui::Spacing();
            }
            ImGui::TreePop();
            ImGui::Spacing();
        }


        if (ImGui::TreeNode("Debug")) {
            ImGui::InputInt("x", &mConfig.debugX);
            ImGui::InputInt("y", &mConfig.debugY);
            ImGui::Checkbox("show", &mConfig.showDebugPixel);
            ImGui::Checkbox("debug", &mConfig.debugPixel);
            ImGui::TreePop();
            ImGui::Spacing();
        }

        ImGui::End();
    }

    if(valuesChanged) {
        restartRender();
    }
}

void Application::handleCore() {
    if(!mWLM.isRenderRunning()) {
        mWLM.startRender();
    }

    if(mRestartRender) {
        mConfig.renderMode = mNewRenderMode;
        mRestartRender = false;
        mWLM.restartRender();
    }
}

void Application::showBuffer() {
    /// Cap FPS with frame time
    /// Don't put new buffer right after restart, it'll be mostly unrendered
    if(timePassed(mBufferUpdateTimePoint, mConfig.frameUpdateTime) &&
            timePassed(mRenderRestartTimePoint, 10) ) {
        /// Sometimes we need to debug one pixel, this routine helps to do that
        Pixel temp;
        Pixel* buf = mWLM.getImageBuffer();
        if(mConfig.showDebugPixel) {
            mConfig.debugY = mConfig.debugY < mConfig.renderHeight ? mConfig.debugY : mConfig.renderHeight - 1;
            mConfig.debugX = mConfig.debugX < mConfig.renderWidth ? mConfig.debugX : mConfig.renderWidth - 1;
            temp = buf[mConfig.debugY * mConfig.renderWidth + mConfig.debugX];
            buf[mConfig.debugY * mConfig.renderWidth + mConfig.debugX] = {255, 0, 0};

        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int32_t) mConfig.renderWidth, (int32_t) mConfig.renderHeight,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, mWLM.getImageBuffer());

        if(mConfig.showDebugPixel) {
            buf[mConfig.debugY * mConfig.renderWidth + mConfig.debugX] = temp;
        }
    }
}

void Application::renderGUI() {
    ImGui::Render();
    int32_t displayWidth, displayHeight;
    glfwGetFramebufferSize(mWindowGLFW, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);

    vec4& bgCol = mConfig.appBGColor;
    glClearColor(bgCol.x, bgCol.y, bgCol.z, bgCol.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(mWindowGLFW);
}

void Application::restartRender() {
    mRestartRender = true;
    mRenderRestartTimePoint = std::chrono::high_resolution_clock::now();
}

bool Application::timePassed(TimePoint& timePoint, uint64_t duration) {
    TimePoint mEnd = std::chrono::high_resolution_clock::now();
    int64_t msFromLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(mEnd - timePoint).count();
    if(msFromLastUpdate > duration) {
        timePoint = mEnd;
        return true;
    }
    return false;
}

void Application::glfwErrorCallback(int error, const char* description) {
    std::stringstream ss;
    ss << "Glfw Error " << error << ": " << description;
    throw std::runtime_error(ss.str());
}