#include "UserInterface.h"

#include <iostream>
#include "Styles/Colors.h"
#include "Styles/Sizes.h"

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

bool UserInterface::Setup(GLFWwindow* pWindow)
{
    bool response = true;

    // Initialize all Imgui integration with GLFW and OpenGL
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Dark theme for the window
    ImGui::StyleColorsDark();
    response = ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
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

void UserInterface::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UserInterface::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set Imgui window maximized
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    return;
}

void UserInterface::m_NewButton(const char* label, ImVec2& buttonSize, unsigned int color, 
                                iButtonCallback& callback, const char* audioName, const char* action)
{
    // Set the button's background color to green
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    if (ImGui::Button(label, buttonSize)) {
        callback.Execute(audioName, action);
    }
    // Restore the default button style color
    ImGui::PopStyleColor();
    return;
}

void UserInterface::BuildFrame(const char* windowName, 
                               const std::map<const char*, std::map<const char*, float>>&audioInfo, iButtonCallback& callback)
{
    // Set the entire ImGui window background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, LIGHT_BLUE);

    // Set all text font color
    ImGui::PushStyleColor(ImGuiCol_Text, DARK_BLUE);

    // Removes Imgui window resize, title bar and collapse button
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
    // Start Imgui window context
    ImGui::Begin(windowName, NULL, flags);
    // Increase font default scale
    ImGui::SetWindowFontScale(TEXT_FONT_SIZE);

    // UI
    // ---------------------------------------------
    
    // Get current window sizes to allow window resizing
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    const int SEPARATOR_HEIGHT = displaySize[1] / 3;

    int separatorIndex = 0;
    for (std::pair<const char*, std::map<const char*, float>> audio : audioInfo)
    {
        // Draw the frame rectangle
        // --------------------------------------------
        int separatorHeight = SEPARATOR_PADDING + (separatorIndex * SEPARATOR_HEIGHT);

        ImVec2 frameSize(0, separatorHeight);

        ImGui::SetCursorPos(frameSize);
        ImGui::Separator();
        // --------------------------------------------

        // Audio name as title in top left corner
        // --------------------------------------------
        int posTextWidth = TEXT_PADDING_LEFT;
        int posTextHeight = separatorHeight + TEXT_PADDING_TOP;

        ImVec2 textPos(posTextWidth, posTextHeight);

        // Increase font title scale
        ImGui::SetCursorPos(textPos);
        ImGui::SetWindowFontScale(TEXT_FONT_SIZE_TITLE);
        ImGui::Text(audio.first);
        ImGui::SetWindowFontScale(TEXT_FONT_SIZE);

        ImGui::NewLine();
        // --------------------------------------------

        // Buttons for audio control
        // --------------------------------------------
        int posButtonWidth = BUTTON_PADDING_LEFT;
        int posButtonHeight = posTextHeight + BUTTON_PADDING_TOP + BUTTON_HEIGHT_L;

        ImVec2 buttonPos(posButtonWidth, posButtonHeight);
        ImVec2 buttonSize(BUTTON_WIDTH_L, BUTTON_HEIGHT_L);

        ImGui::SetCursorPos(buttonPos);
        this->m_NewButton("PLAY", buttonSize, GREEN, callback, audio.first, "PLAY");
        ImGui::SameLine();
        this->m_NewButton("PAUSE", buttonSize, GREY_LIGHT_BLUE, callback, audio.first, "PAUSE");
        ImGui::SameLine();
        this->m_NewButton("STOP", buttonSize, RED, callback, audio.first, "STOP");
        // --------------------------------------------

        ImVec2 buttonMSize(BUTTON_WIDTH_M, BUTTON_HEIGHT_M);
        // Volume management
        // --------------------------------------------
        ImGui::SameLine(0, CONTAINER_MARGIN);
        // Begin a horizontal layout
        ImGui::BeginGroup();

        this->m_NewButton("-", buttonMSize, GREY_LIGHT_BLUE, callback, audio.first, "INCREASE_VOLUME");
        ImGui::SameLine();
        ImGui::Text("VOLUME");
        ImGui::SameLine();
        this->m_NewButton("+", buttonMSize, GREY_LIGHT_BLUE, callback, audio.first, "DECREASE_VOLUME");

        // End the horizontal layout
        ImGui::EndGroup();
        // --------------------------------------------
 
        // Pitch management
        // --------------------------------------------
        ImGui::SameLine(0, CONTAINER_MARGIN);
        // Begin a horizontal layout
        ImGui::BeginGroup();

        this->m_NewButton("-", buttonMSize, GREY_LIGHT_BLUE, callback, audio.first, "INCREASE_PITCH");
        ImGui::SameLine();
        ImGui::Text("PITCH");
        ImGui::SameLine();
        this->m_NewButton("+", buttonMSize, GREY_LIGHT_BLUE, callback, audio.first, "DECREASE_PITCH");

        // End the horizontal layout
        ImGui::EndGroup();
        // --------------------------------------------
 
        // Pan management
        // --------------------------------------------
        ImGui::SameLine(0, CONTAINER_MARGIN);
        // Begin a horizontal layout
        ImGui::BeginGroup();

        this->m_NewButton("-", buttonMSize, GREY_LIGHT_BLUE, callback, audio.first, "INCREASE_PAN");
        ImGui::SameLine();
        ImGui::Text("PAN");
        ImGui::SameLine();
        this->m_NewButton("+", buttonMSize, GREY_LIGHT_BLUE, callback, audio.first, "DECREASE_PAN");

        // End the horizontal layout
        ImGui::EndGroup();
        // --------------------------------------------

        separatorIndex += 1;
    } // for (std::pair<const char*, std::map<const char*, float>> audio : audioInfo)
    // ---------------------------------------------

    // End Imgui window context
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::End();
    return;
}

void UserInterface::RenderFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return;
}

void UserInterface::DefaultBtnCallback(const char* btnName)
{
    printf("%s not implemented yet!\n", btnName);
    return;
}