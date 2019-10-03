#pragma once

#include "main.h"

#define OFF_ENGINE_PATTERN	"\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF\x68\xFF\xFF\xFF\xFF\x89\x86\xFF\xFF\xFF\xFF\xFF\x15\xFF\xFF\xFF\xFF"
#define OFF_SVC_MSG_PATTERN	"\xBF\xFF\xFF\xFF\xFF\x8B\x04\xB5\xFF\xFF\xFF\xFF\x85\xC0\x74\xFF\x81\xFF\xFF\xFF\xFF\xFF\x7F\x04\x8B\x0F\xEB\x05"

#define INT_DIGITS 19

extern char native_itoa_buffer[INT_DIGITS + 2];

int GetRnd( int min , int max );

PUserMsg UserMsgByName( char* szMsgName );
PEngineMsg EngineMsgByName( char* szMsgName );
pcmd_t CommandByName( char* szName );

pfnUserMsgHook HookUserMsg( char *szMsgName , pfnUserMsgHook pfn );
pfnEngineMessage HookEngineMsg( char *szMsgName , pfnEngineMessage pfn );

char* native_strcpy( char *dest , const char *src );
char* native_strncpy( char *dest , const char *src , int n );
char* native_strcat( char *dest , const char *src );
char* native_strstr( char *in , char *str );

int native_strcmp( char *pStr1 , char *pStr2 );
int native_strlen( char *pStr );
int native_strncmp( char const* _Str1 , char const* _Str2 , size_t MaxCount );

void native_memcpy( void * dst , const void * src , size_t count );
void* native_memcpy2( void * dst , const void * src , size_t count );
void native_memset( void *szBuffer , DWORD dwLen , DWORD dwSym );

char* AllocChar( DWORD dwSize );
void FreeChar( char* AllocChar );
char* ConvertBSTRToString( BSTR pSrc );
BSTR ConvertStringToBSTR( const char* pSrc );

PCHAR native_itoa( int i );