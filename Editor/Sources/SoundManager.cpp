#include "../Headers/SoundManager.hpp"

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

void SoundManager::addSound(std::string name, std::string source, Vector3* origin, float dist, soundSettings setting)
{
	Sound* sound = new Sound(origin, dist);
	soundSystem->createSound("Resources/whitenoise.wav", setting, NULL, &sound->sound);
	soundList[name] = sound;
}

void SoundManager::playSound(std::string name)
{
	std::map<std::string, Sound*>::iterator it = soundList.find(name);

	if (soundList.end() != it) // if found the sound
		soundSystem->playSound(it->second->sound, NULL, false, &it->second->channel);
}

void SoundManager::update()
{
	for (auto c : soundList)
		c.second->channel->setVolume(soundAttenuation(Vector3(*c.second->origin - *playerPosPointer).Length()));
}

float SoundManager::soundAttenuation(float dist)
{
	return exp(-0.1f * dist);
}
