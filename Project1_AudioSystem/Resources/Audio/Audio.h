#pragma once

#include <fmod/fmod.hpp>


class Audio
{
public:
	// ctors and dtors
	Audio();
	~Audio();

	// Cleans any needed audio data
	void Destroy();

	const char* name;
	FMOD::Sound* pAudio;
	int idChannel;
};