#include "_nospread.h"

#pragma warning(disable:4244)

cNoSpread g_NoSpread;

unsigned int glSeed = 0;

unsigned int seed_table[256] =
{
	28985, 27138, 26457, 9451, 17764, 10909, 28790, 8716, 6361, 4853, 17798, 21977, 19643, 20662, 10834, 20103,
	27067, 28634, 18623, 25849, 8576, 26234, 23887, 18228, 32587, 4836, 3306, 1811, 3035, 24559, 18399, 315,
	26766, 907, 24102, 12370, 9674, 2972, 10472, 16492, 22683, 11529, 27968, 30406, 13213, 2319, 23620, 16823,
	10013, 23772, 21567, 1251, 19579, 20313, 18241, 30130, 8402, 20807, 27354, 7169, 21211, 17293, 5410, 19223,
	10255, 22480, 27388, 9946, 15628, 24389, 17308, 2370, 9530, 31683, 25927, 23567, 11694, 26397, 32602, 15031,
	18255, 17582, 1422, 28835, 23607, 12597, 20602, 10138, 5212, 1252, 10074, 23166, 19823, 31667, 5902, 24630,
	18948, 14330, 14950, 8939, 23540, 21311, 22428, 22391, 3583, 29004, 30498, 18714, 4278, 2437, 22430, 3439,
	28313, 23161, 25396, 13471, 19324, 15287, 2563, 18901, 13103, 16867, 9714, 14322, 15197, 26889, 19372, 26241,
	31925, 14640, 11497, 8941, 10056, 6451, 28656, 10737, 13874, 17356, 8281, 25937, 1661, 4850, 7448, 12744,
	21826, 5477, 10167, 16705, 26897, 8839, 30947, 27978, 27283, 24685, 32298, 3525, 12398, 28726, 9475, 10208,
	617, 13467, 22287, 2376, 6097, 26312, 2974, 9114, 21787, 28010, 4725, 15387, 3274, 10762, 31695, 17320,
	18324, 12441, 16801, 27376, 22464, 7500, 5666, 18144, 15314, 31914, 31627, 6495, 5226, 31203, 2331, 4668,
	12650, 18275, 351, 7268, 31319, 30119, 7600, 2905, 13826, 11343, 13053, 15583, 30055, 31093, 5067, 761,
	9685, 11070, 21369, 27155, 3663, 26542, 20169, 12161, 15411, 30401, 7580, 31784, 8985, 29367, 20989, 14203,
	29694, 21167, 10337, 1706, 28578, 887, 3373, 19477, 14382, 675, 7033, 15111, 26138, 12252, 30996, 21409,
	25678, 18555, 13256, 23316, 22407, 16727, 991, 9236, 5373, 29402, 6117, 15241, 27715, 19291, 19888, 19847
};

void cNoSpread::GetSpreadOffset( unsigned int seed , int future , float *inangles , float *velocity , float *outangles )
{
	float forward[3] , right[3] , up[3] , vecDir[3];
	float view[3] , dest[3] , spread[2];

	g_Engine.pfnAngleVectors( inangles , forward , right , up );

	GetSpreadXY( seed , future , velocity , spread );

	vecDir[0] = forward[0] + spread[0] * right[0] + spread[1] * up[0];
	view[0] = 8192 * vecDir[0];

	vecDir[1] = forward[1] + spread[0] * right[1] + spread[1] * up[1];
	view[1] = 8192 * vecDir[1];

	vecDir[2] = forward[2] + spread[0] * right[2] + spread[1] * up[2];
	view[2] = 8192 * vecDir[2];

	VectorAngles( view , dest );
	dest[0] *= -1;

	outangles[0] = inangles[0] - dest[0];
	outangles[1] = inangles[1] - dest[1];
	outangles[2] = 0;
}

void cNoSpread::HUD_PostRunCmd( struct local_state_s *to , struct usercmd_s *cmd , int runfuncs , double time , unsigned int random_seed )
{
	if ( runfuncs )
	{
		g_Local.spread.random_seed = random_seed;
		g_Local.spread.gtime = time;
		g_Local.spread.prcflags = to->client.flags;

		if ( cmd->buttons & IN_ATTACK && g_WeaponList.CanCurWeaponAttack() )
		{
			PrimaryAttack();
		}
		else if ( !( cmd->buttons & ( IN_ATTACK | IN_ATTACK2 ) ) )
		{
			if ( g_Local.spread.firing )
			{
				g_Local.spread.firing = false;

				if ( g_Local.spread.recoil > 15 )
					g_Local.spread.recoil = 15;

				g_Local.spread.recoiltime = time + 0.4;
			}
			if ( g_WeaponList.IsCurWeaponSec() )
			{
				g_Local.spread.recoil = 0;
			}
			else if ( g_Local.spread.recoil > 0 )
			{
				if ( g_Local.spread.recoiltime <= time )
				{
					g_Local.spread.recoiltime = g_Local.spread.recoiltime + 0.0225;
					g_Local.spread.recoil--;
				}
			}
		}

		for ( int i = 0; i < g_WeaponList.WeaponListIndex; i++ )
		{
			if ( g_WeaponList.WeaponListData[i].Id == to->client.m_iId )
			{
				if ( !g_WeaponList.WeaponListData[i].CurWeapon ) // FIX: This doesn't catch when you have a weapon and you buy the same weapon
				{
					g_Local.spread.recoil = 0;
					g_Local.spread.prevtime = 0;
					DefaultSpread( g_WeaponList.WeaponListData[i].Id );
					g_Local.spread.recoiltime = time;
					g_Local.spread.firing = true;
				}

				g_WeaponList.WeaponListData[i].CurWeapon = true;
			}
			else
				g_WeaponList.WeaponListData[i].CurWeapon = false;
		}

		for ( int i = 0; i<MAX_WEAPONS; i++ )
		{
			int index = g_WeaponList.GetWeaponIndexByID( i );

			if ( index == -1 )
				continue;

			if ( to->weapondata[i].m_iId )
				native_memcpy( &g_WeaponList.WeaponListData[index].weapondata , &to->weapondata[i] , sizeof( weapon_data_t ) );
			else
				native_memset( &g_WeaponList.WeaponListData[index].weapondata , 0 , sizeof( weapon_data_t ) );
		}

		if ( g_WeaponList.IsReloading() )
		{
			int Id = g_WeaponList.GetCurWeaponId();

			if ( Id == -1 )
				return;

			g_Local.spread.recoil = 0;
			g_Local.spread.prevtime = 0;
			DefaultSpread( Id );
			g_Local.spread.recoiltime = time;
			g_Local.spread.firing = false;
		}
	}
}

void cNoSpread::CL_CreateMove( usercmd_s *cmd )
{
	float offset[3] = { 0 };

	if ( !g_Local.bBadWeapon )
		g_NoSpread.GetSpreadOffset( g_Local.spread.random_seed , 2 , cmd->viewangles , g_Local.vVelocity , offset );

	VectorCopy( offset , g_Local.vNoSpreadAng );
}

void cNoSpread::DefaultSpread( int weaponid )
{
	if ( weaponid == WEAPONLIST_DEAGLE )
		g_Local.spread.spreadvar = 0.9f;
	else if ( weaponid == WEAPONLIST_MP5 )
		g_Local.spread.spreadvar = 0.0f;
	else if ( weaponid == WEAPONLIST_AK47 )
		g_Local.spread.spreadvar = 0.2f;
	else if ( weaponid == WEAPONLIST_SG552 )
		g_Local.spread.spreadvar = 0.2f;
	else if ( weaponid == WEAPONLIST_AUG )
		g_Local.spread.spreadvar = 0.3f;
	else if ( weaponid == WEAPONLIST_M4A1 )
		g_Local.spread.spreadvar = 0.2f;
	else if ( weaponid == WEAPONLIST_M249 )
		g_Local.spread.spreadvar = 0.2f;
	else if ( weaponid == WEAPONLIST_MAC10 )
		g_Local.spread.spreadvar = 0.15f;
	else if ( weaponid == WEAPONLIST_UMP45 )
		g_Local.spread.spreadvar = 0;
	else if ( weaponid == WEAPONLIST_TMP )
		g_Local.spread.spreadvar = 0.2f;
	else if ( weaponid == WEAPONLIST_P90 )
		g_Local.spread.spreadvar = 0.15f;
	else if ( weaponid == WEAPONLIST_P228 )
		g_Local.spread.spreadvar = 0.9f;
	else if ( weaponid == WEAPONLIST_FIVESEVEN )
		g_Local.spread.spreadvar = 0.92f;
	else if ( weaponid == WEAPONLIST_ELITE )
		g_Local.spread.spreadvar = 0.88f;
	else if ( weaponid == WEAPONLIST_GLOCK18 )
		g_Local.spread.spreadvar = 0.9f;
	else if ( weaponid == WEAPONLIST_USP )
		g_Local.spread.spreadvar = 0.92f;
	else if ( weaponid == WEAPONLIST_G3SG1 )
		g_Local.spread.spreadvar = 0.2f;
	else if ( weaponid == WEAPONLIST_SG550 )
		g_Local.spread.spreadvar = 0.2f;
	else
		g_Local.spread.spreadvar = 0;
}

void cNoSpread::NoSpread()
{
	Vector spreadangles , forward , eyepos , screen;

	if ( !g_Local.bBadWeapon && g_Local.bAlive )
	{
		pmtrace_t tr;

		VectorAdd( g_Local.vCmdViewAngles , g_Local.vNoSpreadAng , spreadangles );

		g_Engine.pfnAngleVectors( spreadangles , forward , NULL , NULL );

		g_Engine.pEventAPI->EV_SetTraceHull( 2 );
		eyepos = g_Local.vViewOrg;
		g_Engine.pEventAPI->EV_PlayerTrace( eyepos , eyepos + ( forward * 8192 ) , PM_GLASS_IGNORE , -1 , &tr );

		if ( CalcScreen( tr.endpos , g_Local.vDrawSpr ) )
		{
			g_Local.vDrawSpr.x = g_Screen.iWidth - g_Local.vDrawSpr.x;
			g_Local.vDrawSpr.y = g_Screen.iHeight - g_Local.vDrawSpr.y;
		}
	}
}

unsigned int cNoSpread::U_Random( void )
{
	glSeed *= 69069;
	glSeed += seed_table[glSeed & 0xff];

	return ( ++glSeed & 0x0fffffff );
}

void cNoSpread::U_Srand( unsigned int seed )
{
	glSeed = seed_table[seed & 0xff];
}

int cNoSpread::UTIL_SharedRandomLong( unsigned int seed , int low , int high )
{
	unsigned int range;

	U_Srand( (int)seed + low + high );

	range = high - low + 1;
	if ( !( range - 1 ) )
	{
		return low;
	}
	else
	{
		int offset;
		int rnum;

		rnum = U_Random();

		offset = rnum % range;

		return ( low + offset );
	}
}

float cNoSpread::UTIL_SharedRandomFloat( unsigned int seed , float low , float high )
{
	unsigned int range;

	U_Srand( (int)seed + *(int *)&low + *(int *)&high );

	U_Random();
	U_Random();

	range = (unsigned int)( high - low );
	if ( !range )
	{
		return low;
	}
	else
	{
		int tensixrand;
		float offset;

		tensixrand = U_Random() & 65535;

		offset = (float)tensixrand / 65536.0f;

		return ( low + offset * range );
	}
}

float cNoSpread::GetVecSpread( float speed )
{
	float spread;

	int id = g_Local.iWeaponID;

	switch ( id )
	{
		case WEAPONLIST_DEAGLE:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed )
					spread = 0.25 * ( 1.0f - g_Local.spread.spreadvar );
				else if ( g_Local.spread.prcflags & FL_DUCKING )
					spread = 0.115 * ( 1.0f - g_Local.spread.spreadvar );
				else
					spread = 0.13 * ( 1.0f - g_Local.spread.spreadvar );
			}
			else
				spread = 1.5 * ( 1.0f - g_Local.spread.spreadvar );

			break;
		case WEAPONLIST_ELITE:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed )
					spread = 0.175 * ( 1.0f - g_Local.spread.spreadvar );
				else if ( g_Local.spread.prcflags & FL_DUCKING )
					spread = 0.08 * ( 1.0f - g_Local.spread.spreadvar );
				else
					spread = 0.1 * ( 1.0f - g_Local.spread.spreadvar );
			}
			else
				spread = 1.3 * ( 1.0f - g_Local.spread.spreadvar );

			break;
		case WEAPONLIST_GALIL:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 140.0f )
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0375;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + 0.03;

			break;
		case WEAPONLIST_FAMAS:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 140.0f )
					spread = g_Local.spread.spreadvar * 0.07 + 0.03;
				else
					spread = g_Local.spread.spreadvar * 0.020;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;
			break;
		case WEAPONLIST_FIVESEVEN:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed )
					spread = 0.255 * ( 1.0f - g_Local.spread.spreadvar );
				else if ( g_Local.spread.prcflags & FL_DUCKING )
					spread = 0.075 * ( 1.0f - g_Local.spread.spreadvar );
				else
					spread = 0.15 * ( 1.0f - g_Local.spread.spreadvar );
			}
			else
				spread = 1.5 * ( 1.0f - g_Local.spread.spreadvar );

			break;
		case WEAPONLIST_GLOCK18:
			if ( !g_WeaponList.IsCurWeaponInBurst() )
			{
				if ( g_Local.spread.prcflags & FL_ONGROUND )
				{
					if ( speed )
						spread = 0.165 * ( 1.0f - g_Local.spread.spreadvar );
					else if ( g_Local.spread.prcflags & FL_DUCKING )
						spread = 0.075 * ( 1.0f - g_Local.spread.spreadvar );
					else
						spread = 0.1 * ( 1.0f - g_Local.spread.spreadvar );
				}
				else
					spread = 1.0f - g_Local.spread.spreadvar;
			}
			else
			{
				if ( g_Local.spread.prcflags & FL_ONGROUND )
				{
					if ( speed )
						spread = 0.185 * ( 1.0f - g_Local.spread.spreadvar );
					else if ( g_Local.spread.prcflags & FL_DUCKING )
						spread = 0.095 * ( 1.0f - g_Local.spread.spreadvar );
					else
						spread = 0.3 * ( 1.0f - g_Local.spread.spreadvar );
				}
				else
					spread = 1.2 * ( 1.0f - g_Local.spread.spreadvar );
			}

			break;
		case WEAPONLIST_P228:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed )
					spread = 0.255 * ( 1.0f - g_Local.spread.spreadvar );
				else if ( g_Local.spread.prcflags & FL_DUCKING )
					spread = 0.075 * ( 1.0f - g_Local.spread.spreadvar );
				else
					spread = 0.15 * ( 1.0f - g_Local.spread.spreadvar );
			}
			else
				spread = 1.5 * ( 1.0f - g_Local.spread.spreadvar );

			break;
		case WEAPONLIST_G3SG1:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 100.0f )
					spread = 0.15f;
				else if ( g_Local.spread.prcflags & FL_DUCKING )
					spread = 0.04f * ( 1.0f - g_Local.spread.spreadvar );
				else
					spread = 0.06f * ( 1.0f - g_Local.spread.spreadvar );
			}
			else
				spread = 0.45f * ( 1.0f - g_Local.spread.spreadvar );

			if ( !( g_Local.iFOV < 90.0f ) )
				spread += .0016f;

			break;
		case WEAPONLIST_SG550:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed )
					spread = 0.15f;
				else if ( g_Local.spread.prcflags & FL_DUCKING )
					spread = 0.04 * ( 1.0f - g_Local.spread.spreadvar );
				else
					spread = 0.05 * ( 1.0f - g_Local.spread.spreadvar );
			}
			else
				spread = 0.45 * ( 1.0f - g_Local.spread.spreadvar );

			if ( !( g_Local.iFOV < 90.0f ) )
				spread += 0.025f;

			break;
		case WEAPONLIST_USP:
			if ( !g_WeaponList.IsCurWeaponSilenced() )
			{
				if ( g_Local.spread.prcflags & FL_ONGROUND )
				{
					if ( speed )
						spread = 0.225 * ( 1.0f - g_Local.spread.spreadvar );
					else if ( g_Local.spread.prcflags & FL_DUCKING )
						spread = 0.08 * ( 1.0f - g_Local.spread.spreadvar );
					else
						spread = 0.1 * ( 1.0f - g_Local.spread.spreadvar );
				}
				else
					spread = 1.2 * ( 1.0f - g_Local.spread.spreadvar );
			}
			else
			{
				if ( g_Local.spread.prcflags & FL_ONGROUND )
				{
					if ( speed )
						spread = 0.25 * ( 1.0f - g_Local.spread.spreadvar );
					else if ( g_Local.spread.prcflags & FL_DUCKING )
						spread = 0.125 * ( 1.0f - g_Local.spread.spreadvar );
					else
						spread = 0.15 * ( 1.0f - g_Local.spread.spreadvar );
				}
				else
					spread = 1.3 * ( 1.0f - g_Local.spread.spreadvar );
			}

			break;
		case WEAPONLIST_AK47:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 140.0f )
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0275;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;

			break;
		case WEAPONLIST_SG552:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 140.0f )
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_AUG:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 140.0f )
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_M249:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 140.0f )
					spread = g_Local.spread.spreadvar * 0.095 + 0.045;
				else
					spread = g_Local.spread.spreadvar * 0.03;
			}
			else
				spread = g_Local.spread.spreadvar * 0.5 + .045;

			break;
		case WEAPONLIST_M4A1:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( !g_WeaponList.IsCurWeaponSilenced() )
				{
					if ( speed > 140.0f )
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.02;
				}
				else
				{
					if ( speed > 140.0f )
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.025;
				}
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_MP5:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.2 * g_Local.spread.spreadvar;
			break;
		case WEAPONLIST_MAC10:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.375 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_P90:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed > 170.0f )
					spread = g_Local.spread.spreadvar * 0.115;
				else
					spread = g_Local.spread.spreadvar * 0.045;
			}
			else
				spread = g_Local.spread.spreadvar * 0.3;

			break;
		case WEAPONLIST_TMP:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.25 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_UMP45:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.24 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_AWP:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed < 10.0f )
				{
					if ( g_Local.spread.prcflags & FL_DUCKING )
						spread = 0;
					else
						spread = 0.001f;
				}
				else if ( speed < 140.0f )
				{
					spread = 0.1f;
				}
				else
					spread = 0.25f;
			}
			else
				spread = 0.85f;

			if ( !( g_Local.iFOV < 90.0f ) )
				spread += 0.08f;

			break;
		case WEAPONLIST_SCOUT:
			if ( g_Local.spread.prcflags & FL_ONGROUND )
			{
				if ( speed < 170.0f )
				{
					if ( g_Local.spread.prcflags & FL_DUCKING )
						spread = 0;
					else
						spread = 0.007f;
				}
				else
					spread = 0.075f;
			}
			else
				spread = 0.2f;

			if ( !( g_Local.iFOV < 90.0f ) )
				spread += 0.025f;

			break;
		default:
			spread = 0;
			break;
	}

	return spread;
}

void cNoSpread::PrimaryAttack( void )
{
	int id = g_WeaponList.GetCurWeaponId();

	switch ( id )
	{
		case WEAPONLIST_DEAGLE:
			if ( g_Local.spread.prevtime )
			{
				g_Local.spread.spreadvar = g_Local.spread.spreadvar - ( 0.35 * ( 0.4 - ( g_Local.spread.gtime - g_Local.spread.prevtime ) ) );

				if ( g_Local.spread.spreadvar > 0.9 )
					g_Local.spread.spreadvar = 0.9f;
				else if ( g_Local.spread.spreadvar < 0.55 )
					g_Local.spread.spreadvar = 0.55f;
			}

			g_Local.spread.recoil++;
			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_ELITE:
			if ( g_Local.spread.prevtime )
			{
				g_Local.spread.spreadvar = g_Local.spread.spreadvar - ( 0.275 * ( 0.325 - ( g_Local.spread.gtime - g_Local.spread.prevtime ) ) );

				if ( g_Local.spread.spreadvar > 0.88 )
					g_Local.spread.spreadvar = 0.88f;
				else if ( g_Local.spread.spreadvar < 0.55 )
					g_Local.spread.spreadvar = 0.55f;
			}

			g_Local.spread.recoil++;
			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_GALIL:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = ( (float)g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil ) / 200.0f + 0.35f;

			if ( g_Local.spread.spreadvar > 1.25 )
				g_Local.spread.spreadvar = 1.25f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_FAMAS:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = ( (float)g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil ) / 215.0f + 0.3f;

			if ( g_Local.spread.spreadvar > 1.0 )
				g_Local.spread.spreadvar = 1.0f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_FIVESEVEN:
			if ( g_Local.spread.prevtime )
			{
				g_Local.spread.spreadvar = g_Local.spread.spreadvar - ( 0.25 * ( 0.275 - ( g_Local.spread.gtime - g_Local.spread.prevtime ) ) );

				if ( g_Local.spread.spreadvar > 0.92 )
					g_Local.spread.spreadvar = 0.92f;
				else if ( g_Local.spread.spreadvar < 0.725 )
					g_Local.spread.spreadvar = 0.725f;
			}

			g_Local.spread.recoil++;
			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_GLOCK18:
			if ( !g_WeaponList.IsCurWeaponInBurst() )
				g_Local.spread.recoil++;

			if ( g_Local.spread.prevtime )
			{
				g_Local.spread.spreadvar = g_Local.spread.spreadvar - ( 0.275 * ( 0.325 - ( g_Local.spread.gtime - g_Local.spread.prevtime ) ) );

				if ( g_Local.spread.spreadvar > 0.9 )
					g_Local.spread.spreadvar = 0.9f;
				else if ( g_Local.spread.spreadvar < 0.6 )
					g_Local.spread.spreadvar = 0.6f;
			}

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_P228:
			if ( g_Local.spread.prevtime )
			{
				g_Local.spread.spreadvar = g_Local.spread.spreadvar - ( 0.3 * ( 0.325 - ( g_Local.spread.gtime - g_Local.spread.prevtime ) ) );

				if ( g_Local.spread.spreadvar > 0.9 )
					g_Local.spread.spreadvar = 0.9f;
				else if ( g_Local.spread.spreadvar < 0.6 )
					g_Local.spread.spreadvar = 0.6f;
			}

			g_Local.spread.recoil++;
			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_G3SG1:
			if ( g_Local.spread.brokentime )
			{
				g_Local.spread.spreadvar = 0.55 + ( 0.3 * ( g_Local.spread.gtime - g_Local.spread.brokentime ) );

				if ( g_Local.spread.spreadvar > 0.98 )
					g_Local.spread.spreadvar = 0.98f;
			}

			g_Local.spread.recoil++;
			g_Local.spread.brokentime = g_Local.spread.gtime;
			g_Local.spread.firing = true;

			break;
		case WEAPONLIST_SG550:
			if ( g_Local.spread.brokentime )
			{
				g_Local.spread.spreadvar = 0.65 + ( 0.35 * ( g_Local.spread.gtime - g_Local.spread.brokentime ) );

				if ( g_Local.spread.spreadvar > 0.98 )
					g_Local.spread.spreadvar = 0.98f;
			}

			g_Local.spread.recoil++;
			g_Local.spread.brokentime = g_Local.spread.gtime;
			g_Local.spread.firing = true;

			break;
		case WEAPONLIST_USP:
			if ( g_Local.spread.prevtime )
			{
				g_Local.spread.spreadvar = g_Local.spread.spreadvar - ( 0.275 * ( 0.3 - ( g_Local.spread.gtime - g_Local.spread.prevtime ) ) );

				if ( g_Local.spread.spreadvar > 0.92 )
					g_Local.spread.spreadvar = 0.92f;
				else if ( g_Local.spread.spreadvar < 0.6 )
					g_Local.spread.spreadvar = 0.6f;
			}

			g_Local.spread.recoil++;
			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_AK47:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 200.0f + 0.35;

			if ( g_Local.spread.spreadvar > 1.25 )
				g_Local.spread.spreadvar = 1.25f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_SG552:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 220.0f + 0.3;

			if ( g_Local.spread.spreadvar > 1 )
				g_Local.spread.spreadvar = 1.0f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_AUG:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 215.0f + 0.3;

			if ( g_Local.spread.spreadvar > 1.0 )
				g_Local.spread.spreadvar = 1.0f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_M249:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 175.0f + 0.4;

			if ( g_Local.spread.spreadvar > 0.9 )
				g_Local.spread.spreadvar = 0.9f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_M4A1:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 220.0f + 0.3;

			if ( g_Local.spread.spreadvar > 1 )
				g_Local.spread.spreadvar = 1.0f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_MP5:
			g_Local.spread.recoil++;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 220.1f + 0.45; // CS 1.6 FIX

			if ( g_Local.spread.spreadvar > 0.75 )
				g_Local.spread.spreadvar = 0.75f;

			g_Local.spread.prevtime = g_Local.spread.gtime;
			g_Local.spread.firing = true;

			break;
		case WEAPONLIST_MAC10:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 200.0f + 0.6;

			if ( g_Local.spread.spreadvar > 1.65 )
				g_Local.spread.spreadvar = 1.65f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_P90:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil / 175.0f + 0.45;

			if ( g_Local.spread.spreadvar > 1 )
				g_Local.spread.spreadvar = 1.0f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_TMP:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 200.0f + 0.55;

			if ( g_Local.spread.spreadvar > 1.4 )
				g_Local.spread.spreadvar = 1.4f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_UMP45:
			g_Local.spread.recoil++;
			g_Local.spread.firing = true;

			g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil / 210.0f + 0.5;

			if ( g_Local.spread.spreadvar > 1.0 )
				g_Local.spread.spreadvar = 1.0f;

			g_Local.spread.prevtime = g_Local.spread.gtime;

			break;
		case WEAPONLIST_AWP:
			g_Local.spread.recoil++;
			g_Local.spread.prevtime = g_Local.spread.gtime;
			g_Local.spread.firing = true;

			break;
		case WEAPONLIST_SCOUT:
			g_Local.spread.recoil++;
			g_Local.spread.prevtime = g_Local.spread.gtime;
			g_Local.spread.firing = true;

			break;
		default:
			break;
	}

	return;
}

void cNoSpread::VectorAngles( const float *forward , float *angles )
{
	float tmp , yaw , pitch;

	if ( forward[1] == 0 && forward[0] == 0 )
	{
		yaw = 0;
		pitch = 0;
	}
	else
	{
		yaw = myatan2( forward[1] , forward[0] ) * 180 / M_PI;

		if ( yaw < 0 )
			yaw += 360;

		tmp = mysqrt( forward[0] * forward[0] + forward[1] * forward[1] );

		pitch = myatan2( forward[2] , tmp ) * 180 / M_PI;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void cNoSpread::GetSpreadXY( unsigned int seed , int future , float *velocity , float *vec )
{
	float vecspread , speed;

	vec[0] = UTIL_SharedRandomFloat( seed + future , -0.5 , 0.5 ) + UTIL_SharedRandomFloat( seed + 1 + future , -0.5 , 0.5 );
	vec[1] = UTIL_SharedRandomFloat( seed + 2 + future , -0.5 , 0.5 ) + UTIL_SharedRandomFloat( seed + 3 + future , -0.5 , 0.5 );

	speed = mysqrt( velocity[0] * velocity[0] + velocity[1] * velocity[1] );

	vecspread = GetVecSpread( speed );

	vec[0] *= vecspread;
	vec[1] *= vecspread;
}