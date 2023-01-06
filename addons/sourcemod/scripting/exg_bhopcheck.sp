#include <sourcemod>
#include <sdktools>
#include <smutils>
public Plugin myinfo = {
	name = " EXG BHOP CHECK",
	author = "Rainsfield&WackoD&EXGNullab",
	description = "For Server MACRO CHECK USE",
	version = "1.0",
	url = "https://zegod.cn"
};
public OnPluginStart()
{
    HookEvent("player_jump",Event_PlayerJump,EventHookMode_Post);
}
public void Event_PlayerJump(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event,"userid"));
	float player_velocity[3];
	GetEntPropVector(client,,"m_vecVelocity",player_velocity);
	player_velocity[2]=0.0;
	int player_speed = GetVectorLength(player_velocity);

	
}	