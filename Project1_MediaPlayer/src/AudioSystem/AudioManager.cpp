#include "AudioSystem/AudioManager/AudioManager.h"
#include "AudioSystem/AudioUtils.h"

#include <iostream>


namespace audio
{
	const int AudioManager::m_MAX_CHANNELS = 30;

	AudioManager::AudioManager()
	{
		this->m_isInitialized = false;

		this->m_mappAudio = {};
		this->m_vecpChannel = {};

		this->m_idNextChannel = 0;

		this->m_pSystem = nullptr;
	}

	AudioManager::~AudioManager()
	{
	}

	bool AudioManager::Initialize()
	{
		if (this->m_isInitialized)
		{
			return false;
		}

		FMOD_RESULT result;
		result = FMOD::System_Create(&this->m_pSystem);
		if (result != FMOD_OK)
		{
			FMODCheckError(result);
			return false;
		}

		result = this->m_pSystem->init(this->m_MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			FMODCheckError(result);
			// Cleanup
			result = this->m_pSystem->close();
			if (result != FMOD_OK)
			{
				FMODCheckError(result);
			}
			return false;
		}

		printf("AudioManager::Initialize(): Successful!\n");

		for (int i = 0; i < this->m_MAX_CHANNELS; i++)
		{
			this->m_vecpChannel.push_back(new Channel);
		}

		this->m_isInitialized = true;

		return true;
	}

	void AudioManager::ClearAudioMap()
	{
		FMOD_RESULT result;
		for (std::pair<const char*, Audio*> pair : this->m_mappAudio)
		{
			result = pair.second->pAudio->release();
			FMODCheckError(result);

			delete pair.first;
			delete pair.second;
		}
	}

	void AudioManager::Destroy()
	{
		if (!this->m_isInitialized)
			return;

		FMOD_RESULT result;

		this->ClearAudioMap();

		result = this->m_pSystem->close();
		FMODCheckError(result);

		result = this->m_pSystem->release();
		FMODCheckError(result);

		this->m_isInitialized = false;
	}

	/**
	  * LoadAudio - Checks if the provided file is already loaded in our
	  * cache, if it is not in our cache we can load it.
	  * 
	  * @param file: Audio file name to load
	  * @param loadType: Which type will be used to load this file into memory (1 - sample, 2 - stream)
	  * @param audioName: Sets the audio name key, after removing path
	  * 
	  * @return: The name of the audio file used as key
	  */
	const char* AudioManager::LoadAudio(const char* file, int loadType)
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!this->m_isInitialized)
			return NULL;

		// Search our cache if we have already loaded this file
		// If we have already loaded the file, return
		const char* audioName = audio::GetFileFromPath(file);
		if (this->IsAudioLoaded(audioName))
		{
			printf("AudioManager::LoadAudio() Audio already loaded!\n");
			return NULL;
		}

		// Since we have not already loaded this file, we can add it to our
		// AudioMap as a cached value.
		// We can add this first so we can reference the audio file when
		// Creating the sound.;
		this->m_mappAudio.insert(std::pair<const char*, Audio*>(audioName, new Audio()));

		FMOD_RESULT result;
		switch (loadType)
		{
		case 1:
			// This call loads our audio entirely into memory
			result = this->m_pSystem->createSound(file, FMOD_DEFAULT, 0, &this->m_mappAudio[audioName]->pAudio);
			break;
		case 2:
			// This will only load the audio resource data as needed
			// to play the audio.
			result = this->m_pSystem->createStream(file, FMOD_DEFAULT, 0, &this->m_mappAudio[audioName]->pAudio);
			break;
		default:
			// Not recognized load type
			printf("AudioManager::LoadAudio() Load type not recognized: %d\n", loadType);
			return NULL;
		}

		if (result != FMOD_OK)
		{
			printf("AudioManager::LoadAudio(%s): Error loading file: ", audioName);
			FMODCheckError(result);
			return NULL;
		}

		this->m_mappAudio[audioName]->name = audioName;
		printf("AudioManager::LoadAudio(%s): Loaded successful!\n", audioName);

		return audioName;
	}

	int AudioManager::PlayAudio(const char* audioName)
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!this->m_isInitialized)
		{
			printf("Not initialized!\n");
			return this->m_idNextChannel;	// Still want to return a valid channel id
		}

		// Check our cache to see if the audio exists.
		std::map<const char*, Audio*>::iterator it = this->m_mappAudio.find(audioName);
		if (it == this->m_mappAudio.end())
		{
			printf("Audio not found!\n");
			return this->m_idNextChannel;	// Still want to return a valid channel id
		}
		printf("AudioFound!\n");

		// Get the current channel id, and calculate the next one
		int idChannel = this->m_idNextChannel;
		Channel* channel = this->m_vecpChannel[idChannel];

		// Attemp to play our sound.
		FMOD_RESULT result = this->m_pSystem->playSound(it->second->pAudio, 0, false, &channel->pChannel);
		if(result != FMOD_OK)
		{
			FMODCheckError(result);
			return this->m_idNextChannel;
		}

		this->m_idNextChannel = (this->m_idNextChannel + 1) % this->m_MAX_CHANNELS;

		// Return the channel id used so we can modify it if needed.
		return idChannel;
	}

	void AudioManager::StopChannel(int id)
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!this->m_isInitialized)
		{
			printf("Not initialized!\n");
			return;
		}

		this->m_vecpChannel[id]->pChannel->stop();
		return;
	}

	void AudioManager::SetPaused(int id, bool value)
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!this->m_isInitialized)
		{
			printf("Not initialized!\n");
			return;
		}

		this->m_vecpChannel[id]->pChannel->setPaused(value);
		return;
	}

	void AudioManager::Update()
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!this->m_isInitialized)
		{
			return;
		}

		// Call system update, this needs to occur once in a while. It is 
		// not mandatory for each frame.
		FMOD_RESULT result = this->m_pSystem->update();

		if (result != FMOD_OK)
		{
			FMODCheckError(result);
			Destroy();
		}
	}

	void AudioManager::SetChannelVolume(int id, float value)
	{
		// This call sets the volume on the channel. We can't modify 
		// the value, we can only set it.
		FMOD_RESULT result = this->m_vecpChannel[id]->pChannel->setVolume(value);
		FMODCheckError(result);
	}

	void AudioManager::SetChannelPitch(int id, float value)
	{
		// This call sets the pitch value on the channel. We can't modify 
		// the value, we can only set it.
		FMOD_RESULT result = this->m_vecpChannel[id]->pChannel->setPitch(value);
		FMODCheckError(result);
	}

	void AudioManager::SetChannelPan(int id, float value)
	{
		// This call sets the pan value on the channel. We can't modify 
		// the value, we can only set it.
		FMOD_RESULT result = this->m_vecpChannel[id]->pChannel->setPan(value);
		if (result != FMOD_OK)
		{
			FMODCheckError(result);
			return;
		}
		this->m_vecpChannel[id]->pan = value;
	}

	bool AudioManager::IsChannelPlaying(int id)
	{
		// Check if the channel id is currently playing or not.
		// If it is not playing we can do a cleanup
		bool isPlaying;
		FMOD_RESULT result = this->m_vecpChannel[id]->pChannel->isPlaying(&isPlaying);
		FMODCheckError(result);
		return isPlaying;
	}

	void AudioManager::GetPlaybackPosition(int id, unsigned int& value)
	{
		// Use this call to get the playback position of a channel.
		FMOD_RESULT result = this->m_vecpChannel[id]->pChannel->getPosition(&value, FMOD_TIMEUNIT_MS);
		FMODCheckError(result);
	}

	bool AudioManager::IsAudioLoaded(const char* audioName)
	{
		// Check our cache to see if the audio exists.
		std::map<const char*, Audio*>::iterator it = this->m_mappAudio.find(audioName);
		if (it == this->m_mappAudio.end())
		{
			// Audio not found
			return false;
		}
		
		return true;
	}

	void AudioManager::GetAudioLength(const char* audioName, unsigned int& value)
	{
		std::map<const char*, Audio*>::iterator it = this->m_mappAudio.find(audioName);
		if (it == this->m_mappAudio.end())
		{
			// Audio not found
			return;
		}

		FMOD_RESULT result = it->second->pAudio->getLength(&value, FMOD_TIMEUNIT_MS);
		FMODCheckError(result);
	}

	void AudioManager::GetChannelPitch(int id, float& value)
	{
		// Use this call to get the pitch value of a channel.
		FMOD_RESULT result = this->m_vecpChannel[id]->pChannel->getPitch(&value);
		FMODCheckError(result);
	}

	void AudioManager::GetChannelPan(int id, float& value)
	{
		// Since fmod channel doesn't have a getpan function
		// we need to keep track of this in the channel
		value = this->m_vecpChannel[id]->pan;
	}

	void AudioManager::GetChannelVolume(int id, float& value)
	{
		// Use this call to get the volume value of a channel.
		FMOD_RESULT result = this->m_vecpChannel[id]->pChannel->getVolume(&value);
		FMODCheckError(result);
	}
} // end audio namespace