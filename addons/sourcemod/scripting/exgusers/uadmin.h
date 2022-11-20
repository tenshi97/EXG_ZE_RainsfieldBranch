void uAdminOnPluginStart()
{
    RegConsoleCmd("sm_nban",NomBanCommand);
    RegConsoleCmd("sm_nunban",NomUnbanCommand);
}
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
	if(args<=1||args>3)
	{
		PrintToChat(client," \x05[NomBan]用法:\x01!nban 玩家 时长(分钟) (原因可选)");
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
		        PrintToChat(client," \x05[NomBan]用法:\x01!nban 玩家 时长(分钟) (原因可选)");
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
                AddNomBanLog(target_list[i],duration,arg3,client);
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
		PrintToChat(client," \x05[NomBan]用法:\x01!nunban 玩家");
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