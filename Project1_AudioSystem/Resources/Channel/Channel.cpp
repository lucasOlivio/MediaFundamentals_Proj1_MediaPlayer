#include "Channel.h"

Channel::Channel()
{
	this->m_pChannel = nullptr;
    this->m_volume = 1.0f;
    this->m_pitch = 1.0f;
    this->m_pan = 0.0f;
    this->m_playing = false;
}

Channel::~Channel()
{
}

void Channel::Destroy()
{
	this->m_pChannel->stop();
	delete this->m_pChannel;
}

// Getters and setters
float Channel::GetVolume() const {
    return this->m_volume;
}

void Channel::SetVolume(float volume) {
    this->m_volume = volume;
    this->m_pChannel->setVolume(volume);
}

float Channel::GetPitch() const {
    return this->m_pitch;
}

void Channel::SetPitch(float pitch) {
    this->m_pitch = pitch;
    this->m_pChannel->setPitch(pitch);
}

float Channel::GetPan() const {
    return this->m_pan;
}

void Channel::SetPan(float pan) {
    this->m_pan = pan;
    this->m_pChannel->setPan(pan);
}

bool Channel::IsPlaying() const {
    return this->m_playing;
}

void Channel::SetPlaying(bool playing) {
    this->m_playing = playing;
    this->m_pChannel->setPaused(playing);
}

void Channel::StopChannel()
{
    this->m_playing = false;
    this->m_pChannel->stop();
}

FMOD::Channel* Channel::GetpChannel()
{
    return this->m_pChannel;
}