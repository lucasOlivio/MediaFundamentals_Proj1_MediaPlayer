#include "MediaPlayer/MediaPlayer.h"

#include <fstream>
#include <sstream>
#include <string>

MediaPlayer::MediaPlayer()
{
    this->pAudioManager = nullptr;

    this->m_vecAudioInfo = {};
}

MediaPlayer::~MediaPlayer()
{
}

void MediaPlayer::m_LoadAudios()
{
    std::ifstream audioDB("audioDB.txt");

    if (!audioDB.is_open())
    {
        printf("Audio DB not found!\n");
        return;
    }

    const int MAX_PATH_SIZE = 150;
    int loadType;
    char filePath[MAX_PATH_SIZE];
    // TODO: Understand better the char pointers and how to get paths with spaces
    while (audioDB >> loadType >> filePath)
    {
        char* heapFilePath = new char[MAX_PATH_SIZE]; // Avoid overriding our pointer
        strcpy_s(heapFilePath, MAX_PATH_SIZE, filePath);

        const char* audioName = this->pAudioManager->LoadAudio(heapFilePath, loadType);

        this->m_vecAudioInfo[audioName] = {};

        this->m_vecAudioInfo[audioName]["IDCHANNEL"] = -1;
        this->m_vecAudioInfo[audioName]["PLAYING"] = 0;
        this->m_vecAudioInfo[audioName]["VOLUME"] = 0;
        this->m_vecAudioInfo[audioName]["PITCH"] = 0;
        this->m_vecAudioInfo[audioName]["PAN"] = 0;
        this->m_vecAudioInfo[audioName]["PLAYBACK"] = 0;
    }

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

    // Load audios
    // ------------------------------------------------
    this->m_LoadAudios();
    // ------------------------------------------------

    return true;
}

void MediaPlayer::Destroy()
{
    this->pAudioManager->Destroy();

    delete this->pAudioManager;

    this->m_vecAudioInfo.clear();

    return;
}

const std::map<const char* /*audio name*/,
                std::map<const char* /*info key*/,
                               float /*info value*/ >>& MediaPlayer::GetAudioInfo()
{
    for (std::pair<const char*, std::map<const char*, float>> pair : this->m_vecAudioInfo)
    {
        int idChannel = this->m_vecAudioInfo[pair.first]["IDCHANNEL"];
        // Initialize each info that will be displayed
        float volume = 0.0f;
        float pitch = 0.0f;
        float pan = 0.0f;
        unsigned int playback = 0;
        bool playing = 0;

        if (idChannel > -1)
        {
            // Audio currently in a channel, so update info
            this->pAudioManager->GetChannelVolume(idChannel, volume);
            this->pAudioManager->GetChannelPitch(idChannel, pitch);
            this->pAudioManager->GetChannelPan(idChannel, pan);
            this->pAudioManager->GetPlaybackPosition(idChannel, playback);
            playing = this->pAudioManager->IsChannelPlaying(idChannel);
        }

        this->m_vecAudioInfo[pair.first]["PLAYING"] = playing;
        this->m_vecAudioInfo[pair.first]["VOLUME"] = volume;
        this->m_vecAudioInfo[pair.first]["PITCH"] = pitch;
        this->m_vecAudioInfo[pair.first]["PAN"] = pan;
        this->m_vecAudioInfo[pair.first]["PLAYBACK"] = playback;
    }
    return this->m_vecAudioInfo;
}

// UI Actions
// --------------------------------------------------------------------
void MediaPlayer::PlayAudio(const char* audioName)
{
    if (!this->pAudioManager->IsAudioLoaded(audioName))
    {
        // Audio not loaded!
        return;
    }
    this->m_vecAudioInfo[audioName]["CHANNEL"] = this->pAudioManager->PlayAudio(audioName);
    return;
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
