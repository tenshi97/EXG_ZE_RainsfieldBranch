int hd_server_port;
void HdOnPluginStart()
{
	hd_server_port = FindConVar("hostport").IntValue;
	RegAdminCmd("sm_hd",ServerHdConfig,ADMFLAG_GENERIC);
}
Action ServerHdConfig(int client,int args)
{
	if(client<=0||client>=65)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	Menu menu = CreateMenu(ServerHdMenuHandler);
	menu.SetTitle("服务器活动设置");
	menu.AddItem("","发放箱子");
	menu.AddItem("","通关奖励");
	menu.AddItem("","随机掉落");
	menu.AddItem("","订价倍率");
	menu.Display(client, MENU_TIME_FOREVER);
	return Plugin_Handled;
}

int ServerHdMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(param == 0)	
		{
			SendCrate(client);
		}
		else if (param == 1)
		{
//			PassBonusSet(client);
		}
		else if (param == 2)
		{
//			RandomDropSet(client);
		}
		else if (param == 3)
		{
//			NominateCostModify(client);
		}
	}
}

void SendCrate(int client)
{
	Menu menu = CreateMenu(SendCrateHandler);
	menu.SetTitle("发放箱子");
	menu.AddItem("","全体玩家-活动箱子");
	menu.AddItem("","全体活人-活动箱子");
	menu.AddItem("","指定玩家-活动箱子");
	menu.Display(client, MENU_TIME_FOREVER);
}

int SendCrateHandler(Menu menu, MenuAction action, int client, int param)
{
	int item_id = Store_GetItemIdbyUniqueId("uid_lootbox_freecase1");
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(param == 0)	
		{
			PrintToChatAll(" \x05[活动系统]\x01善良的狗管理给全体玩家发放了一个\x9 活动箱子");
			for(int i=1;i<=64;i++)
			{
				if(IsClientInGame(i))
				{
					if(!IsFakeClient(i))
					{
						if(!Store_HasClientItem(i,item_id))
						{
							Store_GiveItem(i,item_id,0,0,0);
						}
					}
				}
			}
		}
		else if (param == 1)
		{
			PrintToChatAll(" \x05[活动系统]\x01善良的狗管理给全体存活人类发放了一个\x9 活动箱子");
			for(int i=1;i<=64;i++)
			{
				if(IsClientInGame(i))
				{
					if(!IsFakeClient(i))
					{
						if(GetClientTeam(i)==3)
						{
							if(!Store_HasClientItem(i,item_id))
							{
								Store_GiveItem(i,item_id,0,0,0);
							}
						}
					}
				}
			}
		}
		else if (param == 2)
		{
			Menu tmenu = CreateMenu(SendCrateToPlayerMenuHandler);
			tmenu.SetTitle("赠送箱子");
			for(int i=1;i<=64;i++)
			{
				if(!IsClientInGame(i))	continue;
				if(IsFakeClient(i))	continue;
				char player_name[64];
				char index[4];
				IntToString(i, index, sizeof(index));
				GetClientName(i, player_name, sizeof(player_name));
				tmenu.AddItem(index,player_name);
			}
			tmenu.Display(client,MENU_TIME_FOREVER);
		}
	}
}

int SendCrateToPlayerMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		char index[4];
		char buffer[256];
		int item_id = Store_GetItemIdbyUniqueId("uid_lootbox_freecase1");
		menu.GetItem(param, index, sizeof(index));
		int target = StringToInt(index);
		char client_name[64];
		char target_name[64];
		if(!Store_HasClientItem(target,item_id))
		{
			GetClientName(client,client_name,sizeof(client_name));
			GetClientName(target,target_name,sizeof(target_name));
			Format(buffer,sizeof(buffer),"\x05[活动系统]\x01管理员\x07%s\x01给\x07%s\x01赠送了一个\x09活动箱子\x01,这其中必有什么交易",client_name,target_name);
			PrintToChatAll(buffer);
			Store_GiveItem(target,item_id,0,0,0);
		}			
	}	
}