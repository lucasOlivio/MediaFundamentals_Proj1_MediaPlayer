#include "Managers/AudioManager/AudioManager.h"
#include <conio.h>


int main()
{
	AudioManager* pAudioManager = new AudioManager();
	pAudioManager->Setup();
	pAudioManager->LoadAudio("Assets\\Audio\\piano.wav", 1);
	pAudioManager->LoadAudio("Assets\\Audio\\samba.wav", 2);

	bool m_running = true;

	while (m_running)
	{
		pAudioManager->Update();

		if (_kbhit())
		{
			int key = _getch();
			if (key == '1')
			{
				pAudioManager->PlayAudio("Assets\\Audio\\piano.wav");
				continue;
			}
			if (key == '2')
			{
				pAudioManager->PlayAudio("Assets\\Audio\\samba.wav");
				continue;
			}
			if (key == '0')
			{
				m_running = false;
				pAudioManager->Destroy();
				continue;
			}
		}
	}

	return 0;
}