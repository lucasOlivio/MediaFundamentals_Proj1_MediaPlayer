#include "AudioSystem/MediaPlayer/MediaPlayerCallback.h"

MediaPlayerCallback::MediaPlayerCallback(MediaPlayer& mediaPlayer) : m_mediaPlayer(mediaPlayer)
{
}

MediaPlayerCallback::~MediaPlayerCallback()
{
}

void MediaPlayerCallback::Execute(const char* key, const char* action)
{
    // Audio controls
    if (strcmp(action, "PLAY") == 0)
    {
        this->m_mediaPlayer.PlayAudio(key);
        return;
    }
    if (strcmp(action, "PAUSE") == 0)
    {
        this->m_mediaPlayer.PauseAudio(key);
        return;
    }
    if (strcmp(action, "STOP") == 0)
    {
        this->m_mediaPlayer.StopAudio(key);
        return;
    }

    // Volume
    if (strcmp(action, "INCREASE_VOLUME") == 0)
    {
        this->m_mediaPlayer.AdjustVolume(key, 0.1f);
        return;
    }
    if (strcmp(action, "DECREASE_VOLUME") == 0)
    {
        this->m_mediaPlayer.AdjustVolume(key, -0.1f);
        return;
    }

    // Pitch
    if (strcmp(action, "INCREASE_PITCH") == 0)
    {
        this->m_mediaPlayer.AdjustPitch(key, 0.1f);
        return;
    }
    if (strcmp(action, "DECREASE_PITCH") == 0)
    {
        this->m_mediaPlayer.AdjustPitch(key, -0.1f);
        return;
    }

    // Pan
    if (strcmp(action, "INCREASE_PAN") == 0)
    {
        this->m_mediaPlayer.AdjustPan(key, 0.1f);
        return;
    }
    if (strcmp(action, "DECREASE_PAN") == 0)
    {
        this->m_mediaPlayer.AdjustPan(key, -0.1f);
        return;
    }

    printf("[%s] Not implemented yet!\n", action);
    return;
}