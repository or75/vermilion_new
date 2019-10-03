#pragma once

#include "../main.h"

class cAimbot
{
public:

	int iNeed;
	int iTargetID;
	int iFovTargetID;
	int iPreTargetID;
	int iCurrentClamp;

	int iMaxBones;
	int iMaxHitbox;

	int iClipState;
	int iClipCount;

	bool bClamp;
	bool bAimEnable;
	bool bShotDelay;
	bool bRecoilTime;
	bool bRecoilnFov;

	DWORD dwAimKillTime;
	DWORD dwAimShotDelay;

	float fRecoilX;
	float fRecoilY;

	cAimbot()
	{
		iNeed = 0;
		iTargetID = 0;
		iFovTargetID = 0;
		iPreTargetID = 0;
		iCurrentClamp = 0;
		iClipState = 0;
		iClipCount = 0;
		bClamp = false;
		bAimEnable = true;
		bShotDelay = false;
		bRecoilTime = false;
		bRecoilnFov = false;
		dwAimKillTime = 0;
		dwAimShotDelay = 0;
		fRecoilX = 0.f;
		fRecoilY = 0.f;
	}

	bool bVisible( int iIndex );
	bool bCheckTeam( int iIndex );

	void UpdateTargerId();
	void UpdateAimInfo( int iIndex );
	void UpdatePlayerInfo();

	void DrawFov( int iIndex );
	void DrawFovCrosshair();

	float GetRecoilVar();
	
	void SmoothAimAngles( float *MyViewAngles , float *AimAngles , float *OutAngles , float Smoothing );
	void ApplySilentAngles( float *angles , usercmd_s *cmd );

	void GetBoneOrigin( cl_entity_s *Entity );
	void GetHitboxOrigin( cl_entity_s *Entity );

	Vector vCalcOriginOffset( int iIndex );
	Vector vCalcBoneOffset( int iIndex );
	Vector vCalcHitboxOffset( int iIndex );

	void CL_WeaponControl();
	void CL_CreateMove( usercmd_s *cmd );
};

extern cAimbot g_Aimbot;