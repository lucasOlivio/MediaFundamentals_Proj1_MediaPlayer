#pragma once

#include "../../Resources/Audio/Audio.h";
#include "../../Resources/Channel/Channel.h";
#include <map>
#include <vector>
#include <string>


class AudioManager
{
private:
	static const unsigned int m_MAX_CHANNELS;

	bool m_isInitialized;

	std::vector<Channel*> m_pChannels;

	std::map<const char* /*Audio file name*/,
				  Audio* /*Audio object*/> m_pAudios;

	FMOD::System* m_pSystem;

	unsigned int m_nextChannel;

public:
	// ctors and dtours
	AudioManager();
	~AudioManager();

	bool Setup();
	void Destroy();

	Audio* FindAudio(const char* audioName);

	/*
	* Loads the audio from file and set to a new audio resource.
	* 
	* @param: fileName - Audio file name.
	* @param: playType - If the audio is going to be played as sample (1) or stream (2).
	*/
	void LoadAudio(const char* fileName, int playType);

	// Remove audio from channel and unload from memory
	void RemoveAudio(const char* audioName);

	 // Plays the audio in the next available channel
	void PlayAudio(const char* audioName);

	// Pause audio playing in its respective channel
	void PauseAudio(const char* audioName);

	// Stop and release the audio from the channel
	void StopAudio(const char* audioName);

	// Update FMOD system
	void Update();

	//
	void PrintInfo();

	// Adjust channel's configurations
	void SetChannelVolume(int id, float newVolume);
	void SetChannelPitch(int id, float newPitch);
	void SetChannelPan(int id, float newPan);
};