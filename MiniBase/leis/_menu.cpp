#include "_menu.h"

cMenu g_Menu;

cMenu::cMenu()
{
	hHeap = HeapCreate( 0 , INT_DIGITS , 0 );
	pValue = (PCHAR)HeapAlloc( hHeap , HEAP_ZERO_MEMORY , INT_DIGITS );

	Visible = false;
	SubMenu = false;

	M_EntryPos = 0;
	M_EntrySize = 0;

	S_EntryPos = 0;
	S_EntrySize = 0;

	dwLastSaveTime = 0;
	dwLastLoadTime = 0;

	ClearMenu();
	
	DisableReload = false;
}

void cMenu::InitializeMenu()
{
	char CFG_AIMBOT_SETTINGS[] = { 'A','i','m','b','o','t',' ','S','e','t','t','i','n','g','s',0 };
	char CFG_TRIGGERBOT_SETTINGS[] = { 'T','r','i','g','g','e','r','b','o','t',' ','S','e','t','t','i','n','g','s',0 };
	char CFG_WEAPON_SETTINGS[] = { 'W','e','a','p','o','n',' ','S','e','t','t','i','n','g','s',0 };
	char CFG_KNIFEBOT_SETTINGS[] = { 'K','n','i','f','e','b','o','t',' ','S','e','t','t','i','n','g','s',0 };
	char CFG_ESP_SETTINGS[] = { 'E','S','P',' ','S','e','t','t','i','n','g','s',0 };
	char CFG_MISC_SETTINGS[] = { 'M','i','s','c',' ','S','e','t','t','i','n','g','s',0 };

	int SelectIndex = 0;

	ClearMenu();
	
	if ( cvar.wpn )
	{
		AddMenu( CFG_AIMBOT_SETTINGS );
		AddMenu( CFG_WEAPON_SETTINGS );
	}

	AddMenu( CFG_TRIGGERBOT_SETTINGS );
	AddMenu( CFG_KNIFEBOT_SETTINGS );
	AddMenu( CFG_ESP_SETTINGS );
	AddMenu( CFG_MISC_SETTINGS );

	if ( M_EntryPos > M_EntrySize )
		M_EntryPos = M_EntrySize - 1;

	if ( cvar.wpn )
	{
		char CFG_AIM_DEATHMATCH[] = { 'a','i','m','_','D','e','a','t','h','m','a','t','c','h',0 };
		char CFG_AIM_THROUGH[] = { 'a','i','m','_','A','i','m','t','h','r','o','u','g','h',0 };
		char CFG_AIM_SILENT[] = { 'a','i','m','_','S','i','l','e','n','t',0 };
		char CFG_AIM_TYPE[] = { 'a','i','m','_','T','y','p','e',0 };

		char CFG_AIM_SAVESETTINGS[] = { 'a','i','m','_','S','a','v','e','S','e','t','t','i','n','g','s',0 };
		char CFG_AIM_LOADSETTINGS[] = { 'a','i','m','_','L','o','a','d','S','e','t','t','i','n','g','s',0 };

		char CFG_AIM_ACTIVE[] = { 'a','i','m','_','A','c','t','i','v','e',0 };
		char CFG_AIM_SMOOTH[] = { 'a','i','m','_','S','m','o','o','t','h',0 };
		char CFG_AIM_FOV[] = { 'a','i','m','_','F','o','v',0 };
		char CFG_AIM_FOV_TYPE[] = { 'a','i','m','_','F','o','v','T','y','p','e',0 };
		char CFG_AIM_FOV_DIST[] = { 'a','i','m','_','F','o','v','D','i','s','t',0 };
		char CFG_AIM_MODE[] = { 'a','i','m','_','M','o','d','e',0 };
		char CFG_AIM_AIMSPOT[] = { 'a','i','m','_','A','i','m','s','p','o','t',0 };

		if ( M_EntryPos == SelectIndex++ )
		{
			AddSubMenu( CFG_AIM_DEATHMATCH , &cvar.aim_Deathmatch , 0 , 1 , 1 );
			AddSubMenu( CFG_AIM_THROUGH , &cvar.aim_Aimthrough , 0 , 1 , 1 );
			AddSubMenu( CFG_AIM_SILENT , &cvar.aim_Silent , 0 , 1 , 1 );
			AddSubMenu( CFG_AIM_TYPE , &cvar.aim_Type , 0 , 1 , 1 );
			AddSubMenu( CFG_AIM_SAVESETTINGS , &cvar.aim_SaveSettings , 0 , 1 , 1 );
			AddSubMenu( CFG_AIM_LOADSETTINGS , &cvar.aim_LoadSettings , 0 , 1 , 1 );
		}
		if ( M_EntryPos == SelectIndex++ )
		{
			AddSubMenu( CFG_AIM_ACTIVE , &cvar.weapon[cvar.wpn].aim_Active , 0 , 1 , 1 );
			AddSubMenu( CFG_AIM_SMOOTH , &cvar.weapon[cvar.wpn].aim_Smooth , 0 , 180 , 1 );
			AddSubMenu( CFG_AIM_FOV , &cvar.weapon[cvar.wpn].aim_Fov , 1 , 180 , 1 );
			AddSubMenu( CFG_AIM_FOV_TYPE , &cvar.weapon[cvar.wpn].aim_FovType , 0 , 2 , 1 );
			AddSubMenu( CFG_AIM_FOV_DIST , &cvar.weapon[cvar.wpn].aim_FovDist , 5 , 7000 , 5 );
			AddSubMenu( CFG_AIM_MODE , &cvar.weapon[cvar.wpn].aim_Mode , 0 , 2 , 1 );

			AddSubMenu( CFG_AIM_AIMSPOT , &cvar.weapon[cvar.wpn].aim_Aimspot , 0, 22 , 1 );

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

				AddSubMenu( CFG_AIM_BULLET_CONTROL , &cvar.weapon[cvar.wpn].aim_BulletControl , 0 , 10 , 1 );
				AddSubMenu( CFG_AIM_BULLET_ONE_DISTANCE , &cvar.weapon[cvar.wpn].aim_BulletOneDistance , 0 , 5000 , 10 );
				AddSubMenu( CFG_AIM_BULLET_ONE_CLAMPTYPE , &cvar.weapon[cvar.wpn].aim_BulletOneClampType , 0 , 2 , 1 );
				AddSubMenu( CFG_AIM_BULLET_ONE_AIMSPOT , &cvar.weapon[cvar.wpn].aim_BulletOneAimSpot , 0 , 22 , 1 );
				AddSubMenu( CFG_AIM_BULLET_ONE_SMOOTH , &cvar.weapon[cvar.wpn].aim_BulletOneSmooth , 0 , 180 , 1 );
				AddSubMenu( CFG_AIM_BULLET_ONE_FOV , &cvar.weapon[cvar.wpn].aim_BulletOneFov , 1 , 180 , 1 );

				AddSubMenu( CFG_AIM_BULLET_TWO_DISTANCE , &cvar.weapon[cvar.wpn].aim_BulletTwoDistance , 0 , 5000 , 10 );
				AddSubMenu( CFG_AIM_BULLET_TWO_CLAMPTYPE , &cvar.weapon[cvar.wpn].aim_BulletTwoClampType , 0 , 2 , 1 );
				AddSubMenu( CFG_AIM_BULLET_TWO_AIMSPOT , &cvar.weapon[cvar.wpn].aim_BulletTwoAimSpot , 0 , 22 , 1 );
				AddSubMenu( CFG_AIM_BULLET_TWO_SMOOTH , &cvar.weapon[cvar.wpn].aim_BulletTwoSmooth , 0 , 180 , 1 );
				AddSubMenu( CFG_AIM_BULLET_TWO_FOV , &cvar.weapon[cvar.wpn].aim_BulletTwoFov , 1 , 180 , 1 );
			}

			char CFG_AIM_TRIGGER_FOV[] = { 'a','i','m','_','T','r','i','g','g','e','r','F','o','v',0 };
			char CFG_AIM_TRIGGER_HEAD_ONLY[] = { 'a','i','m','_','T','r','i','g','g','e','r','H','e','a','d','O','n','l','y',0 };
			char CFG_AIM_SHOT_DELAY[] = { 'a','i','m','_','S','h','o','t','D','e','l','a','y',0 };
			char CFG_AIM_KILL_DELAY[] = { 'a','i','m','_','K','i','l','l','D','e','l','a','y',0 };
			char CFG_AIM_PREDICTION[] = { 'a','i','m','_','P','r','e','d','i','c','t','i','o','n',0 };
			char CFG_AIM_DISTANCE[] = { 'a','i','m','_','D','i','s','t','a','n','c','e',0 };

			AddSubMenu( CFG_AIM_TRIGGER_FOV , &cvar.weapon[cvar.wpn].aim_TriggerFov , 1 , 90 , 1 );
			AddSubMenu( CFG_AIM_TRIGGER_HEAD_ONLY , &cvar.weapon[cvar.wpn].aim_TriggerHeadOnly , 0 , 1 , 1 );

			AddSubMenu( CFG_AIM_SHOT_DELAY , &cvar.weapon[cvar.wpn].aim_ShotDelay , 0 , 3000 , 1 );

			if ( !g_Local.bPistolWeapon && !g_Local.bShotGunWeapon && !g_Local.bSniperWeapon )
				AddSubMenu( CFG_AIM_KILL_DELAY , &cvar.weapon[cvar.wpn].aim_KillDelay , 0 , 10000 , 10 );

			AddSubMenu( CFG_AIM_PREDICTION , &cvar.weapon[cvar.wpn].aim_Prediction , -2000 , 2000 , 10 );
			AddSubMenu( CFG_AIM_DISTANCE , &cvar.weapon[cvar.wpn].aim_Distance , 0 , 1 , 1 );
		}
	}
	if ( M_EntryPos == SelectIndex++ )
	{
		char CFG_TRIGGER_ENABLE[] = { 't','r','i','g','g','e','r','_','E','n','a','b','l','e',0 };
		char CFG_TRIGGER_DEATHMATCH[] = { 't','r','i','g','g','e','r','_','D','e','a','t','h','m','a','t','c','h',0 };
		char CFG_TRIGGER_AIMTHROUND[] = { 't','r','i','g','g','e','r','_','A','i','m','t','h','r','o','u','g','h',0 };
		char CFG_TRIGGER_DISTANCEMIN[] = { 't','r','i','g','g','e','r','_','D','i','s','t','a','n','c','e','M','i','n',0 };
		char CFG_TRIGGER_DISTANCEMAX[] = { 't','r','i','g','g','e','r','_','D','i','s','t','a','n','c','e','M','a','x',0 };
		char CFG_TRIGGER_KEY[] = { 't','r','i','g','g','e','r','_','K','e','y',0 };
		char CFG_TRIGGER_KEYMODE[] = { 't','r','i','g','g','e','r','_','K','e','y','M','o','d','e',0 };
		char CFG_TRIGGER_ONLYZOOM[] = { 't','r','i','g','g','e','r','_','O','n','l','y','Z','o','o','m',0 };

		AddSubMenu( CFG_TRIGGER_ENABLE , &cvar.trigger_Enable , 0 , 1 , 1 );
		AddSubMenu( CFG_TRIGGER_DEATHMATCH , &cvar.trigger_Deathmatch , 0 , 1 , 1 );
		AddSubMenu( CFG_TRIGGER_AIMTHROUND , &cvar.trigger_Aimthrough , 0 , 1 , 1 );
		AddSubMenu( CFG_TRIGGER_DISTANCEMIN , &cvar.trigger_DistanceMin , 0 , 5000 , 1 );
		AddSubMenu( CFG_TRIGGER_DISTANCEMAX , &cvar.trigger_DistanceMax , 0 , 5000 , 1 );
		AddSubMenu( CFG_TRIGGER_KEYMODE , &cvar.trigger_KeyMode , 1 , 2 , 1 );
		AddSubMenu( CFG_TRIGGER_ONLYZOOM , &cvar.trigger_OnlyZoom , 0 , 1 , 1 );
	}
	if ( M_EntryPos == SelectIndex++ )
	{
		char CFG_KNIFEBOT_ACTIVE[] = { 'k','n','f','_','A','c','t','i','v','e',0 };
		char CFG_KNIFEBOT_AIM[] = { 'k','n','f','_','A','i','m',0 };
		char CFG_KNIFEBOT_AIM_FOV[] = { 'k','n','f','_','A','i','m','F','o','v',0 };
		char CFG_KNIFEBOT_TEAM[] = { 'k','n','f','_','T','e','a','m',0 };
		char CFG_KNIFEBOT_ATTACK[] = { 'k','n','f','_','A','t','t','a','c','k',0 };
		char CFG_KNIFEBOT_DST_ATTACK[] = { 'k','n','f','_','D','i','s','t','A','t','t','a','c','k',0 };
		char CFG_KNIFEBOT_DST_ATTACK2[] = { 'k','n','f','_','D','i','s','t','A','t','t','a','c','k','2',0 };

		AddSubMenu( CFG_KNIFEBOT_ACTIVE , &cvar.knf_Active , 0 , 1 , 1 );
		AddSubMenu( CFG_KNIFEBOT_AIM , &cvar.knf_Aim , 0 , 1 , 1 );
		AddSubMenu( CFG_KNIFEBOT_AIM_FOV , &cvar.knf_AimFov , 90 , 360 , 1 );
		AddSubMenu( CFG_KNIFEBOT_TEAM , &cvar.knf_Team , 0 , 1 , 1 );
		AddSubMenu( CFG_KNIFEBOT_ATTACK , &cvar.knf_Attack , 1 , 2 , 1 );
		AddSubMenu( CFG_KNIFEBOT_DST_ATTACK , &cvar.knf_DistAttack , 1 , 100 , 1 );
		AddSubMenu( CFG_KNIFEBOT_DST_ATTACK2 , &cvar.knf_DistAttack2 , 1 , 100 , 1 );
	}
	if ( M_EntryPos == SelectIndex++ )
	{
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

		AddSubMenu( CFG_ESP_STYLE , &cvar.esp_Style , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_SIZE , &cvar.esp_Size , 1 , 20 , 1 );
		AddSubMenu( CFG_ESP_VISIBLE , &cvar.esp_Visible , 0 , 2 , 1 );
		AddSubMenu( CFG_ESP_NAME , &cvar.esp_Name , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_WEAPON , &cvar.esp_Weapon , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_SOUND , &cvar.esp_Sound , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_SOUND_TYPE , &cvar.esp_SoundType , 1 , 2 , 1 );
		AddSubMenu( CFG_ESP_BAREL , &cvar.esp_Barrel , 0 , 5000 , 100 );
		AddSubMenu( CFG_ESP_WORLD_WPN , &cvar.esp_WorldWeapons , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_WORLD_SPR , &cvar.esp_WorldSprite , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_DRAW_FONT , &cvar.esp_DrawFont , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_DRAWAIMSPOT , &cvar.esp_DrawAimspot , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_DRAW_FOV , &cvar.esp_DrawFov , 0 , 1 , 1 );
		AddSubMenu( CFG_ESP_DRAW_TRIGGER , &cvar.esp_DrawTrigger , 0 , 1 , 1 );
	}
	if ( M_EntryPos == SelectIndex++ )
	{
		char CFG_MISC_BHOP[] = { 'm','i','s','c','_','B','h','o','p',0 };
		char CFG_MISC_PUNCH[] = { 'm','i','s','c','_','P','u','n','c','h',0 };
		char CFG_MISC_LAMBERT[] = { 'm','i','s','c','_','L','a','m','b','e','r','t',0 };
		char CFG_MISC_FAST_ZOOM[] = { 'm','i','s','c','_','F','a','s','t','Z','o','o','m',0 };
		char CFG_MISC_ANTI_SCREEN[] = { 'm','i','s','c','_','A','n','t','i','S','c','r','e','e','n',0 };
		char CFG_MISC_FASTRUN[] = { 'm','i','s','c','_','F','a','s','t','R','u','n',0 };
		char CFG_MISC_REMOVE_FLASH[] = { 'm','i','s','c','_','R','e','m','o','v','e','F','l','a','s','h',0 };
		char CFG_MISC_REMOVE_FLASH_MSG[] = { 'm','i','s','c','_','F','l','a','s','h','M','e','s','s','a','g','e',0 };

		AddSubMenu( CFG_MISC_BHOP , &cvar.misc_Bhop , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_PUNCH , &cvar.misc_Punch , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_LAMBERT , &cvar.misc_Lambert , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_FAST_ZOOM , &cvar.misc_FastZoom , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_ANTI_SCREEN , &cvar.misc_AntiScreen , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_FASTRUN , &cvar.misc_FastRun , 0 , 1 , 1 );
		AddSubMenu( CFG_MISC_REMOVE_FLASH , &cvar.misc_RemoveFlash , 0 , 255 , 1 );
		AddSubMenu( CFG_MISC_REMOVE_FLASH_MSG , &cvar.misc_FlashMessage , 0 , 1 , 1 );
	}
}

void cMenu::DrawMenu( int x , int y )
{
	if ( !Visible )
	{
		dwLastSaveTime = 0;
		dwLastLoadTime = 0;

		cvar.aim_SaveSettings = 0;
		cvar.aim_LoadSettings = 0;

		native_memset( subm_entry , sizeof( subm_entry ) , 0 );
		native_memset( menu_entry , sizeof( menu_entry ) , 0 );

		return;
	}

	InitializeMenu();

	char CFG_MENU_ON[] = { 'o','n',0 };
	char CFG_MENU_OFF[] = { 'o','f','f',0 };

	menu_head_width = SUBM_ENTRY_WIDTH + MENU_ENTRY_WIDTH + SUBM_PADDING_LEFT;

	x -= ( menu_head_width / 2 );

	g_Drawing.DrawBox( x , y , menu_head_width , ENTRY_HEIGHT , 1 , 20 , 20 , 20 , 255 );
	g_Drawing.FillArea( x + 1 , y + 1 , menu_head_width - 1 , ENTRY_HEIGHT - 1 , 20 , 20 , 20 , 170 );

	if ( cvar.aim_SaveSettings && !dwLastSaveTime )
	{
		dwLastSaveTime = GetTickCount() + 1500;
		cvar.SaveCvarWeapon();
	}
	else if ( dwLastSaveTime > GetTickCount() && !dwLastLoadTime )
	{
		char cfg_leis_saved[] = { 'S','e','t','t','i','n','g','s',' ','S','a','v','e','d',0 };

		Verdana1.Print( x + ( menu_head_width / 2 ) , y , 0 , 255 , 0 , 255 , FL_CENTER , cfg_leis_saved );
	}
	else
	{
		dwLastSaveTime = 0;
		cvar.aim_SaveSettings = 0;
	}

	if ( cvar.aim_LoadSettings && !dwLastLoadTime )
	{
		dwLastLoadTime = GetTickCount() + 1500;
		cvar.LoadCvarWeapon();
	}
	else if ( dwLastLoadTime > GetTickCount() && !dwLastSaveTime )
	{
		char cfg_leis_loaded[] = { 'S','e','t','t','i','n','g','s',' ','L','o','a','d','e','d',0 };

		Verdana1.Print( x + ( menu_head_width / 2 ) , y , 0 , 255 , 0 , 255 , FL_CENTER , cfg_leis_loaded );
	}
	else
	{
		dwLastLoadTime = 0;
		cvar.aim_LoadSettings = 0;
	}

	if ( !dwLastSaveTime && !dwLastLoadTime )
	{
		char cfg_verm_message[] = { 'V','e','r','m','i','l','l','i','o','n',' ','H','a','c','k',0 };

		Verdana1.Print( x + ( menu_head_width / 2 ) , y , 255 , 64 , 64 , 255 , FL_CENTER , cfg_verm_message );
	}
	
	y += ENTRY_HEIGHT + MENU_PADDING_TOP;

	g_Drawing.DrawBox( x , y , MENU_ENTRY_WIDTH , ENTRY_HEIGHT * M_EntrySize + 2 , 1 , 20 , 20 , 20 , 255 );
	g_Drawing.FillArea( x + 1 , y + 1 , MENU_ENTRY_WIDTH - 1 , ENTRY_HEIGHT * M_EntrySize , 20 , 20 , 20 , 170 );

	for ( int i = 0; i < M_EntrySize; i++ )
	{
		if ( i == M_EntryPos )
		{
			if ( !SubMenu )
				g_Drawing.FillArea( x + 1 , y + 1 + ENTRY_HEIGHT * i , MENU_ENTRY_WIDTH - 2 , ENTRY_HEIGHT , 100 , 100 , 100 , 120 );
		}

		for ( byte a = 0; a < sizeof( m_entry::name ); a++ )
			menu_entry[i].name[a] ^= MENU_ENTRY_KEY;

		Verdana1.Print( x + TEXT_PADDING_LEFT , y + ENTRY_HEIGHT * i , 255 , 64 , 64 , 255 , 0 , menu_entry[i].name );

		for ( byte a = 0; a < sizeof( m_entry::name ); a++ )
			menu_entry[i].name[a] ^= MENU_ENTRY_KEY;
	}

	int subm_x = x + MENU_ENTRY_WIDTH + SUBM_PADDING_LEFT;

	g_Drawing.DrawBox( subm_x , y , SUBM_ENTRY_WIDTH , ENTRY_HEIGHT * S_EntrySize + 2 , 1 , 20 , 20 , 20 , 255 );
	g_Drawing.FillArea( subm_x + 1 , y + 1 , SUBM_ENTRY_WIDTH - 1 , ENTRY_HEIGHT * S_EntrySize , 20 , 20 , 20 , 170 );

	if ( S_EntryPos > ( S_EntrySize - 1 ) )
		S_EntryPos = ( S_EntrySize - 1 );

	for ( int i = 0; i < S_EntrySize; i++ )
	{
		BYTE r , g , b;

		r = 255; g = 64; b = 64;

		if ( i == S_EntryPos )
		{
			if ( SubMenu )
				g_Drawing.FillArea( subm_x + 1 , y + 1 + ENTRY_HEIGHT * i , SUBM_ENTRY_WIDTH - 2 , ENTRY_HEIGHT , 100 , 100 , 100 , 120 );
		}

		for ( byte a = 0; a < sizeof( s_entry::name ); a++ )
			subm_entry[i].name[a] ^= SUBM_ENTRY_KEY;

		if ( subm_entry[i].min == 0 && subm_entry[i].max == 1 )
		{
			Verdana1.Print( subm_x + TEXT_PADDING_LEFT , y + ENTRY_HEIGHT * i , r , g , b , 255 , 0 , subm_entry[i].name );

			if ( *subm_entry[i].value == 0 )
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r , g , b , 255 , 0 , CFG_MENU_OFF );
			else
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r , g , b , 255 , 0 , CFG_MENU_ON );
		}
		else
		{
			Verdana1.Print( subm_x + TEXT_PADDING_LEFT , y + ENTRY_HEIGHT * i , r , g , b , 255 , 0 , subm_entry[i].name );

			if ( *subm_entry[i].value == 0 )
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r , g , b , 255 , 0 , CFG_MENU_OFF );
			else
			{
				pValue = native_itoa( *subm_entry[i].value );
				Verdana1.Print( subm_x + SUBM_ENTRY_WIDTH - SUBM_VALUE_PADDING , y + ENTRY_HEIGHT * i , r , g , b , 255 , 0 , pValue );
			}
		}

		for ( byte a = 0; a < sizeof( s_entry::name ); a++ )
			subm_entry[i].name[a] ^= SUBM_ENTRY_KEY;
	}
}

void cMenu::KeyEvent( int iKeynum )
{
	if ( iKeynum == cvar.key_ToggleMenu )
		Visible = !Visible;

	if ( iKeynum == cvar.key_TogglePanic && !DisableReload )
	{
		cvar.DisableAll();
		DisableReload = true;
		Visible = false;
	}
	else if ( iKeynum == cvar.key_TogglePanic && DisableReload )
	{
		cvar.LoadCvarFromIni();
		DisableReload = false;
	}

	ExitMenu = false;

	if ( !Visible )
		return;

	if ( iKeynum == cvar.key_NavEnter && !SubMenu )
	{
		S_EntryPos = 0;
		SubMenu = true;
	}
	else if ( iKeynum == cvar.key_NavLeave && !SubMenu )
	{
		Visible = false;
		ExitMenu = true;
	}
	else if ( iKeynum == cvar.key_NavLeave && SubMenu )
	{
		SubMenu = false;
	}
	else if ( iKeynum == cvar.key_NavUp && !SubMenu )
	{
		if ( M_EntryPos > 0 )
			M_EntryPos--;
		else
			M_EntryPos = M_EntrySize - 1;
	}
	else if ( iKeynum == cvar.key_NavDown && !SubMenu )
	{
		if ( M_EntryPos < ( M_EntrySize - 1 ) )
			M_EntryPos++;
		else
			M_EntryPos = 0;
	}
	else if ( iKeynum == cvar.key_NavUp && SubMenu )
	{
		if ( S_EntryPos > 0 )
			S_EntryPos--;
		else
			S_EntryPos = S_EntrySize - 1;
	}
	else if ( iKeynum == cvar.key_NavDown && SubMenu )
	{
		if ( S_EntryPos < ( S_EntrySize - 1 ) )
			S_EntryPos++;
		else
			S_EntryPos = 0;
	}
	else if ( iKeynum == cvar.key_NavLeft && SubMenu )
	{
		*subm_entry[S_EntryPos].value -= subm_entry[S_EntryPos].step;

		if ( *subm_entry[S_EntryPos].value < subm_entry[S_EntryPos].min )
			*subm_entry[S_EntryPos].value = subm_entry[S_EntryPos].min;
	}
	else if ( iKeynum == cvar.key_NavRight && SubMenu )
	{
		*subm_entry[S_EntryPos].value += subm_entry[S_EntryPos].step;

		if ( *subm_entry[S_EntryPos].value > subm_entry[S_EntryPos].max )
			*subm_entry[S_EntryPos].value = subm_entry[S_EntryPos].max;
	}
}

void cMenu::AddMenu( char* name )
{
	native_memset( menu_entry[M_EntrySize].name , sizeof( m_entry::name ) , 0 );
	native_strcpy( menu_entry[M_EntrySize].name , name );

	for ( byte i = 0; i < sizeof( m_entry::name ); i++ )
		menu_entry[M_EntrySize].name[i] ^= MENU_ENTRY_KEY;

	M_EntrySize++;
}

void cMenu::AddSubMenu( char* name , int* value , int min , int max , int step )
{
	native_memset( subm_entry[S_EntrySize].name , sizeof( s_entry::name ) , 0 );
	native_strcpy( subm_entry[S_EntrySize].name , name );

	for ( byte i = 0; i < sizeof( s_entry::name ); i++ )
		subm_entry[S_EntrySize].name[i] ^= SUBM_ENTRY_KEY;

	subm_entry[S_EntrySize].value = value;
	subm_entry[S_EntrySize].min = min;
	subm_entry[S_EntrySize].max = max;
	subm_entry[S_EntrySize].step = step;

	S_EntrySize++;
}

void cMenu::ClearMenu()
{
	for ( byte i = 0; i < M_EntrySize; i++ )
	{
		native_memset( &menu_entry[i].name , sizeof( m_entry::name ) , 0 );
	}

	for ( byte i = 0; i < S_EntrySize; i++ )
	{
		native_memset( &subm_entry[i].name , sizeof( s_entry::name ) , 0 );

		subm_entry[i].value = 0;
		subm_entry[i].min = 0;
		subm_entry[i].max = 0;
		subm_entry[i].step = 0;
	}

	M_EntrySize = 0;
	S_EntrySize = 0;
}