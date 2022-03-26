bool TeamVoice_Client_ChatToggle[65];
bool TeamVoice_Client_ReceiveToggle[65];
void VoiceChatOnPluginStart()
{
	for(int i=1;i<=64;i++)
	{
		TeamVoice_Client_ChatToggle[i]=false;
		TeamVoice_Client_ReceiveToggle[i]=false;
	}
	RegConsoleCmd("sm_vcs",VoiceChatSettingCommand);
}
void VoiceChatOnMapStart()
{
	for(int i=1;i<=64;i++)
	{
		TeamVoice_Client_ChatToggle[i]=false;
		TeamVoice_Client_ReceiveToggle[i]=false;
	}	
}
void VoiceChatOnClientConnected(int client)
{
	TeamVoice_Client_ChatToggle[client]=false;
	TeamVoice_Client_ReceiveToggle[client]=false;
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			SetListenOverride(client,i,Listen_Yes);
			SetListenOverride(i,client,Listen_Yes);
		}
	}

}
/*public void OnClientSpeakingEx(int client)
{

}*/
Action VoiceChatSettingCommand(int client,int args)
{
	char buffer[256];
	if(!IsClientInGame(client))	return Plugin_Handled;
	Menu menu = CreateMenu(VoiceChatSettingMenuHandler);
	menu.SetTitle("语音设置");
	Format(buffer,sizeof(buffer),"团队语音:%s\n[开启后仅同一阵营可听到你的语音]",TeamVoice_Client_ChatToggle[client]?"开":"关");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"团队频道:%s\n[开启后你仅能听到同一阵营的语音]",TeamVoice_Client_ReceiveToggle[client]?"开":"关");
	menu.AddItem("",buffer);
	menu.Display(client,MENU_TIME_FOREVER);
	return Plugin_Handled;
}

int VoiceChatSettingMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(param == 0)	
		{
			if(TeamVoice_Client_ChatToggle[client])
			{
				TeamVoice_Client_ChatToggle[client]=false;
				for(int i=1;i<=64;i++)
				{
					if(IsClientInGame(i))
					{
						SetListenOverride(client,i,Listen_Yes);
					}
				}
			}
			else
			{
				TeamVoice_Client_ChatToggle[client]=true;
				for(int i=1;i<=64;i++)
				{
					if(IsClientInGame(i))
					{
						if(IsInSameTeam(client,i))
						{
							SetListenOverride(i,client,Listen_Yes);
						}
						else
						{
							SetListenOverride(i,client,Listen_No);
						}
					}
				}				
			}
		}
		else if (param == 1)
		{
			if(TeamVoice_Client_ReceiveToggle[client])
			{
				TeamVoice_Client_ReceiveToggle[client]=false;
				for(int i=1;i<=64;i++)
				{
					if(IsClientInGame(i))
					{
						SetListenOverride(client,i,Listen_Yes);
					}
				}
			}
			else
			{
				TeamVoice_Client_ReceiveToggle[client]=true;
				for(int i=1;i<=64;i++)
				{
					if(IsClientInGame(i))
					{
						if(IsInSameTeam(client,i))
						{
							SetListenOverride(client,i,Listen_Yes);
						}
						else
						{
							SetListenOverride(client,i,Listen_No);
						}
					}
				}				
			}			
		}
		VoiceChatSettingCommand(client,0);
	}
}