#pragma once

#include <fmod/fmod.hpp>

namespace audio
{
	struct Channel
	{
		const char* name;
		FMOD::Channel* pChannel;

		// Since fmod channel doesn't have a getpan function
		// we need to keep track of this in the channel
		float pan;
	};
}