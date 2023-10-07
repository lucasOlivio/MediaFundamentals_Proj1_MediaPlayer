#pragma once

#include "UserInterface.h"
#include "RenderingSystem.h"
#include "ShaderManager.h"
#include "MediaPlayer/MediaPlayer.h"
#include <map>


class MainSystem
{
private:
	// Manages the interface setup and build for each frame
	UserInterface* pUserInterface;
	// Manages all our audio output and info
	MediaPlayer* pMediaPlayer;
	// Will take care of all opengl integrations, cameras and window management
	RenderingSystem* pRenderingSystem;
	// Deals with all the shaders compilation etc.
	ShaderManager* pShaderManager;

	const char* programName;

public:
	// Ctors and dtors
	MainSystem();
	~MainSystem();

	bool Setup();
	void Destroy();

	// Where we stay updating frames and getting user input
	void MainLoop();
};