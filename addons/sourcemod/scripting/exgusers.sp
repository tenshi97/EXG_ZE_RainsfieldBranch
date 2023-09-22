#pragma semicolon 1
#pragma newdecls required

//////////////////////////////
//    PLUGIN DEFINITION     //
//////////////////////////////
#define PLUGIN_NAME         "EXG_CSGO_Users"
#define PLUGIN_AUTHOR       "Rainsfield&WackoD&ExgNullable"
#define PLUGIN_DESCRIPTION  "EXG CSGO SERVER USER SYSTEM"
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
#include <clientprefs>
#include <cstrike>
#include <sdkhooks>
#include <sdktools>
#include <csgopwapi>
#include <exg_group_tag>
#include <herolevel>
#include <leader>
#include <mostactive>
#include <nextmap>
#include <outputinfo>
#include <rainsfield>
#include <server_redirect>
#include <store>
#include <timers>
#include <weddings>
//#include <json>
//#include <outputinfo>

#include "exgusers/users.h"
#include "exgusers/adminlog.h"
#include "exgusers/announcement.h"
#include "exgusers/basic_func.h"
#include "exgusers/db.h"
#include "exgusers/monitor.h"
#include "exgusers/nametag.h"
#include "exgusers/nomban.h"
#include "exgusers/profitban.h"
#include "exgusers/say.h"
#include "exgusers/server.h"
#include "exgusers/storeplus.h"
#include "exgusers/timer.h"
#include "exgusers/uadmin.h"

//////////////////////////////
//          DEFINE          //
//////////////////////////////
bool g_pStore = false;
SERVER_LOG g_current_server;
const int TIME_PERMANENT = 2000000000;
int pay_limit[65] = {0};

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
	LoadTranslations("common.phrases");
	DbOnPluginStart();
	AnnouncementOnPluginStart();
	UsersAdmOnPluginStart();
	ServerOnPluginStart();
	TimerOnPluginStart();
	uAdminOnPluginStart();
	StorePlusOnPluginStart();
	NameTagOnPluginStart();
}

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	CreateNative("EXGUSERS_AddAdminLog", Native_EXGUSERS_AddAdminLog);
	CreateNative("EXGUSERS_GetUserInfo", Native_EXGUSERS_GetUserInfo);
	CreateNative("EXGUSERS_GetServerByPort", Native_EXGUSERS_GetServerByPort);
	CreateNative("EXGUSERS_GetUserUID", Native_EXGUSERS_GetUserUID);
	CreateNative("EXGUSERS_GetUserPbanState", Native_EXGUSERS_GetUserPbanState);
	CreateNative("EXGUSERS_GetUserNameTag", Native_GetUserNameTag);
	//	CreateNative("EXGUSERS_GetUserInfoByUID",Native_EXGUSERS_GetUserInfoByUID);
	RegPluginLibrary("exgusers");
	MarkNativeAsOptional("Store_GetClientCredits");
	MarkNativeAsOptional("Store_SetClientCredits");
	MarkNativeAsOptional("Store_GetItemId");
	MarkNativeAsOptional("Store_GiveItem");
	MarkNativeAsOptional("Store_HasClientItem");
}

public void OnMapStart()
{
	if (!isDbConnected()) return;
	AnnouncementOnMapStart();
	StorePlusOnMapStart();
}

public void OnClientPostAdminCheck(int client)
{
	UsersOnClientInServer(client);
	StorePlusOnClientConnected(client);
}

public void OnClientDisconnect(int client)
{
	MonitorOnClientDisconnect(client);
	UsersOnClientDisconnect(client);
}

public void RY_Daily_TimerCheck_Update(int start_time, int new_time, int days_delta)
{
	ClearPayLimit();
}
