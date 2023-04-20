#include "input.h"
#include "main.h"
#include "sound.h"



	FMOD_SYSTEM* g_sound_system;
	FMOD_SOUND* m_sound;
	FMOD_CHANNEL* m_channel;

	float m_volume;
	FMOD_BOOL m_bool;


int Init() {
	FMOD_System_Create(&g_sound_system);
	FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, NULL);

	return 0;
}

int Release() {
	FMOD_System_Close(g_sound_system);
	FMOD_System_Release(g_sound_system);

	return 0;
}


int play() {
	FMOD_System_PlaySound(g_sound_system, m_sound, NULL, false, &m_channel);

	return 0;
}

int pause() {
	FMOD_Channel_SetPaused(m_channel, true);

	return 0;
}

int resume() {
	FMOD_Channel_SetPaused(m_channel, false);

	return 0;
}

int stop() {
	FMOD_Channel_Stop(m_channel);

	return 0;
}

int soundUpdate() {
	FMOD_Channel_IsPlaying(m_channel, &m_bool);

	if (m_bool) {
		FMOD_System_Update(g_sound_system);
	}

	return 0;
}



