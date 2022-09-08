bool g_pStore = false;
#include <clientprefs>
#include <cstrike>
#include <sdktools>
#include <sdkhooks>
//#include <json>
//#include <outputinfo>
#include <zombiereloaded>
#include <store>
#include <timers>
#include <nextmap>
#include <rainsfield>
#include <server_redirect>
#include <leader>
#include <mostactive>
#include <outputinfo>
#pragma semicolon 1
#pragma newdecls required
#include "zescape/basic_func.h"
#include "exgusers/users.h"
#include "exgusers/db.h"
#include "exgusers/announcement.h"
#include "exgusers/say.h"
#include "exgusers/adminlog.h"
#include "exgusers/server.h"
#include "exgusers/monitor.h"
public Plugin myinfo = {
	name = " EXG_CSGO_Users",
	author = "Rainsfield&WackoD&ExgNullable",
	description = "EXG CSGO SERVER USER SYSTEM",
	version = "1.0",
	url = "https://zegod.cn"
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
	DbOnPluginStart();
	AnnouncementOnPluginStart();
	UsersAdmOnPluginStart();
	AdminLogOnPluginStart();
	ServerOnPluginStart();
	MonitorOnPluginStart();
}

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	RegPluginLibrary("exgusers");

	CreateNative("EXGUSERS_AddAdminLog",Native_EXGUSERS_AddAdminLog);
	CreateNative("EXGUSERS_GetUserInfo",Native_EXGUSERS_GetUserInfo);
	CreateNative("EXGUSERS_GetServerByPort",Native_EXGUSERS_GetServerByPort);
	
	MarkNativeAsOptional("Store_GetClientCredits");
	MarkNativeAsOptional("Store_SetClientCredits");
	MarkNativeAsOptional("Store_GetItemIdbyUniqueId");
	MarkNativeAsOptional("Store_GiveItem");
	MarkNativeAsOptional("Store_HasClientItem");
}
public void OnMapStart()
{
	if(!isDbConnected())
		return;

	AnnouncementOnMapStart();
}

public void OnClientPostAdminCheck(int client)
{
	UsersOnClientInServer(client);
}

public void OnClientDisconnect(int client)
{
	MonitorOnClientDisconnect(client);
	UsersOnClientDisconnect(client);
}