#pragma semicolon 1
#pragma newdecls required

//////////////////////////////
//    PLUGIN DEFINITION     //
//////////////////////////////
#define PLUGIN_NAME         "EXG_Zombie_Escape_UserSystem"
#define PLUGIN_AUTHOR       "Rainsfield&WackoD&EXGNullable"
#define PLUGIN_DESCRIPTION  "EXG ZombieEscape UserSystem"
#define PLUGIN_VERSION      "1.0"
#define PLUGIN_URL          "https://zegod.cn"

public Plugin myinfo =
{
	name 		= PLUGIN_NAME,
	author 		= PLUGIN_AUTHOR,
	description = PLUGIN_DESCRIPTION,
	version 	= PLUGIN_VERSION,
	url 		= PLUGIN_URL
};

//////////////////////////////
//          INCLUDES        //
//////////////////////////////
#include <cstrike>
#include <sdktools>
#include <sdkhooks>
#include <outputinfo>
#include <zombiereloaded>
#include <store>
#include <timers>
#include <nextmap>
#include <server_redirect>
#include <rainsfield>
#include <leader>
#include <mostactive>
#include <sourcecomms>
#include <SteamWorks>
#include <weddings>
//#include <json>

#include "usersze/db.h"
#include "usersze/items.h"

//////////////////////////////
//          DEFINE          //
/////////////////////////////
public void OnPluginStart()
{
	HookEvent("round_start", 	OnRoundStart, 	EventHookMode_Post);
	HookEvent("round_end", 		OnRoundEnd, 	EventHookMode_Post);
	DbOnPluginStart();
}

public void EXGUSERS_OnUserLoaded(int client)
{
	ZEITEMSOnUserLoaded(client);
}