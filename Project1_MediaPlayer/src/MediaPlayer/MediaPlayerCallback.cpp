#include "MediaPlayer/MediaPlayerCallback.h"

MediaPlayerCallback::MediaPlayerCallback(MediaPlayer& mediaPlayer) : m_mediaPlayer(mediaPlayer)
{
}

MediaPlayerCallback::~MediaPlayerCallback()
{
}

void MediaPlayerCallback::Execute(const char* key, const char* action)
{
    if (action == "PLAY")
    {
        this->m_mediaPlayer.PlayAudio(key);
    }
    else
    {
        printf("[%s] Not implemented yet!\n", action);
    }
}