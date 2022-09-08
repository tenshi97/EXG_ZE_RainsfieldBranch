bool g_pStore = false;
#include <clientprefs>
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
#pragma semicolon 1
#pragma newdecls required
#include "zescape/entwatchedit.h"
#include "zescape/weaponedit.h"

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
	EWEditOnPluginStart();
	WeaponEditOnPluginStart();
}

public void OnMapStart() 
{
	EWEditOnMapStart();
	WeaponEditOnMapStart();
}
public void OnEntityCreated(int entity, const char[] classname)
{
	EWEditOnEntityCreated(entity, classname);	
}


public void OnPluginEnd() 
{

}

public void OnMapEnd() 
{

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