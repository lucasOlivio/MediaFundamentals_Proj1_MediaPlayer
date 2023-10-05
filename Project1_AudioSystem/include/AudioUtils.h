#pragma once

#include <fmod/fmod.h>

namespace audio
{
	void CheckError(FMOD_RESULT result, const char* file, int line);
#define FMODCheckError(result) CheckError(result, __FILE__, __LINE__)
}