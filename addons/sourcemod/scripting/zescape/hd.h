int hd_server_port;
bool passbonus_status;
bool passbonus_type;
int passbonus_extracredits;
bool passbonus_extracrates;
int crate_randomdrop_pos1;
int crate_randomdrop_pos2;
void HdOnPluginStart()
{
	passbonus_status = false;
	hd_server_port = FindConVar("hostport").IntValue;
	RegAdminCmd("sm_hd",ServerHdConfig,ADMFLAG_GENERIC);
	crate_randomdrop_pos1=0;
	crate_randomdrop_pos2=0;
}
void HdOnMapStart()
{
	passbonus_status = false;
}
Action ServerHdConfig(int client,int args)
{
	if(client<=0||client>=65)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	CreateHdConfigMenu(client);
	return Plugin_Handled;
}
void CreateHdConfigMenu(int client)
{
	Menu menu = CreateMenu(ServerHdMenuHandler);
	menu.SetTitle(g_pStore?"服务器活动设置":"服务器活动设置/暂时关闭");
	int iStyle = (g_pStore) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED;
	menu.AddItem("","发放箱子",iStyle);
	menu.AddItem("","通关奖励",iStyle);
	menu.AddItem("","随机掉落",iStyle);
	menu.AddItem("","订价倍率",iStyle);
	menu.Display(client, MENU_TIME_FOREVER);
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
			PassBonusSet(client);
		}
		else if (param == 2)
		{
			RandomDropSet(client);
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
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int SendCrateHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(!g_pStore)
		{
			return 0;
		}
		int item_id = Store_GetItemIdbyUniqueId("uid_lootbox_freecase1");
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
							PrintCenterText(i,"你获得了一个特殊箱子，请及时打开");
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
								PrintCenterText(i,"你获得了一个特殊箱子，请及时打开");
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
	else if (param == MenuCancel_ExitBack) 	CreateHdConfigMenu(client);
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
		PrintToChatAll(buffer);
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

void PassBonusSet(int client)
{
	char buffer[256];
	Menu menu=CreateMenu(PassBonusMenuHandler);
	menu.SetTitle("通关奖励-仅本地图有效");
	Format(buffer,sizeof(buffer),"通关奖励状态:%s",passbonus_status?"开启":"关闭");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"通关奖励类型:%s",passbonus_type?"伐木":"攻略");
	menu.AddItem("",buffer,passbonus_status?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	Format(buffer,sizeof(buffer),"额外积分:%d",passbonus_extracredits);
	menu.AddItem("",buffer,passbonus_status?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	Format(buffer,sizeof(buffer),"通关箱子:%s",passbonus_extracrates?"有":"无");
	menu.AddItem("",buffer,passbonus_status?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int PassBonusMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char buffer[256];
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(param == 0)
		{
			if(passbonus_status)
			{
				passbonus_status=false;
				passbonus_extracredits=0;
				passbonus_extracrates=false;
				PrintToChatAll(" \x05[活动系统]管理员关闭了地图活动");
			}
			else
			{
				passbonus_status=true;
				PrintToChatAll(" \x05[活动系统]\x01管理员开启了地图活动");
				Format(buffer,sizeof(buffer)," \x05[活动系统]\x01类型:%s 额外积分:%d 额外箱子:%s",passbonus_type?"伐木":"攻略",passbonus_extracredits,passbonus_extracrates?"有":"无");
				PrintToChatAll(buffer);
			}
		}
		else if(param == 1)
		{
			if(passbonus_type)	//伐木
			{
				passbonus_type = false; //改为攻略
			}
			else
			{
				passbonus_type = true;
			}
			PassBonusSet(client);
		}
		else if(param == 2)
		{
			SetExtraCredits(client);
		}
		else if(param == 3)
		{
			if(passbonus_extracrates)
			{
				passbonus_extracrates = false;
			}
			else
			{
				passbonus_extracrates = true;
			}
			PassBonusSet(client);
		}
	}
	else if (param == MenuCancel_ExitBack) 	CreateHdConfigMenu(client);
}

void SetExtraCredits(int client)
{
	char buffer[256];
	Format(buffer,sizeof(buffer),"通关奖励积分设置:\n当前:%d积分",passbonus_extracredits);
	Menu menu = CreateMenu(SetExtraCreditsMenuHandler);
	menu.SetTitle(buffer);
	SetMenuPagination(menu, MENU_NO_PAGINATION);
	menu.AddItem("10","+10积分",passbonus_extracredits>=2000?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("50","+50积分",passbonus_extracredits>=2000?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("100","+100积分",passbonus_extracredits>=2000?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("500","+500积分",passbonus_extracredits>=2000?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("-10","-10积分",passbonus_extracredits<=0?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("-50","-50积分",passbonus_extracredits<=0?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("-100","-100积分",passbonus_extracredits<=0?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("-500","-500积分",passbonus_extracredits<=0?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.AddItem("","返回");
	menu.Display(client,MENU_TIME_FOREVER);
}

int SetExtraCreditsMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(param==8)
		{
			PassBonusSet(client);
		}
		else
		{
			char sitem[5];
			menu.GetItem(param,sitem,sizeof(sitem));
			int credits_delta = StringToInt(sitem);
			passbonus_extracredits+=credits_delta;
			passbonus_extracredits = Min(2000,passbonus_extracredits);
			passbonus_extracredits = Max(0,passbonus_extracredits);
			SetExtraCredits(client);
		}
	}
}

void HdOnRoundEnd(int winner)
{
	int client_credits;
	if(!g_pStore)
	{
		return;
	}
	int item_id = Store_GetItemIdbyUniqueId("uid_lootbox_freecase1");
	if(winner==3)
	{
		if(passbonus_status)
		{
			if(GetClientCount(true)<30)
			{
				PrintToChatAll(" \x05[活动系统}人数不足30，无法完成!");
			}
			if(passbonus_type)
			{
				PrintToChatAll(" \x05[活动系统]\x01地图伐木活动完成");
				for(int i=1;i<=64;i++)
				{
					if(IsClientInGame(i))
					{
						if(GetClientTeam(i)==3)
						{
							if(IsPlayerAlive(i))
							{
								client_credits = Store_GetClientCredits(i);
								Store_SetClientCredits(i,client_credits+passbonus_extracredits);
								PrintToChat(i," \x05[活动系统]\x01完成地图活动，获得了\x07%d\x01积分",passbonus_extracredits);
								if(!Store_HasClientItem(i,item_id))
								{
									Store_GiveItem(i,item_id,0,0,0);
									PrintToChat(i," \x05[活动系统]\x01完成地图活动，获得了特殊箱子");
								}
							}
						}
					}
				}
			}
			else
			{
				passbonus_status = false;
				PrintToChatAll(" \x05[活动系统]\x01地图攻略活动完成");
				for(int i=1;i<=64;i++)
				{
					if(IsClientInGame(i))
					{
						if(GetClientTeam(i)==3)
						{
							if(IsPlayerAlive(i))
							{
								client_credits = Store_GetClientCredits(i);
								Store_SetClientCredits(i,client_credits+passbonus_extracredits);
								PrintToChat(i," \x05[活动系统]\x01完成地图活动，获得了\x07%d\x01积分",passbonus_extracredits);
								if(!Store_HasClientItem(i,item_id))
								{
									Store_GiveItem(i,item_id,0,0,0);
									PrintToChat(i," \x05[活动系统]\x01完成地图活动，获得了特殊箱子");
								}
							}
						}
					}
				}
			}
		}
		int crate_randomdrop_pos;
		if(Pmap.difficulty<2)
		{
			crate_randomdrop_pos = crate_randomdrop_pos1;
		}
		else
		{
			crate_randomdrop_pos = crate_randomdrop_pos2;
		}
		if(crate_randomdrop_pos==0)	return;
		int alive_human = 0;
		int alive_human_list[64];
		char select_client_name[64];
		for(int i=1;i<=64;i++)
		{
			if(IsClientInGame(i))
			{
				if(GetClientTeam(i)==3)
				{
					if(IsPlayerAlive(i))
					{
						alive_human_list[alive_human]=i;
						alive_human++;
					}
				}
			}
		}
		if(alive_human==0)	return;
		int r_result = GetURandomInt()%(101);
		PrintToChatAll(" \x05[活动系统]\x01随机掉落检定:%d/%d,%s",r_result,crate_randomdrop_pos,r_result<=crate_randomdrop_pos?"成功":"失败");
		if(r_result<=crate_randomdrop_pos)
		{
			int random_client = GetURandomInt()%alive_human;
			int select_client = alive_human_list[random_client];
			GetClientName(select_client,select_client_name,sizeof(select_client_name));
			PrintToChatAll(" \x05[活动系统]\x09%s\x01获得了随机箱子掉落，恭喜这个B",select_client_name);
			if(!Store_HasClientItem(select_client,item_id))
			{
				Store_GiveItem(select_client,item_id,0,0,0);
			}
		}
	}
}

void RandomDropSet(int client)
{
	char buffer[256];
	Menu menu=CreateMenu(RandomDropMenuHandler);
	menu.SetTitle("随机掉落设置");
	Format(buffer,sizeof(buffer),"掉落概率[困难以下]:%d/100",crate_randomdrop_pos1);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"掉落概率[困难及以上]:%d/100",crate_randomdrop_pos2);
	menu.AddItem("",buffer);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int RandomDropMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(param == 1)
		{
			if(crate_randomdrop_pos1<=10&&crate_randomdrop_pos1>0)
			{
				crate_randomdrop_pos1 = 0;
			}
			else
			{
				if(crate_randomdrop_pos1<=0)
				{
					crate_randomdrop_pos1 = 90;
				}
				else
				{
					crate_randomdrop_pos1 -= 10;
				}
			}
		}
		else if(param == 2)
		{
			if(crate_randomdrop_pos2<=10&&crate_randomdrop_pos2>0)
			{
				crate_randomdrop_pos2 = 0;
			}
			else
			{
				if(crate_randomdrop_pos2<=0)
				{
					crate_randomdrop_pos2 = 90;
				}
				else
				{
					crate_randomdrop_pos2 -= 10;
				}
			}
		}
		RandomDropSet(client);
	}
	else if (param == MenuCancel_ExitBack) 	CreateHdConfigMenu(client);
}

