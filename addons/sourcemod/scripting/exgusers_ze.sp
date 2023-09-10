#include <cstrike>
#include <sdktools>
#include <sdkhooks>
//#include <json>
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
#include "usersze/db.h"
#include "usersze/items.h"
public void OnPluginStart()
{
	HookEvent("round_start", OnRoundStart, EventHookMode_Post);
	HookEvent("round_end", OnRoundEnd, EventHookMode_Post);
	DbOnPluginStart();
}

public void OnMapStart()
{

}
public void OnMapEnd()
{

}
void OnRoundStart(Event event, const char[] name, bool dontBroadcast)
{

}
void OnRoundEnd(Event event, const char[] name, bool dontBroadcast)
{
}
public void EXGUSERS_OnUserLoaded(int client)
{
	ZEITEMSOnUserLoaded(client);
}
public Plugin myinfo = {
	name = " EXG_Zombie_Escape_UserSystem",
	author = "Rainsfield&WackoD&EXGNullable",
	description = "EXG ZombieEscape UserSystem",
	version = "1.0",
	url = "https://zegod.cn"
};