#include "_cvar.h"

cCvar cvar;

void cCvar::InitPatch()
{
	IniPatch = (PCHAR)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , MAX_PATH );
	IniValue = (PCHAR)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , INT_DIGITS );
	IniSection = (PCHAR)HeapAlloc( GetProcessHeap() , HEAP_ZERO_MEMORY , INT_DIGITS );

	native_strcpy( IniPatch , BaseDir );
	native_memset( BaseDir , MAX_PATH , 0 );

	int len = native_strlen( IniPatch );
	
	IniPatch[len - 1] = 'i';
	IniPatch[len - 2] = 'n';
	IniPatch[len - 3] = 'i';

	for ( DWORD i = 0; i < MAX_PATH; i++ )
		IniPatch[i] ^= INI_PATH_KEY;
	
	bInitPatch = true;
}

void cCvar::LoadCvarFromIni()
{
	char CFG_AIMBOT[] = { 'A','i','m','b','o','t',0 };
	char CFG_TRIGGERBOT[] = { 'T','r','i','g','g','e','r','b','o','t',0 };
	char CFG_KNIFEBOT[] = { 'K','n','i','f','e','b','o','t',0 };
	char CFG_ESP[] = { 'E','s','p',0 };
	char CFG_MISC[] = { 'M','i','s','c',0 };
	char CFG_KEY[] = { 'K','e','y',0 };

	char CFG_AIM_DEATHMATCH[] = { 'a','i','m','_','D','e','a','t','h','m','a','t','c','h',0 };
	char CFG_AIM_THROUGH[] = { 'a','i','m','_','A','i','m','t','h','r','o','u','g','h',0 };
	char CFG_AIM_SILENT[] = { 'a','i','m','_','S','i','l','e','n','t',0 };
	char CFG_AIM_TYPE[] = { 'a','i','m','_','T','y','p','e',0 };

	char CFG_TRIGGER_ENABLE[] = { 't','r','i','g','g','e','r','_','E','n','a','b','l','e',0 };
	char CFG_TRIGGER_DEATHMATCH[] = { 't','r','i','g','g','e','r','_','D','e','a','t','h','m','a','t','c','h',0 };
	char CFG_TRIGGER_AIMTHROUND[] = { 't','r','i','g','g','e','r','_','A','i','m','t','h','r','o','u','g','h',0 };
	char CFG_TRIGGER_DISTANCEMIN[] = { 't','r','i','g','g','e','r','_','D','i','s','t','a','n','c','e','M','i','n',0 };
	char CFG_TRIGGER_DISTANCEMAX[] = { 't','r','i','g','g','e','r','_','D','i','s','t','a','n','c','e','M','a','x',0 };
	char CFG_TRIGGER_KEY[] = { 't','r','i','g','g','e','r','_','K','e','y',0 };
	char CFG_TRIGGER_KEYMODE[] = { 't','r','i','g','g','e','r','_','K','e','y','M','o','d','e',0 };
	char CFG_TRIGGER_ONLYZOOM[] = { 't','r','i','g','g','e','r','_','O','n','l','y','Z','o','o','m',0 };

	char CFG_KNIFEBOT_ACTIVE[] = { 'k','n','f','_','A','c','t','i','v','e',0 };
	char CFG_KNIFEBOT_AIM[] = { 'k','n','f','_','A','i','m',0 };
	char CFG_KNIFEBOT_AIM_FOV[] = { 'k','n','f','_','A','i','m','F','o','v',0 };
	char CFG_KNIFEBOT_TEAM[] = { 'k','n','f','_','T','e','a','m',0 };
	char CFG_KNIFEBOT_ATTACK[] = { 'k','n','f','_','A','t','t','a','c','k',0 };
	char CFG_KNIFEBOT_DST_ATTACK[] = { 'k','n','f','_','D','i','s','t','A','t','t','a','c','k',0 };
	char CFG_KNIFEBOT_DST_ATTACK2[] = { 'k','n','f','_','D','i','s','t','A','t','t','a','c','k','2',0 };

	char CFG_ESP_STYLE[] = { 'e','s','p','_','S','t','y','l','e',0 };
	char CFG_ESP_SIZE[] = { 'e','s','p','_','S','i','z','e',0 };
	char CFG_ESP_VISIBLE[] = { 'e','s','p','_','V','i','s','i','b','l','e',0 };
	char CFG_ESP_NAME[] = { 'e','s','p','_','N','a','m','e',0 };
	char CFG_ESP_WEAPON[] = { 'e','s','p','_','W','e','a','p','o','n',0 };
	char CFG_ESP_SOUND[] = { 'e','s','p','_','S','o','u','n','d',0 };
	char CFG_ESP_SOUND_TYPE[] = { 'e','s','p','_','S','o','u','n','d','T','y','p','e',0 };
	char CFG_ESP_BAREL[] = { 'e','s','p','_','B','a','r','r','e','l',0 };
	char CFG_ESP_WORLD_WPN[] = { 'e','s','p','_','W','o','r','l','d','W','e','a','p','o','n','s',0 };
	char CFG_ESP_WORLD_SPR[] = { 'e','s','p','_','W','o','r','l','d','S','p','r','i','t','e',0 };
	char CFG_ESP_DRAW_FONT[] = { 'e','s','p','_','D','r','a','w','F','o','n','t',0 };
	char CFG_ESP_DRAWAIMSPOT[] = { 'e','s','p','_','D','r','a','w','A','i','m','s','p','o','t',0 };
	char CFG_ESP_DRAW_FOV[] = { 'e','s','p','_','D','r','a','w','F','o','v',0 };
	char CFG_ESP_DRAW_TRIGGER[] = { 'e','s','p','_','D','r','a','w','T','r','i','g','g','e','r',0 };

	char CFG_MISC_BHOP[] = { 'm','i','s','c','_','B','h','o','p',0 };
	char CFG_MISC_PUNCH[] = { 'm','i','s','c','_','P','u','n','c','h',0 };
	char CFG_MISC_LAMBERT[] = { 'm','i','s','c','_','L','a','m','b','e','r','t',0 };
	char CFG_MISC_FAST_ZOOM[] = { 'm','i','s','c','_','F','a','s','t','Z','o','o','m',0 };
	char CFG_MISC_ANTI_SCREEN[] = { 'm','i','s','c','_','A','n','t','i','S','c','r','e','e','n',0 };
	char CFG_MISC_FASTRUN[] = { 'm','i','s','c','_','F','a','s','t','R','u','n',0 };
	char CFG_MISC_REMOVE_FLASH[] = { 'm','i','s','c','_','R','e','m','o','v','e','F','l','a','s','h',0 };
	char CFG_MISC_REMOVE_FLASH_MSG[] = { 'm','i','s','c','_','F','l','a','s','h','M','e','s','s','a','g','e',0 };

	char CFG_TOGGLE_MENU[] = { 'k','e','y','_','T','o','g','g','l','e','M','e','n','u',0 };
	char CFG_TOGGLE_PANIC[] = { 'k','e','y','_','T','o','g','g','l','e','P','a','n','i','c',0 };
	char CFG_NAV_ENTER[] = { 'k','e','y','_','N','a','v','E','n','t','e','r',0 };
	char CFG_NAV_LEAVE[] = { 'k','e','y','_','N','a','v','L','e','a','v','e',0 };
	char CFG_NAV_UP[] = { 'k','e','y','_','N','a','v','U','p',0 };
	char CFG_NAV_DOWN[] = { 'k','e','y','_','N','a','v','D','o','w','n',0 };
	char CFG_NAV_LEFT[] = { 'k','e','y','_','N','a','v','L','e','f','t',0 };
	char CFG_NAV_RIGHT[] = { 'k','e','y','_','N','a','v','R','i','g','h','t',0 };

	char CFG_KEY_MOUSE3[] = { 'M','O','U','S','E','3',0 };

	if ( !bInitPatch )
		InitPatch();

	aim_Deathmatch = ReadInt( CFG_AIMBOT , CFG_AIM_DEATHMATCH , 0 , IniPatch );
	aim_Aimthrough = ReadInt( CFG_AIMBOT , CFG_AIM_THROUGH , 0 , IniPatch );
	aim_Silent = ReadInt( CFG_AIMBOT , CFG_AIM_SILENT , 0 , IniPatch );
	aim_Type = ReadInt( CFG_AIMBOT , CFG_AIM_TYPE , 0 , IniPatch );

	for ( int i = 1; i <= 30; i++ )
	{
		wpn = GetSectionFromIndex( i );
		LoadCvarWeapon();
	}

	aim_SaveSettings = 0;
	aim_LoadSettings = 0;

	trigger_Enable = ReadInt( CFG_TRIGGERBOT , CFG_TRIGGER_ENABLE , 0 , IniPatch );
	trigger_Deathmatch = ReadInt( CFG_TRIGGERBOT , CFG_TRIGGER_DEATHMATCH , 0 , IniPatch );
	trigger_Aimthrough = ReadInt( CFG_TRIGGERBOT , CFG_TRIGGER_AIMTHROUND , 0 , IniPatch );
	trigger_DistanceMin = ReadInt( CFG_TRIGGERBOT , CFG_TRIGGER_DISTANCEMIN , 0 , IniPatch );
	trigger_DistanceMax = ReadInt( CFG_TRIGGERBOT , CFG_TRIGGER_DISTANCEMAX , 5000 , IniPatch );
	trigger_Key = ReadKey( CFG_TRIGGERBOT , CFG_TRIGGER_KEY , CFG_KEY_MOUSE3 , IniPatch , true );
	trigger_KeyMode = ReadInt( CFG_TRIGGERBOT , CFG_TRIGGER_KEYMODE , 2 , IniPatch );
	trigger_OnlyZoom = ReadInt( CFG_TRIGGERBOT , CFG_TRIGGER_ONLYZOOM , 0 , IniPatch );
	
	knf_Active = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_ACTIVE , 0 , IniPatch );
	knf_Aim = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_AIM , 0 , IniPatch );
	knf_AimFov = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_AIM_FOV , 180 , IniPatch );
	knf_Team = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_TEAM , 0 , IniPatch );
	knf_Attack = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_ATTACK , 2 , IniPatch );
	knf_DistAttack = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_DST_ATTACK , 72 , IniPatch );
	knf_DistAttack2 = ReadInt( CFG_KNIFEBOT , CFG_KNIFEBOT_DST_ATTACK2 , 64 , IniPatch );

	esp_Style = ReadInt( CFG_ESP , CFG_ESP_STYLE , 0 , IniPatch );
	esp_Size = ReadInt( CFG_ESP , CFG_ESP_SIZE , 10 , IniPatch );
	esp_Visible = ReadInt( CFG_ESP , CFG_ESP_VISIBLE , 2 , IniPatch );
	esp_Name = ReadInt( CFG_ESP , CFG_ESP_NAME , 0 , IniPatch );
	esp_Weapon = ReadInt( CFG_ESP , CFG_ESP_WEAPON , 0 , IniPatch );
	esp_Sound = ReadInt( CFG_ESP , CFG_ESP_SOUND , 0 , IniPatch );
	esp_SoundType = ReadInt( CFG_ESP , CFG_ESP_SOUND_TYPE , 1 , IniPatch );
	esp_Barrel = ReadInt( CFG_ESP , CFG_ESP_BAREL , 0 , IniPatch );
	esp_WorldWeapons = ReadInt( CFG_ESP , CFG_ESP_WORLD_WPN , 0 , IniPatch );
	esp_WorldSprite = ReadInt( CFG_ESP , CFG_ESP_WORLD_SPR , 0 , IniPatch );
	esp_DrawFont = ReadInt( CFG_ESP , CFG_ESP_DRAW_FONT , 0 , IniPatch );
	esp_DrawAimspot = ReadInt( CFG_ESP , CFG_ESP_DRAWAIMSPOT , 0 , IniPatch );
	esp_DrawFov = ReadInt( CFG_ESP , CFG_ESP_DRAW_FOV , 0 , IniPatch );
	esp_DrawTrigger = ReadInt( CFG_ESP , CFG_ESP_DRAW_TRIGGER , 0 , IniPatch );

	misc_Bhop = ReadInt( CFG_MISC , CFG_MISC_BHOP , 0 , IniPatch );
	misc_Punch = ReadInt( CFG_MISC , CFG_MISC_PUNCH , 0 , IniPatch );
	misc_Lambert = ReadInt( CFG_MISC , CFG_MISC_LAMBERT , 0 , IniPatch );
	misc_FastZoom = ReadInt( CFG_MISC , CFG_MISC_FAST_ZOOM , 0 , IniPatch );
	misc_AntiScreen = ReadInt( CFG_MISC , CFG_MISC_ANTI_SCREEN , 1 , IniPatch );
	misc_FastRun = ReadInt( CFG_MISC , CFG_MISC_FASTRUN , 0 , IniPatch );
	misc_RemoveFlash = ReadInt( CFG_MISC , CFG_MISC_REMOVE_FLASH , 200 , IniPatch );
	misc_FlashMessage = ReadInt( CFG_MISC , CFG_MISC_REMOVE_FLASH_MSG , 0 , IniPatch );

	char CFG_KEY_INS[] = { 'I','N','S',0 };
	char CFG_KEY_DEL[] = { 'D','E','L',0 };
	char CFG_KEY_ENTER[] = { 'E','N','T','E','R',0 };
	char CFG_KEY_BACKSPACE[] = { 'B','A','C','K','S','P','A','C','E',0 };
	char CFG_KEY_UPARROW[] = { 'U','P','A','R','R','O','W',0 };
	char CFG_KEY_DOWNARROW[] = { 'D','O','W','N','A','R','R','O','W',0 };
	char CFG_KEY_LEFTARROW[] = { 'L','E','F','T','A','R','R','O','W',0 };
	char CFG_KEY_RIGHTARROW[] = { 'R','I','G','H','T','A','R','R','O','W',0 };

	key_ToggleMenu = ReadKey( CFG_KEY , CFG_TOGGLE_MENU , CFG_KEY_INS , IniPatch , false );
	key_TogglePanic = ReadKey( CFG_KEY , CFG_TOGGLE_PANIC , CFG_KEY_DEL , IniPatch , false );
	key_NavEnter = ReadKey( CFG_KEY , CFG_NAV_ENTER , CFG_KEY_ENTER , IniPatch , false );
	key_NavLeave = ReadKey( CFG_KEY , CFG_NAV_LEAVE , CFG_KEY_BACKSPACE , IniPatch , false );
	key_NavUp = ReadKey( CFG_KEY , CFG_NAV_UP , CFG_KEY_UPARROW , IniPatch , false );
	key_NavDown = ReadKey( CFG_KEY , CFG_NAV_DOWN , CFG_KEY_DOWNARROW , IniPatch , false );
	key_NavLeft = ReadKey( CFG_KEY , CFG_NAV_LEFT , CFG_KEY_LEFTARROW , IniPatch , false );
	key_NavRight = ReadKey( CFG_KEY , CFG_NAV_RIGHT , CFG_KEY_RIGHTARROW , IniPatch , false );
}

byte cCvar::GetSectionFromIndex( int iIndex )
{
	if ( iIndex == WEAPONLIST_GLOCK18 )
	{
		char CFG_WPN_GLOCK[] = { 'g','l','o','c','k',0 };
		native_strcpy( IniSection , CFG_WPN_GLOCK );
	}
	else if ( iIndex == WEAPONLIST_USP )
	{
		char CFG_WPN_USP[] = { 'u','s','p',0 };
		native_strcpy( IniSection , CFG_WPN_USP );
	}
	else if ( iIndex == WEAPONLIST_DEAGLE )
	{
		char CFG_WPN_DEAGLE[] = { 'd','e','a','g','l','e',0 };
		native_strcpy( IniSection , CFG_WPN_DEAGLE );
	}
	else if ( iIndex == WEAPONLIST_ELITE )
	{
		char CFG_WPN_ELITE[] = { 'e','l','i','t','e',0 };
		native_strcpy( IniSection , CFG_WPN_ELITE );
	}
	else if ( iIndex == WEAPONLIST_FIVESEVEN )
	{
		char CFG_WPN_FIVESEVEN[] = { 'f','i','v','e','s','e','v','e','n',0 };
		native_strcpy( IniSection , CFG_WPN_FIVESEVEN );
	}
	else if ( iIndex == WEAPONLIST_XM1014 )
	{
		char CFG_WPN_XM1014[] = { 'x','m','1','0','1','4',0 };
		native_strcpy( IniSection , CFG_WPN_XM1014 );
	}
	else if ( iIndex == WEAPONLIST_M3 )
	{
		char CFG_WPN_M3[] = { 'm','3',0 };
		native_strcpy( IniSection , CFG_WPN_M3 );
	}
	else if ( iIndex == WEAPONLIST_P228 )
	{
		char CFG_WPN_P228[] = { 'p','2','2','8',0 };
		native_strcpy( IniSection , CFG_WPN_P228 );
	}
	else if ( iIndex == WEAPONLIST_AK47 )
	{
		char CFG_WPN_AK47[] = { 'a','k','4','7',0 };
		native_strcpy( IniSection , CFG_WPN_AK47 );
	}
	else if ( iIndex == WEAPONLIST_M4A1 )
	{
		char CFG_WPN_M4A1[] = { 'm','4','a','1',0 };
		native_strcpy( IniSection , CFG_WPN_M4A1 );
	}
	else if ( iIndex == WEAPONLIST_FAMAS )
	{
		char CFG_WPN_FAMAS[] = { 'f','a','m','a','s',0 };
		native_strcpy( IniSection , CFG_WPN_FAMAS );
	}
	else if ( iIndex == WEAPONLIST_GALIL )
	{
		char CFG_WPN_GALIL[] = { 'g','a','l','i','l',0 };
		native_strcpy( IniSection , CFG_WPN_GALIL );
	}
	else if ( iIndex == WEAPONLIST_SG550 )
	{
		char CFG_WPN_SG550[] = { 's','g','5','5','0',0 };
		native_strcpy( IniSection , CFG_WPN_SG550 );
	}
	else if ( iIndex == WEAPONLIST_AUG )
	{
		char CFG_WPN_AUG[] = { 'a','u','g',0 };
		native_strcpy( IniSection , CFG_WPN_AUG );
	}
	else if ( iIndex == WEAPONLIST_M249 )
	{
		char CFG_WPN_M249[] = { 'm','2','4','9',0 };
		native_strcpy( IniSection , CFG_WPN_M249 );
	}
	else if ( iIndex == WEAPONLIST_MP5 )
	{
		char CFG_WPN_MP5[] = { 'm','p','5',0 };
		native_strcpy( IniSection , CFG_WPN_MP5 );
	}
	else if ( iIndex == WEAPONLIST_P90 )
	{
		char CFG_WPN_P90[] = { 'p','9','0',0 };
		native_strcpy( IniSection , CFG_WPN_P90 );
	}
	else if ( iIndex == WEAPONLIST_MAC10 )
	{
		char CFG_WPN_MAC10[] = { 'm','a','c','1','0',0 };
		native_strcpy( IniSection , CFG_WPN_MAC10 );
	}
	else if ( iIndex == WEAPONLIST_TMP )
	{
		char CFG_WPN_TMP[] = { 't','m','p',0 };
		native_strcpy( IniSection , CFG_WPN_TMP );
	}
	else if ( iIndex == WEAPONLIST_UMP45 )
	{
		char CFG_WPN_UMP45[] = { 'u','m','p','4','5',0 };
		native_strcpy( IniSection , CFG_WPN_UMP45 );
	}
	else if ( iIndex == WEAPONLIST_AWP )
	{
		char CFG_WPN_AWP[] = { 'a','w','p',0 };
		native_strcpy( IniSection , CFG_WPN_AWP );
	}
	else if ( iIndex == WEAPONLIST_SCOUT )
	{
		char CFG_WPN_SCOUT[] = { 's','c','o','u','t',0 };
		native_strcpy( IniSection , CFG_WPN_SCOUT );
	}
	else if ( iIndex == WEAPONLIST_G3SG1 )
	{
		char CFG_WPN_G3SG1[] = { 'g','3','s','g','1',0 };
		native_strcpy( IniSection , CFG_WPN_G3SG1 );
	}
	else if ( iIndex == WEAPONLIST_SG552 )
	{
		char CFG_WPN_SG552[] = { 's','g','5','5','2',0 };
		native_strcpy( IniSection , CFG_WPN_SG552 );
	}

	return iIndex;
}

void cCvar::LoadCvarWeapon()
{
	if ( wpn )
	{
		char CFG_AIM_ACTIVE[] = { 'a','i','m','_','A','c','t','i','v','e',0 };
		char CFG_AIM_SMOOTH[] = { 'a','i','m','_','S','m','o','o','t','h',0 };
		char CFG_AIM_FOV[] = { 'a','i','m','_','F','o','v',0 };
		char CFG_AIM_FOV_TYPE[] = { 'a','i','m','_','F','o','v','T','y','p','e',0 };
		char CFG_AIM_FOV_DIST[] = { 'a','i','m','_','F','o','v','D','i','s','t',0 };
		char CFG_AIM_MODE[] = { 'a','i','m','_','M','o','d','e',0 };
		char CFG_AIM_AIMSPOT[] = { 'a','i','m','_','A','i','m','s','p','o','t',0 };

		weapon[wpn].aim_Active = ReadInt( IniSection , CFG_AIM_ACTIVE , 0 , IniPatch );
		weapon[wpn].aim_Smooth = ReadInt( IniSection , CFG_AIM_SMOOTH , 0 , IniPatch );
		weapon[wpn].aim_Fov = ReadInt( IniSection , CFG_AIM_FOV , 50 , IniPatch );
		weapon[wpn].aim_FovType = ReadInt( IniSection , CFG_AIM_FOV_TYPE , 0 , IniPatch );
		weapon[wpn].aim_FovDist = ReadInt( IniSection , CFG_AIM_FOV_DIST , 500 , IniPatch );
		weapon[wpn].aim_Mode = ReadInt( IniSection , CFG_AIM_MODE , 0 , IniPatch );

		weapon[wpn].aim_Aimspot = ReadInt( IniSection , CFG_AIM_AIMSPOT , 21 , IniPatch );

		if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
		{
			char CFG_AIM_BULLET_CONTROL[] = { 'a','i','m','_','B','u','l','l','e','t','C','o','n','t','r','o','l',0 };
			char CFG_AIM_BULLET_ONE_DISTANCE[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','D','i','s','t','a','n','c','e',0 };
			char CFG_AIM_BULLET_ONE_CLAMPTYPE[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','C','l','a','m','p','T','y','p','e',0 };
			char CFG_AIM_BULLET_ONE_AIMSPOT[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','A','i','m','S','p','o','t',0 };
			char CFG_AIM_BULLET_ONE_SMOOTH[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','S','m','o','o','t','h',0 };
			char CFG_AIM_BULLET_ONE_FOV[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','F','o','v',0 };

			char CFG_AIM_BULLET_TWO_DISTANCE[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','D','i','s','t','a','n','c','e',0 };
			char CFG_AIM_BULLET_TWO_CLAMPTYPE[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','C','l','a','m','p','T','y','p','e',0 };
			char CFG_AIM_BULLET_TWO_AIMSPOT[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','A','i','m','S','p','o','t',0 };
			char CFG_AIM_BULLET_TWO_SMOOTH[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','S','m','o','o','t','h',0 };
			char CFG_AIM_BULLET_TWO_FOV[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','F','o','v',0 };

			weapon[wpn].aim_BulletControl = ReadInt( IniSection , CFG_AIM_BULLET_CONTROL , 1 , IniPatch );
			weapon[wpn].aim_BulletOneDistance = ReadInt( IniSection , CFG_AIM_BULLET_ONE_DISTANCE , 500 , IniPatch );
			weapon[wpn].aim_BulletOneClampType = ReadInt( IniSection , CFG_AIM_BULLET_ONE_CLAMPTYPE , 0 , IniPatch );
			weapon[wpn].aim_BulletOneAimSpot = ReadInt( IniSection , CFG_AIM_BULLET_ONE_AIMSPOT , 21 , IniPatch );
			weapon[wpn].aim_BulletOneSmooth = ReadInt( IniSection , CFG_AIM_BULLET_ONE_SMOOTH , 5 , IniPatch );
			weapon[wpn].aim_BulletOneFov = ReadInt( IniSection , CFG_AIM_BULLET_ONE_FOV , 15 , IniPatch );

			weapon[wpn].aim_BulletTwoDistance = ReadInt( IniSection , CFG_AIM_BULLET_TWO_DISTANCE , 1600 , IniPatch );
			weapon[wpn].aim_BulletTwoClampType = ReadInt( IniSection , CFG_AIM_BULLET_TWO_CLAMPTYPE , 1 , IniPatch );
			weapon[wpn].aim_BulletTwoAimSpot = ReadInt( IniSection , CFG_AIM_BULLET_TWO_AIMSPOT , 21 , IniPatch );
			weapon[wpn].aim_BulletTwoSmooth = ReadInt( IniSection , CFG_AIM_BULLET_TWO_SMOOTH , 5 , IniPatch );
			weapon[wpn].aim_BulletTwoFov = ReadInt( IniSection , CFG_AIM_BULLET_TWO_FOV , 15 , IniPatch );			
		}

		char CFG_AIM_TRIGGER_FOV[] = { 'a','i','m','_','T','r','i','g','g','e','r','F','o','v',0 };
		char CFG_AIM_TRIGGER_HEAD_ONLY[] = { 'a','i','m','_','T','r','i','g','g','e','r','H','e','a','d','O','n','l','y',0 };
		char CFG_AIM_SHOT_DELAY[] = { 'a','i','m','_','S','h','o','t','D','e','l','a','y',0 };
		char CFG_AIM_KILL_DELAY[] = { 'a','i','m','_','K','i','l','l','D','e','l','a','y',0 };
		char CFG_AIM_PREDICTION[] = { 'a','i','m','_','P','r','e','d','i','c','t','i','o','n',0 };
		char CFG_AIM_DISTANCE[] = { 'a','i','m','_','D','i','s','t','a','n','c','e',0 };

		weapon[wpn].aim_TriggerFov = ReadInt( IniSection , CFG_AIM_TRIGGER_FOV , 35 , IniPatch );
		weapon[wpn].aim_TriggerHeadOnly = ReadInt( IniSection , CFG_AIM_TRIGGER_HEAD_ONLY , 1 , IniPatch );

		weapon[wpn].aim_ShotDelay = ReadInt( IniSection , CFG_AIM_SHOT_DELAY , 0 , IniPatch );

		if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
			weapon[wpn].aim_KillDelay = ReadInt( IniSection , CFG_AIM_KILL_DELAY , 0 , IniPatch );

		weapon[wpn].aim_Prediction = ReadInt( IniSection , CFG_AIM_PREDICTION , 0 , IniPatch );
		weapon[wpn].aim_Distance = ReadInt( IniSection , CFG_AIM_DISTANCE , 0 , IniPatch );
	}
}

void cCvar::SaveCvarWeapon()
{
	if ( wpn )
	{
		char CFG_AIM_ACTIVE[] = { 'a','i','m','_','A','c','t','i','v','e',0 };
		char CFG_AIM_SMOOTH[] = { 'a','i','m','_','S','m','o','o','t','h',0 };
		char CFG_AIM_FOV[] = { 'a','i','m','_','F','o','v',0 };
		char CFG_AIM_FOV_TYPE[] = { 'a','i','m','_','F','o','v','T','y','p','e',0 };
		char CFG_AIM_FOV_DIST[] = { 'a','i','m','_','F','o','v','D','i','s','t',0 };
		char CFG_AIM_MODE[] = { 'a','i','m','_','M','o','d','e',0 };
		char CFG_AIM_AIMSPOT[] = { 'a','i','m','_','A','i','m','s','p','o','t',0 };

		SaveInt( IniSection , CFG_AIM_ACTIVE , weapon[wpn].aim_Active , IniPatch );
		SaveInt( IniSection , CFG_AIM_SMOOTH , weapon[wpn].aim_Smooth , IniPatch );
		SaveInt( IniSection , CFG_AIM_FOV , weapon[wpn].aim_Fov , IniPatch );
		SaveInt( IniSection , CFG_AIM_FOV_TYPE , weapon[wpn].aim_FovType , IniPatch );
		SaveInt( IniSection , CFG_AIM_FOV_DIST , weapon[wpn].aim_FovDist , IniPatch );
		SaveInt( IniSection , CFG_AIM_MODE , weapon[wpn].aim_Mode , IniPatch );
		SaveInt( IniSection , CFG_AIM_AIMSPOT , weapon[wpn].aim_Aimspot , IniPatch );

		if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
		{
			char CFG_AIM_BULLET_CONTROL[] = { 'a','i','m','_','B','u','l','l','e','t','C','o','n','t','r','o','l',0 };
			char CFG_AIM_BULLET_ONE_DISTANCE[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','D','i','s','t','a','n','c','e',0 };
			char CFG_AIM_BULLET_ONE_CLAMPTYPE[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','C','l','a','m','p','T','y','p','e',0 };
			char CFG_AIM_BULLET_ONE_AIMSPOT[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','A','i','m','S','p','o','t',0 };
			char CFG_AIM_BULLET_ONE_SMOOTH[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','S','m','o','o','t','h',0 };
			char CFG_AIM_BULLET_ONE_FOV[] = { 'a','i','m','_','B','u','l','l','e','t','O','n','e','F','o','v',0 };

			char CFG_AIM_BULLET_TWO_DISTANCE[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','D','i','s','t','a','n','c','e',0 };
			char CFG_AIM_BULLET_TWO_CLAMPTYPE[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','C','l','a','m','p','T','y','p','e',0 };
			char CFG_AIM_BULLET_TWO_AIMSPOT[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','A','i','m','S','p','o','t',0 };
			char CFG_AIM_BULLET_TWO_SMOOTH[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','S','m','o','o','t','h',0 };
			char CFG_AIM_BULLET_TWO_FOV[] = { 'a','i','m','_','B','u','l','l','e','t','T','w','o','F','o','v',0 };

			SaveInt( IniSection , CFG_AIM_BULLET_CONTROL , weapon[wpn].aim_BulletControl , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_ONE_DISTANCE , weapon[wpn].aim_BulletOneDistance , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_ONE_CLAMPTYPE , weapon[wpn].aim_BulletOneClampType , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_ONE_AIMSPOT , weapon[wpn].aim_BulletOneAimSpot , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_ONE_SMOOTH , weapon[wpn].aim_BulletOneSmooth , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_ONE_FOV , weapon[wpn].aim_BulletOneFov , IniPatch );

			SaveInt( IniSection , CFG_AIM_BULLET_TWO_DISTANCE , weapon[wpn].aim_BulletTwoDistance , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_TWO_CLAMPTYPE , weapon[wpn].aim_BulletTwoClampType , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_TWO_AIMSPOT , weapon[wpn].aim_BulletTwoAimSpot , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_TWO_SMOOTH , weapon[wpn].aim_BulletTwoSmooth , IniPatch );
			SaveInt( IniSection , CFG_AIM_BULLET_TWO_FOV , weapon[wpn].aim_BulletTwoFov , IniPatch );
		}

		char CFG_AIM_TRIGGER_FOV[] = { 'a','i','m','_','T','r','i','g','g','e','r','F','o','v',0 };
		char CFG_AIM_TRIGGER_HEAD_ONLY[] = { 'a','i','m','_','T','r','i','g','g','e','r','H','e','a','d','O','n','l','y',0 };
		char CFG_AIM_SHOT_DELAY[] = { 'a','i','m','_','S','h','o','t','D','e','l','a','y',0 };
		char CFG_AIM_KILL_DELAY[] = { 'a','i','m','_','K','i','l','l','D','e','l','a','y',0 };
		char CFG_AIM_PREDICTION[] = { 'a','i','m','_','P','r','e','d','i','c','t','i','o','n',0 };
		char CFG_AIM_DISTANCE[] = { 'a','i','m','_','D','i','s','t','a','n','c','e',0 };

		SaveInt( IniSection , CFG_AIM_TRIGGER_FOV , weapon[wpn].aim_TriggerFov , IniPatch );
		SaveInt( IniSection , CFG_AIM_TRIGGER_HEAD_ONLY , weapon[wpn].aim_TriggerHeadOnly , IniPatch );
		
		SaveInt( IniSection , CFG_AIM_SHOT_DELAY , weapon[wpn].aim_ShotDelay , IniPatch );

		if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
			SaveInt( IniSection , CFG_AIM_KILL_DELAY , weapon[wpn].aim_KillDelay , IniPatch );

		SaveInt( IniSection , CFG_AIM_PREDICTION , weapon[wpn].aim_Prediction , IniPatch );
		SaveInt( IniSection , CFG_AIM_DISTANCE , weapon[wpn].aim_Distance , IniPatch );
	}
}

void cCvar::DisableAll()
{
	if ( wpn )
	{
		weapon[wpn].aim_Active = 0;
	}

	esp_Style = 0;
	esp_Name = 0;
	esp_Weapon = 0;
	esp_Sound = 0;
	esp_Barrel = 0;
	esp_WorldWeapons = 0;
	esp_WorldSprite = 0;
	esp_DrawAimspot = 0;
	esp_DrawFov = 0;
	esp_DrawTrigger = 0;

	knf_Active = 0;

	misc_Bhop = 0;
	misc_Punch = 0;
	misc_Lambert = 0;
	misc_FastZoom = 0;
	misc_FastRun = 0;
	misc_RemoveFlash = 0;
	misc_FlashMessage = 0;
}

int cCvar::ReadInt( char* szSection , char* szKey , int iDefaultValue , char* Patch )
{
	for ( DWORD i = 0; i < MAX_PATH; i++ )
		IniPatch[i] ^= INI_PATH_KEY;

	int ret =  GetPrivateProfileIntA( szSection , szKey , iDefaultValue , Patch );

	for ( DWORD i = 0; i < MAX_PATH; i++ )
		IniPatch[i] ^= INI_PATH_KEY;

	return ret;
}

void cCvar::SaveInt( char* szSection , char* szKey , int iValue , char* Patch )
{
	IniValue = native_itoa( iValue );

	for ( DWORD i = 0; i < MAX_PATH; i++ )
		IniPatch[i] ^= INI_PATH_KEY;

	WritePrivateProfileStringA( szSection , szKey , IniValue , Patch );

	for ( DWORD i = 0; i < MAX_PATH; i++ )
		IniPatch[i] ^= INI_PATH_KEY;
}

int cCvar::ReadKey( char* szSection , char* szKey , char* cDefaultValue , char* Patch , bool window )
{
	for ( DWORD i = 0; i < MAX_PATH; i++ )
		IniPatch[i] ^= INI_PATH_KEY;

	char cTempString[INT_DIGITS];
	GetPrivateProfileStringA( szSection , szKey , cDefaultValue , cTempString , 16 , Patch );

	for ( DWORD i = 0; i < MAX_PATH; i++ )
		IniPatch[i] ^= INI_PATH_KEY;

	if ( !window )
		return CsCharToKey( cTempString );
	else
		return WindowCharToKey( cTempString );
}