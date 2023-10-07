#pragma once

// Must be imported before opengl stuff
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "iButtonCallback.h"
#include <map>

/*
* This will be used as the default callback function for all buttons
*/
typedef void (*BtnCallbacks)(const char* audioName);

class UserInterface
{
private:
	void m_NewButton(const char* label, ImVec2& buttonSize, unsigned int color, 
					iButtonCallback& callback, const char* audioName, const char* action);
public:
	UserInterface();
	~UserInterface();

	// Initialize all IMGUI components
	bool Setup(GLFWwindow* pWindow);

	void Destroy();

	// Creates new frame for Imgui window
	void NewFrame();

	// Builds all the UI for this frame
	void BuildFrame(const char* windowName, const std::map<const char* /*audio name*/,
														std::map<const char* /*info key*/,
														               float /*info value*/ >>& audioInfo, iButtonCallback& callback);

	// Render all UI elements built in this frame
	void RenderFrame();

	// Default button callback for undefined callbacks
	void DefaultBtnCallback(const char* btnName);
};