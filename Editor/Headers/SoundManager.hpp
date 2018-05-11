#pragma once
#include "fmod_errors.h"
#include "fmod.hpp"
#include <map>
#include <string>
#include "SimpleMath.h"

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

	Sound(Vector3* origin, float dist)
	{
		sound = nullptr;
		channel = nullptr;

		this->origin = origin;
		maxDist = dist;
	}
	~Sound()
	{
		sound->release();
	}
	FMOD::Sound* sound;
	FMOD::Channel* channel;

	Vector3* origin;
	float maxDist;
};

class  SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void setPlayerPosPointer(Vector3* pointer);
	void addSound(std::string name, std::string source, Vector3* origin, float dist, soundSettings setting);
	void playSound(std::string name);
	void update();
private:
	float soundAttenuation(float dist);
	FMOD::System *soundSystem;
	std::map<std::string, Sound*> soundList;
	Vector3* playerPosPointer;
};