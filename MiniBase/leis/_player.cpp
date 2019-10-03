#include "_player.h"

local_s g_Local;
player_s g_Player[33];
cPlayers g_Players;

#pragma warning(disable:4244)

bool cPlayers::IsAliveEntity( cl_entity_s *Entity )
{
	return ( Entity && !( Entity->curstate.effects & EF_NODRAW ) &&
			 Entity->player && Entity->curstate.movetype != 6 && Entity->curstate.movetype != 0 );
}

bool cPlayers::isValidEntity( cl_entity_s *Entity )
{
	if ( Entity->player && g_Local.iIndex != Entity->index && Entity->curstate.movetype != 6 && 
		 Entity->curstate.movetype != 0 && !( Entity->curstate.messagenum<g_Engine.GetLocalPlayer()->curstate.messagenum ) &&
		 !( g_Engine.GetLocalPlayer()->curstate.iuser1 == 4 && g_Engine.GetLocalPlayer()->curstate.iuser2 == Entity->index ) )
	{
		return true;
	}

	return false;
}

void cPlayers::UpdateLocalEntity()
{
	g_Local.Entity = g_Engine.GetLocalPlayer();
	g_Local.iIndex = g_Local.Entity->index;
	g_Local.bAlive = IsAliveEntity( g_Local.Entity );

	g_Engine.pEventAPI->EV_LocalPlayerViewheight( g_Local.vViewOrg );

	VectorAdd( g_Local.Entity->origin , g_Local.vViewOrg , g_Local.vViewOrg );
}

void cPlayers::UpdatePlayerInfo()
{
	g_Players.UpdateLocalEntity();
	g_NoSpread.NoSpread();
	
	for ( int i = 1; i <= 32; i++ )
	{
		if ( g_Local.Entity->index == i )
			continue;

		cl_entity_s *Entity = g_Engine.GetEntityByIndex( i );

		if ( !Entity || !Entity->player )
		{
			g_Player[i].Entity = nullptr;
			continue;
		}

		g_Player[i].Entity = Entity;
		g_Engine.pfnGetPlayerInfo( i , &g_Player[i].Info );
		
		g_Player[i].bAlive = IsAliveEntity( Entity );
		g_Player[i].bUpdated = isValidEntity( Entity );
		g_Player[i].bVisible = CalcPlayerVisibility( i );

		g_Player[i].vVelocity = Entity->curstate.origin - Entity->prevstate.origin;
		g_Player[i].fFrametime = Entity->curstate.animtime - Entity->prevstate.animtime;

		g_Player[i].fDistance = VectorDistance( g_Local.vViewOrg , Entity->origin );

		if ( g_Player[i].fFrametime == 0 )
			g_Player[i].fFrametime = g_Local.fFrametime;
	
		
		if ( g_Player[i].bAlive && g_Player[i].bUpdated )
		{
			if ( cvar.weapon[cvar.wpn].aim_Active )
			{
				g_Aimbot.UpdateAimInfo( i );

				if ( !bSnapshot && !bScreenshot )
				{
					g_Aimbot.DrawFov( i );
				}
			}

			if ( !bSnapshot && !bScreenshot )
			{
				g_Esp.DrawPlayerESP( i );

				if ( cvar.esp_DrawTrigger )
					g_Cheat.TriggerShowFov( i );
			}
		}
	}

	g_Aimbot.UpdatePlayerInfo();

	if ( !bSnapshot && !bScreenshot )
	{

#if (HACK_TYPE == 1)
		char cfg_verm_public[] = { 'V','e','r','m','i','l','l','i','o','n',' ','[','P','U','B','L','I','C',']',0 };
		Verdana1.PrintFont( 5 , 130 , 205 , 0 , 0 , 255 , 0 , cfg_verm_public );
#endif

		g_Aimbot.DrawFovCrosshair();
		g_Cheat.TriggerShowStatus();

		g_Esp.DrawAllEntityESP();
		g_Esp.DrawNoFlash();

		if ( cvar.esp_Sound )
			g_Sound.DrawSoundEsp();

		if ( cvar.misc_Punch )
		{
			float recoil = ( cvar.aim_Type ? ( ( ( g_Screen.iWidth * g_Screen.iHeight ) * 20.0f ) / 2073600.0f ) : g_Aimbot.GetRecoilVar() );

			float fRecoilY = g_Local.vPunchangle[0] * recoil;
			float fRecoilX = g_Local.vPunchangle[1] * recoil;

			g_Drawing.FillArea( g_Screen.iWidth / 2 - fRecoilX - 1 , g_Screen.iHeight / 2 + fRecoilY - 1 , 3 , 3 , 255 , 1 , 1 , 255 );
		}
	}
}

void cPlayers::ClearPlayers()
{
	native_memset( g_Player , sizeof( g_Player ) , 0 );
}