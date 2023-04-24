#include <fmod.h>
#include "menu.h"
#pragma once
FMOD_SYSTEM* fsystem;
FMOD_SOUND* sound[];
FMOD_RESULT result;
FMOD_CHANNEL* channel;
unsigned int version;

FMOD_SOUND* fanclub;
FMOD_SOUND* sound2;
FMOD_CHANNEL* channel;
FMOD_CHANNEL* channel2;

void AudioSystem();

void AudioCreate(const char*, FMOD_SOUND**);

void AudioPlay(FMOD_SOUND* sound, FMOD_CHANNEL** channel);

//void AudioSong(FMOD_CHANNEL* channel);