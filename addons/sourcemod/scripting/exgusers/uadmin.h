void uAdminOnPluginStart()
{
	RegAdminCmd("sm_nban",		NomBanCommand,		ADMFLAG_GENERIC);
	RegAdminCmd("sm_nunban",	NomUnbanCommand,	ADMFLAG_GENERIC);
	RegAdminCmd("sm_pban",		PftBanCommand,		ADMFLAG_GENERIC);
	RegAdminCmd("sm_punban",	PftUnbanCommand,	ADMFLAG_GENERIC);

	//RegAdminCmd("sm_uadmin",UAdminCommand,ADMFLAG_GENERIC);
	//RegAdminCmd("sm_aimadmin",AimAdminCommand,ADMFLAG_GENERIC);
}

/*Action UAdminCommand(int client,int args)
{
	if(args!=1)
	{
		PrintToChat(client, " \x05[用户管理系统}用法错误");
		return Plugin_Handled;
	}
	char arg1[256];
	GetCmdArg(1,arg1,sizeof(arg1));
	//STEAMID
	if(StrContains("STEAM_",arg1,false))
	{
		Datapack dp = new DataPack();
		DbTQuery(,arg1);
	}
}*/

Action NomBanCommand(int client, int args)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	int target_count;
	int target_list[65];
	char target_name[256];
	bool tn_is_ml;
	int uid;
	if(args <= 1 || args > 3)
	{
		PrintToChat(client, " \x05[订图封禁] 用法:\x01!nban 玩家 时长(分钟) (原因可选)");
		return Plugin_Handled;
	}
	else
	{
		GetCmdArg(1, arg1, sizeof(arg1));
		GetCmdArg(2, arg2, sizeof(arg2));
		int duration = StringToInt(arg2);
		if(duration == 0)
		{
			if(strcmp(arg2, "0", false) != 0)
			{
				PrintToChat(client, " \x05[订图封禁] 用法:\x01!nban 玩家 时长(分钟) (原因可选)");
				return Plugin_Handled;
			}
		}
		if(args==3)
		{
			GetCmdArg(3,arg3,sizeof(arg3));
		}
		target_count = ProcessTargetString(arg1, client, target_list, 64, COMMAND_FILTER_CONNECTED, target_name, sizeof(target_name), tn_is_ml);
		if(target_count>0)
		{
			for(int i=0;i<target_count;i++)
			{
				AddNomBanLog(target_list[i], duration, arg3, client);
			}
		}
	}
	return Plugin_Handled;
}

Action NomUnbanCommand(int client, int args)
{
	char arg1[256];
	int target_count;
	int target_list[65];
	char target_name[256];
	bool tn_is_ml;
	int uid;
	if(args<1||args>=2)
	{
		PrintToChat(client, " \x05[订图封禁] 用法:\x01!nunban 玩家");
		return Plugin_Handled;
	}
	else
	{
		GetCmdArg(1,arg1,sizeof(arg1));
		target_count = ProcessTargetString(arg1,client,target_list,64,COMMAND_FILTER_CONNECTED,target_name,sizeof(target_name),tn_is_ml);
		if(target_count>0)
		{
			for(int i=0;i<target_count;i++)
			{
				RemoveNomBanLog(target_list[i],client);
			}
		}
	}
	return Plugin_Handled;
}

Action PftBanCommand(int client, int args)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	int target_count;
	int target_list[65];
	char target_name[256];
	bool tn_is_ml;
	int uid;
	if(args<=1||args>3)
	{
		PrintToChat(client, " \x05[用户系统] 用法:\x01!pban 玩家 时长(分钟) (原因可选)");
		return Plugin_Handled;
	}
	else
	{
		GetCmdArg(1,arg1,sizeof(arg1));
		GetCmdArg(2,arg2,sizeof(arg2));
		int duration = StringToInt(arg2);
		if(duration == 0 )
		{
			if(strcmp(arg2,"0",false)!=0)
			{
				PrintToChat(client, " \x05[用户系统] 用法:\x01!pban 玩家 时长(分钟) (原因可选)");
				return Plugin_Handled;
			}
		}
		if(args==3)
		{
			GetCmdArg(3,arg3,sizeof(arg3));
		}
		target_count = ProcessTargetString(arg1,client,target_list,64,COMMAND_FILTER_CONNECTED,target_name,sizeof(target_name),tn_is_ml);
		if(target_count>0)
		{
			for(int i=0;i<target_count;i++)
			{
				AddPftBanLog(target_list[i],duration,arg3,client);
			}
		}
	}
	return Plugin_Handled;
}

Action PftUnbanCommand(int client, int args)
{
	char arg1[256];
	int target_count;
	int target_list[65];
	char target_name[256];
	bool tn_is_ml;
	int uid;
	if(args<1||args>=2)
	{
		PrintToChat(client, " \x05[用户系统] 用法:\x01!punban 玩家");
		return Plugin_Handled;
	}
	else
	{
		GetCmdArg(1,arg1,sizeof(arg1));
		target_count = ProcessTargetString(arg1,client,target_list,64,COMMAND_FILTER_CONNECTED,target_name,sizeof(target_name),tn_is_ml);
		if(target_count>0)
		{
			for(int i=0;i<target_count;i++)
			{
				RemovePftBanLog(target_list[i],client);
			}
		}
	}
	return Plugin_Handled;
}