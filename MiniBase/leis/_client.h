#pragma once

#include "../main.h"

#define MAX_ENTITY		1024
#define MAX_SOUNDS		384

typedef void( *Snapshot_t )( );
typedef void( *Screenshot_t )( );

struct entity_s
{
	char name[64];
	int iImportant;
	Vector vOrigin;
	BYTE type;			// 1 - weapon , 2 - sprite
};

struct sound_s
{
	DWORD dwTime;
	Vector vOrigin;
};

struct noflash_s
{
	float Flashed;
	float FadeEnd;
	screenfade_t pScreenFade;
};

class cEntity
{
public:

	cEntity() { EntityIndex = 0; ClearEntity(); }

	int EntityIndex;
	entity_s entity[MAX_ENTITY];

	void ClearEntity();
	void AddEntity( char* name , int iImportant , Vector vOrigin , BYTE type );
};

class cSound
{
public:

	int SoundIndex;
	sound_s sound[MAX_SOUNDS];

	void ClearSound();
	void AddSound( DWORD dwTime , Vector vOrigin );
	void DrawSoundEsp();
};

class cLeis
{
public:

	byte ScreenTimer;

	static void AntiSnapshot();
	static void AntiScreenshot();

	void AntiScreen();
	void InitHack();

	void HUD_Redraw();
	void HUD_Key_Event( int keynum );
	void HUD_PlayerMove( struct playermove_s *ppmove );
	void V_CalcRefdef( struct ref_params_s *pparams );
	void StudioEntityLight( struct alight_s *plight );
	void HUD_PostRunCmd( struct local_state_s *to , struct usercmd_s *cmd , int runfuncs , double time , unsigned int random_seed );
	int HUD_AddEntity( int type , struct cl_entity_s *ent, const char *modelname );
	void CL_CreateMove( float frametime , usercmd_s *cmd );
};

extern cLeis g_Leis;
extern cEntity g_Entity;
extern cSound g_Sound;
extern noflash_s noflash;

extern bool bSnapshot;
extern bool bScreenshot;

#define WEAPONLIST_P228			1
#define	WEAPONLIST_UNKNOWN1		2
#define	WEAPONLIST_SCOUT		3
#define	WEAPONLIST_HEGRENADE	4
#define	WEAPONLIST_XM1014		5
#define	WEAPONLIST_C4			6
#define	WEAPONLIST_MAC10		7
#define	WEAPONLIST_AUG			8
#define	WEAPONLIST_SMOKEGRENADE	9
#define	WEAPONLIST_ELITE		10
#define	WEAPONLIST_FIVESEVEN	11
#define	WEAPONLIST_UMP45		12
#define	WEAPONLIST_SG550		13
#define	WEAPONLIST_GALIL		14
#define	WEAPONLIST_FAMAS		15
#define	WEAPONLIST_USP			16
#define	WEAPONLIST_GLOCK18		17
#define	WEAPONLIST_AWP			18
#define	WEAPONLIST_MP5			19
#define	WEAPONLIST_M249			20
#define	WEAPONLIST_M3			21
#define	WEAPONLIST_M4A1			22
#define	WEAPONLIST_TMP			23
#define	WEAPONLIST_G3SG1		24
#define	WEAPONLIST_FLASHBANG	25
#define	WEAPONLIST_DEAGLE		26
#define	WEAPONLIST_SG552		27
#define	WEAPONLIST_AK47			28
#define	WEAPONLIST_KNIFE		29
#define	WEAPONLIST_P90			30