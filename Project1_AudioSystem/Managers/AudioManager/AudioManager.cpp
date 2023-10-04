#include "AudioManager.h"


const unsigned int AudioManager::m_MAX_CHANNELS = 5;

AudioManager::AudioManager()
{
	this->m_isInitialized = false;
	this->m_pChannels = {};
	this->m_pAudios = {};
	this->m_pSystem = nullptr;
	this->m_nextChannel = 0;
}

AudioManager::~AudioManager()
{
}

bool AudioManager::Setup()
{
	if (this->m_isInitialized)
	{
		return true;
	}

	FMOD_RESULT result;
	result = FMOD::System_Create(&this->m_pSystem);
	if (result != FMOD_OK)
	{
		printf("AudioManager::Setup() Failed to create the FMOD Sytem!\n");
		return false;
	}

	result = this->m_pSystem->init(this->m_MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		printf("AudioManager::Setup() Failed to initialize the FMOD Sytem!\n");
		this->Destroy();
		return false;
	}

	for (unsigned int i = 0; i < this->m_MAX_CHANNELS; i++)
	{
		this->m_pChannels.push_back(new Channel());
	}

	this->m_isInitialized = true;

    return true;
}

void AudioManager::Destroy()
{
	for (Channel* pChannel : this->m_pChannels)
	{
		pChannel->Destroy();
		delete pChannel;
	}
	this->m_pChannels.clear();

	for (std::pair<const char*, Audio*> pairAudio : this->m_pAudios)
	{
		pairAudio.second->Destroy();
		delete pairAudio.second;
	}
	this->m_pAudios.clear();

	this->m_pSystem->close();

	this->m_isInitialized = false;

	return;
}

Audio* AudioManager::FindAudio(const char* audioName)
{
	std::map<const char*, Audio*>::iterator itAudio = this->m_pAudios.find(audioName);
	if (itAudio == this->m_pAudios.end())
	{
		printf("AudioManager::FindAudio() Audio %s not found!\n", audioName);
		return nullptr;
	}

	return itAudio->second;
}

void AudioManager::LoadAudio(const char* fileName, int playType)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	if (this->FindAudio(fileName))
	{
		printf("AudioManager::LoadAudio() Audio already loaded!\n");
		return;
	}

	this->m_pAudios.insert(std::pair<const char*, Audio*>(fileName, new Audio()));

	FMOD_RESULT result;

	switch (playType)
	{
	case 1:
		result = this->m_pSystem->createSound(fileName, FMOD_DEFAULT, 0, &this->m_pAudios[fileName]->pAudio);
		break;
	case 2:
		result = this->m_pSystem->createStream(fileName, FMOD_DEFAULT, 0, &this->m_pAudios[fileName]->pAudio);
		break;
	default:
		printf("AudioManager::LoadAudio() Play type not recognized!");
		return;
	}

	if (result != FMOD_OK)
	{
		printf("AudioManager::LoadAudio() Failed to load the audio: %s\n", fileName);
		return;
	}

	printf("AudioManager::LoadAudio() %s audio loaded successful!\n", fileName);

	return;
}

void AudioManager::RemoveAudio(const char* audioName)
{
	// Here we need the iterator to also remove the audio from the map
	std::map<const char*, Audio*>::iterator itAudio = this->m_pAudios.find(audioName);
	if (itAudio == this->m_pAudios.end())
	{
		printf("AudioManager::RemoveAudio() Audio %s not found!\n", audioName);
		return;
	}

	// If audio is playing in a channel we need to stop and clear channel first
	if (itAudio->second->idChannel > -1)
	{
		this->StopAudio(audioName);
	}

	itAudio->second->Destroy();
	delete itAudio->second;
	this->m_pAudios.erase(audioName);

	return;
}

void AudioManager::PlayAudio(const char* audioName)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	Audio* pAudioLoaded = this->FindAudio(audioName);
	if (!pAudioLoaded)
	{
		printf("AudioManager::PlayAudio() Audio %s not found!\n", audioName);
		return;
	}

	FMOD_RESULT result;
	Channel* pChannel = this->m_pChannels[this->m_nextChannel];
	pChannel->SetPlaying(true);
	FMOD::Channel* pFMODChannel = pChannel->GetpChannel();
	result = this->m_pSystem->playSound(pAudioLoaded->pAudio, 0, false, &pFMODChannel);
	if (result != FMOD_OK)
	{
		printf("AudioManager::PlayAudio() Error to play %s!\n", audioName);
		return;
	}
	pAudioLoaded->idChannel = this->m_nextChannel;

	this->m_nextChannel = (this->m_nextChannel + 1) % this->m_MAX_CHANNELS;

	return;
}

void AudioManager::PauseAudio(const char* audioName)
{
	Audio* pAudioLoaded = this->FindAudio(audioName);
	if (!pAudioLoaded)
	{
		printf("AudioManager::PauseAudio() Audio %s not found!\n", audioName);
		return;
	}

	this->m_pChannels[pAudioLoaded->idChannel]->SetPlaying(false);

	return;
}

void AudioManager::StopAudio(const char* audioName)
{
	Audio* pAudioLoaded = this->FindAudio(audioName);
	if (!pAudioLoaded)
	{
		printf("AudioManager::StopAudio() Audio %s not found!\n", audioName);
		return;
	}

	this->m_pChannels[pAudioLoaded->idChannel]->StopChannel();

	return;
}

void AudioManager::Update()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	FMOD_RESULT result;
	result = this->m_pSystem->update();
	if (result != FMOD_OK)
	{
		printf("AudioManager::Update() Error updating fmod system! Shutting all down...\n");
		this->Destroy();
		return;
	}
	return;
}

void AudioManager::PrintInfo()
{
	return;
}

void AudioManager::SetChannelVolume(int id, float newVolume)
{
	if (id < 0 || id >= this->m_MAX_CHANNELS)
	{
		printf("AudioManager::SetChannelVolume Channel id %d out of array range!", id);
	}
	this->m_pChannels[id]->SetVolume(newVolume);
	return;
}

void AudioManager::SetChannelPitch(int id, float newPitch)
{
	if (id < 0 || id >= this->m_MAX_CHANNELS)
	{
		printf("AudioManager::SetChannelPitch Channel id %d out of array range!", id);
	}
	this->m_pChannels[id]->SetPitch(newPitch);
	return;
}

void AudioManager::SetChannelPan(int id, float newPan)
{
	if (id < 0 || id >= this->m_MAX_CHANNELS)
	{
		printf("AudioManager::SetChannelPan Channel id %d out of array range!", id);
	}
	this->m_pChannels[id]->SetPan(newPan);
	return;
}
