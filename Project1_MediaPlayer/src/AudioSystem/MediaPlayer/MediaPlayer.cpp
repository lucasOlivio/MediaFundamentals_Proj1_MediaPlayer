#include "AudioSystem/MediaPlayer/MediaPlayer.h"
#include "Utils.h"

#include <fstream>
#include <sstream>
#include <string>

// Parameters for the audio management
const float MIN_PITCH = 0.0;
const float MAX_PITCH = 2.0f;
const float MIN_VOLUME = 0.0f;
const float MAX_VOLUME = 1.0f;
const float MIN_PAN = -1.0f;
const float MAX_PAN = 1.0f;

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
    // Opening our little "database" to get which audios to load 
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
    // For now we don't accept paths with spaces
    // first col loadType (1 or 2), second col filePath
    while (audioDB >> loadType >> filePath)
    {
        if (loadType < 1 || loadType > 2)
        {
            printf("%d Load type not accepted!", loadType);
            continue;
        }
        char* heapFilePath = new char[MAX_PATH_SIZE]; // Avoid overriding our pointer
        strcpy_s(heapFilePath, MAX_PATH_SIZE, filePath);

        // Load audio and initializes all audio info used by the UI
        const char* audioName = this->pAudioManager->LoadAudio(heapFilePath, loadType);

        this->m_vecAudioInfo[audioName] = {};

        this->m_vecAudioInfo[audioName]["IDCHANNEL"] = -1;
        this->m_vecAudioInfo[audioName]["VOLUME"] = 0;
        this->m_vecAudioInfo[audioName]["PITCH"] = 0;
        this->m_vecAudioInfo[audioName]["PAN"] = 0;
        this->m_vecAudioInfo[audioName]["PLAYBACK"] = 0;
        this->m_vecAudioInfo[audioName]["LENGTH"] = 0;
        this->m_vecAudioInfo[audioName]["PLAYING"] = 0;
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
        unsigned int totalLength = 0;
        bool playing = 0;

        if (idChannel > -1)
        {
            // Audio currently in a channel, so update info
            this->pAudioManager->GetChannelVolume(idChannel, volume);
            this->pAudioManager->GetChannelPitch(idChannel, pitch);
            this->pAudioManager->GetChannelPan(idChannel, pan);
            this->pAudioManager->GetPlaybackPosition(idChannel, playback);
            this->pAudioManager->GetAudioLength(pair.first, totalLength);
            playing = this->pAudioManager->IsChannelPlaying(idChannel);
        }

        this->m_vecAudioInfo[pair.first]["VOLUME"] = volume;
        this->m_vecAudioInfo[pair.first]["PITCH"] = pitch;
        this->m_vecAudioInfo[pair.first]["PAN"] = pan;

        // Wrap pitch and pan around 1.0 for simplifying display
        float pitchRatio; myMath::WrapRatio(pitchRatio, pitch, MIN_PITCH, MAX_PITCH);
        this->m_vecAudioInfo[pair.first]["PITCH_RATIO"] = pitchRatio;
        float panRatio; myMath::WrapRatio(panRatio, pan, MIN_PAN, MAX_PAN);
        this->m_vecAudioInfo[pair.first]["PAN_RATIO"] = panRatio;

        this->m_vecAudioInfo[pair.first]["PLAYBACK"] = playback;
        this->m_vecAudioInfo[pair.first]["LENGTH"] = totalLength;
        this->m_vecAudioInfo[pair.first]["PLAYING"] = playing;

        if (!playing && playback > 0)
        {
            // Music is over, so we can reset the channel
            this->StopAudio(pair.first);
        }
    }
    return this->m_vecAudioInfo;
}

void MediaPlayer::GetAudioIDChannel(const char* audioName, int& idChannel)
{
    idChannel = -1;
    if (!this->pAudioManager->IsAudioLoaded(audioName))
    {
        // Audio not loaded!
        return;
    }

    idChannel = this->m_vecAudioInfo[audioName]["IDCHANNEL"];
}

// UI Actions
// --------------------------------------------------------------------
void MediaPlayer::PlayAudio(const char* audioName)
{
    int idChannel;
    GetAudioIDChannel(audioName, idChannel);
    // Check if its already playing in a channel
    // If so we can just unpause
    if (idChannel > -1)
    {
        this->pAudioManager->SetPaused(idChannel, false);
        return;
    }

    // If not we have to set the next channel available for the audio to play
    this->m_vecAudioInfo[audioName]["IDCHANNEL"] = this->pAudioManager->PlayAudio(audioName);
    return;
}

void MediaPlayer::PauseAudio(const char* audioName)
{
    int idChannel;
    GetAudioIDChannel(audioName, idChannel);
    if (idChannel < 0)
    {
        // Audio not in any channel!
        return;
    }

    this->pAudioManager->SetPaused(idChannel, true);
    return;
}

void MediaPlayer::StopAudio(const char* audioName)
{
    int idChannel;
    GetAudioIDChannel(audioName, idChannel);
    if (idChannel < 0)
    {
        // Audio not in any channel!
        return;
    }

    this->pAudioManager->StopChannel(idChannel);
    this->m_vecAudioInfo[audioName]["IDCHANNEL"] = -1;
    return;
}

void MediaPlayer::AdjustPitch(const char* audioName, float value)
{
    int idChannel;
    GetAudioIDChannel(audioName, idChannel);
    if (idChannel < 0)
    {
        // Audio not in any channel!
        return;
    }

    float newValue = this->m_vecAudioInfo[audioName]["PITCH"] + value;
    myMath::WrapMinMax(newValue, MIN_PITCH, MAX_PITCH);

    this->pAudioManager->SetChannelPitch(idChannel, newValue);
    return;
}

void MediaPlayer::AdjustVolume(const char* audioName, float value)
{
    int idChannel;
    GetAudioIDChannel(audioName, idChannel);
    if (idChannel < 0)
    {
        // Audio not in any channel!
        return;
    }

    float newValue = this->m_vecAudioInfo[audioName]["VOLUME"] + value;
    myMath::WrapMinMax(newValue, MIN_VOLUME, MAX_VOLUME);

    this->pAudioManager->SetChannelVolume(idChannel, newValue);
}

void MediaPlayer::AdjustPan(const char* audioName, float value)
{
    int idChannel;
    GetAudioIDChannel(audioName, idChannel);
    if (idChannel < 0)
    {
        // Audio not in any channel!
        return;
    }

    float newValue = this->m_vecAudioInfo[audioName]["PAN"] + value;
    myMath::WrapMinMax(newValue, MIN_PAN, MAX_PAN);

    this->pAudioManager->SetChannelPan(idChannel, newValue);
}
