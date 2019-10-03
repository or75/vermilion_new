#pragma once

#include "../main.h"

#define INI_PATH_KEY	0x29E1E

class cCvar
{
public:

	PCHAR IniPatch;
	PCHAR IniValue;
	PCHAR IniSection;

	bool bInitPatch;

	byte wpn;

	cCvar()
	{
		IniPatch = nullptr;
		IniValue = nullptr;
		IniSection = nullptr;
		bInitPatch = false;
		wpn = 0;
	}

	int aim_Deathmatch;
	int aim_Aimthrough;
	int aim_Silent;
	int aim_Type;

	struct weapon_buffer_s
	{
		int aim_Active;	
		int aim_Smooth;
		int aim_Fov;
		int aim_FovType;
		int aim_FovDist;
		int aim_Mode;
		int aim_Aimspot;

		int aim_BulletControl;

		int aim_BulletOneDistance;
		int aim_BulletOneClampType;
		int aim_BulletOneAimSpot;
		int aim_BulletOneSmooth;
		int aim_BulletOneFov;

		int aim_BulletTwoDistance;
		int aim_BulletTwoClampType;
		int aim_BulletTwoAimSpot;
		int aim_BulletTwoSmooth;
		int aim_BulletTwoFov;
		
		int aim_TriggerFov;
		int aim_TriggerHeadOnly;
		int aim_ShotDelay;
		int aim_KillDelay;
		int aim_Prediction;
		int aim_Distance;
	} weapon[31];

	int aim_SaveSettings;
	int aim_LoadSettings;

	int trigger_Enable;
	int trigger_Deathmatch;
	int trigger_Aimthrough;
	int trigger_DistanceMin;
	int trigger_DistanceMax;
	int trigger_Key;
	int trigger_KeyMode;
	int trigger_OnlyZoom;

	int knf_Active;
	int knf_Aim;
	int knf_AimFov;
	int knf_Team;
	int knf_Attack;
	int knf_DistAttack;
	int knf_DistAttack2;
		
	int esp_Style;
	int esp_Size;
	int esp_Visible;
	int esp_Name;
	int esp_Weapon;
	int esp_Sound;
	int esp_SoundType;
	int esp_Barrel;
	int esp_WorldWeapons;
	int esp_WorldSprite;
	int esp_DrawFont;
	int esp_DrawAimspot;
	int esp_DrawFov;
	int esp_DrawTrigger;

	int misc_Bhop;
	int misc_Punch;
	int misc_Lambert;
	int misc_FastZoom;
	int misc_AntiScreen;
	int misc_FastRun;
	int misc_RemoveFlash;
	int misc_FlashMessage;

	int key_ToggleMenu;
	int key_TogglePanic;
	int key_NavEnter;
	int key_NavLeave;
	int key_NavUp;
	int key_NavDown;
	int key_NavLeft;
	int key_NavRight;
	
	void InitPatch();

	void LoadCvarFromIni();

	byte GetSectionFromIndex( int iIndex );

	void LoadCvarWeapon();
	void SaveCvarWeapon();
	
	void DisableAll();

	int ReadInt( char* szSection , char* szKey , int iDefaultValue , char* Patch );
	void SaveInt( char* szSection , char* szKey , int iValue , char* Patch );

	int ReadKey( char* szSection , char* szKey , char* cDefaultValue , char* Patch , bool window );
};

extern cCvar cvar;