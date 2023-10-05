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

		std::map<const char*, Audio*> m_mappAudio;	// This is our cache
		std::vector<Channel*> m_vecpChannel;		// Maintaint a list of channels 

		int m_idNextChannel;					// The next channel Id that we can return

		FMOD::System* m_pSystem;

	public:
		// Ctors & dtors
		AudioManager();
		~AudioManager();

		// System calls
		bool Initialize();
		void Update();
		void Destroy();

		// Clear and release Audios from Audio map
		void ClearAudioMap();

		// Loading Audio
		void LoadAudio(const char* file, int loadType);

		// Play audio
		int PlayAudio(const char* AudioName);

		// Modifying Channel Values
		void SetChannelVolume(int id, float value);
		void SetChannelPitch(int id, float value);
		void SetChannelPan(int id, float value);

		// Retrieving information
		bool IsChannelPlaying(int id);
		void GetPlaybackPosition(int id, unsigned int& value);
		void GetChannelPitch(int id, float &value);
		void GetChannelPan(int id, float& value);
		void GetChannelVolume(int id, float& value);


		void PrintInfo() const;
	};
}