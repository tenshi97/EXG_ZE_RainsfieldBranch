void SpecOnPluginStart()
{
	RegConsoleCmd("sm_gc",SpecCommand);
}
Action SpecCommand(int client,int args)
{
	if(client<=0)
		return Plugin_Handled;

	if (!IsClientInGame(client))
		return Plugin_Handled;
	char target_name[64];
	if (args != 1)
	{
		Menu menu =CreateMenu(SpecMenuHandler);

		for(int i=1;i<=64;i++)
		{
			if(IsClientInGame(i))
			{
				char index[4];
				IntToString(i, index, sizeof(index));
				GetClientName(i, target_name, sizeof(target_name));
				menu.AddItem(index,target_name);
			}
		}
		menu.Display(client, MENU_TIME_FOREVER);
		return Plugin_Handled;
	}

	if (args == 1)
	{
		GetCmdArgString(target_name,sizeof(target_name));
		ReplaceString(target_name, sizeof(target_name), "\"", "");

		int target = FindTarget(client, target_name,false,true);

		if(target<=0 || target>=65)
		{
			PrintToChat(client, " \x04[观察系统] \x07无法指定玩家!");
			return Plugin_Handled;
		}

		if (IsClientInGame(target))
		{
			if(!IsPlayerAlive(target))
			{
				PrintToChat(client, " \x04[观察系统] \x07目标玩家必须存活!");
				return Plugin_Handled;				
			}

			if(client == target)
			{
				PrintToChat(client," \x04[观察系统] \x07不能观察自己!");
				return Plugin_Handled;
			}
			ForcePlayerSuicide(client);
			if (GetClientTeam(client) != 1)
			{
				ChangeClientTeam(client, 1);
			}
			DataPack dp = new DataPack();
			dp.WriteCell(client);
			dp.WriteCell(target);
			RequestFrame(RFC_SetObsTarget, dp);
		}
	}
	return Plugin_Handled;
}

int SpecMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select) {
		char index[4];
		menu.GetItem(param, index, sizeof(index));
		int target = StringToInt(index);
		if(!IsPlayerAlive(target))
		{
			PrintToChat(client, " \x04[观察系统] \x07目标玩家必须存活!");
			return;				
		}
		if(client == target)
		{
			PrintToChat(client," \x04[观察系统] \x07不能观察自己!");
			return;
		}
		ForcePlayerSuicide(client);
		if (GetClientTeam(client) != 1)
		{
			ChangeClientTeam(client, 1);
			ForcePlayerSuicide(client);
		}
		DataPack dp =new DataPack();
		dp.WriteCell(client);
		dp.WriteCell(target);
		RequestFrame(RFC_SetObsTarget,dp);
	}
}

public void RFC_SetObsTarget(DataPack dp)
{
	dp.Reset();
	int client = dp.ReadCell();
	int target = dp.ReadCell();
	ForcePlayerSuicide(client);
	SetEntPropEnt(client, Prop_Send, "m_hObserverTarget", target); 
	delete dp;
}

