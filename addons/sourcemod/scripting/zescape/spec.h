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
	
	if (args != 1)
	{
		PrintToChat(client, " \x04[观察系统] \x07用法: !gc \"用户名\"");
		return Plugin_Handled;
	}

	if (args == 1)
	{
		char target_name[64];
		GetCmdArgString(target_name,sizeof(target_name));
		ReplaceString(target_name, sizeof(target_name), "\"", "");

		int target = FindTarget(client, target_name);

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

			if (GetClientTeam(client) != 1)
			{
				ChangeClientTeam(client, 1);
				ForcePlayerSuicide(client);
			}
				DataPack dp = new DataPack();
				dp.WriteCell(target);
				dp.WriteCell(target);
				RequestFrame(RFC_SetObsTarget, dp);
			
		}
	}
	return Plugin_Handled;
}

public void RFC_SetObsTarget(DataPack dp)
{
	dp.Reset();
	int client = dp.ReadCell();
	int target = dp.ReadCell();

	SetEntPropEnt(client, Prop_Send, "m_hObserverTarget", target); 
	delete dp;
}