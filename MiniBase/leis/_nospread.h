#pragma once

#include "../main.h"

class cNoSpread;

class cNoSpread
{
public:
	void GetSpreadOffset( unsigned int seed , int future , float *inangles , float *velocity , float *outangles );
	void HUD_PostRunCmd( struct local_state_s *to , struct usercmd_s *cmd , int runfuncs , double time , unsigned int random_seed );
	void CL_CreateMove( usercmd_s *cmd );
	void DefaultSpread( int iWeaponID );
	void NoSpread();
private:

	unsigned int U_Random( void );
	void U_Srand( unsigned int seed );
	int UTIL_SharedRandomLong( unsigned int seed , int low , int high );
	float UTIL_SharedRandomFloat( unsigned int seed , float low , float high );

	float GetVecSpread( float speed );
	void PrimaryAttack( void );
	void VectorAngles( const float *forward , float *angles );
	void GetSpreadXY( unsigned int seed , int future , float *velocity , float *vec );
};

extern cNoSpread g_NoSpread;