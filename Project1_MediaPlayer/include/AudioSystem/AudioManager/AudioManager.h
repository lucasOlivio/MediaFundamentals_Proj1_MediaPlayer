#pragma once

#include "Audio.h"
#include "Channel.h"

#include <fmod/fmod.hpp>
#include <map>
#include <vector>

namespace audio
{
	class AudioManager
	{
	private:
		static const int m_MAX_CHANNELS;

		bool m_isInitialized;

		std::map<const char*, Audio*> m_mappAudio; // This is our cache
		std::vector<Channel*> m_vecpChannel;	   // Maintaint a list of channels

		int m_idNextChannel;					// The next channel Id that we can return

		FMOD::System* m_pSystem;

		// Clear and release resources
		void m_ClearAudioMap();

	public:
		// Ctors & dtors
		AudioManager();
		~AudioManager();

		// System calls
		bool Initialize();
		void Update();
		void Destroy();

		// Loading Audio
		const char* LoadAudio(const char* file, int loadType);

		// Play audio in the next available channel
		int PlayAudio(const char* audioName);

		// Modifying Channel Values
		void StopChannel(int id);
		void SetPaused(int id, bool value);
		void SetChannelVolume(int id, float value);
		void SetChannelPitch(int id, float value);
		void SetChannelPan(int id, float value);

		// Retrieving information
		bool IsChannelPlaying(int id);
		bool IsAudioLoaded(const char* audioName);
		void GetAudioLength(const char* audioName, unsigned int& value);
		void GetPlaybackPosition(int id, unsigned int& value);
		void GetChannelPitch(int id, float &value);
		void GetChannelPan(int id, float& value);
		void GetChannelVolume(int id, float& value);
	};
}