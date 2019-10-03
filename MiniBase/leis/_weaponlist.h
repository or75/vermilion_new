#pragma once

#include "../main.h"

#define WALL_PEN0 0
#define WALL_PEN1 1
#define WALL_PEN2 2

#define CLIPCAP_AK47 30
#define CLIPCAP_AUG 30
#define CLIPCAP_AWP 10
#define CLIPCAP_DEAGLE 7
#define CLIPCAP_ELITE 15
#define CLIPCAP_FIVESEVEN 20
#define CLIPCAP_G3SG1 20
#define CLIPCAP_GLOCK18 20
#define CLIPCAP_M249 100
#define CLIPCAP_M3 8
#define CLIPCAP_M4A1 30
#define CLIPCAP_MAC10 30
#define CLIPCAP_MP5 30
#define CLIPCAP_P228 13
#define CLIPCAP_P90 50
#define CLIPCAP_SCOUT 10
#define CLIPCAP_SG550 30
#define CLIPCAP_SG552 30
#define CLIPCAP_TMP 30
#define CLIPCAP_UMP45 25
#define CLIPCAP_USP 12
#define CLIPCAP_XM1014 7
#define CLIPCAP_GALIL 35
#define CLIPCAP_FAMAS 25

#define BULLET_NONE 0
#define BULLET_AK47 11
#define BULLET_AUG 12
#define BULLET_AWP 10
#define BULLET_DEAGLE 13
#define BULLET_ELITE 1
#define BULLET_FIVESEVEN 15
#define BULLET_G3SG1 11
#define BULLET_GLOCK18 1
#define BULLET_M249 12
#define BULLET_M4A1 12
#define BULLET_MAC10 9
#define BULLET_MP5 1
#define BULLET_P228 15
#define BULLET_P90 14
#define BULLET_SCOUT 11
#define BULLET_SG550 12
#define BULLET_SG552 12
#define BULLET_TMP 1
#define BULLET_UMP45 9
#define BULLET_USP 9
#define BULLET_M3 0
#define BULLET_XM1014 0
#define BULLET_GALIL 12
#define BULLET_FAMAS 12

#define USP_SILENCER (1 << 0)
#define GLOCK18_BURST (1 << 1)
#define M4A1_SILENCER (1 << 2)

#define MAX_WEAPON_LIST	29
#define MAX_WEAPON_NAME 32

struct weapon_list_s
{
	char name[MAX_WEAPON_NAME];
	int len;
	char command[MAX_WEAPON_NAME];
	int AmmoType;
	int Ammo2Type;
	int Max1;
	int Max2;
	int Slot;
	int SlotPos;
	int Id;
	int Flags;
	int ClipCap;	// Number of bullets the clip can hold
	int CAmmo;		// Current ammo in clip
	int XAmmo;		// Extra ammo
	bool Active;	// True if we have this weapon
	bool CurWeapon; // True if this is the current weapon
	int penetrate;
	weapon_data_t weapondata;
	float distance;
	int bullettype;
	float wallpierce1;
	float wallpierce2;
	int damage1;
	int damage2;
};

class cWeaponList
{
public:

	int OldWeaponBits;
	int WeaponListIndex;

	weapon_list_s* WeaponListData;
	
	cWeaponList()
	{
		WeaponListIndex = 0;
		OldWeaponBits = 0;
	}

	void SetWeaponData( weapon_list_s* weapon );

	weapon_list_s* GetCurWeapon();
	weapon_list_s* GetSecWeapon();

	bool IsReloading();
	bool IsCurWeaponSec();
	bool CanCurWeaponAttack();
	bool IsCurWeaponInBurst();
	bool IsCurWeaponSilenced();

	int GetCurWeaponId();
	//int GetCurWeaponAmmo();
	int GetWeaponIndexByID( int WeaponID );

	void WeaponListCurWeapon( int CurWeapon , int WeaponID , int Ammo );
	void WeaponListAmmoX( int ID , int Count );
	void WeaponListUpdate( int WeaponBits );

	void WeaponListAdd( char* weaponname , int ammo1type , int max1 , int ammo2type , int max2 , int slot , int slotpos , int id , int flags );
	void WeaponListClear();
};

extern cWeaponList g_WeaponList;