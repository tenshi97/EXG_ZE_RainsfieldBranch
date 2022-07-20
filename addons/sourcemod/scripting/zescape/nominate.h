enum struct Nomlist_Log
{
	int id;
	char name[64];
	int nominator_steamid;
	char nominator_name[64];
	char nominator_steamauth[PLATFORM_MAX_PATH];
	char nominator_steampage[PLATFORM_MAX_PATH];
	int nom_cost;
}

const int Nominate_Max_Num = 5;
bool Nominate_ALLOW;
ArrayList Nom_Map_List;
Map_Info g_Nextmap;
bool g_MapVote_Initiated;
bool g_MapVote_Proceeding;
char g_sPlayerSearchFilter[MAXPLAYERS + 1][PLATFORM_MAX_PATH];

void NominateOnPluginStart()
{
	RegConsoleCmd("sm_nominate",NominateCommand);
	RegConsoleCmd("sm_nom",NominateCommand);
	RegConsoleCmd("sm_yd",NominateCommand);
	RegConsoleCmd("sm_yds",NomListCommand);
	RegConsoleCmd("sm_nomlist",NomListCommand);
	Nominate_ALLOW = true;
	Nom_Map_List = CreateArray(sizeof(Nomlist_Log));
}
void NominateOnMapStart()
{
	Nom_Map_List.Clear();
	Nominate_ALLOW = true;
}
void NominateOnMapEnd()
{
	Nom_Map_List.Clear();
	CloseHandleSafe(Nom_Map_List);
	Nom_Map_List = CreateArray(sizeof(Nomlist_Log));
}
Action NominateCommand(int client,int args)
{
	if (!IsClientInGame(client)) return Plugin_Handled;
	if (IsFakeClient(client))	return Plugin_Handled;
	if (!args) {
		g_sPlayerSearchFilter[client] = "";
		NominateMapMenu(client);
		return Plugin_Handled;
	}
	if(!isDbConnected())
	{
		PrintToChat(client,"[预定地图:地图数据库尚未载入]");
	}

	GetCmdArg(1,g_sPlayerSearchFilter[client],sizeof(g_sPlayerSearchFilter[]));
	NominateMapMenu(client,g_sPlayerSearchFilter[client]);
	return Plugin_Handled;	
}

Action NomListCommand(int client,int args)
{
	if (!IsClientInGame(client)) return Plugin_Handled;
	if (IsFakeClient(client))	return Plugin_Handled;
	if(g_MapVote_Initiated|g_MapVote_Proceeding)
	{
		PrintToChat(client," \x05[EMC]\x01当前不可打开预定列表");
		return Plugin_Handled;
	}
	BuildNomlistMenu(client);
	return Plugin_Handled;	
}

public void NominateOnClientDisconnect(int client)
{
	if(IsFakeClient(client))	return;
	int nom_index;
	nom_index=is_Nominator(client);
	char buffer[256];
	Nomlist_Log nom_log;
	if(nom_index!=-1)
	{
		GetArrayArray(Nom_Map_List,nom_index,nom_log,sizeof(nom_log));
		Format(buffer,sizeof(buffer)," \x05[EMC]\x07%s\x01的订阅者\x09%s\x01退出了服务器，订阅取消并返还积分",nom_log.name,nom_log.nominator_name);
		PrintToServer(buffer);
		PrintToChatAll(buffer);
		Nom_Map_List.Erase(nom_index);
	}
}
void NominateMapMenu(int client,char trie_search[PLATFORM_MAX_PATH]="")
{
	Menu menu = CreateMenu(NominateMenuHandler);
	Map_Log map;
	Map_Info map_detail;
	menu.SetTitle("预定地图");
	char szBuffer[PLATFORM_MAX_PATH];
	for(int i = 0 ; i < Map_List.Length ; i++)
	{
		GetArrayArray(Map_List, i, map, sizeof(map));

		// 有长度 && 地图名不包含 && 中文名不包含 && 标签不包含
		if (strlen(trie_search) && StrContains(map.name,trie_search,false) == -1 && StrContains(map.name_cn,trie_search) == -1)
			continue;
		Maps.GetArray(map.name, map_detail, sizeof(map_detail));
		if (map_detail.vis == 0)
			continue;
		
		Format(szBuffer, sizeof(szBuffer), "%s [%s]", map.name, map.name_cn);
		menu.AddItem(map.name, szBuffer);
	}
	if(strlen(trie_search))
	{
		if(menu.ItemCount == 0)
		{
			menu.Close();
			PrintToChat(client,"\x05预定地图：找不到匹配地图");
			return;
		}
		else if(menu.ItemCount == 1)
		{
			menu.GetItem(0, map.name, sizeof(map.name));
			menu.Close();
			Maps.GetArray(map.name,map_detail,sizeof(map_detail));
			NomMapInfoMenu(client,map_detail);
		}
		else if(menu.ItemCount > 1)
		{
			menu.Display(client, MENU_TIME_FOREVER);			
		}
	}
	else
	{
		menu.Display(client, MENU_TIME_FOREVER);
	}	
}

int NominateMenuHandler(Menu menu, MenuAction action, int client, int param) 
{
	Map_Info Tmap;
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{                             
		char buffer[PLATFORM_MAX_PATH];                                            
		menu.GetItem(param,buffer,sizeof(buffer));
		Maps.GetArray(buffer,Tmap,sizeof(Tmap));
		NomMapInfoMenu(client,Tmap);
	}	
}
void NomMapInfoMenu(int client,Map_Info Tmap)
{
	Menu menu = CreateMenu(NomMapInfoMenuHandler);
	char buffer[PLATFORM_MAX_PATH];
	char cooldown_state[PLATFORM_MAX_PATH];
	char ctime[64];
	int current_time;
	bool cooldown_over;
	cooldown_over = false;
	current_time = GetTime();
	FormatTime(ctime,64,NULL_STRING,Tmap.last_run_time);
	if(!DiffTimeTransform1(cooldown_state,current_time,Tmap.last_run_time,Tmap.cooldown*60))
	{
		Format(cooldown_state,sizeof(cooldown_state),"冷却完成");
		cooldown_over = true;
	}
	Format(buffer,sizeof(buffer),"地图预定:%s\n\
		最后运行时间:%s\n\
		冷却时间:%d分钟(%s)\n\
		订价:%d积分[VIP减半]%s\n\
		地图译名:%s",Tmap.name,ctime,Tmap.cooldown,cooldown_state,Tmap.cost,IsInClockTimePeriod(0,12)?"[优惠时间:生效]":"[优惠时间:未生效]",Tmap.name_cn);
	menu.SetTitle(buffer);
	int server_port = FindConVar("hostport").IntValue;
	bool g_Interval_Allow = true;
	if(server_port == 27015)
	{
		if(Tmap.difficulty>=2)
		{
			if(g_Map_Interval_Count>0)
			{
				g_Interval_Allow = false;
			}
		}
	}
	menu.AddItem(Tmap.name,"预定地图",(g_Interval_Allow&&Tmap.available&&(cooldown_over||Tmap.temp_cooldown)&&Tmap.download&&Tmap.exist) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(Tmap.name,"重置冷却",GetAdminFlag(GetUserAdmin(client),Admin_Generic) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(Tmap.name,"强制提名",GetAdminFlag(GetUserAdmin(client),Admin_Generic) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(Tmap.name,"强制更换",GetAdminFlag(GetUserAdmin(client),Admin_Generic) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"地图难度:%s",difficulty_name[Tmap.difficulty]);
	menu.AddItem(Tmap.name,buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"地图标签:");
	for(int i=0;i<=9;i++)
	{
		if(Tmap.tag&label_code[i])
		{
			Format(buffer,sizeof(buffer),"%s|%s|",buffer,label_name[i]);
		}
	}
	menu.AddItem(Tmap.name,buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"疲劳状态:%d(+%d)\n仅对1F困难图生效",g_Map_Interval_Count,Tmap.interval);
	menu.AddItem(Tmap.name,buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"预定设置:[开放预定:%s][下载站:%s添加]\n[文件存在:%s]",Tmap.available?"是":"否",Tmap.download?"已":"未",Tmap.exist?"是":"否");
	if(Tmap.tag&label_code[9])
	{
		Format(buffer,sizeof(buffer),"%s[人数上限:35]",buffer);
	}
	menu.AddItem(Tmap.name,buffer,ITEMDRAW_DISABLED);

	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int NomMapInfoMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	Map_Info map;
	char client_name[64];
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		GetClientName(client,client_name,sizeof(client_name));
		menu.GetItem(param,map.name,sizeof(map.name));
		Maps.GetArray(map.name,map,sizeof(map));
		char buffer[256];
		if(param == 0)	
		{
			NominateMap(client,map,0);
		}
		else if (param == 1)
		{
			map.temp_cooldown = true;
			Maps.SetArray(map.name,map,sizeof(map),true);
			Format(buffer,sizeof(buffer)," \x05[EMC] \x09%s \x01重置了地图 \x07%s \x01的冷却",client_name,map.name);
			PrintToChatAll(buffer);
		}
		else if (param == 2)
		{
			NominateMap(client,map,1);
		}
		else if (param == 3)
		{
			Format(buffer,sizeof(buffer)," \x05[EMC]\x01管理员强制更换地图为:\x07%s",map.name);
			PrintToChatAll(buffer);
			ForceChangeLevel(map.name,"强制更换");
		}
	}
	else if (param == MenuCancel_ExitBack)
	{
		NominateMapMenu(client, g_sPlayerSearchFilter[client]);
	}
}


void NominateMap(int client,Map_Info map,int forcenom=0)
{
	char buffer[PLATFORM_MAX_PATH];
	Nomlist_Log nommap,nom_log;
	int nom_index;
	nom_index = is_Nominator(client);
	nommap.id = map.id;
	strcopy(nommap.name,sizeof(nommap.name),map.name);
	nommap.nominator_steamid = GetSteamAccountID(client,true);
	GetClientAuthId(client,AuthId_Steam2,nommap.nominator_steamauth,sizeof(nommap.nominator_steamauth),true);
	GetClientName(client,nommap.nominator_name,sizeof(nommap.nominator_name));
	int credits = 0;
	if (g_pStore)
		credits = Store_GetClientCredits(client);
	if(map.tag&label_code[9]&&forcenom==0)
	{
		if(GetClientCount(true)>35)
		{
			PrintToChat(client," \x05[地图系统]\x01人数超过\x09 35 \x01人，无法订阅该地图");
			return;
		}
	}
	if(!Nominate_ALLOW||Nom_Map_List.Length>=Nominate_Max_Num)
	{
		PrintCenterText(client,"[EMC]当前无法预定地图");
		return;
	}
	if(isNominated(map))
	{
		PrintCenterText(client,"[EMC]该地图已被预定!");
		return;
	}
	int credits_temp=0;
	if(!g_pStore || credits>=map.cost || forcenom)
	{
		if(nom_index!=-1)
		{	
			GetArrayArray(Nom_Map_List,nom_index,nom_log,sizeof(nom_log));
			credits_temp = nom_log.nom_cost;
			CancelNom(nom_index,client);
		}
		nommap.nom_cost = 0;
		if(!forcenom && map.cost > 0)
		{
			if (GetUserFlagBits(client) && ADMFLAG_RESERVATION)
			{
				PrintToServer("vip");
				nommap.nom_cost = RoundFloat(map.cost * 0.5);
				PrintToServer("%i",map.cost);
			}
			else
			{
				if(IsInClockTimePeriod(0,12))
				{
					nommap.nom_cost = RoundFloat(map.cost * 0.5);
				}
				else
				{
					nommap.nom_cost = map.cost;
				}
			}

			if (g_pStore)
			{
				Store_SetClientCredits(client,credits-nommap.nom_cost+credits_temp);
				Format(buffer,sizeof(buffer),"[EMC]消费积分%d",nommap.nom_cost);
				PrintCenterText(client,buffer);
			}
		}
		Nom_Map_List.PushArray(nommap,sizeof(nommap));
		if(forcenom)
		{
			Format(buffer,sizeof(buffer)," \x05[EMC] \x09%s\x06[\x07%s\x06] \x01强制预定了地图 \x07%s",nommap.nominator_name,nommap.nominator_steamauth,nommap.name);
		}
		else
		{
			Format(buffer,sizeof(buffer)," \x05[EMC] \x09%s\x06[\x07%s\x06] \x01预定了地图 \x07%s",nommap.nominator_name,nommap.nominator_steamauth,nommap.name);
		}
		PrintToConsoleAll(buffer);
		PrintToChatAll(buffer);
		Format(buffer,sizeof(buffer)," \x05[EMC] \x01当前已预定 \x03%d \x01张地图",Nom_Map_List.Length);
		PrintToChatAll(buffer);
	}
	else
	{
		PrintCenterText(client,"[EMC]积分不足!");
	}
}

bool isNominated(Map_Info map)
{
	Nomlist_Log nom_log;
	for(int i = 0 ; i < Nom_Map_List.Length ; i++)
	{
		GetArrayArray(Nom_Map_List,i,nom_log,sizeof(nom_log));
		if(strcmp(map.name,nom_log.name)==0)
		{
			return true;
		}
	}
	return false;
}
int is_Nominator(int client)
{
	Nomlist_Log nom_log;
	int steamid;
	steamid = GetSteamAccountID(client,true);
	for(int i = 0 ; i < Nom_Map_List.Length ; i++)
	{
		GetArrayArray(Nom_Map_List,i,nom_log,sizeof(nom_log));
		if(steamid==nom_log.nominator_steamid)
		{
			return i;
		}
	}
	return -1;	
}

//待实现：断线取消订阅

void BuildNomlistMenu(int client)
{
	char buffer[256],temp[3];
	Menu menu = CreateMenu(NomlistMenuHandler);
	Nomlist_Log map;
	menu.SetTitle("订图记录");
	int nom_index;
	nom_index = is_Nominator(client);
	if(Nom_Map_List.Length)
	{
		for(int i =0 ; i < Nom_Map_List.Length ; i++)
		{
			GetArrayArray(Nom_Map_List,i,map,sizeof(map));
			if(i==nom_index)
			{
				Format(buffer,sizeof(buffer),"地图名:%s\n预定者:%s(你) 点击取消预定",map.name,map.nominator_name);
				IntToString(i,temp,sizeof(temp));
				menu.AddItem(temp,buffer,ITEMDRAW_DEFAULT);
			}
			else
			{
				Format(buffer,sizeof(buffer),"地图名:%s\n预定者:%s",map.name,map.nominator_name);
				IntToString(i,temp,sizeof(temp));
				menu.AddItem(temp,buffer,GetAdminFlag(GetUserAdmin(client),Admin_Generic) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
			}
		}
	}
	else
	{
		PrintToChat(client," \x05[EMC]\x01当前还没有预定的地图!");
	}
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int NomlistMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if(action == MenuAction_End)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		CancelNom(param,client);
	}
	else if (param == MenuCancel_ExitBack) BuildNomlistMenu(client);
}

void CancelNom(int index,int opt)	
{
	Nomlist_Log nom_log;
	char buffer[256];
	int opt_steamid;
	opt_steamid = GetSteamAccountID(opt,true);
	int nominator_credits,nominator_index;
	char opt_name[64];
	GetArrayArray(Nom_Map_List,index,nom_log,sizeof(nom_log));
	nominator_index = -1;
	if(nom_log.nominator_steamid==opt_steamid)
	{
		Format(buffer,sizeof(buffer)," \x05[EMC]\x09%s\x01取消了自己预定的\x09%s\x01,费用\x09%d\x01积分已归还",nom_log.nominator_name,nom_log.name,nom_log.nom_cost);
		PrintToChatAll(buffer);
		
		if (g_pStore)
		{
			nominator_credits = Store_GetClientCredits(opt);
			Store_SetClientCredits(opt,nominator_credits+nom_log.nom_cost);
		}
	}
	else
	{
		GetClientName(opt,opt_name,sizeof(opt_name));
		Format(buffer,sizeof(buffer),"[EMC]%s强制取消了%s预定的%s",opt_name,nom_log.nominator_name,nom_log.name);
		for(int i = 1;i < MaxClients;i++)
		{
			if(GetSteamAccountID(i,true)==nom_log.nominator_steamid)
			{
				nominator_index = i;
				break;
			}
		}
		if(nominator_index != -1)
		{
			if(IsClientInGame(nominator_index))
			{
				if (g_pStore)
				{
					nominator_credits = Store_GetClientCredits(nominator_index);
					Store_SetClientCredits(nominator_index,nominator_credits+nom_log.nom_cost);	
					PrintToChatAll("[EMC]费用已归还本人");
				}
			}
		}

	}
	Nom_Map_List.Erase(index);
}