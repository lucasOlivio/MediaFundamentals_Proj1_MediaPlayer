#pragma once

class iButtonCallback
{
public:
	virtual ~iButtonCallback() {};
	/*
	* All buttons in the UI will use the Execute function after been pressed
	* 
	* @param key: Will be the key used to define the main context the button is in (In this case, the audio name)
	* @param action: The action that should be executed with this button been pressed (Ex: IncreaseVolume, DecreaseVolume, etc.)
	*/
	virtual void Execute(const char* key, const char* action) = 0;
};