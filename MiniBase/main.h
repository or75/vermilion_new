#pragma once

#include <Windows.h>
#include <TlHelp32.h>

#pragma comment(linker,"/entry:DllMain")
#pragma comment(linker, "/merge:.CRT=.rdata")

#include "ValveSDK/engine/wrect.h"
#include "ValveSDK/engine/cl_dll.h"
#include "ValveSDK/engine/r_studioint.h"
#include "ValveSDK/engine/cl_entity.h"
#include "ValveSDK/misc/com_model.h"
#include "ValveSDK/engine/triangleapi.h"
#include "ValveSDK/engine/pmtrace.h"
#include "ValveSDK/engine/pm_defs.h"
#include "ValveSDK/engine/pm_info.h"
#include "ValveSDK/common/ref_params.h"
#include "ValveSDK/common/studio_event.h"
#include "ValveSDK/common/net_api.h"
#include "ValveSDK/common/r_efx.h"
#include "ValveSDK/engine/cvardef.h"
#include "ValveSDK/engine/util_vector.h"
#include "ValveSDK/misc/parsemsg.h"
#include "ValveSDK/engine/studio.h"
#include "ValveSDK/engine/event_args.h"
#include "ValveSDK/engine/event_flags.h"
#include "ValveSDK/common/event_api.h"
#include "ValveSDK/common/screenfade.h"
#include "ValveSDK/engine/keydefs.h"
#include "ValveSDK/common/engine_launcher_api.h"
#include "ValveSDK/common/entity_types.h"

#include "struct.h"
#include "offset.h"
#include "client.h"
#include "utils.h"
#include "usermsg.h"
#include "enginemsg.h"
#include "font.h"
#include "drawing.h"

#include "leis/_math.h"
#include "leis/_utils.h"
#include "leis/_cvar.h"
#include "leis/_client.h"
#include "leis/_xorstr.h"
#include "leis/_menu.h"
#include "leis/_cheat.h"
#include "leis/_aimbot.h"
#include "leis/_player.h"
#include "leis/_esp.h"
#include "leis/_nospread.h"
#include "leis/_weaponlist.h"

extern cl_clientfunc_t *g_pClient;
extern cl_enginefunc_t *g_pEngine;
extern engine_studio_api_t *g_pStudio;

extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;
extern engine_studio_api_t g_Studio;

extern PUserMsg pUserMsgBase;
extern PEngineMsg pEngineMsgBase;

extern SCREENINFO g_Screen;

extern PCHAR BaseDir;