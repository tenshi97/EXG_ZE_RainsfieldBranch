#include <clientprefs>
#include <cstrike>
#include <sdktools>
#include <sdkhooks>
#include <json>
#include <outputinfo>
#include <zombiereloaded>

#pragma semicolon 1
#pragma newdecls required
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

public void OnPluginStart()	{
	FlyOnPluginStart();
	EventOnPluginStart();
}
public void OnMapStart() {
	PrintToServer("[OnMapStart]");
	//RoundOnMapStart();
}

public int ZR_OnClientInfected(int client, int attacker, bool motherInfect, bool respawnOverride, bool respawn) {
	if(motherInfect)
	{
		OnRoundInfected();
	}
}