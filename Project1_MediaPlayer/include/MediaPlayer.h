#pragma once

#include "AudioManager.h"
// Must be imported before opengl stuff
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "RenderingSystem.h"
#include "ShaderManager.h"


class MediaPlayer
{
private:
	// Will take care with all the FMOD integration and channels management
	audio::AudioManager* pAudioManager;
	// Will take care of all opengl integrations, cameras and window management
	RenderingSystem* pRenderingSystem;
	// Deals with all the shaders compilation etc.
	ShaderManager* pShaderManager;

	// Initialize all IMGUI components
	bool m_SetupImgui();

	// Creates new frame for Imgui window
	void m_NewFrameImgui();

	// All UI interface, buttons, etc. Build per frame
	void m_UIImgui();

	// Render all IMGUI UI elements
	void m_RenderImgui();

	const char* programName;

public:
	// Ctors and dtors
	MediaPlayer();
	~MediaPlayer();

	bool Setup();
	void Destroy();

	// Where we stay updating frames and getting user input
	void MainLoop();

	// Buttons callbacks
	void PlayAudio(const char* audioName, int playMode);
	void PauseAudio(const char* audioName);
	void StopAudio(const char* audioName);
	void AdjustPitch(const char* audioName, float value);
	void AdjustVolume(const char* audioName, float value);
	void AdjustPan(const char* audioName, float value);
};