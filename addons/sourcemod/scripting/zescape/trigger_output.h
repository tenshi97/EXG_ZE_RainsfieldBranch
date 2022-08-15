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

void TriggerOnEntityCreated(int entity, const char[] classname)
{
	if (IsValidEntity(entity))
	{
		if (strcmp(classname, "trigger_once", false) == 0 || strcmp(classname, "trigger_multiple", false) != 0)
		{
			SDKHook(entity, SDKHook_SpawnPost, TriggerOnTriggerSpawnPost);
		}
		else if (strcmp(classname, "func_button", false) == 0)
		{
			SDKHook(entity, SDKHook_SpawnPost, TriggerOnButtonSpawnPost);
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
		ReplaceString(activator_auth, sizeof(activator_auth), "STEAM_1:", "", false);

		char button_name[64];
		GetEntPropString(entity, Prop_Data, "m_iName", button_name, sizeof(button_name));

		if (button_name[0] == NULL_STRING[0])
		{
			GetEntPropString(entity, Prop_Data, "m_iGlobalname", button_name, sizeof(button_name));
		}
		
		int hammerid = GetEntProp(entity, Prop_Data, "m_iHammerID");

		PrintToChatAll(" \x04[触发检测] \x09%s(%s) \x01触发了 \x06机关%s(%d)", activator_name, activator_auth, button_name, hammerid);
		PrintToConsoleAll("[触发检测] %s(%s) 触发了 机关(Trigger)%s(%d)", activator_name, activator_auth, button_name, hammerid);
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