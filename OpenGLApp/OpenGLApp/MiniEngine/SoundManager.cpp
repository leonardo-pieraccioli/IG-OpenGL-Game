#include "SoundManager.h"

SoundManager& SoundManager::Instance() {
	static auto&& instance = SoundManager();
	return (instance);
}

void SoundManager::setup() {
	using namespace irrklang;
	this->SoundEngine = createIrrKlangDevice();
	return;
}

void SoundManager::playSound(const char* path, bool loop) {
	this->SoundEngine->play2D(path, loop);
}
