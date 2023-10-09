#include "AudioSystem/AudioUtils.h"
#include <iostream>

namespace audio
{
	void CheckError(FMOD_RESULT result, const char* file, int line)
	{
		if (result != FMOD_OK)
		{
			printf("FMOD Error [%d]: '%s' at %d\n", static_cast<int>(result), file, line);
		}
	}

	char* GetFileFromPath(const char* cPath)
	{
		std::string sPath = cPath;
		std::string baseFilename = sPath.substr(sPath.find_last_of("/\\") + 1);
		char* cBaseFileName = new char[baseFilename.size() + 1];
		strcpy_s(cBaseFileName, baseFilename.size() + 1, baseFilename.c_str());
		return cBaseFileName;
	}
}