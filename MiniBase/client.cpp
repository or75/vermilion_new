#include "client.h"

bool FirstFrame = false;
bool ClearDisconnect = false;

net_status_t g_nStatus;

#include <wininet.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "wininet.lib")

#define MYAC_PRIVATE_KEY	/*5BE6D8BF8E4B8F837-*/XorStr<0xC7,19,0xA10D5352>("\xF2\x8A\x8C\xFC\x8F\xF4\x8F\x88\xF7\x95\xE5\x90\xEB\x92\xED\xE5\xE0\xF5"+0xA10D5352).s
#define MYAC_CRYPT_KEY		0x79A41

#define MYAC_LICENSE_HOST	/*http://csxr.ru/indigo/myac.php?hash=*/XorStr<0x92,37,0xD04FEAF6>("\xFA\xE7\xE0\xE5\xAC\xB8\xB7\xFA\xE9\xE3\xEE\xB3\xEC\xEA\x8F\xC8\xCC\xC7\xCD\xC2\xC9\x88\xC5\xD0\xCB\xC8\x82\xDD\xC6\xDF\x8F\xD9\xD3\xC0\xDC\x88"+0xD04FEAF6).s
#define MYAC_LICENSE_HKEY	/*http://csxr.ru/indigo/myac.php?hkey=*/XorStr<0xEB,37,0xEC6EEDA5>("\x83\x98\x99\x9E\xD5\xDF\xDE\x91\x80\x8C\x87\xD8\x85\x8D\xD6\x93\x95\x98\x94\x99\x90\x2F\x6C\x7B\x62\x67\x2B\x76\x6F\x78\x36\x62\x60\x69\x74\x33"+0xEC6EEDA5).s

char* HashMD5( char* data )
{
	DWORD dwStatus = 0;
	DWORD cbHash = 16;
	int i = 0;
	HCRYPTPROV cryptProv;
	HCRYPTHASH cryptHash;
	BYTE hash[16];
	char *hex = "0123456789abcdef";
	char *strHash = AllocChar( 33 );
	if( !CryptAcquireContextA( &cryptProv , NULL , MS_DEF_PROV , PROV_RSA_FULL , CRYPT_VERIFYCONTEXT ) )
	{
		return NULL;
	}
	if( !CryptCreateHash( cryptProv , CALG_MD5 , 0 , 0 , &cryptHash ) )
	{
		CryptReleaseContext( cryptProv , 0 );
		return NULL;
	}
	if( !CryptHashData( cryptHash , (BYTE*)data , lstrlenA( data ) , 0 ) )
	{
		CryptReleaseContext( cryptProv , 0 );
		CryptDestroyHash( cryptHash );
		return NULL;
	}
	if( !CryptGetHashParam( cryptHash , HP_HASHVAL , hash , &cbHash , 0 ) )
	{
		CryptReleaseContext( cryptProv , 0 );
		CryptDestroyHash( cryptHash );
		return NULL;
	}
	for( i = 0; i < cbHash; i++ )
	{
		strHash[i * 2] = hex[hash[i] >> 4];
		strHash[( i * 2 ) + 1] = hex[hash[i] & 0xF];
	}
	CryptReleaseContext( cryptProv , 0 );
	CryptDestroyHash( cryptHash );
	return strHash;
}

char* GetHDD()
{
	char pcid_root_cimv2[] = { 'R','O','O','T','\\\\','C','I','M','V','2',0 };
	char pcid_wql[] = { 'W','Q','L',0 };
	char pcid_w32[] =
	{
		'S','E','L','E','C','T',' ','*',' ','F','R','O','M',' ',
		'W','i','n','3','2','_','P','h','y','s','i','c','a','l',
		'M','e','d','i','a',0
	};
	char pcid_prc[] = { 'S','e','r','i','a','l','N','u','m','b','e','r',0 };

	HRESULT hres;

	char* Result = NULL;

	IWbemLocator *pLoc = NULL;
	IWbemServices *pSvc = NULL;
	IEnumWbemClassObject* pEnumerator = NULL;
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	VARIANT vtProp;

	hres = CoInitializeEx( 0 , COINIT_MULTITHREADED );
	hres = CoInitializeSecurity( NULL , -1 , NULL , NULL , RPC_C_AUTHN_LEVEL_DEFAULT ,
								 RPC_C_IMP_LEVEL_IMPERSONATE , NULL , EOAC_NONE , NULL );

	if( FAILED( hres ) )
	{
		CoUninitialize();
	}

	hres = CoCreateInstance( CLSID_WbemLocator , 0 , CLSCTX_INPROC_SERVER , IID_IWbemLocator , (LPVOID *)&pLoc );

	if( FAILED( hres ) )
	{
		CoUninitialize();
	}

	hres = pLoc->ConnectServer( ConvertStringToBSTR( pcid_root_cimv2 ) , NULL , NULL , 0 , NULL , 0 , 0 , &pSvc );

	if( FAILED( hres ) )
	{
		pLoc->Release();
		CoUninitialize();
	}

	hres = CoSetProxyBlanket( pSvc , RPC_C_AUTHN_WINNT , RPC_C_AUTHZ_NONE , NULL , RPC_C_AUTHN_LEVEL_CALL ,
							  RPC_C_IMP_LEVEL_IMPERSONATE , NULL , EOAC_NONE );

	if( FAILED( hres ) )
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
	}

	hres = pSvc->ExecQuery( ConvertStringToBSTR( pcid_wql ) ,
							ConvertStringToBSTR( pcid_w32 ) ,
							WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY , NULL , &pEnumerator );

	if( FAILED( hres ) )
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
	}

	HRESULT hr = pEnumerator->Next( WBEM_INFINITE , 1 , &pclsObj , &uReturn );
	hr = pclsObj->Get( ConvertStringToBSTR( pcid_prc ) , 0 , &vtProp , 0 , 0 );
	Result = ConvertBSTRToString( vtProp.bstrVal );
	VariantClear( &vtProp );

	pclsObj->Release();

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return Result;
}

void reverse( char s[] )
{
	int i , j;
	char c;
	for( i = 0 , j = lstrlenA( s ) - 1; i<j; i++ , j-- )
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void nt_itoa( int n , char s[] )
{
	int i , sign;
	if( ( sign = n ) < 0 )
		n = -n;
	i = 0;
	do
	{
		s[i++] = n % 10 + '0';
	}
	while( ( n /= 10 ) > 0 );
	if( sign < 0 )
		s[i++] = '-';
	s[i] = '\0';
	reverse( s );
}

void BuildLicenseQuery( char* hash )
{
	char* query = AllocChar( 64 );
	char* hdd = GetHDD();
	char* rndv = AllocChar( 3 );
	nt_itoa( GetRnd( 1 , 10 ) , rndv );
	lstrcpyA( query , hdd );
	lstrcatA( query , "|" );
	lstrcatA( query , rndv );
	char* uhash = HashMD5( query );
	lstrcpyA( hash , uhash );
	FreeChar( uhash );
	FreeChar( rndv );
	FreeChar( hdd );
	FreeChar( query );
}

bool CheckMd5Request( char* ReciveHash )
{
	char* temp_value = AllocChar( 3 );
	char* license_ok = AllocChar( 22 );
	char* md5_license;

	for( int i = 1; i <= 10; i++ )
	{
		native_memset( license_ok , 22 , 0 );
		native_memset( temp_value , 3 , 0 );

		lstrcpyA( license_ok , MYAC_PRIVATE_KEY );

		nt_itoa( i , temp_value );
		lstrcatA( license_ok , temp_value );
		md5_license = HashMD5( license_ok );

		if( lstrcmpA( ReciveHash , md5_license ) == 0 )
		{
			FreeChar( md5_license );
			FreeChar( license_ok );
			FreeChar( temp_value );

			return true;
		}

		FreeChar( md5_license );
	}

	FreeChar( license_ok );
	FreeChar( temp_value );

	return false;
}

bool CheckLicenseRequest()
{
	DWORD rec_timeout = 5000;
	DWORD NumberOfBytesRead = 0;

	HINTERNET hInternet = InternetOpenA( "" , INTERNET_OPEN_TYPE_PRECONFIG , NULL , NULL , 0 );
	InternetSetOptionA( hInternet , INTERNET_OPTION_RECEIVE_TIMEOUT , &rec_timeout , sizeof( rec_timeout ) );

	if( !hInternet )
		return false;

	char* ReciveHash = AllocChar( 33 );
	char* UrlRequst = AllocChar( 128 );
	char* UserHash = AllocChar( 33 );

	BuildLicenseQuery( UserHash );

	lstrcpyA( UrlRequst , MYAC_LICENSE_HOST );

	lstrcatA( UrlRequst , UserHash );

	HINTERNET OpenAddress = InternetOpenUrlA( hInternet , UrlRequst , NULL , 0 ,
											  INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION , 0 );

	if( !OpenAddress )
	{
		InternetCloseHandle( hInternet );
		return false;
	}

	if( InternetReadFile( OpenAddress , ReciveHash , 33 , &NumberOfBytesRead ) && NumberOfBytesRead )
	{
		InternetCloseHandle( OpenAddress );
		InternetCloseHandle( hInternet );

		FreeChar( UserHash );
		FreeChar( UrlRequst );

		if( CheckMd5Request( ReciveHash ) )
		{
			FreeChar( ReciveHash );
			return true;
		}
		else
		{
			FreeChar( ReciveHash );
			return false;
		}
	}

	return false;
}

bool CheckHostKeyRequest()
{
	DWORD rec_timeout = 5000;
	DWORD NumberOfBytesRead = 0;

	HINTERNET hInternet = InternetOpenA( "" , INTERNET_OPEN_TYPE_PRECONFIG , NULL , NULL , 0 );
	InternetSetOptionA( hInternet , INTERNET_OPTION_RECEIVE_TIMEOUT , &rec_timeout , sizeof( rec_timeout ) );

	if( !hInternet )
		return false;

	char* UrlRequst = AllocChar( 128 );
	char* TempValue = AllocChar( 7 );
	char* ReciveKey = AllocChar( 33 );
	char* RValidKey = AllocChar( 33 );

	int RTmpVal = ( GetRnd( 100000 , 999999 ) );

	RTmpVal ^ MYAC_CRYPT_KEY;

	nt_itoa( RTmpVal , TempValue );

	lstrcpyA( UrlRequst , MYAC_LICENSE_HKEY );

	lstrcatA( UrlRequst , TempValue );

	native_memset( TempValue , 7 , 0 );

	nt_itoa( RTmpVal ^ MYAC_CRYPT_KEY , TempValue );
	lstrcpyA( RValidKey , MYAC_PRIVATE_KEY );

	lstrcatA( RValidKey , TempValue );

	RValidKey = HashMD5( RValidKey );

	FreeChar( TempValue );

	HINTERNET OpenAddress = InternetOpenUrlA( hInternet , UrlRequst , NULL , 0 ,
											  INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION , 0 );

	if( !OpenAddress )
	{
		InternetCloseHandle( hInternet );
		return false;
	}

	if( InternetReadFile( OpenAddress , ReciveKey , 33 , &NumberOfBytesRead ) && NumberOfBytesRead )
	{
		InternetCloseHandle( OpenAddress );
		InternetCloseHandle( hInternet );

		FreeChar( UrlRequst );

		if( lstrcmpA( ReciveKey , RValidKey ) == 0 )
		{
			FreeChar( RValidKey );
			FreeChar( ReciveKey );
			FreeChar( UrlRequst );

			return true;
		}
	}

	return false;
}

void HookUserMessages()
{
	pUserMsgBase = (PUserMsg)offset.FindUserMsgBase();

	char reset_hud_msg[] = { 'R','e','s','e','t','H','U','D',0 };
	char set_fov_msg[] = { 'S','e','t','F','O','V',0 };
	char team_info_msg[] = { 'T','e','a','m','I','n','f','o',0 };
	char cur_weapon_msg[] = { 'C','u','r','W','e','a','p','o','n',0 };
	char death_msg_msg[] = { 'D','e','a','t','h','M','s','g',0 };
	char weaponlist_msg[] = { 'W','e','a','p','o','n','L','i','s','t',0 };
	char ammox_msg[] = { 'A','m','m','o','X',0 };

	pResetHUD = HookUserMsg( reset_hud_msg , ResetHUD );
	pSetFOV = HookUserMsg( set_fov_msg , SetFOV );
	pTeamInfo = HookUserMsg( team_info_msg , TeamInfo );
	pCurWeapon = HookUserMsg( cur_weapon_msg , CurWeapon );
	pDeathMsg = HookUserMsg( death_msg_msg , DeathMsg );
	pWeaponList = HookUserMsg( weaponlist_msg , WeaponList );
	pAmmoX = HookUserMsg( ammox_msg , AmmoX );
}

void HookEngineMessages()
{
	pEngineMsgBase = (PEngineMsg)offset.FindSVCMessages();

	char svc_sound_msg[] = { 's','v','c','_','s','o','u','n','d',0 };
	char svc_spawnstatic_sound_msg[] = { 's','v','c','_','s','p','a','w','n','s','t','a','t','i','c','s','o','u','n','d',0 };

	pSVC_sound = HookEngineMsg( svc_sound_msg , SVC_Sound );
	pSVC_SpawnStaticSound = HookEngineMsg( svc_spawnstatic_sound_msg , SVC_SpawnStaticSound );
}

void UnHookFunction();

void InitHack()
{
	//if( CheckLicenseRequest() && CheckHostKeyRequest() )
	//{
		g_Leis.InitHack();
	//}
	//else
	//{
	//	UnHookFunction();
	//}
}

void HUD_Frame( double time )
{
	if( !FirstFrame )
	{
		g_Screen.iSize = sizeof( SCREENINFO );

		offset.HLType = g_Studio.IsHardware() + 1;

		HookUserMessages();
		HookEngineMessages();

		InitHack();

		FirstFrame = true;
	}

	g_Engine.pfnGetScreenInfo( &g_Screen );
	g_Engine.pNetAPI->Status( &( g_nStatus ) );

	g_Client.HUD_Frame( time );

	if( g_nStatus.connected && !ClearDisconnect )
	{
		ClearDisconnect = true;
	}
	else if( !g_nStatus.connected && ClearDisconnect || !g_Engine.pfnGetServerTime() )
	{
		g_Entity.ClearEntity();
		g_Sound.ClearSound();
		g_WeaponList.WeaponListClear();
		g_Players.ClearPlayers();

		ClearDisconnect = false;
	}
}

void HUD_Redraw( float time , int intermission )
{
	g_Client.HUD_Redraw( time , intermission );

	if( g_nStatus.connected )
		g_Leis.HUD_Redraw();
}

int HUD_Key_Event( int down , int keynum , const char *pszCurrentBinding )
{
	if( g_nStatus.connected )
	{
		if( down )
		{
			g_Leis.HUD_Key_Event( keynum );

			if( g_Menu.Visible || g_Menu.ExitMenu )
			{
				if( keynum == cvar.key_NavEnter )
					return 0;
				else if( keynum == cvar.key_TogglePanic )
					return 0;
				else if( keynum == cvar.key_NavLeave )
					return 0;
				else if( keynum == cvar.key_NavUp )
					return 0;
				else if( keynum == cvar.key_NavDown )
					return 0;
				else if( keynum == cvar.key_NavLeft )
					return 0;
				else if( keynum == cvar.key_NavRight )
					return 0;
			}
		}
	}

	return g_Client.HUD_Key_Event( down , keynum , pszCurrentBinding );
}

void HUD_PlayerMove( struct playermove_s *ppmove , int server )
{
	g_Client.HUD_PlayerMove( ppmove , server );

	if( g_nStatus.connected )
		g_Leis.HUD_PlayerMove( ppmove );
}

void V_CalcRefdef( struct ref_params_s *pparams )
{
	if( g_nStatus.connected )
		g_Leis.V_CalcRefdef( pparams );
	else
		g_Client.V_CalcRefdef( pparams );
}

void StudioEntityLight( struct alight_s *plight )
{
	if( g_nStatus.connected )
		g_Leis.StudioEntityLight( plight );

	g_Studio.StudioEntityLight( plight );
}

int HUD_AddEntity( int type , struct cl_entity_s *ent , const char *modelname )
{
	if( g_nStatus.connected )
		return g_Leis.HUD_AddEntity( type , ent , modelname );
	else
		return g_Client.HUD_AddEntity( type , ent , modelname );
}

void CL_CreateMove( float frametime , usercmd_s *cmd , int active )
{
	g_Client.CL_CreateMove( frametime , cmd , active );

	if( g_nStatus.connected )
	{
		g_Players.UpdatePlayerInfo();
		g_Leis.CL_CreateMove( frametime , cmd );
	}
}

void HUD_PostRunCmd( struct local_state_s *from , struct local_state_s *to , struct usercmd_s *cmd , int runfuncs , double time , unsigned int random_seed )
{
	g_Client.HUD_PostRunCmd( from , to , cmd , runfuncs , time , random_seed );

	if( g_nStatus.connected )
		g_Leis.HUD_PostRunCmd( to , cmd , runfuncs , time , random_seed );
}

int HUD_UpdateClientData( client_data_t *pcldata , float flTime )
{
	if( pcldata )
		g_WeaponList.WeaponListUpdate( pcldata->iWeaponBits );

	return g_Client.HUD_UpdateClientData( pcldata , flTime );
}

void HookFunction()
{
	g_pClient->HUD_Frame = HUD_Frame;
	g_pClient->HUD_Redraw = HUD_Redraw;
	g_pClient->HUD_Key_Event = HUD_Key_Event;
	g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	g_pClient->V_CalcRefdef = V_CalcRefdef;
	g_pStudio->StudioEntityLight = StudioEntityLight;
	g_pClient->HUD_AddEntity = HUD_AddEntity;
	g_pClient->CL_CreateMove = CL_CreateMove;
	g_pClient->HUD_PostRunCmd = HUD_PostRunCmd;
	g_pClient->HUD_UpdateClientData = HUD_UpdateClientData;
}

void UnHookFunction()
{
	g_pClient->HUD_Frame = g_Client.HUD_Frame;
	g_pClient->HUD_Redraw = g_Client.HUD_Redraw;
	g_pClient->HUD_Key_Event = g_Client.HUD_Key_Event;
	g_pClient->HUD_PlayerMove = g_Client.HUD_PlayerMove;
	g_pClient->V_CalcRefdef = g_Client.V_CalcRefdef;
	g_pStudio->StudioEntityLight = g_Studio.StudioEntityLight;
	g_pClient->HUD_AddEntity = g_Client.HUD_AddEntity;
	g_pClient->CL_CreateMove = g_Client.CL_CreateMove;
	g_pClient->HUD_PostRunCmd = g_Client.HUD_PostRunCmd;
	g_pClient->HUD_UpdateClientData = g_Client.HUD_UpdateClientData;
}