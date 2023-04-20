#pragma once
#include <fmod.h>


#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

struct CSound
{
	FMOD_SYSTEM* g_sound_system;
	FMOD_SOUND* m_sound;
	FMOD_CHANNEL* m_channel;

	float m_volume;
	FMOD_BOOL m_bool;

};





static int Init();
static int Release();

int play();
int pause();
int resume();
int stop();
int volumeUp();
int volumeDown();

int soundUpdate();

