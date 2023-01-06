
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
#include <smlib2>
#pragma semicolon 1
#pragma newdecls required
#include "zescape/entwatchedit.h"
#include "zescape/weaponedit.h"
#include "zescape/zelevel.h"
#include "zescape/leaderspr.h"
bool g_pStore;
public Plugin myinfo = {
	name = " EXG_Zombie_Escape_MISC",
	author = "Rainsfield&WackoD&EXGNullable",
	description = "EXG ZombieEscape Rainsfield's Branch Plugins Miscellous",
	version = "1.0",
	url = "https://zegod.cn"
};
public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{

	MarkNativeAsOptional("Store_GetClientCredits");
	MarkNativeAsOptional("Store_SetClientCredits");
	MarkNativeAsOptional("Store_GetItemIdbyUniqueId");
	MarkNativeAsOptional("Store_GiveItem");
	MarkNativeAsOptional("Store_HasClientItem");

	CreateNative("EXGZE_GetLevelDiffName",Native_GetLevelDiffName);
	CreateNative("EXGZE_GetCurrentLevel",Native_GetCurrentLevel);
	CreateNative("EXGZE_ToggleSprite",Native_ToggleSprite);

	RegPluginLibrary("exg_ze_misc");
}

public void OnLibraryAdded(const char[] name)
{
	// Validate library
	if (StrContains(name, "store", false) != -1)
	{
		g_pStore = (GetFeatureStatus(FeatureType_Native, "Store_GetClientCredits") == FeatureStatus_Available);
	}
}

public void OnLibraryRemoved(const char[] name)
{
	// Validate library
	if (StrContains(name, "store", false) != -1)
	{
		g_pStore = (GetFeatureStatus(FeatureType_Native, "Store_GetClientCredits") == FeatureStatus_Available);
	}
}

public void OnPluginStart()
{
	HookEvent("round_end", OnRoundEnd, EventHookMode_Post);
	HookEvent("round_start", OnRoundStart, EventHookMode_Post);
	EWEditOnPluginStart();
	WeaponEditOnPluginStart();
	LevelOnPluginStart();
	LeaderSpriteOnPluginStart();
}

public void OnMapStart()
{
	EWEditOnMapStart();
	WeaponEditOnMapStart();
	LevelOnMapStart();
	LeaderSpriteOnMapStart();
}
public void OnEntityCreated(int entity, const char[] classname)
{
	EWEditOnEntityCreated(entity, classname);
	LevelOnEntityCreated(entity,classname);
}


public void OnRoundStart(Event event, const char[] name, bool dontBroadcast)
{
	LeaderSpriteOnRoundStart();
}
public void OnRoundEnd(Event event, const char[] name, bool dontBroadcast)
{
	LevelOnRoundEnd();
}

public void OnClientDisconnect(int client)
{
	EWEditOnClientDisconnect(client);
	WeaponEditOnClientDisconnect(client);
}
public void OnClientPostAdminCheck(int client)
{

}
public Action OnClientSayCommand(int client, const char[] command, const char[] sArgs)
{
	if(client<=0||client>64)	return Plugin_Continue;
	if(!IsClientInGame(client)||IsChatTrigger())	return Plugin_Continue;
	if(EntAdmIsClientModifying(client,sArgs))
	{
		return Plugin_Handled;
	}
	if(WeaponIsClientModifying(client,sArgs))
	{
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public int ZR_OnClientInfected(int client, int attacker, bool motherInfect, bool respawnOverride, bool respawn)
{
	if(client == Leader_CurrentLeader())
	{
		HideLeaderIcon();
	}
	else
	{
		if(motherInfect&&Leader_CurrentLeader()!=-1)
		{
			CreateLeaderIcon(Leader_CurrentLeader());
		}
	}
}