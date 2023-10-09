#include "RenderingSystem/RenderingSystem.h"
#include "Utils.h"

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

RenderingSystem::RenderingSystem() {
    this->m_isInitialized = false;

    this->cameraEye = {};
    this->cameraTarget = {};
    this->upVector = {};

    this->pWindow = nullptr;
}

RenderingSystem::~RenderingSystem()
{
}

bool RenderingSystem::SetupWindow(const char* windowName) 
{
    if (this->m_isInitialized)
    {
        printf("System already initialized!\n");
        return true;
    }
    // Setup camera
    // ------------------------------------------------
    this->cameraEye = glm::vec3(1);
    this->cameraTarget = glm::vec3(0);
    this->upVector = glm::vec3(0, 1, 0);
    // ------------------------------------------------

    // Setup GLFW
    // ------------------------------------------------
    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

    int windowWidth = 1280;
    int windowHeight = 720;
    this->pWindow = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (!this->pWindow) {
        return false;
    }
    // Lock the window size
    glfwSetWindowSizeLimits(this->pWindow, windowWidth, windowHeight, windowWidth, windowHeight);

    glfwMakeContextCurrent(this->pWindow);
    glfwSwapInterval(1);
    // ------------------------------------------------
    
    // Setup GLAD
    // ------------------------------------------------
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // ------------------------------------------------

    this->m_isInitialized = true;
    printf("RenderingSystem::SetupWindow(): Successful!\n");

    return true;
}

// Destroy all glfw, glad and pointers objects
void RenderingSystem::Destroy() 
{
    if (!this->m_isInitialized)
    {
        return;
    }
    if (this->pWindow) {
        glfwDestroyWindow(this->pWindow);
    }
    glfwTerminate();

    this->m_isInitialized = false;
}

void RenderingSystem::m_UpdateViewport()
{
    if (!this->m_isInitialized)
    {
        return;
    }

    int width, height;

    glfwGetFramebufferSize(this->pWindow, &width, &height);

    glViewport(0, 0, width, height);
}

void RenderingSystem::NewFrame(GLint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    // Clear frame for new drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return;
}

void RenderingSystem::EndFrame()
{
    glfwSwapBuffers(this->pWindow);
    glfwPollEvents();
    return;
}

bool RenderingSystem::WindowShouldClose()
{
    return glfwWindowShouldClose(this->pWindow);
}

// Set window name using glfw and cameras positions
void RenderingSystem::SetWindowName(const char* windowName) 
{
    if (!this->m_isInitialized)
    {
        return;
    }
    glfwSetWindowTitle(this->pWindow, windowName);
}
