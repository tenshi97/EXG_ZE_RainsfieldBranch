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
#pragma semicolon 1
#pragma newdecls required
#include "zescape/basic_func.h"
#include "exgusers/users.h"
#include "exgusers/db.h"
#include "exgusers/announcement.h"
public Plugin myinfo = {
	name = " EXG_CSGO_Users",
	author = "Rainsfield&WackoD&",
	description = "EXG CSGO SERVER USER SYSTEM",
	version = "1.0",
	url = "https://zegod.cn"
}
public void OnPluginStart()
{
	DbOnPluginStart();
	AnnouncementOnPluginStart();
	UsersAdmOnPluginStart();
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