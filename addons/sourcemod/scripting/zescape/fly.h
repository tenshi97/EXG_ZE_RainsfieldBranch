void FlyOnPluginStart() {
	RegConsoleCmd("sm_ufly", FlyCommand);
	RegConsoleCmd("sm_go", FlyCommand);
	RegConsoleCmd("sm_fly", FlyCommand);
	RegConsoleCmd("sm_efly", FlyCommand);
//	AddCommandListener(AimFly, "chatwheel_ping");
//	AddCommandListener(AimFly, "player_ping");
}

Action FlyCommand(int client, int args) {
	if(!Pmap.ego)
	{
		PrintCenterText(client,"当前地图禁用开局传送");
		return Plugin_Handled;
	}
	if (!IsClientInGame(client)) return Plugin_Handled;
	if (!args) {
		FlyMenu(client);
		return Plugin_Handled;
	}
	char arg[32];
	GetCmdArg(1, arg, sizeof(arg));
	FlyTarget(client, FindTarget(client, arg, true, false));
	return Plugin_Handled;
}

public Action AimFly(int client, const char[] command, int argc)
{
	if(!Pmap.ego)
	{
		PrintCenterText(client,"当前地图禁用开局传送");
		return Plugin_Handled;
	}
	if(!IsClientInGame(client))	return Plugin_Handled;
	if (Round.Infected)	return Plugin_Handled;
	int target;
	target = GetClientAimTarget(client,true);
	if(target>0||target<=64)
	{
		if(!IsClientInGame(target))	return Plugin_Handled;
		if (!IsClientInGame(target)) return Plugin_Handled;
		if(IsValidHumanorBot(target))
		{
			FlyTarget(client, target);			
		}
	}
	return Plugin_Handled;
}

void FlyMenu(int client) {
	if(!Pmap.ego)
	{
		PrintCenterText(client,"当前地图禁用开局传送");
		return;
	}
	Menu menu = CreateMenu(FlyMenuHandler);
	menu.SetTitle("开局传送-选择传送目标");
	char index[4], name[64];
	for (int i = 1; i <= MaxClients; i++) {
		if (i == client) continue;
		if (!IsValidHumanorBot(i)) continue;
		IntToString(i, index, sizeof(index));
		GetClientName(i, name, sizeof(name));
		menu.AddItem(index, name);
	}
	menu.Display(client, MENU_TIME_FOREVER);
}

int FlyMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select) {
		char index[4];
		menu.GetItem(param, index, sizeof(index));
		FlyTarget(client, StringToInt(index));
	}
}

void FlyTarget(int client, int target) {
	if (Round.Infected) {
		PrintCenterText(client, "[开局传送失败：尸变已发生]:");
		return;
	}
	if (target == -1) {
		PrintCenterText(client, "[开局传送失败: 目标玩家不存在]");
		return;
	}
	if (client == target) {
		PrintCenterText(client, "[开局传送失败: 不能传送自己]");
		return;
	}
	if (!IsValidHuman(client)) {
		PrintCenterText(client, "[开局传送失败: 活人才能使用]");
		return;
	}
	if (!IsValidHumanorBot(target)) {
		PrintCenterText(client, "[开局传送失败: 不可传送僵尸]");
		return;
	}
	float ori[3], ang[3];
	char name[64];
	GetClientAbsOrigin(target, ori);
	GetClientEyeAngles(target, ang);
	GetClientName(target,name,sizeof(name));
	TeleportEntity(client, ori, ang, NULL_VECTOR);
	PrintCenterText(client,"[飞雷神启动：飞向%s]",name);
}
