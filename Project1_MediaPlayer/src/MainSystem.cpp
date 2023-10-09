#include "MainSystem.h"
#include "AudioSystem/MediaPlayer/MediaPlayerCallback.h"

MainSystem::MainSystem()
{
    this->pRenderingSystem = nullptr;
    this->pShaderManager = nullptr;
    this->pUserInterface = nullptr;
    this->pMediaPlayer = nullptr;

    this->programName = "MyMediaPlayer";
}

MainSystem::~MainSystem()
{
}

bool MainSystem::Setup()
{

    // Setup media player
    // ------------------------------------------------
    this->pMediaPlayer = new MediaPlayer();
    bool mpInitialized = this->pMediaPlayer->Setup();
    if (!mpInitialized)
    {
        printf("Error initializing media player!\n");
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
        this->pMediaPlayer->Destroy();
        return false;
    }
    // ------------------------------------------------

    // Setup Shaders and program
    // ------------------------------------------------
    this->pShaderManager = new ShaderManager();
    this->pShaderManager->SetBasePath("src/RenderingSystem/ShaderManager/Shaders/");
    bool shaderInitialized = this->pShaderManager->SetupShader(this->programName);
    if (!shaderInitialized)
    {
        printf("Error initializing shaders!\n");
        this->pMediaPlayer->Destroy();
        this->pRenderingSystem->Destroy();
        return false;

    }
    // ------------------------------------------------

    // Setup UI
    // ------------------------------------------------
    bool uiInitialized = this->pUserInterface->Setup(this->pRenderingSystem->pWindow);
    if (!uiInitialized)
    {
        printf("Error initializing UI!\n");
        this->pMediaPlayer->Destroy();
        this->pRenderingSystem->Destroy();
        this->pShaderManager->Destroy();
        return false;
    }
    // ------------------------------------------------
    return true;
}

void MainSystem::Destroy()
{
    this->pUserInterface->Destroy();
    this->pMediaPlayer->Destroy();
    this->pRenderingSystem->Destroy();
    this->pShaderManager->Destroy();

    delete this->pUserInterface;
    delete this->pMediaPlayer;
    delete this->pRenderingSystem;
    delete this->pShaderManager;
}

void MainSystem::MainLoop()
{
    // Here we wrap the UI requests so the buttons doesn't need to know about the media player class
    MediaPlayerCallback mpCallback = MediaPlayerCallback(*this->pMediaPlayer);

    while (!this->pRenderingSystem->WindowShouldClose())
    {
        // Update the current program id for shaders
        GLint shaderProgramID = this->pShaderManager->GetIDFromShaderProgramName(this->programName);
        this->pRenderingSystem->NewFrame(shaderProgramID);

        // Initializes new UI frame
        this->pUserInterface->NewFrame();

        // Builds then render UI frame window
        auto mapAudioInfo = this->pMediaPlayer->GetAudioInfo();
        this->pUserInterface->BuildFrame(this->programName, mapAudioInfo, mpCallback);
        this->pUserInterface->RenderFrame();

        this->pRenderingSystem->EndFrame();
    }
    return;
}
