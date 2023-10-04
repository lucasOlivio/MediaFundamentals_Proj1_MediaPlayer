#pragma once

#include <fmod/fmod.hpp>

class Channel
{
private:
	float m_volume;
	float m_pitch;
	float m_pan;
	bool m_playing;

	FMOD::Channel* m_pChannel;

public:
	// ctors and dtors
	Channel();
	~Channel();

	// Cleans any needed channel data
	void Destroy();

	// Getters and setters
	float GetVolume() const;
	void SetVolume(float volume);

	float GetPitch() const;
	void SetPitch(float pitch);

	float GetPan() const;
	void SetPan(float pan);

	bool IsPlaying() const;
	void SetPlaying(bool playing);
	void StopChannel();

	FMOD::Channel* GetpChannel();
};