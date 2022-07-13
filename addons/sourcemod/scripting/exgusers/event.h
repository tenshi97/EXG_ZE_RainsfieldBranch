void EventOnPluginStart() {
	//HookEvent("player_team", OnPlayerTeam, EventHookMode_Pre);
	//HookEvent("player_spawn", OnPlayerSpawn, EventHookMode_Post);
	//HookEvent("player_death", OnPlayerDeathPre, EventHookMode_Pre);
	//HookEvent("player_death", OnPlayerDeathPost, EventHookMode_Post);
	//HookEvent("player_hurt", OnPlayerHurt, EventHookMode_Post);
	//HookEvent("weapon_fire", OnWeaponFire, EventHookMode_Post);
	//HookEvent("grenade_thrown", OnGrenadeThrown, EventHookMode_Post);
	//HookEvent("player_spawned", OnPlayerSpawned, EventHookMode_Post);
	// HookEvent("item_equip", OnItemEquip, EventHookMode_Post);
	// HookEvent("hegrenade_detonate", OnHeGrenadeDetonate, EventHookMode_Post);
	// HookEvent("inferno_startburn", OnInfernoStartBurn, EventHookMode_Pre);
	//HookEvent("decoy_started", OnDecoyStarted, EventHookMode_Post);
	//HookEvent("bullet_impact", OnBulletImpact, EventHookMode_Post);
	//HookEvent("player_jump", OnPlayerJump, EventHookMode_Post);
	//HookEvent("player_falldamage", OnPlayerFall, EventHookMode_Post);
	// HookEvent("cs_win_panel_round", OnWinPanelRound, EventHookMode_Pre);
	//HookEvent("round_prestart", OnRoundPreStart, EventHookMode_Post);
	HookEvent("round_start", OnRoundStart, EventHookMode_Post);
	//HookEvent("round_poststart", OnRoundPostStart, EventHookMode_Post);
	//HookEvent("round_freeze_end", OnRoundFreezeEnd, EventHookMode_Post);
	HookEvent("round_end", OnRoundEnd, EventHookMode_Post);
}
void OnRoundStart(Event event, const char[] name, bool dontBroadcast) 
{
	UserInfoOnRoundStart();
}
void OnRoundEnd(Event event, const char[] name, bool dontBroadcast) 
{
}