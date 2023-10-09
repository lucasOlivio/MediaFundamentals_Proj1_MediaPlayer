#pragma once

#include "iButtonCallback.h"
#include "MediaPlayer.h"

class MediaPlayerCallback : public iButtonCallback
{
private:
	MediaPlayer& m_mediaPlayer;
public:
	MediaPlayerCallback(MediaPlayer& mediaPlayer);
	virtual ~MediaPlayerCallback();

	virtual void Execute(const char* key, const char* action);
};
