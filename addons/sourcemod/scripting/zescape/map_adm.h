/*enum struct Map_Info
{
	int id;
	char name[PLATFORM_MAX_PATH];
	char name_cn[PLATFORM_MAX_PATH];
	int cooldown;
	int last_run_time;
	int available;
	int cost;
	int heat;
	int difficulty;
	int tag;
	int round;
	int wins;
	int translated;
	int download;
	int exist;
	int roundtime;
	int timelimit;
	float infecttime;
	int random;
	int extend;
	bool temp_cooldown;
 	int nohmskill;
 	int nozmskill;
 	int nojk;
 	int nobhoplimit;
 	int interval;
 	int ego;	//EnableGo
 	int vis;	//Visable
 	int tagscale;
 	float dmgscale;
 	float knockback;
 	int zmclass;
 	int mr;
 	float mrx;
 	float mry;
 	float mrz;
}*/
enum AdmLog_Type
{
	DEFAULT = 0,
	CHANGE_COOLDOWN = 1,
	CHANGE_COST = 2,
	TOGGLE_AVAILABLE = 3,
	CHANGE_WEAPON_AVAILABLE = 4,
	CHANGE_WEAPON_NUM = 5,
	CHANGE_WEAPON_PRICE = 6,
	TOGGLE_ZMCLASS = 7,
	TOGGLE_HMCLASS = 8,
	TOGGLE_ZMHERO = 9,
	TOGGLE_HMHERO = 10
};
int tag_num=11;
char difficulty_name[6][10]={"简单","普通","困难","高难","极难","傻逼"};
char label_name[11][64]={"FF","卤粉","弹幕方块","经典","闯关","娱乐","休闲","对抗","长征","感染","活动"};
int label_code[11]={1,2,4,8,16,32,64,128,256,512,1024};
Handle g_MapConfigLoaded;
enum struct Map_Log
{
	int id;
	char name[PLATFORM_MAX_PATH];
	char name_cn[PLATFORM_MAX_PATH]
}
int Map_Id_Max;
StringMap Maps;
ArrayList Map_List;
bool g_Map_Loaded;
ConVar Cvar_SM_NEXTMAP;
int MapListArraySort(int index1,int index2,Handle array,Handle hndl)
{
	Map_Log map1,map2;
	GetArrayArray(array,index1,map1,sizeof(map1));
	GetArrayArray(array,index2,map2,sizeof(map2));
	return strcmp(map1.name,map2.name);
}
void MapAdmOnMapEnd()
{
	Maps.Clear();
	Map_List.Clear();
	CloseHandleSafe(Map_List);
	Map_List = CreateArray(sizeof(Map_Log));
	PrintToServer("[MapAdm MapEnd]");
	g_Map_Loaded = false;
}
void MapAdmOnPluginStart()
{
	g_Map_Loaded = false;
	Maps = CreateTrie();
	Map_List = CreateArray(sizeof(Map_Log));
	Cvar_SM_NEXTMAP = FindConVar("sm_nextmap");
	RegAdminCmd("sm_mdr",MapDataReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapdatareload",MapDataReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mfr",MapFileReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapfilereload",MapFileReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_ma",MapAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapadmin",MapAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapnamecn_update",MapNamecnCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapcd_update",MapCooldownCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapcost_update",MapCostCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_resetma",MapAdminResetCommand,ADMFLAG_GENERIC);
	RegConsoleCmd("sm_mah",MapAdminHistoryCommand);
	RegConsoleCmd("sm_mg",MapLabelCommand);
	if(g_MapConfigLoaded == null)
	{
		g_MapConfigLoaded = CreateGlobalForward("EMC_Forward_MapConfigLoaded",ET_Ignore);
	}
}
void MapAdmOnDbConnected_MapStartPost()
{
	Maps.Clear();
	Map_List.Clear();
	MapDataReload();
}

Action MapLabelCommand(int client,int args)
{
	if(client<=0||client>=65)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	if(IsFakeClient(client))	return Plugin_Handled;
	MapLabelMenuBuild(client);
	return Plugin_Handled;
}
Action MapAdminHistoryCommand(int client,int args)
{
	if(client<=0||client>=65)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	if(IsFakeClient(client))	return Plugin_Handled;
	if (!args) {
		MapAdminHistoryMenuBuild(client);
		return Plugin_Handled;
	}
	char arg[PLATFORM_MAX_PATH];
	GetCmdArg(1, arg, sizeof(arg));
	MapAdminHistoryMenuBuild(client,arg);
	return Plugin_Handled;
}

void MapAdminHistoryMenuBuild(int client,char[] trie_search="")
{
	if(client<=0||client>=65)	return;
	if(!IsClientInGame(client))	return;
	if(IsFakeClient(client))	return;
	Map_Log map;
	Map_Info mapt;
	Menu menu = CreateMenu(MapAdminHistoryMenuHandler);
	menu.SetTitle("地图管理记录查询");
	for(int i = 0 ; i < Map_List.Length ; i++)
	{
		GetArrayArray(Map_List,i,map,sizeof(map));
		if(strlen(trie_search) && StrContains(map.name,trie_search,false)==-1 && StrContains(map.name_cn,trie_search) == -1)	continue;
		menu.AddItem(map.name,map.name);
	}
	if(strlen(trie_search))
	{
		if(menu.ItemCount == 0)
		{
			delete menu;
			PrintToChat(client,"\x05[地图历史]\x01找不到匹配地图");
			return;
		}
		if(menu.ItemCount == 1)
		{
			menu.GetItem(0, map.name, sizeof(map.name));
			delete menu;
			Maps.GetArray(map.name,mapt,sizeof(mapt));
			MapAdminHistoryView(client,mapt);
			return;
		}
		if(menu.ItemCount > 1)
		{
			menu.Display(client, MENU_TIME_FOREVER);
			return;
		}
	}
	else
	{
		menu.Display(client, MENU_TIME_FOREVER);
	}

}
int MapAdminHistoryMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	Map_Info map;
	if (action == MenuAction_End)	delete menu;
	else if (action == MenuAction_Select)
	{
		char buffer[PLATFORM_MAX_PATH];
		menu.GetItem(param,buffer,sizeof(buffer));
		Maps.GetArray(buffer,map,sizeof(map));
		MapAdminHistoryView(client,map);
	}
	return 0;
}
void MapAdminHistoryView(int client,Map_Info map)
{
	char query[512];
	Format(query,sizeof(query),"SELECT * FROM exgusers_mapadmlog WHERE TARGETSTR = '%s' ORDER BY TIMESTAMP DESC LIMIT 300",map.name);
	DbMQuery(MapAdminHistoryViewCallback,query,client);
}
void MapAdminHistoryViewCallback(Handle owner, Handle hndl, char[] error, any data)
{
	int client = data;
	int count=-1;
	if(!hndl)
	{
		PrintToChat(client," \x05[地图管理]\x01该地图没有管理日志!")
		return;
	}
	if(!SQL_FetchRow(hndl))
	{
		PrintToChat(client," \x05[地图管理]\x01该地图没有管理日志!")
		delete hndl;
		return;
	}
	else
	{
		Menu menu = CreateMenu(MapAdminHistoryViewMenuHandler);
		char name[64],buffer[256];
		DbFetchString(hndl,"TARGETSTR",name,sizeof(name));
		menu.SetTitle(name);
		int admin_uid;
		int type;
		int value;
		int timestamp;
		char ctime[64];
		char valuestr[64];
		char admin_name[64];
		count++;
		DbFetchString(hndl,"NAME",admin_name,sizeof(name));
		admin_uid = DbFetchInt(hndl,"UID");
		type = DbFetchInt(hndl,"TYPE");
		timestamp = DbFetchInt(hndl,"TIMESTAMP");
		value = DbFetchInt(hndl,"VALUE");
		DbFetchString(hndl,"VALUESTR",valuestr,sizeof(valuestr));
		FormatTime(ctime,64,NULL_STRING,timestamp);
		switch(type)
		{
			case CHANGE_COOLDOWN:
			{
				Format(buffer,sizeof(buffer),"修改冷却时间至%d分钟[%s]\n操作人:[%d]%s",value,ctime,admin_uid,admin_name);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case CHANGE_COST:
			{
				Format(buffer,sizeof(buffer),"修改地图定价至:%d积分[%s]\n操作人:[%d]%s",value,ctime,admin_uid,admin_name);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case TOGGLE_AVAILABLE:
			{
				Format(buffer,sizeof(buffer),"开放订阅:%s[%s]\n操作人:[%d]%s",value?"开":"关",ctime,admin_uid,admin_name);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case CHANGE_WEAPON_AVAILABLE:
			{
				Format(buffer,sizeof(buffer),"%s开放购买:%s[%s]\n操作人:%s[UID:%d]",valuestr,value?"开":"关",ctime,admin_name,admin_uid);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case CHANGE_WEAPON_NUM:
			{
				Format(buffer,sizeof(buffer),"%s购买数量:%d[%s]\n操作人:%s[UID:%d]",valuestr,value,ctime,admin_name,admin_uid);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case CHANGE_WEAPON_PRICE:
			{
				Format(buffer,sizeof(buffer),"%s购买价格:%d[%s]\n操作人:%s[UID:%d]",valuestr,value,ctime,admin_name,admin_uid);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case TOGGLE_ZMCLASS:
			{
				Format(buffer,sizeof(buffer),"僵尸类型:%s[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case TOGGLE_HMCLASS:
			{
				Format(buffer,sizeof(buffer),"人类类型:%s[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case TOGGLE_ZMHERO:
			{
				Format(buffer,sizeof(buffer),"僵尸英雄:%s[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			case TOGGLE_HMHERO:
			{
				Format(buffer,sizeof(buffer),"人类英雄:%s[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
			default:
			{
				Format(buffer,sizeof(buffer),"未知操作[%s]\n操作人:[%d]%s",ctime,admin_uid,admin_name);
				menu.AddItem("",buffer,ITEMDRAW_DISABLED);
			}
		}
		while(SQL_FetchRow(hndl))
		{
			count++;
			DbFetchString(hndl,"NAME",admin_name,sizeof(name));
			admin_uid = DbFetchInt(hndl,"UID");
			type = DbFetchInt(hndl,"TYPE");
			timestamp = DbFetchInt(hndl,"TIMESTAMP");
			DbFetchString(hndl,"VALUESTR",valuestr,sizeof(valuestr));
			FormatTime(ctime,64,NULL_STRING,timestamp);
			switch(type)
			{
				case CHANGE_COOLDOWN:
				{
					Format(buffer,sizeof(buffer),"修改冷却时间至%d分钟[%s]\n操作人:[%d]%s",value,ctime,admin_uid,admin_name);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case CHANGE_COST:
				{
					Format(buffer,sizeof(buffer),"修改地图定价至:%d积分[%s]\n操作人:[%d]%s",value,ctime,admin_uid,admin_name);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case TOGGLE_AVAILABLE:
				{
					Format(buffer,sizeof(buffer),"开放订阅:%s[%s]\n操作人:[%d]%s",value?"开":"关",ctime,admin_uid,admin_name);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case CHANGE_WEAPON_AVAILABLE:
				{
					Format(buffer,sizeof(buffer),"%s开放购买:%s[%s]\n操作人:%s[UID:%d]",valuestr,value?"开":"关",ctime,admin_name,admin_uid);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case CHANGE_WEAPON_NUM:
				{
					Format(buffer,sizeof(buffer),"%s购买数量:%d[%s]\n操作人:%s[UID:%d]",valuestr,value,ctime,admin_name,admin_uid);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case CHANGE_WEAPON_PRICE:
				{
					Format(buffer,sizeof(buffer),"%s购买价格:%d[%s]\n操作人:%s[UID:%d]",valuestr,value,ctime,admin_name,admin_uid);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case TOGGLE_ZMCLASS:
				{
					Format(buffer,sizeof(buffer),"僵尸类型:%d[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case TOGGLE_HMCLASS:
				{
					Format(buffer,sizeof(buffer),"人类类型:%d[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case TOGGLE_ZMHERO:
				{
					Format(buffer,sizeof(buffer),"僵尸英雄:%d[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				case TOGGLE_HMHERO:
				{
					Format(buffer,sizeof(buffer),"人类英雄:%d[%s]\n操作人:%s[UID:%d]",value?"开":"关",ctime,admin_name,admin_uid);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
				default:
				{
					Format(buffer,sizeof(buffer),"未知操作[%s]\n操作人:[%d]%s",ctime,admin_uid,admin_name);
					menu.AddItem("",buffer,ITEMDRAW_DISABLED);
				}
			}
		}
		menu.Display(client,MENU_TIME_FOREVER);
		delete hndl;
		return;
	}
}

int MapAdminHistoryViewMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End)	delete menu;
	return 0;
}

void MapLabelMenuBuild(int client)
{
	if(client<=0||client>=65)	return;
	if(!IsClientInGame(client))	return;
	if(IsFakeClient(client))	return;
	Menu menu = CreateMenu(MapGroupMenuHandler);
	menu.SetTitle("地图组查询");
	for(int i =0; i < tag_num; i++)
	{
		menu.AddItem("",label_name[i]);
	}
	/*Map_Log map;
	Map_Info mapt;
	for(int i = 0 ; i < Map_List.Length ; i++)
	{
		GetArrayArray(Map_List,i,map,sizeof(map));
		Maps.GetArray(map.name,mapt,sizeof(mapt));
		if(mapt.tag&(1<<10))
		{
			menu.AddItem(mapt.name,mapt.name,GetAdminFlag(GetUserAdmin(client),Admin_Generic) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
		}
	}
	if(menu.ItemCount==0)
	{
		delete menu;
	}
	else
	{
		menu.Display(client, MENU_TIME_FOREVER);
	}*/
	menu.Display(client,MENU_TIME_FOREVER);
	return;
}

int MapGroupMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		BuildGroupMapMenu(client,param);
		/*menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		map.tag-=(1<<10);
		MapCfgUpdate(map);
		PrintToChat(client," \x05[地图管理]\x01移除了地图\x07 %s \x01的\x07[%s]\x01标签",map.name,label_name[10]);
		MapLabelMenuBuild(client);*/
	}
	return 0;
}
void BuildGroupMapMenu(int client,int group)
{
	Menu menu = CreateMenu(GroupMapListHandler);
	menu.SetTitle(label_name[group]);
	Map_Log map;
	Map_Info mapt;
	char group_id[64];
	IntToString(group,group_id,sizeof(group_id));
	menu.AddItem(group_id,"占位",ITEMDRAW_DISABLED);
	for(int i = 0 ; i < Map_List.Length ; i++)
	{
		GetArrayArray(Map_List,i,map,sizeof(map));
		Maps.GetArray(map.name,mapt,sizeof(mapt));
		if(mapt.tag&(label_code[group]))
		{
			menu.AddItem(mapt.name,mapt.name,GetAdminFlag(GetUserAdmin(client),Admin_Generic) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
		}
	}
	if(menu.ItemCount<=1)
	{
		delete menu;
	}
	else
	{
		menu.Display(client, MENU_TIME_FOREVER);
	}
}
int GroupMapListHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	char group_id[64];
	Map_Info map;
	int group;
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		menu.GetItem(0,group_id,sizeof(group_id));
		group = StringToInt(group_id);
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		map.tag-=(label_code[group]);
		MapCfgUpdate(map);
		PrintToChat(client," \x05[地图管理]\x01移除了地图\x07 %s \x01的\x07[%s]\x01标签",map.name,label_name[group]);
	}
	return 0;
}

Action MapDataReloadCommand(int client,int args)
{
	if(isDbConnected())
	{
		MapDataReload();
	}
	return Plugin_Handled;
}
Action MapFileReloadCommand(int client,int args)
{
	if(isDbConnected())
	{
		MapFileReload();
	}
	return Plugin_Handled;
}

void MapDataReload()
{
	Map_Id_Max = 0;
	char query[512];
	Format(query,sizeof(query),"SELECT * FROM zemaps ORDER BY name ASC");
	DbMQuery(MapDataLoadCallback,query);
}

void MapDataLoadCallback(Handle owner, Handle hndl, char[] error, any data)
{
	Map_List.Clear();
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE)	SetFailState("数据库错误: %s", error);
	PrintToServer("[test]1 MDL");
	if (!SQL_GetRowCount(hndl)) {
		PrintToChatAll("当前地图信息未录入");
		return;
	}
	else PrintToServer("DataLoading");
	while (SQL_FetchRow(hndl)) {
		Map_Info map;
		Map_Log mapl;
		map.id = DbFetchInt(hndl, "ID");
		mapl.id = map.id;
		DbFetchString(hndl, "NAME", map.name, sizeof(map.name));
		Format(mapl.name,sizeof(mapl.name),"%s",map.name);
		DbFetchString(hndl, "CN_NAME", map.name_cn, sizeof(map.name_cn));
		Format(mapl.name_cn,sizeof(mapl.name_cn),"%s",map.name_cn);
		map.cooldown = DbFetchInt(hndl, "COOLDOWN");
		map.last_run_time = DbFetchInt(hndl, "LAST_RUN_TIME");
		map.available = DbFetchInt(hndl, "AVAILABLE");
		map.cost = DbFetchInt(hndl, "COST");
		map.heat = DbFetchInt(hndl, "HEAT");
		map.difficulty = DbFetchInt(hndl, "DIFFICULTY");
		map.tag = DbFetchInt(hndl, "TAG");
		map.round = DbFetchInt(hndl, "ROUND");
		map.wins = DbFetchInt(hndl, "WINS");
		map.translated = DbFetchInt(hndl, "TRANSLATED");
		map.download = DbFetchInt(hndl, "DOWNLOAD");
		map.roundtime = DbFetchInt(hndl, "ROUNDTIME");
		map.timelimit = DbFetchInt(hndl, "TIMELIMIT");
		map.infecttime = DbFetchFloat(hndl, "INFECTTIME");
		map.random = DbFetchInt(hndl, "RANDOM");
		map.exist = 0;
		map.extend = DbFetchInt(hndl, "EXTEND");
		map.nohmskill = DbFetchInt(hndl,"NOHMSKILL");
		map.nozmskill = DbFetchInt(hndl,"NOZMSKILL");
		map.nojk = DbFetchInt(hndl,"NOJK");
		map.nobhoplimit = DbFetchInt(hndl,"NOBHOPLIMIT");
		map.interval = DbFetchInt(hndl,"FATIGUE");
		map.ego = DbFetchInt(hndl,"EGO");
		map.vis = DbFetchInt(hndl,"VIS");
		map.dmgscale = DbFetchFloat(hndl,"DMGSCALE");
		map.tagscale = DbFetchInt(hndl,"TAGSCALE");
		map.knockback = DbFetchFloat(hndl,"KNOCKBACK");
		map.zmhpscale = DbFetchFloat(hndl,"ZMHPSCALE");
		map.mr = DbFetchInt(hndl,"MR");
		map.mrx = DbFetchFloat(hndl,"MRX");
		map.mry = DbFetchFloat(hndl,"MRY");
		map.mrz = DbFetchFloat(hndl,"MRZ");
		map.pllower = DbFetchInt(hndl,"PLLOWER");
		map.plupper = DbFetchInt(hndl,"PLUPPER");
		map.zmclass = DbFetchInt(hndl,"ZMCLASS");
		map.hmclass = DbFetchInt(hndl,"HMCLASS");
		map.zmhero = DbFetchInt(hndl,"ZMHERO");
		map.hmhero = DbFetchInt(hndl,"HMHERO");
		map.zmhprecover = DbFetchFloat(hndl,"ZMHPRECOVER");
		int tempcd = DbFetchInt(hndl,"TEMPCD");
		if(tempcd)
		{
			map.temp_cooldown = true;
		}
		else
		{
			map.temp_cooldown = false;
		}
		Maps.SetArray(map.name, map, sizeof(map), true);
//		Format(buffer,sizeof(buffer),"[MapDataLoad]Added Map List:%s",mapl.name);
//		PrintToServer(buffer);
		Map_List.PushArray(mapl,sizeof(mapl));
		if(map.id>Map_Id_Max)	Map_Id_Max = map.id;
		//PrintToServer("%d %s",map.id,map.name);
	}
	MapFileReload();
	delete hndl;
}
void MapFileReload()
{
	PrintToServer("[test]2 MFL");
	DirectoryListing dl = OpenDirectory("maps", false);
	if (!dl) SetFailState("Map File Load Failed");
	char file_name[PLATFORM_MAX_PATH];
	FileType type;
	int index;
	while(dl.GetNext(file_name,sizeof(file_name),type))
	{
		if(type!=FileType_File)	continue;
		index = StrContains(file_name, ".bsp", false);
		if (index == -1) continue;
		file_name[index] = '\0';
		Map_Info map;
		Map_Log mapl;
		if(!Maps.GetArray(file_name,map,sizeof(map)))
		{
			map.name = file_name;
			Map_Id_Max++;
			map.id = Map_Id_Max;
			map.exist = 1;
			map.extend = 3;
			map.temp_cooldown = false;
			Maps.SetArray(map.name,map,sizeof(map),true);
			//PrintToServer("New Map:%d %s",map.id,map.name);
			NewMapFileUpdate(map);
			mapl.id = map.id;
			mapl.name = map.name;
			Map_List.PushArray(mapl,sizeof(mapl));
//			Format(buffer,sizeof(buffer),"[MapFileLoad]Added Map List:%s",mapl.name);
//			PrintToServer(buffer);
		}
		else
		{
			map.exist = 1;
			Maps.SetArray(map.name,map,sizeof(map),true);
		}
	}
	Map_List.SortCustom(MapListArraySort);
	ResetFileExist();
	PrintToServer("[test]4 MFE END");
	g_Map_Loaded = true;
	MapInfoOnDbConnected_MapStartPost();
	delete dl;
}

void NewMapFileUpdate(Map_Info map)
{
	char query[512];
	Format(query,sizeof(query),"INSERT INTO zemaps (ID,NAME) VALUES(%d,'%s')",map.id,map.name);
	DbMQuery(DbQueryErrorCallback,query);
}

void ResetFileExist()
{
	StringMapSnapshot snap= Maps.Snapshot();
	Map_Info map;
	char query[512];
	for (int i =0;i < snap.Length ; i++)
	{
		snap.GetKey(i, map.name, sizeof(map.name));
		Maps.GetArray(map.name, map, sizeof(map));

		Format(query,sizeof(query),"UPDATE zemaps SET EXIST = %d where ID = %d",map.exist,map.id);
		DbMQuery(DbQueryErrorCallback,query);
	}
	snap.Close();
}

Action MapAdminCommand(int client, int args) {
	if (!IsClientInGame(client)) return Plugin_Handled;
	if (!args) {
		MapAdminMenu(client);
		return Plugin_Handled;
	}
	char arg[PLATFORM_MAX_PATH];
	GetCmdArg(1, arg, sizeof(arg));
	MapAdminMenu(client,arg);
	return Plugin_Handled;
}

Action MapAdminResetCommand(int client, int args) {
	if (!IsClientInGame(client)) return Plugin_Handled;
	return Plugin_Handled;
}

void MapAdminMenu(int client,char trie_search[PLATFORM_MAX_PATH]="")
{
	Menu menu = CreateMenu(MapAdminMenuHandler);
	Map_Log map;
	Map_Info mapt;
	menu.SetTitle("地图管理菜单");
	char info[256];
	for(int i = 0 ; i < Map_List.Length ; i++)
	{
		GetArrayArray(Map_List,i,map,sizeof(map));
		if(strlen(trie_search) && StrContains(map.name,trie_search,false)==-1 && StrContains(map.name_cn,trie_search) == -1)
			continue;

		Format(info, 256, "%s", map.name);
		if (map.name_cn[0] == NULL_STRING[0])
			StrCat(info, 256, "[无译名]");

		menu.AddItem(map.name, info);
	}
	if(strlen(trie_search))
	{
		if(menu.ItemCount == 0)
		{
			delete menu;
			PrintToChat(client,"\x05地图管理：找不到匹配地图");
			return;
		}
		if(menu.ItemCount == 1)
		{
			menu.GetItem(0, map.name, sizeof(map.name));
			delete menu;
			Maps.GetArray(map.name,mapt,sizeof(mapt));
			MapAdminConfigMenu(client,mapt);
			return;
		}
		if(menu.ItemCount > 1)
		{
			menu.Display(client, MENU_TIME_FOREVER);
			return;
		}
	}
	else
	{
		menu.Display(client, MENU_TIME_FOREVER);
	}
}

int MapAdminMenuHandler(Menu menu, MenuAction action, int client, int param) {
	Map_Info map;
	if (action == MenuAction_End)	delete menu;
	else if (action == MenuAction_Select)
	{
		char buffer[PLATFORM_MAX_PATH];
		menu.GetItem(param,buffer,sizeof(buffer));
		Maps.GetArray(buffer,map,sizeof(map));
		MapAdminConfigMenu(client,map);
	}
}

void MapAdminConfigMenu(int client,Map_Info map)
{
	Menu menu = CreateMenu(MapAdminCfgHandler);
	char buffer[PLATFORM_MAX_PATH];
	char ctime[64];
	FormatTime(ctime, 64, NULL_STRING,map.last_run_time);
	Format(buffer,sizeof(buffer),"地图管理:\n地图名:%s\n上次运行时间%s",map.name,ctime);
	menu.SetTitle(buffer);
	Format(buffer,sizeof(buffer),"地图译名:%s",map.name_cn);
	menu.AddItem(map.name,buffer);	//0
	Format(buffer,sizeof(buffer),"地图CD(分钟):%d",map.cooldown);
	menu.AddItem(map.name,buffer);	//1
	Format(buffer,sizeof(buffer),"地图订价(积分):%d",map.cost);
	menu.AddItem(map.name,buffer);	//2
	Format(buffer,sizeof(buffer),"难度:%s",difficulty_name[map.difficulty]);
	menu.AddItem(map.name,buffer);	//3
	Format(buffer,sizeof(buffer),"开放订阅:%s",map.available?"是":"否");
	menu.AddItem(map.name,buffer);	//4
	Format(buffer,sizeof(buffer),"已添加下载:%s",map.download?"是":"否");
	menu.AddItem(map.name,buffer);	//5
	Format(buffer,sizeof(buffer),"能否野生出现:%s",map.random?"是":"否");
	menu.AddItem(map.name,buffer);	//6
	Format(buffer,sizeof(buffer),"延长次数:%d",map.extend);
	menu.AddItem(map.name,buffer);	//7
	Format(buffer,sizeof(buffer),"地图时长:%d分钟",map.timelimit);
	menu.AddItem(map.name,buffer);	//8
	Format(buffer,sizeof(buffer),"人类技能:%s",map.nohmskill?"关闭":"开启");
	menu.AddItem(map.name,buffer);	//9
	Format(buffer,sizeof(buffer),"僵尸技能:%s",map.nozmskill?"关闭":"开启");
	menu.AddItem(map.name,buffer);	//10
	Format(buffer,sizeof(buffer),"解卡功能:%s",map.nojk?"关闭":"开启");
	menu.AddItem(map.name,buffer);	//11
	Format(buffer,sizeof(buffer),"连跳限速:%s",map.nobhoplimit?"关闭":"开启");
	menu.AddItem(map.name,buffer);	//12
	if(map.difficulty<=1)
	{
		menu.AddItem(map.name,"疲劳回合:无效(难度低于困难)",ITEMDRAW_DISABLED);	//13
	}
	else
	{
		Format(buffer,sizeof(buffer),"疲劳回合:%d",map.interval);
		menu.AddItem(map.name,buffer);	//13
	}
	Format(buffer,sizeof(buffer),"开局传送:%s",map.ego?"开启":"关闭");
	menu.AddItem(map.name,buffer);	//14
	Format(buffer,sizeof(buffer),"尸变时间:%f秒",map.infecttime);
	menu.AddItem(map.name,buffer);	//15
	Format(buffer,sizeof(buffer),"预订显示:%s",map.vis?"显示":"隐藏");
	menu.AddItem(map.name,buffer);	//16
	Format(buffer,sizeof(buffer),"标签配置:");
	for(int i=0;i<=9;i++)
	{
		if(map.tag&label_code[i])
		{
			Format(buffer,sizeof(buffer),"%s|%s|",buffer,label_name[i]);
		}
	}
	menu.AddItem(map.name,buffer);//17
	Format(buffer,sizeof(buffer),"伤害系数:%f",map.dmgscale);
	menu.AddItem(map.name,buffer);//18
	Format(buffer,sizeof(buffer),"定身系数:%d",map.tagscale);
	menu.AddItem(map.name,buffer);//19
	Format(buffer,sizeof(buffer),"击退系数:%f",map.knockback);
	menu.AddItem(map.name,buffer);//20
	Format(buffer,sizeof(buffer),"血量系数:%f",map.zmhpscale);
	menu.AddItem(map.name,buffer);//21
	Format(buffer,sizeof(buffer),"人数下限:%d",map.pllower);
	menu.AddItem(map.name,buffer);//22
	Format(buffer,sizeof(buffer),"人数上限:%d",map.plupper);
	menu.AddItem(map.name,buffer)//23
	Format(buffer,sizeof(buffer),"僵尸职业:%s",map.zmclass?"开放":"禁止");
	menu.AddItem(map.name,buffer);//24
	Format(buffer,sizeof(buffer),"人类职业:%s",map.hmclass?"开放":"禁止");
	menu.AddItem(map.name,buffer);//25
	Format(buffer,sizeof(buffer),"僵尸英雄:%s",map.zmhero?"开放":"禁止");
	menu.AddItem(map.name,buffer);//26
	Format(buffer,sizeof(buffer),"人类英雄:%s",map.hmhero?"开放":"禁止");
	menu.AddItem(map.name,buffer);//27
	menu.Display(client, MENU_TIME_FOREVER);
}
int MapAdminCfgHandler(Menu menu, MenuAction action, int client, int param)
{
	char buffer[PLATFORM_MAX_PATH];
	Map_Info map;
	char map_name[PLATFORM_MAX_PATH];
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if(param == 0)
		{
			PrintToChat(client,"译名修改参数已发送至控制台");
			Format(buffer,sizeof(buffer),"sm_mapnamecn_update \"%s\" \"%s\"",map.name,map.name_cn);
			PrintToConsole(client,buffer);
		}
		if(param == 1)
		{
			MapCooldownMenu(client,map);
		}
		if(param == 2)
		{
			MapCostMenu(client,map);
		}
		if(param == 3)
		{
			map.difficulty--;
			if(map.difficulty<0)
			{
				map.difficulty = 5;
			}
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 4)
		{
			if(map.available == 1)	map.available =0;
			else map.available = 1;
			int current_time = GetTime();
			USER_LOG userinfo_admin;
			EXGUSERS_GetUserInfo(client,userinfo_admin);
			ADMIN_LOG admlog_add;
			admlog_add.uid = userinfo_admin.uid;
			strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
			admlog_add.type=3;
			admlog_add.value=map.available;
			admlog_add.valuestr="";
			admlog_add.target=0;
			admlog_add.targetstr=map.name;
			admlog_add.timestamp = current_time;
			EXGUSERS_AddAdminLog(admlog_add);

			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 5)
		{
			if(map.download == 1)	map.download =0;
			else map.download = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 6)
		{
			if(map.random == 1)	map.random =0;
			else map.random =1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 7)
		{
			map.extend--;
			if(map.extend<0)
			{
				map.extend = 5;
			}
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 8)
		{
			MapTimeLimitMenu(client,map);
		}
		if(param == 9)
		{
			if(map.nohmskill)	map.nohmskill = 0;
			else map.nohmskill = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 10)
		{
			if(map.nozmskill)	map.nozmskill = 0;
			else map.nozmskill = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 11)
		{
			if(map.nojk)	map.nojk = 0;
			else map.nojk = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 12)
		{
			if(map.nobhoplimit)	map.nobhoplimit = 0;
			else map.nobhoplimit = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 13)
		{
			if(map.interval == 0)	map.interval = 2;
			else map.interval--;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 14)
		{
			if(map.ego == 0)	map.ego = 1;
			else map.ego = 0;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 15)
		{
			MapInfectTimeMenu(client,map);
		}
		if(param == 16)
		{
			if(map.vis == 0)	map.vis = 1;
			else map.vis = 0;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);
		}
		if(param == 17)
		{
			MapTagConfigMenu(client,map);
		}
		if(param == 18)
		{
			MapDmgscaleConfigMenu(client,map);
		}
		if(param == 19)
		{
			MapTagscaleConfigMenu(client,map);
		}
		if(param == 20)
		{
			MapKnockbackConfigMenu(client,map);
		}
		if(param == 21)
		{
			MapZMHpScaleConfigMenu(client,map);
		}
		if(param == 22)
		{
			MapLowerLimitMenu(client,map);
		}
		if(param == 23)
		{
			MapUpperLimitMenu(client,map);
		}
		if(param == 24)
		{
			if(map.zmclass != 0) map.zmclass = 0;
			else map.zmclass = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);

			int current_time = GetTime();
			USER_LOG userinfo_admin;
			EXGUSERS_GetUserInfo(client,userinfo_admin);
			ADMIN_LOG admlog_add;
			admlog_add.uid = userinfo_admin.uid;
			strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
			admlog_add.type=7;
			admlog_add.value=map.zmclass;
			admlog_add.valuestr="";
			admlog_add.target=0;
			admlog_add.targetstr=map.name;
			admlog_add.timestamp = current_time;
			EXGUSERS_AddAdminLog(admlog_add);


		}
		if(param == 25)
		{
			if(map.hmclass != 0) map.hmclass = 0;
			else map.hmclass = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);

			int current_time = GetTime();
			USER_LOG userinfo_admin;
			EXGUSERS_GetUserInfo(client,userinfo_admin);
			ADMIN_LOG admlog_add;
			admlog_add.uid = userinfo_admin.uid;
			strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
			admlog_add.type=8;
			admlog_add.value=map.hmclass;
			admlog_add.valuestr="";
			admlog_add.target=0;
			admlog_add.targetstr=map.name;
			admlog_add.timestamp = current_time;
			EXGUSERS_AddAdminLog(admlog_add);
		}
		if(param == 26)
		{
			if(map.zmhero != 0) map.zmhero = 0;
			else map.zmhero = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);

			int current_time = GetTime();
			USER_LOG userinfo_admin;
			EXGUSERS_GetUserInfo(client,userinfo_admin);
			ADMIN_LOG admlog_add;
			admlog_add.uid = userinfo_admin.uid;
			strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
			admlog_add.type=9;
			admlog_add.value=map.zmhero;
			admlog_add.valuestr="";
			admlog_add.target=0;
			admlog_add.targetstr=map.name;
			admlog_add.timestamp = current_time;
			EXGUSERS_AddAdminLog(admlog_add);
		}
		if(param == 27)
		{
			if(map.hmhero != 0) map.hmhero = 0;
			else map.hmhero = 1;
			MapCfgUpdate(map);
			MapAdminConfigMenu(client,map);

			int current_time = GetTime();
			USER_LOG userinfo_admin;
			EXGUSERS_GetUserInfo(client,userinfo_admin);
			ADMIN_LOG admlog_add;
			admlog_add.uid = userinfo_admin.uid;
			strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
			admlog_add.type=10;
			admlog_add.value=map.hmhero;
			admlog_add.valuestr="";
			admlog_add.target=0;
			admlog_add.targetstr=map.name;
			admlog_add.timestamp = current_time;
			EXGUSERS_AddAdminLog(admlog_add);
		}
	}
	return 0;
}
void MapUpperLimitMenu(int client,Map_Info map)
{
	Menu menu = CreateMenu(MapUpperLimitMenuHandler);
	char buffer[256];
	Format(buffer,sizeof(buffer),"%s:订图人数上限\n当前限制:%d%s",map.name,map.plupper,map.plupper==0?"(不限制)":"");
	menu.SetTitle(buffer);
	menu.AddItem(map.name,"+1人");
	menu.AddItem(map.name,"+5人");
	menu.AddItem(map.name,"+10人");
	menu.AddItem(map.name,"-1人");
	menu.AddItem(map.name,"-5人");
	menu.AddItem(map.name,"-10人");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int MapUpperLimitMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[64];
	Map_Info map;
	switch(action)
	{
		case MenuAction_End:
		{
			delete menu;
		}
		case MenuAction_Select:
		{
			menu.GetItem(param,map_name,sizeof(map_name));
			Maps.GetArray(map_name,map,sizeof(map));
			switch(param)
			{
				case 0:
				{
					map.plupper+=1;
					if(map.plupper>=64)	map.plupper=0;
				}
				case 1:
				{
					map.plupper+=5;
					if(map.plupper>=64)	map.plupper=0;
				}
				case 2:
				{
					map.plupper+=10;
					if(map.plupper>=64)	map.plupper=0;
				}
				case 3:
				{
					map.plupper-=1;
					if(map.plupper<=0)	map.plupper=0;
				}
				case 4:
				{
					map.plupper-=5;
					if(map.plupper<=0)	map.plupper=0;
				}
				case 5:
				{
					map.plupper-=10;
					if(map.plupper<=0)	map.plupper=0;
				}
			}
			MapCfgUpdate(map);
			MapUpperLimitMenu(client,map);
		}
		default:
		{
			if (param == MenuCancel_ExitBack)
			{
				menu.GetItem(0,map_name,sizeof(map_name));
				Maps.GetArray(map_name,map,sizeof(map));
				MapAdminConfigMenu(client,map);
			}
		}
	}
	return 0;
}
void MapLowerLimitMenu(int client,Map_Info map)
{
	Menu menu = CreateMenu(MapLowerLimitMenuHandler);
	char buffer[256];
	Format(buffer,sizeof(buffer),"%s:订图人数下限\n当前限制:%d%s",map.name,map.pllower,map.pllower==0?"(不限制)":"");
	menu.SetTitle(buffer);
	menu.AddItem(map.name,"+1人");
	menu.AddItem(map.name,"+5人");
	menu.AddItem(map.name,"+10人");
	menu.AddItem(map.name,"-1人");
	menu.AddItem(map.name,"-5人");
	menu.AddItem(map.name,"-10人");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int MapLowerLimitMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[64];
	Map_Info map;
	switch(action)
	{
		case MenuAction_End:
		{
			delete menu;
		}
		case MenuAction_Select:
		{
			menu.GetItem(param,map_name,sizeof(map_name));
			Maps.GetArray(map_name,map,sizeof(map));
			switch(param)
			{
				case 0:
				{
					map.pllower+=1;
					if(map.pllower>=64)	map.pllower=0;
				}
				case 1:
				{
					map.pllower+=5;
					if(map.pllower>=64)	map.pllower=0;
				}
				case 2:
				{
					map.pllower+=10;
					if(map.pllower>=64)	map.pllower=0;
				}
				case 3:
				{
					map.pllower-=1;
					if(map.pllower<=0)	map.pllower=0;
				}
				case 4:
				{
					map.pllower-=5;
					if(map.pllower<=0)	map.pllower=0;
				}
				case 5:
				{
					map.pllower-=10;
					if(map.pllower<=0)	map.pllower=0;
				}
			}
			MapCfgUpdate(map);
			MapLowerLimitMenu(client,map);
		}
		default:
		{
			if (param == MenuCancel_ExitBack)
			{
				menu.GetItem(0,map_name,sizeof(map_name));
				Maps.GetArray(map_name,map,sizeof(map));
				MapAdminConfigMenu(client,map);
			}
		}
	}
	return 0;
}
void MapCfgUpdate(Map_Info map)
{
	int hostip_int = FindConVar("hostip").IntValue;
	char hostip[32];
	IPNumToIPV4(hostip_int,hostip,sizeof(hostip));
	char path[PLATFORM_MAX_PATH];
	BuildPath(Path_SM,path,sizeof(path),"mapadm_log/%s.txt",hostip);
	File mapadm_update_output = OpenFile(path,"a",false);
	mapadm_update_output.WriteLine("%s",map.name);
	char query[2048];
	Format(query,sizeof(query),"UPDATE zemaps SET CN_NAME = '%s', COOLDOWN = %d, COST = %d, LAST_RUN_TIME = %d, ROUND = %d,AVAILABLE = %d,DOWNLOAD = %d,DIFFICULTY = %d, RANDOM = %d, EXTEND = %d, TIMELIMIT = %d, NOHMSKILL = %d, NOZMSKILL = %d, NOJK = %d, NOBHOPLIMIT = %d, WINS = %d, FATIGUE = %d, INFECTTIME = %f,EGO = %d,VIS = %d, TAG = %d, DMGSCALE = %f, TAGSCALE = %d, KNOCKBACK = %f, ZMHPSCALE = %f, MR = %d, MRX = %f, MRY = %f, MRZ = %f, PLUPPER = %d, PLLOWER = %d, ZMCLASS =%d, HMCLASS = %d, ZMHERO = %d, HMHERO = %d, ZMHPRECOVER = %f, TEMPCD = %d WHERE ID = %d and NAME = '%s'",map.name_cn,map.cooldown,map.cost,map.last_run_time,map.round,map.available,map.download,map.difficulty,map.random,map.extend,map.timelimit,map.nohmskill,map.nozmskill,map.nojk,map.nobhoplimit,map.wins,map.interval,map.infecttime,map.ego,map.vis,map.tag,map.dmgscale,map.tagscale,map.knockback,map.zmhpscale,map.mr,map.mrx,map.mry,map.mrz,map.plupper,map.pllower,map.zmclass,map.hmclass,map.zmhero,map.hmhero,map.zmhprecover,map.temp_cooldown?1:0,map.id,map.name);
	PrintToServer(query);
	DbMQuery(DbQueryErrorCallback,query);
	Map_Log mapl;
	Maps.SetArray(map.name,map,sizeof(map),true);
	for(int i = 0 ; i < Map_List.Length ; i++)
	{
		GetArrayArray(Map_List,i,mapl,sizeof(mapl));
		if(strcmp(mapl.name,map.name)==0)
		{
			mapl.id = map.id;
			mapl.name = map.name;
			mapl.name_cn = map.name_cn;
			Map_List.SetArray(i,map,sizeof(map));
			break;
		}
	}
	delete mapadm_update_output;
}
Action MapNamecnCommand(int client,int args)
{
	if (args<2)		return Plugin_Handled;
	Map_Info map;
	GetCmdArg(1,map.name,sizeof(map.name));
	if(!Maps.GetArray(map.name,map,sizeof(map)))	return Plugin_Handled;
	GetCmdArg(2, map.name_cn, sizeof(map.name_cn));
	MapCfgUpdate(map);
	PrintToChatAll(" \x05[地图管理] 管理员修改%s的译名为%s",map.name,map.name_cn);
	return Plugin_Handled;
}

void MapCooldownMenu(int client,Map_Info map)
{
	char title[256];
	Menu menu = CreateMenu(MapCooldownCfgHandler);
	Format(title,sizeof(title),"地图冷却时间修改:\n%s\n当前冷却:%d分钟",map.name,map.cooldown);
	menu.SetTitle(title);
	menu.AddItem(map.name,"-10分钟",map.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-1小时",map.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-6小时",map.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-1天",map.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-7天",map.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+10分钟",map.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+1小时",map.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+6小时",map.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+1天",map.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+7天",map.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"自定义");
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);

}

int MapCooldownCfgHandler(Menu menu, MenuAction action, int client, int param)
{
	char buffer[256];
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select) {
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if (param == 0) map.cooldown = Max(0, map.cooldown-10);
		else if (param == 1) map.cooldown = Max(0, map.cooldown-60);
		else if (param == 2) map.cooldown = Max(0, map.cooldown-360);
		else if (param == 3) map.cooldown = Max(0, map.cooldown-1440);
		else if (param == 4) map.cooldown = Max(0, map.cooldown-10080);
		else if (param == 5) map.cooldown = Min(20160, map.cooldown+10);
		else if (param == 6) map.cooldown = Min(20160, map.cooldown+60);
		else if (param == 7) map.cooldown = Min(20160, map.cooldown+360);
		else if (param == 8) map.cooldown = Min(20160, map.cooldown+1440);
		else if (param == 9) map.cooldown = Min(20160, map.cooldown+10080);
		else if (param == 10)
		{
			PrintToChat(client,"更改时间参数已发送至控制台");
			Format(buffer,sizeof(buffer),"sm_mapcd_update \"%s\" %d",map.name,map.cooldown);
			PrintToConsole(client,buffer);
			return 0;
		}
		int current_time = GetTime();
		USER_LOG userinfo_admin;
		EXGUSERS_GetUserInfo(client,userinfo_admin);
		ADMIN_LOG admlog_add;
		admlog_add.timestamp = current_time;
		admlog_add.uid = userinfo_admin.uid;
		strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
		admlog_add.type=1;
		admlog_add.value=map.cooldown;
		admlog_add.valuestr="";
		admlog_add.target=0;
		admlog_add.targetstr=map.name;
		EXGUSERS_AddAdminLog(admlog_add);
		MapCfgUpdate(map);
		MapCooldownMenu(client,map);
		return 0;
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
		return 0;
	}
}

Action MapCooldownCommand(int client,int args)
{
	if (args<2)		return Plugin_Handled;
	Map_Info map;
	int n_cooldown;
	char buffer[64];
	GetCmdArg(1,map.name,sizeof(map.name));
	if(!Maps.GetArray(map.name,map,sizeof(map)))	return Plugin_Handled;
	GetCmdArg(2, buffer, sizeof(buffer));
	n_cooldown = StringToInt(buffer);
	if(n_cooldown<0||n_cooldown>20160)
	{
		PrintToChat(client,"参数不合法，请输入0~20160间的数字");
		return Plugin_Handled;
	}
	map.cooldown = n_cooldown;
	int current_time = GetTime();
	USER_LOG userinfo_admin;
	EXGUSERS_GetUserInfo(client,userinfo_admin);
	ADMIN_LOG admlog_add;
	admlog_add.timestamp = current_time;
	admlog_add.uid = userinfo_admin.uid;
	strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
	admlog_add.type=1;
	admlog_add.value=map.cooldown;
	admlog_add.valuestr="";
	admlog_add.target=0;
	admlog_add.targetstr=map.name;
	EXGUSERS_AddAdminLog(admlog_add);
	MapCfgUpdate(map);
	return Plugin_Handled;
}

void MapCostMenu(int client,Map_Info map)
{
	char title[256];
	Menu menu = CreateMenu(MapCostCfgHandler);
	Format(title,sizeof(title),"地图订价修改:\n%s\n当前订价:%d积分",map.name,map.cost);
	menu.SetTitle(title);
	menu.AddItem(map.name,"-10积分",map.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-50积分",map.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-200积分",map.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-1000积分",map.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-5000积分",map.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+10积分",map.cost<15000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+50积分",map.cost<15000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+200积分",map.cost<15000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+1000积分",map.cost<15000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+5000积分",map.cost<15000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"自定义");
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapCostCfgHandler(Menu menu, MenuAction action, int client, int param)
{
	char buffer[256];
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select) {
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if (param == 0) map.cost = Max(0, map.cost-10);
		else if (param == 1) map.cost = Max(0, map.cost-50);
		else if (param == 2) map.cost = Max(0, map.cost-200);
		else if (param == 3) map.cost = Max(0, map.cost-1000);
		else if (param == 4) map.cost = Max(0, map.cost-5000);
		else if (param == 5) map.cost = Min(15000, map.cost+10);
		else if (param == 6) map.cost = Min(15000, map.cost+50);
		else if (param == 7) map.cost = Min(15000, map.cost+200);
		else if (param == 8) map.cost = Min(15000, map.cost+1000);
		else if (param == 9) map.cost = Min(15000, map.cost+5000);
		else if (param == 10)
		{
			PrintToChat(client,"更改订价参数已发送至控制台");
			Format(buffer,sizeof(buffer),"sm_mapcost_update \"%s\" %d",map.name,map.cost);
			PrintToConsole(client,buffer);
		}
		int current_time = GetTime();
		USER_LOG userinfo_admin;
		EXGUSERS_GetUserInfo(client,userinfo_admin);
		ADMIN_LOG admlog_add;
		admlog_add.timestamp = current_time;
		admlog_add.uid = userinfo_admin.uid;
		strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
		admlog_add.type=2;
		admlog_add.value=map.cost;
		admlog_add.valuestr="";
		admlog_add.target=0;
		admlog_add.targetstr=map.name;
		EXGUSERS_AddAdminLog(admlog_add);
		MapCfgUpdate(map);
		MapCostMenu(client,map);
		return 0;
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
		return 0;
	}
}

Action MapCostCommand(int client,int args)
{
	if (args<2)		return Plugin_Handled;
	Map_Info map;
	int n_cost;
	char buffer[64];
	GetCmdArg(1,map.name,sizeof(map.name));
	if(!Maps.GetArray(map.name,map,sizeof(map)))	return Plugin_Handled;
	GetCmdArg(2, buffer, sizeof(buffer));
	n_cost = StringToInt(buffer);
	if(n_cost<0||n_cost>15000)
	{
		PrintToChat(client,"参数不合法，请输入0~15000间的数字");
		return Plugin_Handled;
	}
	map.cost = n_cost;
	int current_time = GetTime();
	USER_LOG userinfo_admin;
	EXGUSERS_GetUserInfo(client,userinfo_admin);
	ADMIN_LOG admlog_add;
	admlog_add.timestamp = current_time;
	admlog_add.uid = userinfo_admin.uid;
	strcopy(admlog_add.name,sizeof(admlog_add.name),userinfo_admin.name);
	admlog_add.type=2;
	admlog_add.value=map.cost;
	admlog_add.valuestr="";
	admlog_add.target=0;
	admlog_add.targetstr=map.name;
	EXGUSERS_AddAdminLog(admlog_add);
	MapCfgUpdate(map);
	return Plugin_Handled;
}

void MapTimeLimitMenu(int client,Map_Info map)
{
	char title[256];
	Menu menu = CreateMenu(MapTimeLimitMenuHandler);
	Format(title,sizeof(title),"地图时长修改:\n%s\n当前时长:%d分钟",map.name,map.timelimit);
	menu.SetTitle(title);
	menu.AddItem(map.name,"+5分钟",map.timelimit<120?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+10分钟",map.timelimit<120?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+30分钟",map.timelimit<120?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+60分钟",map.timelimit<120?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-5分钟",map.timelimit<120?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-10分钟",map.timelimit>10?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-30分钟",map.timelimit>10?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-60分钟",map.timelimit>10?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapTimeLimitMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select) {
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if (param == 0) map.timelimit = Min(120, map.timelimit+5);
		else if (param == 1) map.timelimit = Min(120, map.timelimit+10);
		else if (param == 2) map.timelimit = Min(120, map.timelimit+30);
		else if (param == 3) map.timelimit = Min(120, map.timelimit+60);
		else if (param == 4) map.timelimit = Max(10, map.timelimit-5);
		else if (param == 5) map.timelimit = Max(10, map.timelimit-10);
		else if (param == 6) map.timelimit = Max(10, map.timelimit-30);
		else if (param == 7) map.timelimit = Max(10, map.timelimit-60);
		MapCfgUpdate(map);
		MapTimeLimitMenu(client,map);
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
		return 0;
	}
}

void MapInfectTimeMenu(int client,Map_Info map)
{
	char title[256];
	Menu menu = CreateMenu(MapInfectTimeHandler);
	Format(title,sizeof(title),"地图尸变时间修改:\n%s\n当前尸变时间:%f秒",map.name,map.infecttime);
	menu.SetTitle(title);
	menu.AddItem(map.name,"+0.1秒",map.infecttime<60?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+0.5秒",map.infecttime<60?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+1秒",map.infecttime<60?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+5秒",map.infecttime<60?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"+10秒",map.infecttime<60?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-0.1秒",map.infecttime>1?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-0.5秒",map.infecttime>1?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-1秒",map.infecttime>1?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-5秒",map.infecttime>1?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"-10秒",map.infecttime>1?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem(map.name,"无限(跑图设置)");
	menu.AddItem(map.name,"默认(CFG设置)");
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapInfectTimeHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if (action == MenuAction_End)
	{
		delete menu;
	}
	else if (action == MenuAction_Select) {
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if (param == 0) map.infecttime = fMin(60.0, map.infecttime+0.1);
		else if (param == 1) map.infecttime = fMin(60.0, map.infecttime+0.5);
		else if (param == 2) map.infecttime = fMin(60.0, map.infecttime+1.0);
		else if (param == 3) map.infecttime = fMin(60.0, map.infecttime+5.0);
		else if (param == 4) map.infecttime = fMin(60.0, map.infecttime+10.0);
		else if (param == 5) map.infecttime = fMax(1.0, map.infecttime-0.1);
		else if (param == 6) map.infecttime = fMax(1.0, map.infecttime-0.5);
		else if (param == 7) map.infecttime = fMax(1.0, map.infecttime-1.0);
		else if (param == 8) map.infecttime = fMax(1.0, map.infecttime-5.0);
		else if (param == 9) map.infecttime = fMax(1.0, map.infecttime-10.0);
		else if (param == 10) map.infecttime = 99999.0;
		else if (param == 11) map.infecttime = -1.0;
		MapCfgUpdate(map);
		MapInfectTimeMenu(client,map);
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
		return 0;
	}
}

void MapTagConfigMenu(int client,Map_Info map)
{
	char buffer[64];
	Menu menu = CreateMenu(MapTagConfigMenuHandler);
	for(int i=0;i<tag_num;i++)
	{
		if(map.tag&label_code[i])
		{
			Format(buffer,sizeof(buffer),"[%s]",label_name[i]);
			menu.AddItem(map.name,buffer);
		}
		else
		{
			menu.AddItem(map.name,label_name[i]);
		}
	}
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int MapTagConfigMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if( action == MenuAction_End )
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if(map.tag&label_code[param])
		{
			map.tag-=label_code[param];
		}
		else
		{
			map.tag+=label_code[param];
		}
		MapCfgUpdate(map);
		MapTagConfigMenu(client,map);
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
		return 0;
	}
}

void MapDmgscaleConfigMenu(int client,Map_Info map)
{
	char buffer[256];
	Menu menu = CreateMenu(MapDmgScaleConfigMenuHandler);
	Format(buffer,sizeof(buffer),"%s\n伤害系数:%.2f",map.name,map.dmgscale);
	menu.SetTitle(buffer);
	menu.AddItem(map.name,"+0.1");
	menu.AddItem(map.name,"+0.5");
	menu.AddItem(map.name,"+1.0");
	menu.AddItem(map.name,"-0.1");
	menu.AddItem(map.name,"-0.5");
	menu.AddItem(map.name,"-1.0");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int MapDmgScaleConfigMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if( action == MenuAction_End )
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if(param==0)	map.dmgscale = fMin(3.0,map.dmgscale+0.1);
		if(param==1)	map.dmgscale = fMin(3.0,map.dmgscale+0.5);
		if(param==2)	map.dmgscale = fMin(3.0,map.dmgscale+1.0);
		if(param==3)	map.dmgscale = fMax(0.1,map.dmgscale-0.1);
		if(param==4)	map.dmgscale = fMax(0.1,map.dmgscale-0.5);
		if(param==5)	map.dmgscale = fMax(0.1,map.dmgscale-1.0);
		MapCfgUpdate(map);
		MapDmgscaleConfigMenu(client,map);
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
	}
}

void MapTagscaleConfigMenu(int client,Map_Info map)
{
	char buffer[256];
	Menu menu = CreateMenu(MapTagscaleConfigMenuHandler);
	Format(buffer,sizeof(buffer),"%s\n定身系数(数值越高定身越高):%d",map.name,map.tagscale);
	menu.SetTitle(buffer);
	menu.AddItem(map.name,"+1");
	menu.AddItem(map.name,"+5");
	menu.AddItem(map.name,"+10");
	menu.AddItem(map.name,"-1");
	menu.AddItem(map.name,"-5");
	menu.AddItem(map.name,"-10");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int MapTagscaleConfigMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if( action == MenuAction_End )
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if(param==0)	map.tagscale = Min(100,map.tagscale+1);
		if(param==1)	map.tagscale = Min(100,map.tagscale+5);
		if(param==2)	map.tagscale = Min(100,map.tagscale+10);
		if(param==3)	map.tagscale = Max(1,map.tagscale-1);
		if(param==4)	map.tagscale = Max(1,map.tagscale-5);
		if(param==5)	map.tagscale = Max(1,map.tagscale-10);
		MapCfgUpdate(map);
		MapTagscaleConfigMenu(client,map);
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
	}
}

void MapKnockbackConfigMenu(int client,Map_Info map)
{
	char buffer[256];
	Menu menu = CreateMenu(MapKnockbackConfigMenuHandler);
	Format(buffer,sizeof(buffer),"%s\n击退系数:%.2f",map.name,map.knockback);
	menu.SetTitle(buffer);
	menu.AddItem(map.name,"+0.1");
	menu.AddItem(map.name,"+0.5");
	menu.AddItem(map.name,"+1.0");
	menu.AddItem(map.name,"-0.1");
	menu.AddItem(map.name,"-0.5");
	menu.AddItem(map.name,"-1.0");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int MapKnockbackConfigMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if( action == MenuAction_End )
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if(param==0)	map.knockback = fMin(3.0,map.knockback+0.1);
		if(param==1)	map.knockback = fMin(3.0,map.knockback+0.5);
		if(param==2)	map.knockback = fMin(3.0,map.knockback+1.0);
		if(param==3)	map.knockback = fMax(0.1,map.knockback-0.1);
		if(param==4)	map.knockback = fMax(0.1,map.knockback-0.5);
		if(param==5)	map.knockback = fMax(0.1,map.knockback-1.0);
		MapCfgUpdate(map);
		MapKnockbackConfigMenu(client,map);
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
	}
}

void MapZMHpScaleConfigMenu(int client,Map_Info map)
{
	char buffer[256];
	Menu menu = CreateMenu(MapZMHpScaleConfigMenuHandler);
	Format(buffer,sizeof(buffer),"%s\n血量系数(1.0-母体3W血):%.2f",map.name,map.zmhpscale);
	menu.SetTitle(buffer);
	menu.AddItem(map.name,"+0.01");
	menu.AddItem(map.name,"+0.1");
	menu.AddItem(map.name,"+0.5");
	menu.AddItem(map.name,"-0.01");
	menu.AddItem(map.name,"-0.1");
	menu.AddItem(map.name,"-0.5");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int MapZMHpScaleConfigMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char map_name[PLATFORM_MAX_PATH];
	Map_Info map;
	if( action == MenuAction_End )
	{
		delete menu;
	}
	else if (action == MenuAction_Select)
	{
		menu.GetItem(param,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		if(param==0)	map.zmhpscale = fMin(2.0,map.zmhpscale+0.01);
		if(param==1)	map.zmhpscale = fMin(2.0,map.zmhpscale+0.1);
		if(param==2)	map.zmhpscale = fMin(2.0,map.zmhpscale+0.5);
		if(param==3)	map.zmhpscale = fMax(0.1,map.zmhpscale-0.01);
		if(param==4)	map.zmhpscale = fMax(0.1,map.zmhpscale-0.1);
		if(param==5)	map.zmhpscale = fMax(0.1,map.zmhpscale-0.5);
		MapCfgUpdate(map);
		MapZMHpScaleConfigMenu(client,map);
	}
	else if (param == MenuCancel_ExitBack)
	{
		menu.GetItem(0,map_name,sizeof(map_name));
		Maps.GetArray(map_name,map,sizeof(map));
		MapAdminConfigMenu(client,map);
	}
}

char[] MapAdmGetMapTag(Map_Info map)
{
	char buffer[64];
	for(int i=0; i<tag_num ;i++)
	{
		if (map.tag & label_code[i])
		{
			Format(buffer, sizeof(buffer),"%s[%s]",buffer, label_name[i]);
		}
		else
		{
			Format(buffer, sizeof(buffer),"[%s]", label_name[i]);
		}
	}
	return buffer;
}