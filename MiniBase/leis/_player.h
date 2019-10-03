#pragma once

#include "../main.h"

#include "_weaponlist.h"

struct spread_info
{
	unsigned int random_seed;

	float gtime;
	float prevtime;
	float brokentime;
	float spreadvar;
	float recoiltime;

	bool firing;

	int WeaponState;
	int prcflags;
	int recoil;
};

struct local_s
{
	int iTeam;
	int iIndex;
	int iFOV;
	int iClip;

	bool bAlive;
	bool bBadWeapon;
	bool bPistolWeapon;
	bool bShotGunWeapon;
	bool bSniperWeapon;

	float flZspeed;
	float flXYspeed;

	float fFrametime;

	float m_flNextPrimaryAttack;
	float m_flNextSecondaryAttack;

	int iWeaponID;
	int iInReload;

	int iFlags;
	int iMoveType;
	int iWaterLevel;

	Vector vForward;
	Vector vViewOrg;
	Vector vPunchangle;
	Vector vVelocity;
	Vector vNoSpreadAng;
	Vector vCmdViewAngles;
	Vector vDrawSpr;	

	cl_entity_s* Entity;

	spread_info spread;
};

struct player_s
{
	int iTeam;

	bool bAlive;
	bool bUpdated;
	bool bVisible;

	Vector vAngle;
	Vector vAimOrigin;
	Vector vVelocity;

	Vector vBone[53];
	Vector vHitbox[21];

	bool isFov;
	bool bEnableAim;
	bool bAttackTime;
	bool bShotDelay;

	DWORD dwStartTime;

	float fDistance;
	float fFrametime;
	float fFov;
	
	cl_entity_s *Entity;

	hud_player_info_t Info;
};

class cPlayers
{
public:

	cPlayers()
	{
		ClearPlayers();
	}

	bool IsAliveEntity( cl_entity_s *Entity );
	bool isValidEntity( cl_entity_s *Entity );

	void UpdateLocalEntity();
	void UpdatePlayerInfo();
	void ClearPlayers();
};

extern local_s g_Local;
extern player_s g_Player[33];
extern cPlayers g_Players;