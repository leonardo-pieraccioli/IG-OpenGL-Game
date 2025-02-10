#pragma once

#include <irrKlang.h>

class SoundManager
{
public:
	irrklang::ISoundEngine* SoundEngine;

	static SoundManager& Instance();
	
	void setup();
	void playSound(const char* path, bool loop); //true if loop, false otherwise
	irrklang::ISound* playSoundWithRetP(const char* path, bool loop);
	void stopSound(irrklang::ISound* sound);
	void stopAllSounds();
private:

};

