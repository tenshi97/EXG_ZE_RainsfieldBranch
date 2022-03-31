bool Button_Spam_Client[65];
//bool Trigger_Spam_Client[65];
void TriggerOutputOnPluginStart()
{
	for(int i=1;i<=64;i++)
	{
		Button_Spam_Client[i]=false;
//		Trigger_Spam_Client[i]=false;
	}
	HookEntityOutput("func_button", "OnPressed", ButtonOnPressed);
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
void ButtonOnPressed(const char[] output, int caller, int activator, float delay)
{
	if (GetEntPropEnt(caller, Prop_Data, "m_hMoveParent") != -1 ||
		GetEntPropEnt(caller, Prop_Data, "m_hMoveChild") != -1 ||
		GetEntPropEnt(caller, Prop_Data, "m_hMovePeer") != -1) return;	
	if(activator<1||activator>64)	return;
	if(!IsClientInGame(activator))	return;
	if(!Button_Spam_Client[activator])
	{
		Button_Spam_Client[activator] = true;
	}
	else
	{
		return;
	}
	CreateTimer(5.0, ButtonOnPressedCallback, activator, TIMER_FLAG_NO_MAPCHANGE);
	char activator_name[64];
	char button_name[64];
	GetClientName(activator,activator_name,sizeof(activator_name));
	GetEntPropString(caller, Prop_Data, "m_iName", button_name, sizeof(button_name));
	PrintToChatAll(" \x04[触发检测] \x09%s \x01触发了 \x06按钮%s", activator_name,button_name);
	PrintToConsoleAll("[触发检测]%s触发了按钮%s", activator_name,button_name);
}

Action ButtonOnPressedCallback(Handle timer, int activator) {
	if (!Button_Spam_Client[activator]) return Plugin_Handled;
	Button_Spam_Client[activator] = false;
	return Plugin_Handled;
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