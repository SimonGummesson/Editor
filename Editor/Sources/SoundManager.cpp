#include "../Headers/SoundManager.hpp"
#include <iostream>
#include <math.h>
SoundManager::SoundManager()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&soundSystem);      // Create the main system object.
	if (result != FMOD_OK)
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));

	result = soundSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
}

SoundManager::~SoundManager()
{
	for (auto c : soundList)
		delete c.second;
}

void SoundManager::setPlayerPosPointer(Vector3 * pointer)
{
	playerPosPointer = pointer;
}

void SoundManager::addSound(std::string name, std::string source, Vector3* origin, float dist, float volumefactor, soundSettings setting, bool ambient)
{
	Sound* sound = new Sound(name, origin, ambient ? 0.f : log(0.01f) / -dist, volumefactor);
	soundSystem->createSound(source.c_str(), setting, NULL, &sound->sound);
	soundList[name] = sound;
}

void SoundManager::playSound(std::string name)
{
	std::map<std::string, Sound*>::iterator it = soundList.find(name);

	if (soundList.end() != it) // if found the sound
	{
		soundSystem->playSound(it->second->sound, NULL, false, &it->second->channel);
		it->second->channel->setVolume(it->second->volumeFactor);
	}
}

void SoundManager::update(Matrix viewMatrix)
{
	for (auto c : soundList)
	{
		if (c.second->maxDist != 0.f)
		{
			// camera position is (0, 0, 0), hence there's no need to do a subraction to create avector from the camera to the sound source
			Vector3 cameraToSource = Vector3::Transform(*c.second->origin, viewMatrix);
			cameraToSource.Normalize();
			/*if (c.second->name == "countryRoads")
				std::cout << cameraToSource.Dot(Vector3(1.f, 0.f, 0.f)) << std::endl;*/

			// The forward vector of the camera can be used to determine the panning,
			// but one problem is that it won't be enough with just using the dot product between a vector from the camera to the sound source and the forward vector. Additional math is needed to determine the panning.
			// Using the right vector of the camera solves all of these problems and makes it possible to just get away with a simple dot product.
			//Since we're in view space the right of the camera is (1, 0, 0)
			c.second->channel->setPan(cameraToSource.Dot(Vector3(1.f, 0.f, 0.f)));
			c.second->channel->setVolume(c.second->volumeFactor * soundAttenuation(c.second->maxDist, Vector3(*c.second->origin - *playerPosPointer).Length()));
		}
	}
}

float SoundManager::soundAttenuation(float a, float dist)
{
	return exp(-a * dist);
}
