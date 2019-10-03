#include "_cheat.h"

cCheat g_Cheat;

#pragma warning(disable:4244)

bool cCheat::CheckTriggerFOV( Vector dest , float* iScreen , float fFov )
{
	float fScrPos[2] =
	{
		myfabs( dest.x - iScreen[0] ),
		myfabs( dest.y - iScreen[1] )
	};

	return ( fScrPos[0] <= fFov && fScrPos[1] <= fFov );
}

bool cCheat::CheckTriggerFOV( Vector dest , float * iScreen , float fFovX , float fFovY )
{
	float fScrPos[2] =
	{
		myfabs( dest.x - iScreen[0] ),
		myfabs( dest.y - iScreen[1] )
	};

	return ( fScrPos[0] <= fFovX && fScrPos[1] <= fFovY );
}

void cCheat::TriggerShowStatus()
{
#if (HACK_TYPE == 1 || HACK_TYPE == 2)

	char TRIGGER_ENABLE[] = { 'T','R','I','G','G','E','R','B','O','T',':',' ','E','N','A','B','L','E','D',0 };
	char TRIGGER_DISABLE[] = { 'T','R','I','G','G','E','R','B','O','T',':',' ','D','I','S','A','B','L','E','D',0 };

	if ( cvar.trigger_KeyMode >= 2 )
	{
		if ( cvar.trigger_Enable && GetTickCount() - dwTriggerStatusTime < 1000 )
		{
			int iWidth , iHeight;
			g_pEngine->pfnDrawConsoleStringLen( TRIGGER_ENABLE , &iWidth , &iHeight );

			if ( bTrigger )
				Verdana1.PrintFont( g_Screen.iWidth - iWidth - 10 , 5 , 205 , 0 , 0 , 255 , FL_CENTER , TRIGGER_ENABLE );
			else
				Verdana1.PrintFont( g_Screen.iWidth - iWidth - 10 , 5 , 205 , 0 , 0 , 255 , FL_CENTER , TRIGGER_DISABLE );
		}
	}
	else if ( bTrigger )
		Verdana1.PrintFont( g_Screen.iWidth - 170 , 5 , 205 , 0 , 0 , 255 , FL_CENTER , TRIGGER_ENABLE );

#endif
}

void cCheat::TriggerShowFov( int iIndex )
{
	if ( cvar.weapon[cvar.wpn].aim_TriggerFov )
	{
		float SRC_TriggerHEAD[2] = { 0 };
		float SRC_TriggerCHEST[2] = { 0 };
		float VecScreen[2] = { 0 };

		if ( CalcScreen( g_Player[iIndex].Entity->origin , VecScreen ) )
		{
			if ( CalcScreen( g_Player[iIndex].vHitbox[11] , SRC_TriggerHEAD ) &&
				 CalcScreen( g_Player[iIndex].vHitbox[7] , SRC_TriggerCHEST ) )
			{
				int x = (int)SRC_TriggerHEAD[0] , y = (int)SRC_TriggerHEAD[1];
				int x1 = (int)SRC_TriggerCHEST[0] , y1 = (int)SRC_TriggerCHEST[1];

				float tfov = ( ( POW( cvar.weapon[cvar.wpn].aim_TriggerFov ) * 90 ) / ( g_Player[iIndex].fDistance * g_Local.iFOV ) );

				if ( cvar.weapon[cvar.wpn].aim_TriggerHeadOnly )
					g_Drawing.DrawBox( x - tfov + 1 , y - tfov + 1 , tfov * 2 + 1 , tfov * 2 + 1 , 1 , 30 , 255 , 30 , 255 ); //HEAD
				else
				{
					float tfovX = ( ( POW( cvar.weapon[cvar.wpn].aim_TriggerFov + 20 ) * 90 ) / ( g_Player[iIndex].fDistance * g_Local.iFOV ) );
					float tfovY = ( ( POW( ( cvar.weapon[cvar.wpn].aim_TriggerFov + 20 ) * 1.25 ) * 90 ) / ( g_Player[iIndex].fDistance * g_Local.iFOV ) );
					g_Drawing.DrawBox( x - tfov + 1 , y - tfov + 1 , tfov * 2 + 1 , tfov * 2 + 1 , 1 , 255 , 30 , 255 , 255 ); //HEAD						
					g_Drawing.DrawBox( x1 - tfovX + 1 , y1 - tfovY + 1 , tfovX * 2 + 1 , tfovY * 2 + 1 , 1 , 255 , 30 , 255 , 255 ); //CHEST
				}
			}
		}
	}
}

void cCheat::TriggerFunction( usercmd_s *cmd )
{
	if ( cvar.trigger_KeyMode >= 2 )
	{
		if ( GetAsyncKeyState( cvar.trigger_Key ) & 1 )
		{
			bTrigger = !bTrigger;
			dwTriggerStatusTime = GetTickCount();
		}
	}
	else
	{
		if ( GetAsyncKeyState( cvar.trigger_Key ) )
			bTrigger = true;
		else
			bTrigger = false;
	}

	bTriggerEnable = false;
	bTriggerShot = false;

	float SRC_TriggerHEAD[2] = { 0 };
	float SRC_TriggerCHEST[2] = { 0 };
	float VecScreen[2] = { 0 };

	for ( int i = 1; i <= 32; i++ )
	{
		if ( g_Player[i].Entity && g_Player[i].bAlive && g_Player[i].bUpdated && 
			 CalcScreen( g_Player[i].vHitbox[11] , SRC_TriggerHEAD ) && bTrigger )
		{
			float fFov = ( ( POW( cvar.weapon[cvar.wpn].aim_TriggerFov ) * 90.0f ) / ( g_Player[i].fDistance * g_Local.iFOV ) );

			if ( cvar.weapon[cvar.wpn].aim_TriggerHeadOnly )
			{
				if ( CheckTriggerFOV( g_Local.vDrawSpr , SRC_TriggerHEAD , fFov ) )
				{
					bTriggerEnable = true;
				}
			}
			else
			{
				float tfovX = ( ( POW( cvar.weapon[cvar.wpn].aim_TriggerFov + 20.0f ) * 90.0f ) / ( g_Player[i].fDistance * g_Local.iFOV ) );
				float tfovY = ( ( POW( ( cvar.weapon[cvar.wpn].aim_TriggerFov + 20.0f ) * 1.25f ) * 90.0f ) / ( g_Player[i].fDistance * g_Local.iFOV ) );

				if ( CalcScreen( g_Player[i].vHitbox[7] , SRC_TriggerCHEST ) )
				{
					if ( CheckTriggerFOV( g_Local.vDrawSpr , SRC_TriggerCHEST , tfovX , tfovY ) 
						 || CheckTriggerFOV( g_Local.vDrawSpr , SRC_TriggerHEAD , fFov ) )
					{
						bTriggerEnable = true;
					}
				}
			}

			if ( g_Player[i].fDistance >= cvar.trigger_DistanceMin &&
				 g_Player[i].fDistance <= cvar.trigger_DistanceMax &&
				 ( cvar.trigger_Aimthrough || g_Player[i].bVisible ) &&
				 ( cvar.trigger_Deathmatch || g_Player[i].iTeam != g_Local.iTeam ) && bTriggerEnable )
			{
				if ( !g_Local.bSniperWeapon )
				{
					bTriggerShot = true;
				}
				else if ( g_Local.bSniperWeapon && g_Local.iFOV != 90 && cvar.trigger_OnlyZoom )
				{
					bTriggerShot = true;
				}
				else if ( g_Local.bSniperWeapon && !cvar.trigger_OnlyZoom )
				{
					bTriggerShot = true;
				}

				if ( bTriggerShot )
				{
					if ( g_Local.m_flNextPrimaryAttack >= 0.0f )
						cmd->buttons &= ~IN_ATTACK;
					else
						cmd->buttons |= IN_ATTACK;
				}
			}
		}
	}
}

void cCheat::KnifeBotFunction( usercmd_s *cmd )
{
	float distance = 0;

	if ( g_Local.iWeaponID == WEAPONLIST_KNIFE && g_Local.bAlive )
	{
		for ( int i = 1; i <= 32; i++ )
		{
			if ( !g_Player[i].bUpdated )continue;
			if ( !g_Player[i].bAlive )continue;

			if ( cvar.knf_Team == 0 && g_Player[i].iTeam == g_Local.iTeam ) { continue; }

			switch ( cvar.knf_Attack )
			{
				case 1:
					distance = (float)cvar.knf_DistAttack;
					break;
				case 2:
					distance = (float)cvar.knf_DistAttack2;
					break;
			}

			if ( g_Player[i].Entity )
			{
				Vector vAttack = g_Player[i].Entity->origin - g_Local.vViewOrg;

				if ( VectorDistance( g_Local.Entity->origin , g_Player[i].Entity->origin ) <= distance )
				{
					float fov = AngleBetvenVectors( g_Local.vForward , vAttack );

					if ( (int)fov <= cvar.knf_AimFov )
					{
						if ( cvar.knf_Aim )
						{
							float attackva[3] = { };
							VectorAngles( vAttack , attackva );

							cmd->viewangles[0] = attackva[0];
							cmd->viewangles[1] = attackva[1];
						}

						switch ( cvar.knf_Attack )
						{
							case 1:
								cmd->buttons |= IN_ATTACK;
								break;
								return;
							case 2:
								cmd->buttons |= IN_ATTACK2;
								break;
								return;
						}
					}
				}
			}
		}
	}
}

void cCheat::FastRunFunction( usercmd_s *cmd )
{
	if ( g_Local.flXYspeed && g_Local.flZspeed == 0 && g_Local.iFlags&FL_ONGROUND && g_Local.iWeaponID == WEAPONLIST_KNIFE )
	{
		if ( ( cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVELEFT ) || ( cmd->buttons&IN_BACK && cmd->buttons&IN_MOVERIGHT ) )
		{
			if ( bFastRun ) { bFastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove -= 89.6f; }
			else { bFastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove += 89.6f; }
		}
		else if ( ( cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVERIGHT ) || ( cmd->buttons&IN_BACK && cmd->buttons&IN_MOVELEFT ) )
		{
			if ( bFastRun ) { bFastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove += 89.6f; }
			else { bFastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove -= 89.6f; }
		}
		else if ( cmd->buttons&IN_FORWARD || cmd->buttons&IN_BACK )
		{
			if ( bFastRun ) { bFastRun = false; cmd->sidemove -= 126.6f; }
			else { bFastRun = true;  cmd->sidemove += 126.6f; }
		}
		else if ( cmd->buttons&IN_MOVELEFT || cmd->buttons&IN_MOVERIGHT )
		{
			if ( bFastRun ) { bFastRun = false; cmd->forwardmove -= 126.6f; }
			else { bFastRun = true;  cmd->forwardmove += 126.6f; }
		}
	}
}

void cCheat::BhopFunction( usercmd_s *cmd )
{
	if ( cmd->buttons & IN_JUMP )
	{
		iBhopCount++;

		bool bGound = g_Local.iMoveType == FL_ONGROUND;

		if ( ( ( iBhopCount % 2 ) == 0 ) || bGound )
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}
	else iBhopCount = 0;
}