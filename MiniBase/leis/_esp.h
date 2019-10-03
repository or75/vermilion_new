#pragma once

#include "../main.h"

class cEsp
{
public:

	void DrawNoFlash();
	void DrawPlayerESP( int iIndex );
	void DrawAllEntityESP();

	void HUD_AddEntity( int type , struct cl_entity_s *ent );
};

extern cEsp g_Esp;