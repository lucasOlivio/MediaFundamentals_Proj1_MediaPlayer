#pragma once

#include "CommonOpengl.h"
#include <glm/vec3.hpp>

// TODO: Dealing with concurrency if the game goes multi-thread
class RenderingSystem {
private:
    bool m_isInitialized;

    // Updates per frame
    void m_UpdateViewport();
public:
    // ctors dtors
    RenderingSystem();
    ~RenderingSystem();

    // Setup camera and glfw
    bool SetupWindow(const char* windowName);
    void Destroy();

    // Scene camera
    glm::vec3 cameraEye;
    glm::vec3 cameraTarget;
    glm::vec3 upVector;

    // GLFW
    GLFWwindow* pWindow;

    // Checks if the window should close using GLFW flags
    bool WindowShouldClose();

    // Set window name using glfw and cameras positions
    void SetWindowName(const char* value);
};