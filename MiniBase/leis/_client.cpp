#include "_client.h"

cLeis g_Leis;
cEntity g_Entity;
cSound g_Sound;
noflash_s noflash;

bool bSnapshot = false;
bool bScreenshot = false;

Snapshot_t Snapshot_s = nullptr;
Screenshot_t Screenshot_s = nullptr;

#pragma warning(disable:4244)

void cEntity::AddEntity( char* name , int iImportant , Vector vOrigin , BYTE type )
{
	if ( EntityIndex < MAX_ENTITY )
	{
		native_strcpy( entity[EntityIndex].name , name );

		entity[EntityIndex].iImportant = iImportant;
		entity[EntityIndex].vOrigin = vOrigin;
		entity[EntityIndex].type = type;

		EntityIndex++;
	}
	else
		ClearEntity();
}

void cEntity::ClearEntity()
{
	native_memset( entity , sizeof( entity ) , 0 );
	EntityIndex = 0;
}

void cSound::AddSound( DWORD dwTime , Vector vOrigin )
{
	if ( SoundIndex < MAX_SOUNDS )
	{
		sound[SoundIndex].dwTime = dwTime;
		sound[SoundIndex].vOrigin = vOrigin;

		SoundIndex++;
	}
	else
		ClearSound();
}

void cSound::ClearSound()
{
	native_memset( sound , sizeof( sound ) , 0 );
	SoundIndex = 0;
}

void cSound::DrawSoundEsp()
{
	for ( int i = 0; i < SoundIndex; i++ )
	{
		if ( g_Sound.sound[i].dwTime + 800 <= GetTickCount() )
		{
			g_Sound.sound[i].dwTime = 0;
			g_Sound.sound[i].vOrigin = Vector( 0 , 0 , 0 );
		}
		else
		{
			float SoundScreen[3] = { 0 };

			if ( CalcScreen( g_Sound.sound[i].vOrigin , SoundScreen ) )
			{
				if ( cvar.esp_SoundType <= 1 )
					Verdana1.Print( SoundScreen[0] , SoundScreen[1] , 255 , 0 , 255 , 255 , FL_CENTER , "*" );
				else
					g_Drawing.DrawBox( SoundScreen[0] - 10 , SoundScreen[1] - 10 , 10 , 10 , 1 , 255 , 255 , 255 , 255 );
			}
		}
	}
}

void cLeis::AntiSnapshot()
{
	if ( cvar.misc_AntiScreen )
		bSnapshot = true;
	else
		Snapshot_s();
}

void cLeis::AntiScreenshot()
{
	if ( cvar.misc_AntiScreen )
		bScreenshot = true;
	else
		Screenshot_s();
}

void cLeis::AntiScreen()
{
	if ( bSnapshot || bScreenshot )
	{
		ScreenTimer--;

		if ( ScreenTimer <= 0 )
		{
			if ( bSnapshot )
			{
				Snapshot_s();
				bSnapshot = false;
			}
			else if ( bScreenshot )
			{
				Screenshot_s();
				bScreenshot = false;
			}

			ScreenTimer = 5;
		}
	}
}

void cLeis::InitHack()
{
	char cfg_verdana[] = { 'V','e','r','d','a','n','a',0 };
	char cfg_arial_black[] = { 'A','r','i','a','l',' ','B','l','a','c','k',0 };

	char cfg_snapshot[] = { 's','n','a','p','s','h','o','t',0 };
	char cfg_screenshot[] = { 's','c','r','e','e','n','s','h','o','t',0 };

	if ( offset.HLType != RENDERTYPE_HARDWARE )
	{
		char err_mode[] = { '[','O','p','e','n','g','l',' ','m','o','d','e',' ','e','r','r','o','r',']',0 };

		offset.Error( err_mode );
	}

	Verdana1.InitText( cfg_verdana , 12 , 0 );
	Verdana2.InitText( cfg_verdana , 14 , 0 );

	ArialBlack.InitText( cfg_arial_black , 28 , 0 );

	ScreenTimer = 5;

	g_Local.Entity = nullptr;
	g_Local.bAlive = false;
	g_Local.iFOV = 90;

	pcmd_t pSnapshot = CommandByName( cfg_snapshot );
	pcmd_t pScreenshot = CommandByName( cfg_screenshot );

	Snapshot_s = (Snapshot_t)pSnapshot->function;
	Screenshot_s = (Snapshot_t)pScreenshot->function;

	pSnapshot->function = (xcommand_t)AntiSnapshot;
	pScreenshot->function = (xcommand_t)AntiScreenshot;

	g_WeaponList.WeaponListData = (weapon_list_s*)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , sizeof( weapon_list_s ) * MAX_WEAPON_LIST );
	g_WeaponList.WeaponListClear();

#if (HACK_TYPE == 1)

	char cfg_verm_consl[] = { 't','o','g','g','l','e','c','o','n','s','o','l','e',0 };

	char cfg_verm_info1[] = { ' ',' ','V','e','r','m','i','l','l','i','o','n',' ','[','P','U','B','L','I','C',' ','V','E','R','S','I','O','N',']',' ','\n',0 };
	char cfg_verm_info2[] = { ' ',' ','b','y',' ','_','o','r','_','7','5',' ','&',' ','p','o','i','u','y','t','r','e','w',' ','\n','\n',0 };
	char cfg_verm_info3[] = { ' ',' ','C','r','e','d','i','t','s',':','\n',0 };
	char cfg_verm_info4[] = { ' ',' ','-',' ','A','n','t','i','V','a','l','v','e','\n',0 };
	char cfg_verm_info5[] = { ' ',' ','-',' ','P','R','O','N','A','X','\n',0 };
	char cfg_verm_info6[] = { ' ',' ','-',' ','m','a','l','a','n','r','i','s','\n',0 };
	char cfg_verm_info7[] = { ' ',' ','-',' ','V','e','4','n','i','y','S','o','o','N','\n',0 };
	char cfg_verm_info8[] = { ' ',' ','-',' ','b','1','5','q','9','5','\n',0 };
	char cfg_verm_info9[] = { ' ',' ','-',' ','O','t','h','e','r','s',' ','w','h','o',' ','h','e','l','p',' ','m','e',' ',':',')','\n',0 };

	g_pEngine->pfnClientCmd( cfg_verm_consl );

	g_pEngine->Con_Printf( cfg_verm_info1 );
	g_pEngine->Con_Printf( cfg_verm_info2 );
	g_pEngine->Con_Printf( cfg_verm_info3 );
	g_pEngine->Con_Printf( cfg_verm_info4 );
	g_pEngine->Con_Printf( cfg_verm_info5 );
	g_pEngine->Con_Printf( cfg_verm_info6 );
	g_pEngine->Con_Printf( cfg_verm_info7 );
	g_pEngine->Con_Printf( cfg_verm_info8 );
	g_pEngine->Con_Printf( cfg_verm_info9 );
#endif

	cvar.LoadCvarFromIni();
}

void cLeis::HUD_Redraw()
{
	g_Players.UpdatePlayerInfo();

	if ( !bSnapshot && !bScreenshot )
	{
		g_Menu.DrawMenu( g_Screen.iWidth / 2 , ( g_Local.bAlive ? 3 : 110 ) );
	}

	AntiScreen();
}

void cLeis::HUD_Key_Event( int keynum )
{
	g_Menu.KeyEvent( keynum );
}

void cLeis::HUD_PlayerMove( struct playermove_s *ppmove )
{
	g_Local.iFlags = ppmove->flags;
	g_Local.iMoveType = ppmove->movetype;
	g_Local.iWaterLevel = ppmove->waterlevel;

	g_Local.flZspeed = ( ppmove->velocity[2] != 0 ) ? -ppmove->velocity[2] : 0;
	g_Local.flXYspeed = mysqrt( POW( ppmove->velocity[0] ) + POW( ppmove->velocity[1] ) );

	g_Local.vVelocity = ppmove->velocity;
}

void cLeis::V_CalcRefdef( struct ref_params_s *pparams )
{
	VectorCopy( pparams->punchangle , g_Local.vPunchangle );

	g_Client.V_CalcRefdef( pparams );
}

void cLeis::StudioEntityLight( struct alight_s *plight )
{
	cl_entity_s* Entity = g_Studio.GetCurrentEntity();
	
	if ( Entity && Entity->player )
	{
		g_Aimbot.GetBoneOrigin( Entity );
		g_Aimbot.GetHitboxOrigin( Entity );

		if ( !bSnapshot && !bScreenshot && cvar.misc_Lambert )
		{
			plight->ambientlight = 128;
		}
	}
}

void cLeis::HUD_PostRunCmd( struct local_state_s *to , struct usercmd_s *cmd , int runfuncs , double time , unsigned int random_seed )
{
	if ( g_Local.bAlive )
	{
		g_NoSpread.HUD_PostRunCmd( to , cmd , runfuncs , time , random_seed );

		g_Local.iInReload = ( to->weapondata[g_Local.iWeaponID].m_fInReload > 0 || !g_Local.iClip );
		g_Local.m_flNextPrimaryAttack = to->weapondata[g_Local.iWeaponID].m_flNextPrimaryAttack;
		g_Local.m_flNextSecondaryAttack = to->weapondata[g_Local.iWeaponID].m_flNextSecondaryAttack;
	}
}

int cLeis::HUD_AddEntity( int type , struct cl_entity_s *ent , const char *modelname )
{
	int ret = g_Client.HUD_AddEntity( type , ent , modelname );

	g_Esp.HUD_AddEntity( type , ent );

	return ret;
}

void cLeis::CL_CreateMove( float frametime , usercmd_s *cmd )
{
	if ( g_Local.bAlive )
	{
		g_Local.bBadWeapon = ( g_Local.iWeaponID == WEAPONLIST_C4 ) || ( g_Local.iWeaponID == WEAPONLIST_FLASHBANG )
			|| ( g_Local.iWeaponID == WEAPONLIST_HEGRENADE ) || ( g_Local.iWeaponID == WEAPONLIST_KNIFE )
			|| ( g_Local.iWeaponID == WEAPONLIST_SMOKEGRENADE );
		
		g_Local.bPistolWeapon = (  ( g_Local.iWeaponID == WEAPONLIST_USP ) || ( g_Local.iWeaponID == WEAPONLIST_GLOCK18 ) 
								  || ( g_Local.iWeaponID == WEAPONLIST_DEAGLE ) || ( g_Local.iWeaponID == WEAPONLIST_ELITE ) 
								  || ( g_Local.iWeaponID == WEAPONLIST_FIVESEVEN ) || ( g_Local.iWeaponID == WEAPONLIST_P228 ) );

		g_Local.bShotGunWeapon = ( ( g_Local.iWeaponID == WEAPONLIST_M3 ) || ( g_Local.iWeaponID == WEAPONLIST_XM1014 ) );

		g_Local.bSniperWeapon = ( ( g_Local.iWeaponID == WEAPONLIST_AWP ) || ( g_Local.iWeaponID == WEAPONLIST_SCOUT )
								  || ( g_Local.iWeaponID == WEAPONLIST_SG550 ) || ( g_Local.iWeaponID == WEAPONLIST_G3SG1 ) );

		if ( !g_Local.bBadWeapon )
			cvar.wpn = cvar.GetSectionFromIndex( g_Local.iWeaponID );
		else
			cvar.wpn = 0;
		
		if ( cmd->buttons & IN_ATTACK && g_Local.bSniperWeapon && g_Local.iFOV == 90 && cvar.misc_FastZoom )
		{
			cmd->buttons &= ~IN_ATTACK;

			if ( g_Local.m_flNextSecondaryAttack >= 0.0f )
				cmd->buttons &= ~IN_ATTACK2;
			else
				cmd->buttons |= IN_ATTACK2;

			if ( g_Local.m_flNextPrimaryAttack >= 0.0f )
				cmd->buttons &= ~IN_ATTACK;
			else
				cmd->buttons |= IN_ATTACK;
					
		}
		
		g_Local.fFrametime = frametime;

		VectorCopy( cmd->viewangles , g_Local.vCmdViewAngles );

		g_NoSpread.CL_CreateMove( cmd );

		if ( cvar.trigger_Enable )
			g_Cheat.TriggerFunction( cmd );

		if ( cvar.knf_Active )
			g_Cheat.KnifeBotFunction( cmd );

		if ( cvar.misc_FastRun )
			g_Cheat.FastRunFunction( cmd );

		if ( cvar.misc_Bhop )
			g_Cheat.BhopFunction( cmd );

		if ( cvar.weapon[cvar.wpn].aim_Active )
			g_Aimbot.CL_CreateMove( cmd );
	}
}