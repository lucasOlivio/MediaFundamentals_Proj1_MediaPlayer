#include "MediaPlayer.h"

MediaPlayer::MediaPlayer()
{
    this->pAudioManager = nullptr;
    this->pRenderingSystem = nullptr;
    this->pShaderManager = nullptr;

    this->programName = "MyMediaPlayer";
}

MediaPlayer::~MediaPlayer()
{
}

bool MediaPlayer::m_SetupImgui()
{
    bool response = true;

    // Initialize all Imgui integration with GLFW and OpenGL
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Dark theme for the window
    ImGui::StyleColorsDark(); 
    response = ImGui_ImplGlfw_InitForOpenGL(this->pRenderingSystem->pWindow, true);
    if (!response)
    {
        printf("Error initializing IMGUI with GLFW!\n");
        return response;
    }

    response = ImGui_ImplOpenGL3_Init("#version 420");
    if (!response)
    {
        printf("Error initializing IMGUI with OpenGL!\n");
        return response;
    }

    return true;
}

void MediaPlayer::m_NewFrameImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set Imgui window maximized
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    return;
}

void MediaPlayer::m_UIImgui()
{
    // Removes Imgui window resize, title bar and collapse button
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
    // Start Imgui window context
    ImGui::Begin(this->programName, NULL, flags);
    // Increase font default scale
    ImGui::SetWindowFontScale(1.8);

    // UI
    // ---------------------------------------------
    // Add buttons for audio control
    
    // ---------------------------------------------

    // End Imgui window context
    ImGui::End();
    return;
}

void MediaPlayer::m_RenderImgui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return;
}

bool MediaPlayer::Setup()
{
    // Setup audio system
    // ------------------------------------------------
    this->pAudioManager = new audio::AudioManager();
    bool audioInitialized = this->pAudioManager->Initialize();
    if (!audioInitialized)
    {
        printf("Error initializing audio system!\n");
        return false;
    }
    // ------------------------------------------------

    // Setup rendering system
    // ------------------------------------------------
    this->pRenderingSystem = new RenderingSystem();
    bool renderingInitialized = this->pRenderingSystem->SetupWindow(this->programName);
    if (!renderingInitialized)
    {
        printf("Error initializing rendering system!\n");
        this->pAudioManager->Destroy();
        return false;
    }
    // ------------------------------------------------

    // Setup Shaders and program
    // ------------------------------------------------
    this->pShaderManager = new ShaderManager();
    this->pShaderManager->SetBasePath("src/ShaderManager/Shaders/");
    bool shaderInitialized = this->pShaderManager->SetupShader(this->programName);
    if (!shaderInitialized)
    {
        printf("Error initializing shaders!\n");
        this->pAudioManager->Destroy();
        this->pRenderingSystem->Destroy();
        return false;

    }
    // ------------------------------------------------

    // Setup UI
    // ------------------------------------------------
    bool uiInitialized = this->m_SetupImgui();
    if (!uiInitialized)
    {
        printf("Error initializing UI!\n");
        this->pAudioManager->Destroy();
        this->pRenderingSystem->Destroy();
        return false;
    }
    // ------------------------------------------------

    // Load audios


    return true;
}

void MediaPlayer::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    this->pAudioManager->Destroy();
    this->pRenderingSystem->Destroy();

    delete this->pAudioManager;
    delete this->pRenderingSystem;

    return;
}

void MediaPlayer::MainLoop()
{
    while (!this->pRenderingSystem->WindowShouldClose())
    {
        GLint shaderProgramID = this->pShaderManager->GetIDFromShaderProgramName(this->programName);
        glUseProgram(shaderProgramID);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Initializes IMGUI frame
        this->m_NewFrameImgui();

        // Builds then render IMGUI window
        this->m_UIImgui();
        this->m_RenderImgui();

        glfwSwapBuffers(this->pRenderingSystem->pWindow);
        glfwPollEvents();
    }
}


void MediaPlayer::PlayAudio(const char* audioName, int playMode)
{
}

void MediaPlayer::PauseAudio(const char* audioName)
{
}

void MediaPlayer::StopAudio(const char* audioName)
{
}

void MediaPlayer::AdjustPitch(const char* audioName, float value)
{
}

void MediaPlayer::AdjustVolume(const char* audioName, float value)
{
}

void MediaPlayer::AdjustPan(const char* audioName, float value)
{
}
