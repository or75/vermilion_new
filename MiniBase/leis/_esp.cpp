#include "_esp.h"

cEsp g_Esp;

void cEsp::DrawNoFlash()
{
	if ( cvar.misc_RemoveFlash > 0 )
	{
		g_Engine.pfnGetScreenFade( &noflash.pScreenFade );

		if ( noflash.pScreenFade.fadeEnd > g_Engine.GetClientTime() )
		{
			noflash.FadeEnd = noflash.pScreenFade.fadeEnd;

			if ( cvar.misc_RemoveFlash >= 255 )
				noflash.pScreenFade.fadeEnd = 255.0f;
			else if ( noflash.pScreenFade.fadealpha > cvar.misc_RemoveFlash )
				noflash.pScreenFade.fadealpha = cvar.misc_RemoveFlash;
		}

		if ( noflash.FadeEnd > g_Engine.GetClientTime() )
			noflash.Flashed = ( noflash.FadeEnd - g_Engine.GetClientTime() ) * noflash.pScreenFade.fadeSpeed;
		else
			noflash.Flashed = 0.0f;

		if ( noflash.Flashed > 255.0f ) noflash.Flashed = 255.0f;
		if ( noflash.Flashed < 0.0f ) noflash.Flashed = 0.0f;

		float Percentage = ( noflash.Flashed / 255.0f ) * 100.0f;

		g_Engine.pfnSetScreenFade( &noflash.pScreenFade );

		if ( Percentage > 0 && cvar.misc_FlashMessage )
		{
			byte r , g , b;
			float alpha = Percentage / 100.f * 255.f;

			if ( Percentage >= 90.0 )
			{
				r = 255 , g = 0 , b = 0;
			}
			else if ( Percentage >= 70.0 && Percentage < 90 )
			{
				r = 255 , g = 255 , b = 0;
			}
			else if ( Percentage < 70.0 )
			{
				r = 0 , g = 255 , b = 0;
			}

			int flash_w = 100;
			int flash_h = 8;
			int flash_box_y = g_Screen.iHeight - 50;
			int flash_box_x = g_Screen.iWidth / 2;

			g_Drawing.DrawBox( flash_box_x - flash_w - 1 , flash_box_y - flash_h - 1 , flash_w * 2 + 2 , flash_h * 2 + 2 , 1 , 0 , 0 , 0 , (int)alpha );
			g_Drawing.FillArea( flash_box_x - flash_w , flash_box_y - flash_h , flash_w * 2 , flash_h * 2 , r , g , b , (int)alpha );

			char FlashMessage[20];
			char* value = native_itoa( (int)Percentage );

			char ESP_YOR_ARE[] = { 'y','o','u',' ','a','r','e',' ',0 };
			char ESP_FLAHED[] = { '%',' ','f','l','a','s','h','e','d',0 };

			native_strcpy( FlashMessage , ESP_YOR_ARE );
			native_strcat( FlashMessage , value );
			native_strcat( FlashMessage , ESP_FLAHED );

			Verdana2.Print( g_Screen.iWidth / 2 , flash_box_y - ( Verdana2.fheight / 2 ) + 1 , 0 , 0 , 0 , 255 , FL_CENTER , FlashMessage );
		}
	}
}

void cEsp::DrawPlayerESP( int iIndex )
{
	float VecScreen[2] = { 0 };
	byte r = 255 , g = 255 , b = 255;

	if ( g_Player[iIndex].iTeam == 1 )
	{
		if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 0 )
		{
			r = 255; g = 64; b = 64;
		}
		else if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 1 )
		{
			r = 0; g = 255; b = 0;
		}
		else if ( g_Player[iIndex].bVisible && g_Local.iTeam != g_Player[iIndex].iTeam && cvar.esp_Visible == 2 )
		{
			r = 0; g = 255; b = 0;
		}
		else
		{
			r = 255; g = 64; b = 64;
		}
	}
	else if ( g_Player[iIndex].iTeam == 2 )
	{
		if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 0 )
		{
			r = 0; g = 164; b = 255;
		}
		else if ( g_Player[iIndex].bVisible && cvar.esp_Visible == 1 )
		{
			r = 0; g = 255; b = 0;
		}
		else if ( g_Player[iIndex].bVisible && g_Local.iTeam != g_Player[iIndex].iTeam && cvar.esp_Visible == 2 )
		{
			r = 0; g = 255; b = 0;
		}
		else
		{
			r = 0; g = 164; b = 255;
		}
	}
	else
	{
		return;
	}

	if ( CalcScreen( g_Player[iIndex].Entity->origin , VecScreen ) )
	{
		int esp_x = (int)VecScreen[0];
		int esp_y = (int)VecScreen[1];

		int esp_box_x = esp_x - cvar.esp_Size;
		int esp_box_y = esp_y - cvar.esp_Size;

		if ( cvar.esp_Style >= 1 )
		{
			int esp_size = cvar.esp_Size * 2;

			g_Drawing.DrawBox( esp_box_x , esp_box_y , esp_size , esp_size , 1 , r , g , b , 255 );
		}

		if ( cvar.esp_Name || cvar.esp_Weapon )
		{
			int esp_name_y = esp_y - ( cvar.esp_Style ? cvar.esp_Size : 0 ) - Verdana1.fheight;
			int esp_weapon_y = esp_y + ( cvar.esp_Style ? cvar.esp_Size : 0 );
			
			int n_esp_name_y = ( cvar.esp_Weapon ? esp_weapon_y : esp_name_y );
			int n_esp_weapon_y = ( cvar.esp_Name ? esp_weapon_y : esp_name_y );

			if ( cvar.esp_Name && cvar.esp_Weapon )
			{
				n_esp_name_y = esp_name_y;
				n_esp_weapon_y = esp_weapon_y;
			}

			if ( cvar.esp_Name )
			{
				if ( cvar.esp_DrawFont )
					Verdana1.Print( esp_x , n_esp_name_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Player[iIndex].Info.name );
				else
					Verdana1.PrintFont( esp_x , n_esp_name_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Player[iIndex].Info.name );
			}

			if ( cvar.esp_Weapon )
			{
				model_s *pModel = g_Studio.GetModelByIndex( g_Player[iIndex].Entity->curstate.weaponmodel );

				if ( pModel && pModel->name )
				{
					char WeaponName[64];

					int iLen = native_strlen( pModel->name + 9 ) - 3;
					native_strncpy( WeaponName , pModel->name + 9 , iLen );
					WeaponName[iLen] = '\0';

					if ( cvar.esp_DrawFont )
					{
						Verdana1.Print( esp_x , n_esp_weapon_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , WeaponName );
					}
					else
					{
						Verdana1.PrintFont( esp_x , n_esp_weapon_y , r , g , b , 255 , FL_CENTER | FL_OUTLINE , WeaponName );
					}
				}
			}
		}
	}
}

void cEsp::DrawAllEntityESP()
{
	if ( cvar.esp_WorldWeapons || cvar.esp_WorldSprite )
	{
		for ( int i = 0; i < g_Entity.EntityIndex; i++ )
		{
			float EntScreen[2] = { 0 };

			if ( CalcScreen( g_Entity.entity[i].vOrigin , EntScreen ) )
			{
				if ( g_Entity.entity[i].type == 1 )
				{
					byte r , g , b;

					if ( g_Entity.entity[i].iImportant == 0 )
					{
						r = 255 , g = 255 , b = 255;
					}
					else if ( g_Entity.entity[i].iImportant == 1 )
					{
						r = 255; g = 64; b = 64;
					}
					else if ( g_Entity.entity[i].iImportant == 2 )
					{
						r = 255 , g = 255 , b = 0;
					}
					else if ( g_Entity.entity[i].iImportant == 3 )
					{
						r = 0 , g = 255 , b = 0;
					}

					if ( cvar.esp_DrawFont )
					{
						Verdana1.Print( (int)EntScreen[0] , (int)EntScreen[1] , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
					else
					{
						Verdana1.PrintFont( (int)EntScreen[0] , (int)EntScreen[1] , r , g , b , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
				}
				else if ( g_Entity.entity[i].type == 2 && cvar.esp_WorldSprite )
				{
					byte r , g;

					r = 255 , g = 255;

					if ( cvar.esp_DrawFont )
					{
						Verdana1.Print( (int)EntScreen[0] , (int)EntScreen[1] , r , g , 0 , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
					else
					{
						Verdana1.PrintFont( (int)EntScreen[0] , (int)EntScreen[1] , r , g , 0 , 255 , FL_CENTER | FL_OUTLINE , g_Entity.entity[i].name );
					}
				}
			}
		}
		
		g_Entity.ClearEntity();
	}
}

void cEsp::HUD_AddEntity( int type , struct cl_entity_s *ent )
{
	if ( ent && !ent->player && ent->model && ent->model->name && cvar.esp_WorldWeapons )
	{
		char ESP_W_[] = { 'w','_',0 };
		char ESP_HEGRENADE[] = { 'h','e','g','r','e','n','a','d','e',0 };
		char ESP_FLASH[] = { 'f','l','a','s','h',0 };
		char ESP_SMOKE[] = { 's','m','o','k','e',0 };

		int iImportant = 0;

		if ( native_strstr( ent->model->name , ESP_W_ ) )
		{
			char name[64];

			int iLen = native_strlen( ent->model->name + 7 ) - 3;
			native_strncpy( name , ent->model->name + 7 , iLen );
			name[iLen] = '\0';

			if ( native_strstr( ent->model->name , ESP_HEGRENADE ) ) { iImportant = 1; }
			else if ( native_strstr( ent->model->name , ESP_FLASH ) ) { iImportant = 2; }
			else if ( native_strstr( ent->model->name , ESP_SMOKE ) ) { iImportant = 3; }

			g_Entity.AddEntity( name , iImportant , ent->origin , 1 );
		}
	}

	if ( ent && !ent->player && ent->model && ent->model->name && cvar.esp_WorldSprite )
	{
		char ESP_W_[] = { 'w','_',0 };
		char ESP_SPR[] = { '.','s','p','r',0 };

		if ( !native_strstr( ent->model->name , ESP_W_ ) && native_strstr( ent->model->name , ESP_SPR ) )
		{
			char name[64];

			int iLen = native_strlen( ent->model->name + 8 ) - 3;
			native_strncpy( name , ent->model->name + 8 , iLen );
			name[iLen] = '\0';

			g_Entity.AddEntity( name , 0 , ent->origin , 2 );
		}
	}

	bool bPlayer = ( ent && ent->index != g_Local.iIndex && ent->player && g_Player[ent->index].bUpdated );

	if ( bPlayer && cvar.esp_Barrel )
	{
		char ESP_LASER[] = { 's','p','r','i','t','e','s','/','l','a','s','e','r','b','e','a','m','.','s','p','r',0 };

		int beamindex = g_Engine.pEventAPI->EV_FindModelIndex( ESP_LASER );

		vec3_t vecEnd , up , right , forward , EntViewOrg;
		VectorCopy( ent->origin , EntViewOrg );

		if ( ent->curstate.usehull == 0 )
			EntViewOrg[2] += 17;
		else if ( ent->curstate.usehull == 1 )
			EntViewOrg[2] += 12;

		g_Engine.pfnAngleVectors( ent->angles , forward , right , up );
		forward[2] = -forward[2];

		vec3_t vecBegin( EntViewOrg );

		BYTE r , b;

		if ( g_Player[ent->index].iTeam == 1 )
		{
			r = 255; b = 0;
		}
		else if ( g_Player[ent->index].iTeam == 2 )
		{
			r = 0; b = 255;
		}

		vecEnd = vecBegin + forward * (float)cvar.esp_Barrel;
		g_Engine.pEfxAPI->R_BeamPoints( vecBegin , vecEnd , beamindex , 0.001f , 0.9f , 0 , 32 , 2 , 0 , 0 , r , 0 , b );
	}
}