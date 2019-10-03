#include "usermsg.h"

pfnUserMsgHook pResetHUD;
pfnUserMsgHook pSetFOV;
pfnUserMsgHook pTeamInfo;
pfnUserMsgHook pCurWeapon;
pfnUserMsgHook pDeathMsg;
pfnUserMsgHook pWeaponList;
pfnUserMsgHook pAmmoX;

int ResetHUD( const char *pszName , int iSize , void *pbuf )
{
	for ( int i = 1; i <= 32; i++ )
	{
		g_Player[i].bAlive = true;
	}

	g_Entity.ClearEntity();
	g_Sound.ClearSound();
	
	return pResetHUD( pszName , iSize , pbuf );
}

int SetFOV( const char *pszName , int iSize , void *pbuf )
{
	BEGIN_READ( pbuf , iSize );

	g_Local.iFOV = READ_BYTE();

	if ( !g_Local.iFOV )
		g_Local.iFOV = 90;

	return pSetFOV( pszName , iSize , pbuf );
}

int TeamInfo( const char *pszName , int iSize , void *pbuf )
{
	char tt_umsg[] = { 'T','E','R','R','O','R','I','S','T',0 };
	char ct_umsg[] = { 'C','T',0 };

	BEGIN_READ( pbuf , iSize );

	int iIndex = READ_BYTE();
	char *szTeam = READ_STRING();

	if ( !native_strcmp( szTeam , tt_umsg ) )
	{
		if ( iIndex == g_Local.iIndex ) { g_Local.iTeam = 1; }
		g_Player[iIndex].iTeam = 1;
	}
	else if ( !native_strcmp( szTeam , ct_umsg ) )
	{
		if ( iIndex == g_Local.iIndex ) { g_Local.iTeam = 2; }
		g_Player[iIndex].iTeam = 2;
	}
	else
	{
		if ( iIndex == g_Local.iIndex ) { g_Local.iTeam = 0; }
		g_Player[iIndex].iTeam = 0;
	}

	return pTeamInfo( pszName , iSize , pbuf );
}

int CurWeapon( const char *pszName , int iSize , void *pbuf )
{
	BEGIN_READ( pbuf , iSize );

	int iState = READ_BYTE();
	int iWeaponID = READ_CHAR();
	int iClip = READ_CHAR();

	if ( iState )
	{
		g_Local.iWeaponID = iWeaponID;
		g_Local.iClip = iClip;
	}

	return pCurWeapon( pszName , iSize , pbuf );
}

int DeathMsg( const char *pszName , int iSize , void *pbuf )
{
	BEGIN_READ( pbuf , iSize );

	int iKiller = READ_BYTE();
	int iVictim = READ_BYTE();

	g_Player[iVictim].bAlive = false;
	g_Player[iVictim].bUpdated = false;
	g_Player[iVictim].bVisible = false;
	g_Player[iVictim].bEnableAim = false;
	g_Player[iVictim].dwStartTime = 0;
	g_Player[iVictim].isFov = false;

	return pDeathMsg( pszName , iSize , pbuf );
}

int WeaponList( const char *pszName , int iSize , void *pbuf )
{
	BEGIN_READ( pbuf , iSize );

	char* weaponname = READ_STRING();

	int ammo1type = READ_CHAR();
	int max1 = READ_BYTE();

	int ammo2type = READ_CHAR();
	int max2 = READ_BYTE();

	int slot = READ_CHAR();
	int slotpos = READ_CHAR();

	int id = READ_CHAR();
	int flags = READ_BYTE();

	g_WeaponList.WeaponListAdd( weaponname , ammo1type , max1 , ammo2type , max2 , slot , slotpos , id , flags );

	return pWeaponList( pszName , iSize , pbuf );
}

int AmmoX( const char *pszName , int iSize , void *pbuf )
{
	BEGIN_READ( pbuf , iSize );

	int ID = READ_BYTE();
	int Count = READ_BYTE();

	g_WeaponList.WeaponListAmmoX( ID , Count );

	return pAmmoX( pszName , iSize , pbuf );
}