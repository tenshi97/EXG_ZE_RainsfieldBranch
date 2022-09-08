void EventOnPluginStart() {
	//HookEvent("player_team", OnPlayerTeam, EventHookMode_Pre);
	//HookEvent("player_spawn", OnPlayerSpawn, EventHookMode_Post);
	//HookEvent("player_death", OnPlayerDeathPre, EventHookMode_Pre);
	HookEvent("player_death", OnPlayerDeathPost, EventHookMode_Post);
	HookEvent("player_hurt", OnPlayerHurt, EventHookMode_Post);
	//HookEvent("weapon_fire", OnWeaponFire, EventHookMode_Post);
	HookEvent("grenade_thrown", OnGrenadeThrown, EventHookMode_Post);
	//HookEvent("player_spawned", OnPlayerSpawned, EventHookMode_Post);
	// HookEvent("item_equip", OnItemEquip, EventHookMode_Post);
	// HookEvent("hegrenade_detonate", OnHeGrenadeDetonate, EventHookMode_Post);
	// HookEvent("inferno_startburn", OnInfernoStartBurn, EventHookMode_Pre);
	//HookEvent("decoy_started", OnDecoyStarted, EventHookMode_Post);
	//HookEvent("bullet_impact", OnBulletImpact, EventHookMode_Post);
	//HookEvent("player_jump", OnPlayerJump, EventHookMode_Post);
	//HookEvent("player_falldamage", OnPlayerFall, EventHookMode_Post);
	// HookEvent("cs_win_panel_round", OnWinPanelRound, EventHookMode_Pre);
	HookEvent("round_prestart", OnRoundPreStart, EventHookMode_Post);
	HookEvent("round_start", OnRoundStart, EventHookMode_Post);
	//HookEvent("round_poststart", OnRoundPostStart, EventHookMode_Post);
	HookEvent("round_freeze_end", OnRoundFreezeEnd, EventHookMode_Post);
	HookEvent("round_end", OnRoundEnd, EventHookMode_Post);
	HookEntityOutput("func_physbox","OnHealthChanged",Hitbox_OnHealthChanged);
	HookEntityOutput("func_physbox_multiplayer","OnHealthChanged",Hitbox_OnHealthChanged);
	HookEntityOutput("func_breakable","OnHealthChanged",Hitbox_OnHealthChanged);
}
void Hitbox_OnHealthChanged(const char[] output, int caller, int activator, float delay)
{
	if(!IsValidEntity(activator))	return;
	if(activator<=0||activator>64)	return;
	if(!IsClientInGame(activator))	return;
	if(IsFakeClient(activator))		return;
	if(!IsPlayerAlive(activator))	return;
	if(ZR_IsClientHuman(activator))
	{
		MissionOnHit(activator);
	}
}
void OnRoundPreStart(Event event, const char[] name, bool dontBroadcast) 
{
	PrintToServer("[OnRoundPreStart]");
	RoundOnRoundPreStart();
}

void OnRoundFreezeEnd(Event event, const char[] name, bool dontBroadcast) 
{
	PrintToServer("[OnRoundFreezeEnd]");
	//RoundOnRoundFreezeEnd();
}
void OnRoundInfected() {
	RoundOnInfected();
	PrintToServer("[OnRoundInfected]");
}
void OnRoundStart(Event event, const char[] name, bool dontBroadcast) 
{
	PrintToServer("[RoundStart]");
	RTVOnRoundStart();
	MapInfoOnRoundStart();
}
void OnRoundEnd(Event event, const char[] name, bool dontBroadcast) 
{
	int winner = event.GetInt("winner");
	PrintToServer("[RoundEnd] [winner:%d]",winner);
	RTVOnRoundEnd();
	MapInfoOnRoundEnd(winner);
	MissionOnRoundEnd(winner);
	QuestOnRoundEnd(winner);
	HdOnRoundEnd(winner);
}
void OnPlayerHurt(Event event, const char[] name, bool dontBroadcast)
{
	int victim = GetClientOfUserId(GetEventInt(event, "userid"));
	int attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	int damage = GetEventInt(event, "dmg_health");
	if(victim<=0||victim>=65||attacker<=0||attacker>=65)	return;
	if(!IsClientInGame(victim)||!IsClientInGame(attacker))	return;
	if(victim==attacker)	return;
	if(IsFakeClient(attacker))	return;
	if(!IsPlayerAlive(attacker))	return;
	if(ZR_IsClientHuman(attacker))
	{
		if(!IsPlayerAlive(victim)||ZR_IsClientZombie(victim))
		{
			MissionHumanDmgCount(attacker,victim,damage);
			QuestHumanDmgCount(attacker,victim,damage);
		}
	}

}
void OnPlayerDeathPost(Event event, const char[] name, bool dontBroadcast)
{
	int victim = GetClientOfUserId(event.GetInt("userid"));
	int attacker = GetClientOfUserId(event.GetInt("attacker"));
	if(victim<=0||victim>=65||attacker<=0||attacker>=65)	return;
	if(!IsClientInGame(victim)||!IsClientInGame(attacker))	return;
	if(victim==attacker)	return;
	if(!IsPlayerAlive(attacker))	return;
	if(ZR_IsClientHuman(attacker))
	{
		MissionHumanKillZombie(attacker);
		QuestHumanKillZombie(attacker);
	}
}
void OnGrenadeThrown(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(event.GetInt("userid"));
	if(client<=0||client>=65)	return;
	if(!IsClientInGame(client))	return;
	if(IsFakeClient(client))	return;
	MissionHumanNadeCount(client);
	QuestHumanNadeCount(client);
}