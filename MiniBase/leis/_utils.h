#pragma once

#include "../main.h"

/* START UTILS */

bool CalcScreen( float *pflOrigin , float *pflVecScreen );
float CalcDistPlayerScreen( float fScreenCenter[2] , float fPlayerPosScreen[2] );
bool PathFree( Vector vInput );
bool CalcPlayerVisibility( int Index );
void VectorAngles( const float *forward , float *angles );
float AngleBetvenVectors( Vector a , Vector b );
int CsCharToKey( char* Key );
int WindowCharToKey( char* Key );

/* END UTILS */