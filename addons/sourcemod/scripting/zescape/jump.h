void JumpOnPluginStart()
{
	RegAdminCmd("sm_jump",JumpCommand,ADMFLAG_GENERIC);
}

Action JumpCommand(int client,int args)
{
	if(!IsClientInGame(client))	return Plugin_Handled;
	char arg1[256];
	char arg2[256];
	int target_count;
	int target_list[65];
	char target_name[256];
	bool tn_is_ml;
	int jump_self;
	jump_self = 0;
	if(args<=1)
	{
		PrintToChat(client," \x05[EXG-服务器跳转]用法:\x01!jump 玩家 1F/2F/3F/4F……");
		return Plugin_Handled;
	}
	else
	{
		GetCmdArg(1,arg1,sizeof(arg1));
		GetCmdArg(2,arg2,sizeof(arg2));
		target_count = ProcessTargetString(arg1,client,target_list,64,COMMAND_FILTER_CONNECTED,target_name,sizeof(target_name),tn_is_ml);
		if(target_count>0)
		{
			for(int i=0;i<target_count;i++)
			{
				if(target_list[i]==client)
				{
					jump_self = client;
				}
				else
				{
					JumpServer(target_list[i],arg2);
				}
				if(jump_self!=0)
				{
					JumpServer(jump_self,arg2);	
				}

			}
		}
	}
	return Plugin_Handled;
}

void JumpServer(int client,char[] dest)
{
	char buffer[256];
	if(!IsClientInGame(client))	return;
	if(IsFakeClient(client))	return;
	GetClientName(client,buffer,sizeof(buffer));
	if(strcmp(dest,"1F")==0||strcmp(dest,"1f")==0)
	{
		RedirectClient(client,"43.248.185.108:28015");
	}
	else if(strcmp(dest,"2F")==0||strcmp(dest,"2f")==0)
	{
		RedirectClient(client,"43.248.185.108:28025");
	}
	else if(strcmp(dest,"3F")==0||strcmp(dest,"3f")==0)
	{
		RedirectClient(client,"43.248.185.108:28035");
	}
	else if(strcmp(dest,"4F")==0||strcmp(dest,"4f")==0)
	{
		RedirectClient(client,"43.248.185.108:28045");
	}
}