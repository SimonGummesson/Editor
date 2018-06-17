#pragma once
#include "fmod_errors.h"
#include "fmod.hpp"
#include <map>
#include <string>
#include "SimpleMath.h"
#include <math.h>
using namespace DirectX::SimpleMath;

enum soundSettings
{
	Loop = FMOD_LOOP_NORMAL,
	NonLoop = FMOD_DEFAULT
};

struct Sound {
	Sound()
	{

	}

	Sound(std::string name, Vector3* origin, float dist, float volumeFactor)
	{
		this->name = name;
		sound = nullptr;
		channel = nullptr;

		this->origin = origin;
		maxDist = dist;
		this->volumeFactor = volumeFactor;
	}
	~Sound()
	{
		sound->release();
	}
	FMOD::Sound* sound;
	FMOD::Channel* channel;

	Vector3* origin;
	float maxDist;
	float volumeFactor;
	std::string name;
};

class  SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void setPlayerPosPointer(Vector3* pointer);
	void addSound(std::string name, std::string source, Vector3* origin, float dist, float volumefactor, soundSettings setting, bool ambient = false);
	void playSound(std::string name);
	void stopSound(std::string name);
	void update(Matrix viewMatrix);
private:
	float soundAttenuation(float a, float dist);
	FMOD::System *soundSystem;
	std::map<std::string, Sound*> soundList;
	Vector3* playerPosPointer;
};