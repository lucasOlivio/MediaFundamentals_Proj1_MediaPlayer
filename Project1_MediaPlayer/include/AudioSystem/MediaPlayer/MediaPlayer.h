#pragma once

#include "AudioSystem/AudioManager/AudioManager.h"

#include <map>


class MediaPlayer
{
private:
	// Will take care with all the FMOD integration and channels management
	audio::AudioManager* pAudioManager;

	// Info needed in frames building 
	std::map<const char* /*audio name*/, 
				std::map<const char* /*info key*/, 
							   float /*info value*/ >> m_vecAudioInfo;

	// Load audio from our "database"
	void m_LoadAudios();
public:

	// Ctors and dtors
	MediaPlayer();
	~MediaPlayer();

	bool Setup();
	void Destroy();

	// Update and retrieve all audio data that we keep track of
	const std::map<const char* /*audio name*/,
					std::map<const char* /*info key*/,
					               float /*info value*/ >>& GetAudioInfo();
	void GetAudioIDChannel(const char* audioName, int& idChannel);

	// UI Actions
	void PlayAudio(const char* audioName);
	void PauseAudio(const char* audioName);
	void StopAudio(const char* audioName);
	void AdjustPitch(const char* audioName, float value);
	void AdjustVolume(const char* audioName, float value);
	void AdjustPan(const char* audioName, float value);
};