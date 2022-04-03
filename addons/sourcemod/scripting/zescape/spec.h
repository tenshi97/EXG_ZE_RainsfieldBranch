void SpecOnPluginStart()
{
	RegConsoleCmd("sm_gc",SpecCommand);
}
Action SpecCommand(int client,int args)
{
	if(client<=0)	return Plugin_Handled;
	if (!IsClientInGame(client)) return Plugin_Handled;
	if (args != 1)
	{
		PrintToChat(client, " \x04[观察系统] 用法:\x07 !spec 用户名");
		return Plugin_Handled;
	}
	if (args == 1)
	{
		char target_name[64];
		GetCmdArgString(target_name,sizeof(target_name));
		int target = FindTarget(client,target_name);
		if(target<=0||target>=65)	return Plugin_Handled;
		if(IsClientInGame(target))
		{
			if(!IsPlayerAlive(target))
			{
				PrintToChat(client, " \x04[观察系统] \x07玩家必须存活");
				return Plugin_Handled;				
			}
			if(client==target)
			{
				PrintToChat(client," \x04[观察系统] \x07不能观察自己");
				return Plugin_Handled;
			}
			if(GetClientTeam(client)!=1)
			{
				if (GetClientTeam(client) != 1 && GetClientTeam(client) != GetClientTeam(target)) SetClientTeam(client, 1);
				if (GetClientTeam(client) != 1 && GetClientTeam(client) == GetClientTeam(target) && IsPlayerAlive(client)) SetClientTeam(client, 1);
			}
			FakeClientCommand(client,"spec_player %d", target);
		}
		if (!IsClientInGame(target)) PrintToChat(client, " \x04[观察系统] \x07找不到目标玩家");
	}
	return Plugin_Handled;
}