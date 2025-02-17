#include "SoundManager.h"

SoundManager& SoundManager::Instance() {
	static auto&& instance = SoundManager();
	return (instance);
}

void SoundManager::setup() {
	using namespace irrklang;
	this->SoundEngine = createIrrKlangDevice();
	this->SoundEngine->setSoundVolume(0.1f);
	return;
}

void SoundManager::playSound(const char* path, bool loop) {
	this->SoundEngine->play2D(path, loop);
}

irrklang::ISound* SoundManager::playSoundWithRetP(const char* path, bool loop)
{
	return this->SoundEngine->play2D(path, loop, false, true, irrklang::ESM_AUTO_DETECT, false);
}

void SoundManager::stopSound(irrklang::ISound* sound)
{
	if (sound) {
		sound->drop();
	}
}

void SoundManager::stopAllSounds()
{
	this->SoundEngine->stopAllSounds();
}

void SoundManager::changePitch(irrklang::ISound* sound, float pitch)
{
	if (sound) {
		sound->setPlaybackSpeed(pitch);
	}
}
