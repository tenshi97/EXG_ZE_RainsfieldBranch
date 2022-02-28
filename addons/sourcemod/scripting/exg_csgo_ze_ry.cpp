#include <clientprefs>
#include <cstrike>
#include <sdktools>
#include <sdkhooks>
#include <json>
#include <outputinfo>
#include <zombiereloaded>

#pragma semicolon 1
#pragma newdecls required
#include "zescape/db.h"
#include "zescape/map_adm.h"
#include "zescape/map_info.h"
#include "zescape/nominate.h"
#include "zescape/round.h"
#include "zescape/fly.h"
#include "zescape/basic_func.h"
#include "zescape/event.h"
#include "zescape/convar.h"
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
}

public void OnMapStart() 
{
	PrintToServer("[OnMapStart]");
	MapInfoOnMapStart();
	//RoundOnMapStart();
	if(!isDbConnected())	return;			//未连接，return，通过Db连接函数的函数执行Post，已连接则通直接Post使得换图后重载各插件数据
	DbOnDbConnected_MapStartPost();	
}
public void OnPluginEnd() 
{
    Db_Close();
}

public int ZR_OnClientInfected(int client, int attacker, bool motherInfect, bool respawnOverride, bool respawn) 
{
	if(motherInfect)
	{
		OnRoundInfected();
	}
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