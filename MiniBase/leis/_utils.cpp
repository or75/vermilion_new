#include "_utils.h"

#pragma warning (disable:4244)
#pragma warning (disable:4723)

bool CalcScreen( float *pflOrigin , float *pflVecScreen )
{
	int iResult = g_Engine.pTriAPI->WorldToScreen( pflOrigin , pflVecScreen );

	if ( pflVecScreen[0] < 1 && pflVecScreen[1] < 1 && pflVecScreen[0] > -1 && pflVecScreen[1] > -1 && !iResult )
	{
		pflVecScreen[0] = pflVecScreen[0] * ( g_Screen.iWidth / 2 ) + ( g_Screen.iWidth / 2 );
		pflVecScreen[1] = -pflVecScreen[1] * ( g_Screen.iHeight / 2 ) + ( g_Screen.iHeight / 2 );
		return true;
	}

	return false;
}

float CalcDistPlayerScreen( float fScreenCenter[2] , float fPlayerPosScreen[2] )
{
	return ( mysqrt( POW( fPlayerPosScreen[0] - fScreenCenter[0] ) + POW( fPlayerPosScreen[1] - fScreenCenter[1] ) ) );
}

bool PathFree( Vector vInput )
{
	pmtrace_t *sTrace = g_Engine.PM_TraceLine( g_Local.vViewOrg , vInput , 0 , 2 , -1 );
	return ( sTrace->fraction >= 1.0f );
}

bool CalcPlayerVisibility( int Index )
{
	byte HitBoxCheckVisible[8] = { 0,2,6,7,8,9,11,19 };

	if ( PathFree( g_Player[Index].vBone[17] ) )
		return true;

	for ( byte i = 0; i < sizeof( HitBoxCheckVisible ); i++ )
	{
		if ( PathFree( g_Player[Index].vHitbox[HitBoxCheckVisible[i]] ) )
			return true;
	}

	return false;
}

bool VisibleOrigin( float *pflFrom , float *pflTo )
{
	if ( !pflFrom || !pflTo ) { return false; }
	pmtrace_t pTrace;
	g_Engine.pEventAPI->EV_SetTraceHull( 2 );
	g_Engine.pEventAPI->EV_PlayerTrace( pflFrom , pflTo , PM_GLASS_IGNORE | PM_STUDIO_BOX , g_Local.iIndex , &pTrace );
	return ( pTrace.fraction == 1.0f );
}

void VectorAngles( const float *forward , float *angles )
{
	float tmp , yaw , pitch;
	if ( forward[1] == 0 && forward[0] == 0 )
	{
		yaw = 0;
		if ( forward[2] > 0 )
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = ( myatan2( forward[1] , forward[0] ) * 180 / M_PI );
		if ( yaw < 0 )
			yaw += 360;
		tmp = mysqrt( forward[0] * forward[0] + forward[1] * forward[1] );
		pitch = ( myatan2( -forward[2] , tmp ) * 180 / M_PI );
		if ( pitch < 0 )
			pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
	while ( angles[0]<-89 ) { angles[0] += 180; angles[1] += 180; }
	while ( angles[0]>89 ) { angles[0] -= 180; angles[1] += 180; }
	while ( angles[1]<-180 ) { angles[1] += 360; }
	while ( angles[1]>180 ) { angles[1] -= 360; }
}

float AngleBetvenVectors( Vector a , Vector b )
{
	float l1 = 0.0f , l2 = 0.0f;
	float sc = a.x*b.x + a.y*b.y + a.z*b.z;
	return mycos( sc / ( l1*l2 ) )*( 180.0 / M_PI );
}

int CsCharToKey( char* Key )
{
	char CFG_KEY_TAB[] = { 'T','A','B',0 };
	char CFG_KEY_ENTER[] = { 'E','N','T','E','R',0 };
	char CFG_KEY_ESCAPE[] = { 'E','S','C','A','P','E',0 };
	char CFG_KEY_BACKSPACE[] = { 'B','A','C','K','S','P','A','C','E',0 };
	char CFG_KEY_UPARROW[] = { 'U','P','A','R','R','O','W',0 };
	char CFG_KEY_DOWNARROW[] = { 'D','O','W','N','A','R','R','O','W',0 };
	char CFG_KEY_LEFTARROW[] = { 'L','E','F','T','A','R','R','O','W',0 };
	char CFG_KEY_RIGHTARROW[] = { 'R','I','G','H','T','A','R','R','O','W',0 };

	char CFG_KEY_INS[] = { 'I','N','S',0 };
	char CFG_KEY_DEL[] = { 'D','E','L',0 };
	char CFG_KEY_PGDN[] = { 'P','G','D','N',0 };
	char CFG_KEY_PGUP[] = { 'P','G','U','P',0 };
	char CFG_KEY_HOME[] = { 'H','O','M','E',0 };
	char CFG_KEY_END[] = { 'E','N','D',0 };

	char CFG_KEY_MWHEELDOWN[] = { 'M','W','H','E','E','L','D','O','W','N',0 };
	char CFG_KEY_MWHEELUP[] = { 'M','W','H','E','E','L','U','P',0 };

	char CFG_KEY_MOUSE1[] = { 'M','O','U','S','E','1',0 };
	char CFG_KEY_MOUSE2[] = { 'M','O','U','S','E','2',0 };
	char CFG_KEY_MOUSE3[] = { 'M','O','U','S','E','3',0 };
	char CFG_KEY_MOUSE4[] = { 'M','O','U','S','E','4',0 };
	char CFG_KEY_MOUSE5[] = { 'M','O','U','S','E','5',0 };

	char CFG_KEY_F1[] = { 'F','1',0 };
	char CFG_KEY_F2[] = { 'F','2',0 };
	char CFG_KEY_F3[] = { 'F','3',0 };
	char CFG_KEY_F4[] = { 'F','4',0 };
	char CFG_KEY_F5[] = { 'F','5',0 };
	char CFG_KEY_F6[] = { 'F','6',0 };
	char CFG_KEY_F7[] = { 'F','7',0 };
	char CFG_KEY_F8[] = { 'F','8',0 };
	char CFG_KEY_F9[] = { 'F','9',0 };
	char CFG_KEY_F10[] = { 'F','1','0',0 };
	char CFG_KEY_F11[] = { 'F','1','1',0 };
	char CFG_KEY_F12[] = { 'F','1','2',0 };

	if ( !native_strcmp( Key , CFG_KEY_TAB ) ) return K_TAB;
	if ( !native_strcmp( Key , CFG_KEY_ENTER ) ) return K_ENTER;
	if ( !native_strcmp( Key , CFG_KEY_ESCAPE ) ) return K_ESCAPE;
	if ( !native_strcmp( Key , CFG_KEY_BACKSPACE ) ) return K_BACKSPACE;
	if ( !native_strcmp( Key , CFG_KEY_UPARROW ) ) return K_UPARROW;
	if ( !native_strcmp( Key , CFG_KEY_DOWNARROW ) ) return K_DOWNARROW;
	if ( !native_strcmp( Key , CFG_KEY_LEFTARROW ) ) return K_LEFTARROW;
	if ( !native_strcmp( Key , CFG_KEY_RIGHTARROW ) ) return K_RIGHTARROW;

	if ( !native_strcmp( Key , CFG_KEY_F1 ) ) return K_F1;
	if ( !native_strcmp( Key , CFG_KEY_F2 ) ) return K_F2;
	if ( !native_strcmp( Key , CFG_KEY_F3 ) ) return K_F3;
	if ( !native_strcmp( Key , CFG_KEY_F4 ) ) return K_F4;
	if ( !native_strcmp( Key , CFG_KEY_F5 ) ) return K_F5;
	if ( !native_strcmp( Key , CFG_KEY_F6 ) ) return K_F6;
	if ( !native_strcmp( Key , CFG_KEY_F7 ) ) return K_F7;
	if ( !native_strcmp( Key , CFG_KEY_F8 ) ) return K_F8;
	if ( !native_strcmp( Key , CFG_KEY_F9 ) ) return K_F9;
	if ( !native_strcmp( Key , CFG_KEY_F10 ) ) return K_F10;
	if ( !native_strcmp( Key , CFG_KEY_F11 ) ) return K_F11;
	if ( !native_strcmp( Key , CFG_KEY_F12 ) ) return K_F12;

	if ( !native_strcmp( Key , CFG_KEY_INS ) ) return K_INS;
	if ( !native_strcmp( Key , CFG_KEY_DEL ) ) return K_DEL;
	if ( !native_strcmp( Key , CFG_KEY_PGDN ) ) return K_PGDN;
	if ( !native_strcmp( Key , CFG_KEY_PGUP ) ) return K_PGUP;
	if ( !native_strcmp( Key , CFG_KEY_HOME ) ) return K_HOME;
	if ( !native_strcmp( Key , CFG_KEY_END ) ) return K_END;

	if ( !native_strcmp( Key , CFG_KEY_MWHEELDOWN ) ) return K_MWHEELDOWN;
	if ( !native_strcmp( Key , CFG_KEY_MWHEELUP ) ) return K_MWHEELUP;

	if ( !native_strcmp( Key , CFG_KEY_MOUSE1 ) ) return K_MOUSE1;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE2 ) ) return K_MOUSE2;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE3 ) ) return K_MOUSE3;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE4 ) ) return K_MOUSE4;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE5 ) ) return K_MOUSE5;

	return 0;
}

int WindowCharToKey( char* Key )
{
	char CFG_KEY_TAB[] = { 'T','A','B',0 };
	char CFG_KEY_ENTER[] = { 'E','N','T','E','R',0 };
	char CFG_KEY_ESCAPE[] = { 'E','S','C','A','P','E',0 };
	char CFG_KEY_SHIFT[] = { 'S','H','I','F','T',0 };
	char CFG_KEY_CTRL[] = { 'C','T','R','L',0 };
	char CFG_KEY_ALT[] = { 'A','L','T',0 };
	char CFG_KEY_CAPSLOCK[] = { 'C','A','P','S','L','O','C','K',0 };

	char CFG_KEY_INS[] = { 'I','N','S',0 };
	char CFG_KEY_DEL[] = { 'D','E','L',0 };
	char CFG_KEY_HOME[] = { 'H','O','M','E',0 };
	char CFG_KEY_END[] = { 'E','N','D',0 };

	char CFG_KEY_MOUSE1[] = { 'M','O','U','S','E','1',0 };
	char CFG_KEY_MOUSE2[] = { 'M','O','U','S','E','2',0 };
	char CFG_KEY_MOUSE3[] = { 'M','O','U','S','E','3',0 };
	char CFG_KEY_MOUSE4[] = { 'M','O','U','S','E','4',0 };
	char CFG_KEY_MOUSE5[] = { 'M','O','U','S','E','5',0 };

	char CFG_KEY_F1[] = { 'F','1',0 };
	char CFG_KEY_F2[] = { 'F','2',0 };
	char CFG_KEY_F3[] = { 'F','3',0 };
	char CFG_KEY_F4[] = { 'F','4',0 };
	char CFG_KEY_F5[] = { 'F','5',0 };
	char CFG_KEY_F6[] = { 'F','6',0 };
	char CFG_KEY_F7[] = { 'F','7',0 };
	char CFG_KEY_F8[] = { 'F','8',0 };
	char CFG_KEY_F9[] = { 'F','9',0 };
	char CFG_KEY_F10[] = { 'F','1','0',0 };
	char CFG_KEY_F11[] = { 'F','1','1',0 };
	char CFG_KEY_F12[] = { 'F','1','2',0 };

	if ( !native_strcmp( Key , CFG_KEY_TAB ) ) return 9;
	if ( !native_strcmp( Key , CFG_KEY_ENTER ) ) return 13;
	if ( !native_strcmp( Key , CFG_KEY_ESCAPE ) ) return 27;
	if ( !native_strcmp( Key , CFG_KEY_SHIFT ) ) return 16;
	if ( !native_strcmp( Key , CFG_KEY_CTRL ) ) return 17;
	if ( !native_strcmp( Key , CFG_KEY_ALT ) ) return 18;
	if ( !native_strcmp( Key , CFG_KEY_CAPSLOCK ) ) return 20;

	if ( !native_strcmp( Key , CFG_KEY_F1 ) ) return 112;
	if ( !native_strcmp( Key , CFG_KEY_F2 ) ) return 113;
	if ( !native_strcmp( Key , CFG_KEY_F3 ) ) return 114;
	if ( !native_strcmp( Key , CFG_KEY_F4 ) ) return 115;
	if ( !native_strcmp( Key , CFG_KEY_F5 ) ) return 116;
	if ( !native_strcmp( Key , CFG_KEY_F6 ) ) return 117;
	if ( !native_strcmp( Key , CFG_KEY_F7 ) ) return 118;
	if ( !native_strcmp( Key , CFG_KEY_F8 ) ) return 119;
	if ( !native_strcmp( Key , CFG_KEY_F9 ) ) return 120;
	if ( !native_strcmp( Key , CFG_KEY_F10 ) ) return 121;

	if ( !native_strcmp( Key , CFG_KEY_INS ) ) return 45;
	if ( !native_strcmp( Key , CFG_KEY_DEL ) ) return 46;
	if ( !native_strcmp( Key , CFG_KEY_END ) ) return 35;
	if ( !native_strcmp( Key , CFG_KEY_HOME ) ) return 36;

	if ( !native_strcmp( Key , CFG_KEY_MOUSE1 ) ) return 1;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE2 ) ) return 2;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE3 ) ) return 4;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE4 ) ) return 5;
	if ( !native_strcmp( Key , CFG_KEY_MOUSE5 ) ) return 6;

	return 0;
}