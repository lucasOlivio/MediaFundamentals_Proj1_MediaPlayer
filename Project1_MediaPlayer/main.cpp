#include "MediaPlayer.h"

int main()
{
	MediaPlayer* pMediaPlayer = new MediaPlayer();
	bool mediaInitialized = pMediaPlayer->Setup();
	if (!mediaInitialized)
	{
		system("pause");
		return EXIT_FAILURE;
	}

	pMediaPlayer->MainLoop();

	pMediaPlayer->Destroy();

	return 0;
}