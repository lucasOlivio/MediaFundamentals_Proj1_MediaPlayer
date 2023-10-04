#include "Audio.h"

Audio::Audio()
{
	this->name = nullptr;
	this->pAudio = nullptr;
	this->idChannel = -1;
}

Audio::~Audio()
{
}

void Audio::Destroy()
{
	this->idChannel = -1;
	this->pAudio->release();
	if (this->name)
	{
		delete this->name;
	}
}