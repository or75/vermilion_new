#pragma once

#include "../main.h"

#define MAX_MENU_COUNT		7
#define MAX_SUB_MENU_COUNT	25

#define TEXT_PADDING_LEFT	3

#define MENU_ENTRY_WIDTH	110
#define MENU_PADDING_TOP	2

#define SUBM_ENTRY_WIDTH	185
#define SUBM_PADDING_LEFT	2
#define SUBM_VALUE_PADDING	35

#define ENTRY_HEIGHT		13

#define MENU_ENTRY_KEY		0xB83FB
#define SUBM_ENTRY_KEY		0xA8B4C

struct m_entry
{
	char name[32];
};

struct s_entry
{
	char name[32];
	int* value;
	int min;
	int max;
	int step;
};

class cMenu
{
public:

	cMenu();

	void InitializeMenu();

	void DrawMenu( int x , int y );
	void KeyEvent( int iKeynum );
	
	bool Visible;
	bool DisableReload;
	bool ExitMenu;

	int menu_head_width;
	
private:

	HANDLE hHeap;
	PCHAR pValue;
	
	int M_EntryPos;
	int M_EntrySize;

	int S_EntryPos;
	int S_EntrySize;
	
	bool SubMenu;

	DWORD dwLastSaveTime;
	DWORD dwLastLoadTime;

	m_entry menu_entry[MAX_MENU_COUNT];
	s_entry subm_entry[MAX_SUB_MENU_COUNT];

	void AddMenu( char* name );
	void AddSubMenu( char* name , int* value , int min , int max , int step );

	void ClearMenu();
};

extern cMenu g_Menu;