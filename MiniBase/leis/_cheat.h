#pragma once

#include "../main.h"

#define HACK_TYPE 2	// 0 - myac,1 - public,2 - vovan
#define INJT_TYPE 0	// 0 - default,1 - injector

class cCheat
{
public:

	int iBhopCount;
	bool bFastRun;
	bool bTrigger;
	bool bTriggerShot;
	bool bTriggerEnable;

	DWORD dwTriggerStatusTime;

	cCheat()
	{
		iBhopCount = 0;
		bFastRun = false;
		bTrigger = false;
		bTriggerShot = false;
		bTriggerEnable = false;
		dwTriggerStatusTime = 0;
	}

	bool CheckTriggerFOV( Vector dest , float* iScreen , float fFov );
	bool CheckTriggerFOV( Vector dest , float* iScreen , float fFovX , float fFovY );

	void TriggerShowStatus();
	void TriggerShowFov( int iIndex );

	void TriggerFunction( usercmd_s *cmd );
	void BhopFunction( usercmd_s *cmd );
	void KnifeBotFunction( usercmd_s *cmd );
	void FastRunFunction( usercmd_s *cmd );
};

extern cCheat g_Cheat;