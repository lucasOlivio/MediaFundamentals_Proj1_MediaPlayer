#pragma once

#include <fmod/fmod.hpp>

namespace audio
{
	struct Audio
	{
		const char* name;
		FMOD::Sound* pAudio;
	};
}