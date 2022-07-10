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
#include "zescape/basic_func.h"
#include "zescape/event.h"
#include "zescape/db.h"
#include "zescape/map_adm.h"
#include "zescape/map_info.h"
#include "zescape/nominate.h"
#include "zescape/rtv.h"
#include "zescape/round.h"
#include "zescape/fly.h"
#include "zescape/jump.h"
//#include "zescape/trigger_output.h"
#include "zescape/voice_chat.h"
#include "zescape/spec.h"
#include "zescape/time.h"
#include "zescape/mission.h"
#include "zescape/quest.h"
#include "zescape/hd.h"
Handle g_Warmup_Timer;
ConVar g_Cvar_Mp_Warmup_Time;

public Plugin myinfo = {
	name = " EXG_Zombie_Escape_RY",
	author = "Rainsfield&WackoD",
	description = "EXG ZombieEscape Rainsfield's Branch Plugins",
	version = "1.0",
	url = "https://zegod.cn"
};
public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
	CreateNative("RY_MapProperty_BanHumanSkills",Native_RY_MapProperty_BanHumanSkills);
	CreateNative("RY_MapProperty_BanZombieSkills",Native_RY_MapProperty_BanZombieSkills);
	CreateNative("RY_MapProperty_BanJK",Native_RY_MapProperty_BanJK);
	CreateNative("RY_MapProperty_NoBhopLimit",Native_RY_MapProperty_NoBhopLimit);
	CreateNative("RY_Mission_GetClientMissionLevel",Native_RY_GetClientMissionLevel);
	CreateNative("RY_Mission_GiveClientExp",Native_RY_GiveClientMissionExp);
	CreateNative("RY_Map_GetMapInfo",Native_RY_Map_GetMapInfo);
	CreateNative("RY_Map_GetCurrentMapInfo",Native_RY_Map_GetCurrentMapInfo);
//	CreateNative("RY_Map_GetMapDataByMapName",Native_Map_GetMapDataByMapName);
//	CreateNative("RY_MapProperty_GetMapPushBackFactor");	

	MarkNativeAsOptional("Store_GetClientCredits");
	MarkNativeAsOptional("Store_SetClientCredits");
	MarkNativeAsOptional("Store_GetItemIdbyUniqueId");
	MarkNativeAsOptional("Store_GiveItem");
	MarkNativeAsOptional("Store_HasClientItem");

	RegPluginLibrary("exg_csgo_ze_ry");
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
	FlyOnPluginStart();
	EventOnPluginStart();
	DbOnPluginStart();
	MapAdmOnPluginStart();
	MapInfoOnPluginStart();
	NominateOnPluginStart();
	RTVOnPluginStart();
	LoadTranslations("common.phrases");
	JumpOnPluginStart();
//	TriggerOutputOnPluginStart();
	VoiceChatOnPluginStart();
	SpecOnPluginStart();
	TimeOnPluginStart();
	MissionOnPluginStart();
	QuestOnPluginStart();
	HdOnPluginStart();
}

public void OnMapStart() 
{
	PrintToServer("[OnMapStart]");
	//TriggerOutputOnMapStart();
	MapInfoOnMapStart();
	RTVOnMapStart();
	WarmUpTimerBuild();
	NominateOnMapStart();
	QuestOnMapStart();
	HdOnMapStart();
	//RoundOnMapStart();
	if(!isDbConnected())	return;			//未连接，return，通过Db连接函数的函数执行Post，已连接则通直接Post使得换图后重载各插件数据
	PrintToServer("DbOnDbConnected_MapStartPost");
	DbOnDbConnected_MapStartPost();	
}
/*
public void OnEntityCreated(int entity, const char[] classname)
{
	TriggerOnEntityCreated(entity, classname);
}
*/
void WarmUpTimerBuild()
{
	g_Warmup_Timer = INVALID_HANDLE;
	KillTimerSafe(g_Warmup_Timer);
	float warmuptime=100.0;
	g_Cvar_Mp_Warmup_Time = FindConVar("mp_warmuptime");
	if(g_Cvar_Mp_Warmup_Time)
	{
		warmuptime = g_Cvar_Mp_Warmup_Time.FloatValue;
	}
	g_Warmup_Timer = CreateTimer(warmuptime,OnWarmUpEnd, _,TIMER_FLAG_NO_MAPCHANGE);
}

Action OnWarmUpEnd(Handle timer)
{
	MapInfoOnWarmUpEnd();
}

public void OnPluginEnd() 
{
    Db_Close();
}

public void OnMapEnd() 
{
	MapAdmOnMapEnd();
	NominateOnMapEnd();
	RTVOnMapEnd();
	MissionOnMapEnd();
	QuestOnMapEnd();
}

public int ZR_OnClientInfected(int client, int attacker, bool motherInfect, bool respawnOverride, bool respawn) 
{
	if(motherInfect)
	{
		OnRoundInfected();
	}
	if(attacker>=1&&attacker<=64&&client>=1&&client<=64)
	{
		if(IsClientInGame(attacker)&&!IsFakeClient(attacker))
		{
			MissionZombieInfectHuman(attacker);
			QuestZombieInfectHuman(attacker);
		}
	}
}

public void OnClientDisconnect(int client)
{
	NominateOnClientDisconnect(client);
	RTVOnClientDisconnect(client);
	MissionOnClientDisconnect(client);
	QuestOnPlayerDisconnected(client);
}
public void OnClientPostAdminCheck(int client)
{
	VoiceChatOnClientConnected(client);
	MissionOnClientConnected(client);
	QuestOnPlayerConnected(client);
}
/*
	int id;
	char name[PLATFORM_MAX_PATH];
	char name_cn[PLATFORM_MAX_PATH];
	int cooldown;
	int last_run_time;
	int available;
	int cost;
	int heat;
	int difficulty;
	int tag;
	int round;
	int wins;
	int translated;
	int download;
	int exist;
*/
//