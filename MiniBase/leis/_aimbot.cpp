#include "_aimbot.h"

cAimbot g_Aimbot;

#pragma warning(disable:4244)

bool cAimbot::bVisible( int iIndex )
{
	if ( cvar.aim_Aimthrough == 0 && !g_Player[iIndex].bVisible )
		return false;

	return true;
}

bool cAimbot::bCheckTeam( int iIndex )
{
	if ( cvar.aim_Deathmatch == 0 && g_Player[iIndex].iTeam == g_Local.iTeam )
		return false;

	return true;
}

void cAimbot::UpdateTargerId()
{
	float max_player_dist = 9999999.9f;
	float max_screen_dist = 9999999.9f;

	float center_screen[2];

	center_screen[0] = g_Screen.iWidth / 2;
	center_screen[1] = g_Screen.iHeight / 2;

	for ( int i = 1; i <= 32; i++ )
	{
		if ( !bCheckTeam( i ) || !bVisible( i ) || !g_Player[i].bUpdated || !g_Player[i].bAlive || g_Local.iIndex == i )
			continue;

		if ( cvar.weapon[cvar.wpn].aim_Distance <= 0 )
		{
			float SCR_Entity[2] = { 0 };
			float SCR_Aim[2] = { 0 };

			if ( g_Player[i].Entity )
			{
				if ( CalcScreen( g_Player[i].Entity->origin , SCR_Entity ) && 
					 CalcScreen( g_Player[i].vAimOrigin , SCR_Aim ) )
				{
					float screen_dist = CalcDistPlayerScreen( center_screen , SCR_Aim );

					g_Player[i].fFov = screen_dist;

					if ( screen_dist < max_screen_dist )
					{
						max_screen_dist = screen_dist;
						iTargetID = i;
						iFovTargetID = i;
					}
				}
			}
		}
		else if ( cvar.weapon[cvar.wpn].aim_Distance >= 1 )
		{
			if ( g_Player[i].fDistance < max_player_dist )
			{
				max_player_dist = g_Player[i].fDistance;
				iTargetID = i;
				iFovTargetID = i;
			}
		}
	}
}

void cAimbot::UpdateAimInfo( int iIndex )
{	
	if ( cvar.weapon[cvar.wpn].aim_Mode <= 0 ) g_Player[iIndex].vAimOrigin = g_Aimbot.vCalcOriginOffset( iIndex );
	if ( cvar.weapon[cvar.wpn].aim_Mode == 1 ) g_Player[iIndex].vAimOrigin = g_Aimbot.vCalcBoneOffset( iIndex );
	if ( cvar.weapon[cvar.wpn].aim_Mode > 1 ) g_Player[iIndex].vAimOrigin = g_Aimbot.vCalcHitboxOffset( iIndex );

	float SCR_Entity[2] = { 0 };
	float SCR_Aim[2] = { 0 };

	if ( g_Player[iIndex].Entity )
	{
		if ( CalcScreen( g_Player[iIndex].Entity->origin , SCR_Entity ) &&
			 CalcScreen( g_Player[iIndex].vAimOrigin , SCR_Aim ) )
		{
			int	iFovX = ( g_Screen.iWidth / 2 ) - (int)SCR_Aim[0];
			int	iFovY = ( g_Screen.iHeight / 2 ) - (int)SCR_Aim[1];

			int iFov = 0;
			int iFovVal = 0;

			if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
			{
				if ( g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
					iFovVal = cvar.weapon[cvar.wpn].aim_BulletOneFov;
				else if ( g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
					iFovVal = cvar.weapon[cvar.wpn].aim_BulletTwoFov;
				else
					iFovVal = cvar.weapon[cvar.wpn].aim_Fov;
			}
			else
				iFovVal = cvar.weapon[cvar.wpn].aim_Fov;

			iFovVal = iFovVal * ( 90 / g_Local.iFOV );

			if ( cvar.weapon[cvar.wpn].aim_FovType == 1 )
				iFov = iFovVal * g_Player[iIndex].fDistance / cvar.weapon[cvar.wpn].aim_FovDist;
			else if ( cvar.weapon[cvar.wpn].aim_FovType > 1 )
				iFov = iFovVal * cvar.weapon[cvar.wpn].aim_FovDist / g_Player[iIndex].fDistance;
			else
				iFov = iFovVal;

			if ( iFovX < iFov && iFovX > -iFov && iFovY < iFov && iFovY > -iFov )
				g_Player[iIndex].isFov = true;
			else
				g_Player[iIndex].isFov = false;
		}
		else
			g_Player[iIndex].isFov = false;
	}
}

void cAimbot::UpdatePlayerInfo()
{
	iPreTargetID = iTargetID;

	if ( cvar.weapon[cvar.wpn].aim_KillDelay <= 0 || ( ( dwAimKillTime + cvar.weapon[cvar.wpn].aim_KillDelay ) <= GetTickCount() && !bAimEnable ) )
	{
		bAimEnable = true;
		dwAimKillTime = 0;
	}

	if ( iTargetID != 0 && bAimEnable )
	{
		if ( !g_Player[iTargetID].bAlive && bAimEnable && cvar.weapon[cvar.wpn].aim_KillDelay )
		{
			if ( !dwAimKillTime )
			{
				dwAimKillTime = GetTickCount();
				bAimEnable = false;
			}
		}

		UpdateTargerId();
	}
	else
	{
		UpdateTargerId();
	}
}

void cAimbot::DrawFov( int iIndex )
{
	if ( cvar.esp_DrawAimspot || cvar.esp_DrawFov )
	{
		float SCR_Entity[2] = { 0 };
		float SCR_Aim[2] = { 0 };

		if ( bCheckTeam( iIndex ) )
		{
			if ( g_Player[iIndex].Entity )
			{
				if ( CalcScreen( g_Player[iIndex].Entity->origin , SCR_Entity ) &&
					 CalcScreen( g_Player[iIndex].vAimOrigin , SCR_Aim ) )
				{
					if ( cvar.esp_DrawAimspot )
						g_Engine.pfnFillRGBA( (int)SCR_Aim[0] , (int)SCR_Aim[1] , 2 , 2 , 255 , 255 , 255 , 255 );

					if ( cvar.esp_DrawFov && cvar.weapon[cvar.wpn].aim_FovType >= 1 )
					{
						int iFov = 0;
						int iFovVal = 0;

						if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
						{
							if ( g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
								iFovVal = cvar.weapon[cvar.wpn].aim_BulletOneFov;
							else if ( g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
								iFovVal = cvar.weapon[cvar.wpn].aim_BulletTwoFov;
							else
								iFovVal = cvar.weapon[cvar.wpn].aim_Fov;
						}
						else
							iFovVal = cvar.weapon[cvar.wpn].aim_Fov;

						iFovVal = iFovVal * ( 90 / g_Local.iFOV );

						if ( cvar.weapon[cvar.wpn].aim_FovType == 1 )
							iFov = iFovVal * g_Player[iIndex].fDistance / cvar.weapon[cvar.wpn].aim_FovDist;
						else if ( cvar.weapon[cvar.wpn].aim_FovType > 1 )
							iFov = iFovVal * cvar.weapon[cvar.wpn].aim_FovDist / g_Player[iIndex].fDistance;

						int	iFovX = (int)SCR_Aim[0] - iFov;
						int	iFovY = (int)SCR_Aim[1] - iFov;

						int r , g , b;

						if ( iFovTargetID == iIndex && bVisible( iFovTargetID ) )
						{
							r = 255 , g = 255 , b = 0;
						}
						else
						{
							r = 255 , g = 10 , b = 255;
						}

						g_Drawing.DrawBox( iFovX , iFovY , iFov * 2 , iFov * 2 , 1 , r , g , b , 255 );
					}
				}
			}
		}
	}
}

void cAimbot::DrawFovCrosshair()
{
	if ( cvar.weapon[cvar.wpn].aim_Active && cvar.esp_DrawFov && cvar.weapon[cvar.wpn].aim_FovType <= 0 )
	{
		int iFovVal = 0;

		if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
		{
			if ( g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
				iFovVal = cvar.weapon[cvar.wpn].aim_BulletOneFov;
			else if ( g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
				iFovVal = cvar.weapon[cvar.wpn].aim_BulletTwoFov;
			else
				iFovVal = cvar.weapon[cvar.wpn].aim_Fov;
		}
		else
			iFovVal = cvar.weapon[cvar.wpn].aim_Fov;

		iFovVal = iFovVal * ( 90 / g_Local.iFOV );

		int iFovX = ( g_Screen.iWidth / 2 ) - iFovVal;
		int iFovY = ( g_Screen.iHeight / 2 ) - iFovVal;

		byte r , g , b;

		if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
		{
			if ( g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
			{
				r = 200 , g = 0 , b = 255;
			}
			else if ( g_Player[iFovTargetID].fDistance > cvar.weapon[cvar.wpn].aim_BulletOneDistance &&
					  g_Player[iFovTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
			{
				r = 128 , g = 128 , b = 128;
			}
			else if ( g_Player[iFovTargetID].fDistance > cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
			{
				r = 0 , g = 0 , b = 0;
			}
		}
		else
		{
			r = 0 , g = 0 , b = 0;
		}
		
		g_Drawing.DrawBox( iFovX , iFovY , iFovVal * 2 , iFovVal * 2 , 1 , r , g , b , 255 );
	}
}

float cAimbot::GetRecoilVar()
{
	float recoil = 0.0f;

	switch ( g_Local.iWeaponID )
	{
		case WEAPONLIST_USP:
			recoil = 1.0f;
			break;
		case WEAPONLIST_GLOCK18:
			recoil = 1.0f;
			break;
		case WEAPONLIST_DEAGLE:
			recoil = 0.2f;
			break;
		case WEAPONLIST_P228:
			recoil = 1.0f;
			break;
		case WEAPONLIST_ELITE:
			recoil = 1.2f;
			break;
		case WEAPONLIST_FIVESEVEN:
			recoil = 1.0f;
			break;
		case WEAPONLIST_M4A1:
			recoil = 1.73f;
			break;
		case WEAPONLIST_AK47:
			recoil = 1.83f;
			break;
		case WEAPONLIST_AUG:
			recoil = 1.6f;
			break;
		case WEAPONLIST_SG552:
			recoil = 1.74f;
			break;
		case WEAPONLIST_M249:
			recoil = 1.85f;
			break;
		case WEAPONLIST_UMP45:
			recoil = 1.1f;
			break;
		case WEAPONLIST_MP5:
			recoil = 1.2f;
			break;
		case WEAPONLIST_TMP:
			recoil = 1.1f;
			break;
		case WEAPONLIST_MAC10:
			recoil = 1.58f;
			break;
		case WEAPONLIST_P90:
			recoil = 1.6f;
			break;
		default:
			recoil = 1.5f;
			break;
	}

	return recoil;
}

void cAimbot::SmoothAimAngles( float *MyViewAngles , float *AimAngles , float *OutAngles , float Smoothing )
{
	VectorSubtract( AimAngles , MyViewAngles , OutAngles );

	if ( OutAngles[1]>180.0f )
		OutAngles[1] -= 360.0f;

	if ( OutAngles[1]<-180.0f )
		OutAngles[1] += 360.0f;

	OutAngles[0] = OutAngles[0] / Smoothing + MyViewAngles[0];
	OutAngles[1] = OutAngles[1] / Smoothing + MyViewAngles[1];

	if ( OutAngles[1] > 360.0f )
		OutAngles[1] -= 360.0f;

	if ( OutAngles[1] < 0.0f )
		OutAngles[1] += 360.0f;
}

void cAimbot::ApplySilentAngles( float *angles , usercmd_s *cmd )
{
	Vector viewforward , viewright , viewup , aimforward , aimright , aimup , vTemp;
	float newforward , newright , newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;
	if ( g_Local.Entity->curstate.movetype == MOVETYPE_WALK ) { g_Engine.pfnAngleVectors( Vector( 0.0f , cmd->viewangles.y , 0.0f ) , viewforward , viewright , viewup ); }
	else { g_Engine.pfnAngleVectors( cmd->viewangles , viewforward , viewright , viewup ); }
	if ( g_Local.Entity->curstate.movetype == MOVETYPE_WALK && !( g_Local.iMoveType == 5 ) )
	{
		cmd->viewangles.x = angles[0];
		cmd->viewangles.y = angles[1];
	}
	if ( g_Local.Entity->curstate.movetype == MOVETYPE_WALK ) { g_Engine.pfnAngleVectors( Vector( 0.0f , cmd->viewangles.y , 0.0f ) , aimforward , aimright , aimup ); }
	else { g_Engine.pfnAngleVectors( cmd->viewangles , aimforward , aimright , aimup ); }
	newforward = DotProduct( forward * viewforward.Normalize() , aimforward ) + DotProduct( right * viewright.Normalize() , aimforward ) + DotProduct( up * viewup.Normalize() , aimforward );
	newright = DotProduct( forward * viewforward.Normalize() , aimright ) + DotProduct( right * viewright.Normalize() , aimright ) + DotProduct( up * viewup.Normalize() , aimright );
	newup = DotProduct( forward * viewforward.Normalize() , aimup ) + DotProduct( right * viewright.Normalize() , aimup ) + DotProduct( up * viewup.Normalize() , aimup );
	if ( angles[0] > 81 ) { cmd->forwardmove = -newforward; }
	else { cmd->forwardmove = newforward; }
	cmd->sidemove = newright;
	cmd->upmove = newup;
}

void cAimbot::GetBoneOrigin( cl_entity_s *Entity )
{
	if ( Entity && Entity->player && Entity->index != -1 && Entity->index != g_Local.iIndex && g_Players.isValidEntity( Entity ) )
	{
		typedef float TransformMatrix[MAXSTUDIOBONES][3][4];

		model_t* pModel = g_Studio.SetupPlayerModel( Entity->index );
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata( pModel );
		TransformMatrix* pbonetransform = (TransformMatrix*)g_Studio.StudioGetBoneTransform();

		iMaxBones = pStudioHeader->numbones;
		
		for ( int i = 0; i < pStudioHeader->numbones; i++ )
		{
			g_Player[Entity->index].vBone[i][0] = ( *pbonetransform )[i][0][3];
			g_Player[Entity->index].vBone[i][1] = ( *pbonetransform )[i][1][3];
			g_Player[Entity->index].vBone[i][2] = ( *pbonetransform )[i][2][3];

			g_Player[Entity->index].vBone[i] = g_Player[Entity->index].vBone[i] + ( g_Player[Entity->index].vVelocity * g_Player[Entity->index].fFrametime ) * cvar.weapon[cvar.wpn].aim_Prediction;
		}
	}
}

void cAimbot::GetHitboxOrigin( cl_entity_s *Entity )
{
	if ( Entity && Entity->player && Entity->index != -1 && Entity->index != g_Local.iIndex && g_Players.isValidEntity( Entity ) )
	{
		Vector vBBMin , vBBMax;
		typedef float BoneMatrix_t[MAXSTUDIOBONES][3][4];

		model_t* pModel = g_Studio.SetupPlayerModel( Entity->index );
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata( pModel );
		BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();
		mstudiobbox_t* pHitbox = (mstudiobbox_t*)( (byte*)pStudioHeader + pStudioHeader->hitboxindex );

		iMaxHitbox = pStudioHeader->numhitboxes;
		
		for ( int i = 0; i < pStudioHeader->numhitboxes; i++ )
		{
			VectorTransform( pHitbox[i].bbmin , ( *pBoneMatrix )[pHitbox[i].bone] , vBBMin );
			VectorTransform( pHitbox[i].bbmax , ( *pBoneMatrix )[pHitbox[i].bone] , vBBMax );

			g_Player[Entity->index].vHitbox[i] = ( vBBMax + vBBMin ) * 0.5f;
			g_Player[Entity->index].vHitbox[i] = g_Player[Entity->index].vHitbox[i] + ( g_Player[Entity->index].vVelocity * g_Player[Entity->index].fFrametime ) * cvar.weapon[cvar.wpn].aim_Prediction;
		}
	}
}

Vector cAimbot::vCalcOriginOffset( int iIndex )
{
	cl_entity_s *ent = g_Engine.GetEntityByIndex( iIndex );
	Vector vAngles , vF , vR , vU , vOut;
	vAngles = Vector( 0.0f , ent->angles[1] , 0.0f );
	g_Engine.pfnAngleVectors( vAngles , vF , vR , vU );

	if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
	{
		if ( g_Player[iIndex].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
		{
			vOut = ent->origin + vF * 10 + vR * 0 + vU*cvar.weapon[cvar.wpn].aim_BulletOneAimSpot;
		}
		else if ( g_Player[iIndex].fDistance > cvar.weapon[cvar.wpn].aim_BulletOneDistance &&
				  g_Player[iIndex].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
		{
			vOut = ent->origin + vF * 10 + vR * 0 + vU*cvar.weapon[cvar.wpn].aim_BulletTwoAimSpot;
		}
		else if ( g_Player[iIndex].fDistance > cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
		{
			vOut = ent->origin + vF * 10 + vR * 0 + vU*cvar.weapon[cvar.wpn].aim_Aimspot;
		}
	}
	else
		vOut = ent->origin + vF * 10 + vR * 0 + vU*cvar.weapon[cvar.wpn].aim_Aimspot;

	return ( vOut + ( g_Player[iIndex].vVelocity * g_Player[iIndex].fFrametime ) * cvar.weapon[cvar.wpn].aim_Prediction );
}

Vector cAimbot::vCalcBoneOffset( int iIndex )
{
	Vector vAngles , vF , vR , vU , vOut;
	vAngles = Vector( 0.0f , g_Player[iIndex].vAngle[1] , 0.0f );
	g_Engine.pfnAngleVectors( vAngles , vF , vR , vU );

	if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
	{
		if ( g_Player[iIndex].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
		{
			vOut = g_Player[iIndex].vBone[cvar.weapon[cvar.wpn].aim_BulletOneAimSpot] + vF + vR + vU;
		}
		else if ( g_Player[iIndex].fDistance > cvar.weapon[cvar.wpn].aim_BulletOneDistance &&
				  g_Player[iIndex].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
		{
			vOut = g_Player[iIndex].vBone[cvar.weapon[cvar.wpn].aim_BulletTwoAimSpot] + vF + vR + vU;
		}
		else if ( g_Player[iIndex].fDistance > cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
		{
			vOut = g_Player[iIndex].vBone[cvar.weapon[cvar.wpn].aim_Aimspot] + vF + vR + vU;
		}
	}
	else
		vOut = g_Player[iIndex].vBone[cvar.weapon[cvar.wpn].aim_Aimspot] + vF + vR + vU;

	return vOut;
}

Vector cAimbot::vCalcHitboxOffset( int iIndex )
{
	Vector vAngles , vF , vR , vU , vOut;
	vAngles = Vector( 0.0f , g_Player[iIndex].vAngle[1] , 0.0f );
	g_Engine.pfnAngleVectors( vAngles , vF , vR , vU );

	if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
	{
		if ( g_Player[iIndex].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
		{
			vOut = g_Player[iIndex].vHitbox[cvar.weapon[cvar.wpn].aim_BulletOneAimSpot] + vF + vR + vU;
		}
		else if ( g_Player[iIndex].fDistance > cvar.weapon[cvar.wpn].aim_BulletOneDistance &&
			g_Player[iIndex].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
		{
			vOut = g_Player[iIndex].vHitbox[cvar.weapon[cvar.wpn].aim_BulletTwoAimSpot] + vF + vR + vU;
		}
		else if ( g_Player[iIndex].fDistance > cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
		{
			vOut = g_Player[iIndex].vHitbox[cvar.weapon[cvar.wpn].aim_Aimspot] + vF + vR + vU;
		}
	}
	else
		vOut = g_Player[iIndex].vHitbox[cvar.weapon[cvar.wpn].aim_Aimspot] + vF + vR + vU;

	return vOut;
}

void cAimbot::CL_WeaponControl()
{
	if ( g_Local.bPistolWeapon || g_Local.bShotGunWeapon || g_Local.bSniperWeapon )
	{
		cvar.weapon[cvar.wpn].aim_BulletControl = 0;
		cvar.weapon[cvar.wpn].aim_BulletOneClampType = 2;
		cvar.weapon[cvar.wpn].aim_BulletTwoClampType = 2;
		cvar.weapon[cvar.wpn].aim_KillDelay = 0;
	}
}

void cAimbot::CL_CreateMove( usercmd_s *cmd )
{
	if ( g_Cheat.bTriggerShot )
		return;

	float fAimAngles[3] = { 0 };
	float fMyCmdAngles[3] = { 0 };
	float fMyViewAngles[3] = { 0 };
	float fSmoothAngles[3] = { 0 };
	float fSmooth = 0;
	Vector fVecScreen;

	VectorCopy( cmd->viewangles , fMyCmdAngles );

	g_Engine.GetViewAngles( fMyViewAngles );

	CL_WeaponControl();

	if ( cmd->buttons & IN_ATTACK && iClipCount < 0 )
	{
		iClipState = g_Local.iClip + 1;
		iClipCount = 0;
	}

	if ( cmd->buttons & IN_ATTACK && !g_Local.bBadWeapon )
	{
		iClipCount = ( iClipState - g_Local.iClip );
	}
	else
	{
		iClipState = g_Local.iClip;
		iClipCount = 0;
	}

	if ( bAimEnable )
	{
		if ( !iTargetID || g_Local.iInReload )
			return;

		VectorAngles( g_Player[iTargetID].vAimOrigin - g_Local.vViewOrg , fAimAngles );

		if ( g_Player[iTargetID].fFov >= 1.f )
		{
			if ( !( cmd->buttons & IN_ATTACK ) && g_Player[iTargetID].isFov && !bShotDelay )
			{
				bShotDelay = true;
			}
			else if ( !g_Player[iTargetID].isFov )
			{
				bShotDelay = false;
			}
		}
		else bShotDelay = false;

		if ( !g_Player[iTargetID].isFov && ( cmd->buttons & IN_ATTACK ) )
		{
			bRecoilnFov = true;
		}
		if ( g_Player[iTargetID].isFov && !( cmd->buttons & IN_ATTACK ) )
		{
			bRecoilnFov = false;
		}

		if ( cmd->buttons & IN_ATTACK && !g_Local.bBadWeapon )
		{		
			if ( iClipCount <= cvar.weapon[cvar.wpn].aim_BulletControl )
				bRecoilTime = false;
			else if ( iClipCount > cvar.weapon[cvar.wpn].aim_BulletControl )
				bRecoilTime = true;
			
			if ( g_Player[iTargetID].isFov && bRecoilnFov )
			{
				bRecoilTime = true;
			}

			if ( g_Local.bPistolWeapon || g_Local.bShotGunWeapon || g_Local.bSniperWeapon )
			{
				if ( bRecoilTime )
					return;
			}

			if ( bRecoilTime && g_Player[iTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
			{
				if ( cvar.aim_Type <= 0 && g_Player[iTargetID].bAlive && bVisible( iTargetID ) )
				{
					fRecoilX = g_Local.vPunchangle[1] * GetRecoilVar();
					fRecoilY = g_Local.vPunchangle[0] * GetRecoilVar();

					fAimAngles[1] -= fRecoilX;
					fAimAngles[0] -= fRecoilY;
				}
			}

			if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
			{
				if ( g_Player[iTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
					fSmooth = cvar.weapon[cvar.wpn].aim_BulletOneSmooth;
				else if ( g_Player[iTargetID].fDistance > cvar.weapon[cvar.wpn].aim_BulletOneDistance &&
					 g_Player[iTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
					fSmooth = cvar.weapon[cvar.wpn].aim_BulletTwoSmooth;
				else
					fSmooth = cvar.weapon[cvar.wpn].aim_Smooth;
			}
			else
				fSmooth = cvar.weapon[cvar.wpn].aim_Smooth;

			if ( cvar.aim_Type <= 0 )
			{
				if ( fSmooth > 0.f && !cvar.aim_Silent )
				{
					SmoothAimAngles( fMyViewAngles , fAimAngles , fSmoothAngles , fSmooth / 10.f );
				}
				else
				{
					VectorCopy( fAimAngles , fSmoothAngles );
				}
			}

			if ( g_Player[iTargetID].isFov )
			{
				g_Player[iTargetID].bEnableAim = true;
			}
			else
			{
				g_Player[iTargetID].bEnableAim = false;
			}

			if ( g_Player[iTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletOneDistance )
			{
				iCurrentClamp = cvar.weapon[cvar.wpn].aim_BulletOneClampType;
			}
			else if ( g_Player[iTargetID].fDistance > cvar.weapon[cvar.wpn].aim_BulletOneDistance &&
					  g_Player[iTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
			{
				iCurrentClamp = cvar.weapon[cvar.wpn].aim_BulletTwoClampType;
			}
			else if ( g_Player[iTargetID].fDistance > cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
			{
				iCurrentClamp = 2;

				if ( g_Player[iTargetID].isFov && bRecoilnFov )
				{
					g_Player[iTargetID].bEnableAim = false;
				}
			}

			if ( iPreTargetID && !g_Player[iPreTargetID].bAlive && iCurrentClamp >= 2 )
			{
				if ( !bClamp )
				{
					bClamp = true;
				}
			}

			if ( g_Player[iTargetID].bEnableAim )
			{
				if ( !g_Player[iTargetID].dwStartTime )
				{
					g_Player[iTargetID].dwStartTime = GetTickCount();
				}

				DWORD dwNow = GetTickCount();

				if ( ( g_Player[iTargetID].dwStartTime + cvar.weapon[cvar.wpn].aim_ShotDelay ) < dwNow )
				{
					g_Player[iTargetID].bShotDelay = false;
				}
				else
				{
					if ( bShotDelay )
						g_Player[iTargetID].bShotDelay = true;
					else
						g_Player[iTargetID].bShotDelay = false;
				}

				if ( iPreTargetID && !g_Player[iPreTargetID].bAlive && iCurrentClamp == 1 )
				{
					if ( !bClamp )
					{
						bClamp = true;
					}
				}
				else if( iPreTargetID && !g_Player[iPreTargetID].bAlive && iCurrentClamp <= 0 )
				{
					bClamp = false;
				}

				if ( !bClamp && g_Player[iTargetID].bAlive && bVisible( iTargetID ) )
				{
					if ( cvar.aim_Type <= 0 )
					{
						if ( cvar.aim_Silent <= 0 )
							g_Engine.SetViewAngles( fSmoothAngles );
						else if ( cvar.aim_Silent )
							ApplySilentAngles( fSmoothAngles , cmd );
					}
					else if ( cvar.aim_Type )
					{
						if ( CalcScreen( g_Player[iTargetID].vAimOrigin , fVecScreen ) )
						{
							float fRecoilFactor = ( ( g_Screen.iWidth * g_Screen.iHeight ) * 20.0f ) / 2073600.0f;

							fRecoilX = g_Local.vPunchangle[1] * fRecoilFactor;
							fRecoilY = g_Local.vPunchangle[0] * fRecoilFactor;

							if ( fSmooth > 0.f )
							{
								float a = ( fVecScreen[0] - g_Screen.iWidth / 2 ) / ( fSmooth / 10.f );
								float b = ( fVecScreen[1] - g_Screen.iHeight / 2 ) / ( fSmooth / 10.f );
								fVecScreen[0] = g_Screen.iWidth / 2 + a;
								fVecScreen[1] = g_Screen.iHeight / 2 + b;
							}

							if ( bRecoilTime && g_Player[iTargetID].fDistance <= cvar.weapon[cvar.wpn].aim_BulletTwoDistance )
							{
								if ( (int)fRecoilX > fVecScreen[0] )
								{
									if ( fSmooth >= 1.0f )
										g_pEngine->pfnSetMousePos( (int)( fVecScreen[0] - fRecoilX / ( fSmooth / 10.f ) ) , (int)( fVecScreen[1] - fRecoilY / ( fSmooth / 10.f ) ) );
									else
										g_pEngine->pfnSetMousePos( (int)( fVecScreen[0] - fRecoilX ) , (int)( fVecScreen[1] - fRecoilY ) );
								}
								else
								{
									if ( fSmooth >= 1.0f )
										g_pEngine->pfnSetMousePos( (int)( fVecScreen[0] + fRecoilX / ( fSmooth / 10.f ) ) , (int)( fVecScreen[1] - fRecoilY / ( fSmooth / 10.f ) ) );
									else
										g_pEngine->pfnSetMousePos( (int)( fVecScreen[0] + fRecoilX ) , (int)( fVecScreen[1] - fRecoilY ) );
								}
							}
							else
								g_pEngine->pfnSetMousePos( (int)fVecScreen[0] , (int)fVecScreen[1] );
						}
					}

					if ( g_Player[iTargetID].bShotDelay )
						cmd->buttons &= ~IN_ATTACK;
				}
			}
			else
			{
				if ( iCurrentClamp == 1 && bClamp )
				{
					bClamp = false;
				}
			}
		}
		else
		{
			g_Player[iTargetID].dwStartTime = 0;
			g_Player[iTargetID].bEnableAim = false;
			g_Player[iTargetID].bAttackTime = false;
			
			iTargetID = 0;
			iPreTargetID = 0;
						
			bClamp = false;
			bRecoilTime = false;
			bRecoilnFov = false;
		}	
	}
}