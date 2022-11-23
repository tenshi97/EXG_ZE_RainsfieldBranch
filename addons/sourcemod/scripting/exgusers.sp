

bool g_pStore = false;
#include <clientprefs>
#include <cstrike>
#include <sdktools>
#include <sdkhooks>
//#include <json>
//#include <outputinfo>
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
#include "exgusers/timer.h"
#include "exgusers/basic_func.h"
#include "exgusers/users.h"
#include "exgusers/db.h"
#include "exgusers/announcement.h"
#include "exgusers/say.h"
#include "exgusers/adminlog.h"
#include "exgusers/server.h"
#include "exgusers/monitor.h"
#include "exgusers/nomban.h"
#include "exgusers/uadmin.h"
const int TIME_PERMANENT = 2000000000;
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
	ServerOnPluginStart();
	MonitorOnPluginStart();
	TimerOnPluginStart();
	uAdminOnPluginStart();
}
public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	CreateNative("EXGUSERS_AddAdminLog",Native_EXGUSERS_AddAdminLog);
	CreateNative("EXGUSERS_GetUserInfo",Native_EXGUSERS_GetUserInfo);
	CreateNative("EXGUSERS_GetServerByPort",Native_EXGUSERS_GetServerByPort);
	CreateNative("EXGUSERS_GetUserUID",Native_EXGUSERS_GetUserUID);
	RegPluginLibrary("exgusers");
	MarkNativeAsOptional("Store_GetClientCredits");
	MarkNativeAsOptional("Store_SetClientCredits");
	MarkNativeAsOptional("Store_GetItemIdbyUniqueId");
	MarkNativeAsOptional("Store_GiveItem");
	MarkNativeAsOptional("Store_HasClientItem");
}
public void OnMapStart()
{
	if(!isDbConnected())	return;
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
