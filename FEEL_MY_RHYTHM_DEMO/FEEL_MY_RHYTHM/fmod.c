#include "fmod.h" // �Ҹ� ��� 
#include "menu.h"

FMOD_SYSTEM* fsystem; 
FMOD_SOUND* sound[]; 
FMOD_RESULT result;  
FMOD_CHANNEL* channel;
unsigned int version;

// ����� �ý����� ����ϴ�.
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

// ����� ������ �о�ɴϴ�.
void AudioCreate(const char* filename, FMOD_SOUND** sound)
{
	result = FMOD_System_CreateSound(fsystem, filename, FMOD_LOOP_OFF, 0, sound);
	if (result != FMOD_OK) return -1;
}

// ������� ����մϴ�.
void AudioPlay(FMOD_SOUND* sound, FMOD_CHANNEL** channel)
{
	result = FMOD_System_PlaySound(fsystem, sound, 0, 0, channel);
	if (result != FMOD_OK) return -1;
}