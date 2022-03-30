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
#include "zescape/trigger_output.h"
#include "zescape/voice_chat.h"
public Plugin myinfo = {
	name = " EXG_Zombie_Escape_RY",
	author = "Rainsfield&WackoD",
	description = "EXG ZombieEscape Rainsfield's Branch Plugins",
	version = "1.0",
	url = "https://zegod.cn"
};

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
	//TriggerOutputOnPluginStart();
	VoiceChatOnPluginStart();

}

public void OnMapStart() 
{
	PrintToServer("[OnMapStart]");
	//TriggerOutputOnMapStart();
	MapInfoOnMapStart();
	RTVOnMapStart();

	NominateOnMapStart();
	//RoundOnMapStart();
	if(!isDbConnected())	return;			//未连接，return，通过Db连接函数的函数执行Post，已连接则通直接Post使得换图后重载各插件数据
	PrintToServer("DbOnDbConnected_MapStartPost");
	DbOnDbConnected_MapStartPost();	
}
public void OnPluginEnd() 
{
    Db_Close();
}
public void OnMapEnd() 
{
	MapAdmOnMapEnd();
	NomOnMapEnd();
	RTVOnMapEnd();
}
public int ZR_OnClientInfected(int client, int attacker, bool motherInfect, bool respawnOverride, bool respawn) 
{
	if(motherInfect)
	{
		OnRoundInfected();
	}
}

public void OnClientDisconnect(int client)
{
	NominateOnClientDisconnect(client);
	RTVOnClientDisconnect(client);
}
public void OnClientPutInServer(int client)
{
	VoiceChatOnClientConnected(client);
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