#include "MainSystem.h"

int main()
{
	MainSystem* pMainSystem = new MainSystem();
	bool systemInitialized = pMainSystem->Setup();
	if (!systemInitialized)
	{
		system("pause");
		return EXIT_FAILURE;
	}

	pMainSystem->MainLoop();

	pMainSystem->Destroy();

	return 0;
}