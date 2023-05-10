#include "fmod.h" // 소리 출력 
#include "menu.h"

FMOD_SYSTEM* fsystem; 
FMOD_SOUND* sound[]; 
FMOD_RESULT result;  
FMOD_CHANNEL* channel;
unsigned int version;

// 오디오 시스템을 만듭니다.
void AudioSystem()
{
	result = FMOD_System_Create(&fsystem, FMOD_VERSION);
	if (result != FMOD_OK) return -1;

	result = FMOD_System_Init(fsystem, 32, FMOD_INIT_NORMAL, NULL);
	if (result != FMOD_OK) return -1;
}
	//result = FMOD_System_GetVersion(fsystem, &version);


FMOD_SOUND* fanclub;
FMOD_SOUND* sound2;
FMOD_CHANNEL* channel;
FMOD_CHANNEL* channel2;

// 오디오 파일을 읽어옵니다.
void AudioCreate(const char* filename, FMOD_SOUND** sound)
{
	result = FMOD_System_CreateSound(fsystem, filename, FMOD_LOOP_OFF, 0, sound);
	if (result != FMOD_OK) return -1;
}

// 오디오를 재생합니다.
void AudioPlay(FMOD_SOUND* sound, FMOD_CHANNEL** channel)
{
	result = FMOD_System_PlaySound(fsystem, sound, 0, 0, channel);
	if (result != FMOD_OK) return -1;
}