// bool Button_Spam_Client[65];
//bool Trigger_Spam_Client[65];
/*
void TriggerOutputOnPluginStart()
{
	for(int i=1;i<=64;i++)
	{
		Button_Spam_Client[i]=false;
//		Trigger_Spam_Client[i]=false;
	}
	// HookEntityOutput("func_button", "OnPressed", ButtonOnPressed);
	// HookEntityOutput("func_button", "OnDamaged", ButtonOnPressed);
//	HookEntityOutput("trigger_once", "OnStartTouch", TriggerOnTouch);
//	HookEntityOutput("trigger_multiple", "OnStartTouch", TriggerOnTouch);
//	HookEntityOutput("trigger_once", "OnEndTouch", TriggerOnTouch);
//	HookEntityOutput("trigger_multiple", "OnEndTouch", TriggerOnTouch);
}
void TriggerOutputOnMapStart()
{
	for(int i=1;i<=64;i++)
	{
		Button_Spam_Client[i]=false;
//		Trigger_Spam_Client[i]=false;
	}	
}
*/
int g_antispam_hammerid=0;
bool Breakable_Recorded = false;
char Breakable_Recorded_Maps_List[][] = 
{
	"ze_lotr_minas_tirith_p5",
	"ze_tilex_ultimate_v2_13e",
	"ze_elevator_escape_p3",
	"ze_death_wish_gf1_1",
	"ze_obj_rescape_v1",
	"ze_obj_blackout_b4"
}
void TriggerOnRoundStart()
{
	if(Breakable_Recorded)
	{
		PrintToChatAll(" \x05[触发检测]\x07本地图启用了破坏实体检测，请注意");
	}
}
public Action ButtonOnPressed(const char[] output, int caller, int activator, float delay)
{
	if (GetEntPropEnt(caller, Prop_Data, "m_hMoveParent") != -1 ||
		GetEntPropEnt(caller, Prop_Data, "m_hMoveChild") != -1 ||
		GetEntPropEnt(caller, Prop_Data, "m_hMovePeer") != -1)
		return Plugin_Continue;

	if(activator<1 || activator>64)
		return Plugin_Continue;
	
	if(!IsClientInGame(activator))
		return Plugin_Continue;
	
	// if(!Button_Spam_Client[activator])
	// {
	// 	Button_Spam_Client[activator] = true;
	// }
	// else
	// {
	// 	return;
	// }
	// CreateTimer(5.0, ButtonOnPressedCallback, activator, TIMER_FLAG_NO_MAPCHANGE);

	char activator_name[64];
	GetClientName(activator, activator_name, sizeof(activator_name));

	char activator_auth[64];
	GetClientAuthId(activator, AuthId_Steam2, activator_auth, sizeof(activator_auth), true);
	ReplaceString(activator_auth, sizeof(activator_auth), "STEAM_1:", "", false);

	char button_name[64];
	GetEntPropString(caller, Prop_Data, "m_iName", button_name, sizeof(button_name));
	
	int hammerid = GetEntProp(caller, Prop_Data, "m_iHammerID");

	PrintToChatAll(" \x04[触发检测] \x09%s(%s) \x01触发了 \x06按钮%s(%d)", activator_name, activator_auth, button_name, hammerid);
	PrintToConsoleAll("[触发检测] %s(%s) 触发了 按钮(Button)%s(%d)", activator_name, activator_auth, button_name, hammerid);

	return Plugin_Continue;
}

// Action ButtonOnPressedCallback(Handle timer, int activator) {
// 	if (!Button_Spam_Client[activator]) return Plugin_Handled;
// 	Button_Spam_Client[activator] = false;
// 	return Plugin_Handled;
// }
void TriggerOnMapStart()
{
	Breakable_Recorded = false;
	char map_name[64];
	GetCurrentMap(map_name,sizeof(map_name));
	for(int i =0;i<=1;i++)
	{
		if(strcmp(map_name,Breakable_Recorded_Maps_List[i],false)==0)
		{
			Breakable_Recorded = true;
			break;
		}
	}
}
void TriggerOnEntityCreated(int entity, const char[] classname)
{
	if (IsValidEntity(entity))
	{
		if (strcmp(classname, "trigger_once", false) == 0)
		{
			SDKHook(entity, SDKHook_SpawnPost, TriggerOnTriggerSpawnPost);
		}
		else if (strcmp(classname, "func_button", false) == 0)
		{
			SDKHook(entity, SDKHook_SpawnPost, TriggerOnButtonSpawnPost);
		}
		if(Breakable_Recorded)
		{
			if(strcmp(classname,"func_breakable",false)==0)
			{
				SDKHook(entity, SDKHook_SpawnPost, TriggerOnBreakableSpawnPost);				
			}
			if(strcmp(classname,"func_physbox",false)==0)
			{
				SDKHook(entity, SDKHook_SpawnPost, TriggerOnBreakableSpawnPost);				
			}
			if(strcmp(classname,"func_physbox_multiplayer",false)==0)
			{
				SDKHook(entity, SDKHook_SpawnPost, TriggerOnBreakableSpawnPost);				
			}
		}
	}
}

public void TriggerOnButtonSpawnPost(int entity)
{
	if (!IsValidEntity(entity))
		return;

	SDKUnhook(entity, SDKHook_SpawnPost, TriggerOnButtonSpawnPost);

	HookSingleEntityOutput(entity, "OnPressed", ButtonOnPressed, true);
	HookSingleEntityOutput(entity, "OnDamaged", ButtonOnPressed, true);
}
public void TriggerOnBreakableSpawnPost(int entity)
{
	if(!IsValidEntity(entity))
	{
		return;
	}
	SDKUnhook(entity,SDKHook_SpawnPost,TriggerOnBreakableSpawnPost);
	HookSingleEntityOutput(entity,"OnBreak",TriggerOnBreak,true);
}
public Action TriggerOnBreak(const char[] output, int caller, int activator, float delay)
{
	int breaker=activator;
	int entity=caller;
	//SDKUnhook(entity,SDKHook_SpawnPost,TriggerOnBreak);	
	if(breaker<=0||breaker>64)
	{
		return Plugin_Continue;
	}
	char activator_name[64];
	char activator_auth[64];
	GetClientName(breaker,activator_name,sizeof(activator_name));
	GetClientAuthId(breaker, AuthId_Steam2, activator_auth, sizeof(activator_auth), true);
	char break_name[64];
	GetEntPropString(entity,Prop_Data,"m_iName",break_name,sizeof(break_name));
	if (break_name[0] == NULL_STRING[0])	//exactly means button_name == NULL_STRING(aka \0)
	{
		GetEntPropString(entity, Prop_Data, "m_iGlobalname", break_name, sizeof(break_name));
	}
	int hammerid = GetEntProp(entity, Prop_Data, "m_iHammerID");
	if(hammerid != g_antispam_hammerid)
	{
		g_antispam_hammerid = hammerid;
		PrintToChatAll(" \x04[触发检测] \x09%s(%s) \x01打碎了 \x06可破坏实体%s(%d)", activator_name, activator_auth, break_name, hammerid);
		PrintToConsoleAll("[触发检测] %s(%s) 打碎了 可破坏实体%s(%d)", activator_name, activator_auth, break_name, hammerid);
	}	
	return Plugin_Continue;
}

public void TriggerOnTriggerSpawnPost(int entity)
{
	if (!IsValidEntity(entity))
		return;

	SDKUnhook(entity, SDKHook_SpawnPost, TriggerOnTriggerSpawnPost);

	SDKHook(entity, SDKHook_StartTouch, TriggerOnStartTouch);
}

public Action TriggerOnStartTouch(int entity, int toucher)
{
	SDKUnhook(entity, SDKHook_StartTouch, TriggerOnStartTouch);
	if(toucher<=0||toucher>64)	return Plugin_Continue;
	if (IsClientInGame(toucher))
	{
		char activator_name[64];
		GetClientName(toucher, activator_name, sizeof(activator_name));

		char activator_auth[64];
		GetClientAuthId(toucher, AuthId_Steam2, activator_auth, sizeof(activator_auth), true);
		//ReplaceString(activator_auth, sizeof(activator_auth), "STEAM_1:", "", false);

		char button_name[64];
		GetEntPropString(entity, Prop_Data, "m_iName", button_name, sizeof(button_name));

		if (button_name[0] == NULL_STRING[0])
		{
			GetEntPropString(entity, Prop_Data, "m_iGlobalname", button_name, sizeof(button_name));
		}
		
		int hammerid = GetEntProp(entity, Prop_Data, "m_iHammerID");
		if(hammerid != g_antispam_hammerid)
		{
			g_antispam_hammerid = hammerid;
			PrintToChatAll(" \x04[触发检测] \x09%s(%s) \x01触发了 \x06机关%s(%d)", activator_name, activator_auth, button_name, hammerid);
			PrintToConsoleAll("[触发检测] %s(%s) 触发了 机关(Trigger)%s(%d)", activator_name, activator_auth, button_name, hammerid);
		}
	}

	return Plugin_Continue;
}

/*void TriggerOnTouch(const char[] output, int caller, int activator, float delay)
{
	if (GetEntPropEnt(caller, Prop_Data, "m_hMoveParent") != -1 ||
		GetEntPropEnt(caller, Prop_Data, "m_hMoveChild") != -1 ||
		GetEntPropEnt(caller, Prop_Data, "m_hMovePeer") != -1) return;
	if(activator<1||activator>64)	return;	
	if(!IsClientInGame(activator))	return;
	if(!Trigger_Spam_Client[activator])
	{
		Trigger_Spam_Client[activator] = true;
	}
	else
	{
		return;
	}
	CreateTimer(5.0, TriggerOnTouchCallback, activator, TIMER_FLAG_NO_MAPCHANGE);
	char activator_name[64];
	char trigger_name[64];
	GetClientName(activator,activator_name,sizeof(activator_name));
	GetEntPropString(caller, Prop_Data, "m_iName", trigger_name, sizeof(trigger_name));
	PrintToConsoleAll("[触发检测]%s触发了%s的%s", activator_name,trigger_name,output);
}

Action TriggerOnTouchCallback(Handle timer, int activator) {
	if (!Trigger_Spam_Client[activator]) return Plugin_Handled;
	Trigger_Spam_Client[activator] = false;
	return Plugin_Handled;
}*/