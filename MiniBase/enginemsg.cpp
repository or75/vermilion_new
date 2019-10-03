#include "main.h"

int* MSG_ReadCount = nullptr;
int* MSG_CurrentSize = nullptr;
int* MSG_BadRead = nullptr;
int MSG_SavedReadCount = 0;
sizebuf_t* MSG_Buffer = nullptr;

HL_MSG_ReadByte MSG_ReadByte = nullptr;
HL_MSG_ReadShort MSG_ReadShort = nullptr;
HL_MSG_ReadLong MSG_ReadLong = nullptr;
HL_MSG_ReadFloat MSG_ReadFloat = nullptr;
HL_MSG_ReadString MSG_ReadString = nullptr;
HL_MSG_ReadCoord MSG_ReadCoord = nullptr;
HL_MSG_ReadBitVec3Coord MSG_ReadBitVec3Coord = nullptr;
HL_MSG_ReadBits MSG_ReadBits = nullptr;
HL_MSG_StartBitReading MSG_StartBitReading = nullptr;
HL_MSG_EndBitReading MSG_EndBitReading = nullptr;

void MSG_SaveReadCount()
{
	MSG_SavedReadCount = *MSG_ReadCount;
}

void MSG_RestoreReadCount()
{
	*MSG_ReadCount = MSG_SavedReadCount;
}

pfnEngineMessage pSVC_sound;
pfnEngineMessage pSVC_SpawnStaticSound;

void SVC_Sound()
{
	MSG_SaveReadCount();

	int field_mask , volume;
	float attenuation;
	int channel , ent , sound_num;
	float origin[3] = { 0 };
	int pitch;

	MSG_StartBitReading( MSG_Buffer );

	field_mask = MSG_ReadBits( 9 );

	if ( field_mask & SND_FL_VOLUME )
		volume = MSG_ReadBits( 8 ) / 255.0;
	else
		volume = DEFAULT_SOUND_PACKET_VOLUME;

	if ( field_mask & SND_FL_ATTENUATION )
		attenuation = MSG_ReadBits( 8 ) / 64.0;

	channel = MSG_ReadBits( 3 );
	ent = MSG_ReadBits( 11 );

	if ( field_mask & SND_FL_LARGE_INDEX )
		sound_num = MSG_ReadBits( 16 );
	else
		sound_num = MSG_ReadBits( 8 );

	MSG_ReadBitVec3Coord( origin );

	if ( field_mask & SND_FL_PITCH )
		pitch = MSG_ReadBits( 8 );

	MSG_EndBitReading( MSG_Buffer );

	if ( !PathFree( origin ) )
	{
		g_Sound.AddSound( GetTickCount() , origin );
	}

	MSG_RestoreReadCount();
	pSVC_sound();
}

void SVC_SpawnStaticSound()
{
	MSG_SaveReadCount();

	float pos[3] = { 0 };

	int soundnum;
	float vol , attenuation;
	int ent , pitch , fFlags;

	MSG_StartBitReading( MSG_Buffer );

	pos[0] = MSG_ReadCoord();
	pos[1] = MSG_ReadCoord();
	pos[2] = MSG_ReadCoord();

	soundnum = MSG_ReadShort();
	vol = MSG_ReadByte() / 255.0;
	attenuation = MSG_ReadByte() / 64.0;
	ent = MSG_ReadShort();
	pitch = MSG_ReadByte();
	fFlags = MSG_ReadByte();

	if ( !PathFree( pos ) )
	{
		g_Sound.AddSound( GetTickCount() , pos );
	}

	MSG_EndBitReading( MSG_Buffer );

	MSG_RestoreReadCount();
	pSVC_SpawnStaticSound();
}