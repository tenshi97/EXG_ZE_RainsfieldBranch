enum struct Map_Info
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
	int infecttime;
	int random;
	int extend;
	bool temp_cooldown;
}
char difficulty_name[5][10]={"简单","普通","困难","高难","极难"};
enum struct Map_Log
{
	int id;
	char name[PLATFORM_MAX_PATH];
	char name_cn[PLATFORM_MAX_PATH];
}
int Map_Id_Max;
StringMap Maps;
ArrayList Map_List;
Map_Info Cmap;
bool g_Map_Loaded;
bool g_Admin_Editing;
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
	PrintToServer("[MapAdm MapEnd]");
	g_Map_Loaded = false;
	g_Admin_Editing = false;
}
void MapAdmOnPluginStart() 
{
	g_Admin_Editing = false;
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
}
void MapAdmOnDbConnected_MapStartPost()
{
	Maps.Clear();
	Map_List.Clear();
	MapDataReload();
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
	char query[256];
	Format(query,sizeof(query),"SELECT * FROM ZEMAPS ORDER BY name ASC");
	DbTQuery(MapDataLoadCallback,query);
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
	char buffer[256];
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
		map.infecttime = DbFetchInt(hndl, "INFECTTIME");	
		map.random = DbFetchInt(hndl, "RANDOM");			
		map.exist = 0;
		map.extend = DbFetchInt(hndl, "EXTEND");	
		Maps.SetArray(map.name, map, sizeof(map), true);
		Format(buffer,sizeof(buffer),"[MapDataLoad]Added Map List:%s",mapl.name);
		PrintToServer(buffer);
		Map_List.PushArray(mapl,sizeof(mapl));
		if(map.id>Map_Id_Max)	Map_Id_Max = map.id;
		map.temp_cooldown = false;
		//PrintToServer("%d %s",map.id,map.name);
	}	
	MapFileReload();
}
void MapFileReload()
{
	PrintToServer("[test]2 MFL");
	DirectoryListing dl = OpenDirectory("maps", false);
	if (!dl) SetFailState("Map File Load Failed");
	char file_name[PLATFORM_MAX_PATH];
	FileType type;
	int index;
	char buffer[256];
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
			Format(buffer,sizeof(buffer),"[MapFileLoad]Added Map List:%s",mapl.name);
			PrintToServer(buffer);
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
	MapInfoOnDbConnected_MapStartPost();
}

void NewMapFileUpdate(Map_Info map)
{
	char query[256];
	Format(query,sizeof(query),"INSERT INTO ZEMAPS (ID,NAME) VALUES(%d,'%s')",map.id,map.name);
	DbTQuery(DbQueryErrorCallback,query);
}

void ResetFileExist()
{
	PrintToServer("[test]3 MFE");
	StringMapSnapshot snap= Maps.Snapshot();
	Map_Info map;
	char query[256];
	for (int i =0;i < snap.Length ; i++)
	{
		snap.GetKey(i, map.name, sizeof(map.name));
		Maps.GetArray(map.name, map, sizeof(map));

		Format(query,sizeof(query),"UPDATE ZEMAPS SET EXIST = %d where ID = %d",map.exist,map.id);
		DbTQuery(DbQueryErrorCallback,query);		
	}
	snap.Close();
}

Action MapAdminCommand(int client, int args) {
	if (!IsClientInGame(client)) return Plugin_Handled;
	if (!args) {
		MapAdminMenu(client);
		return Plugin_Handled;
	}
	if(!g_Admin_Editing)
	{
		PrintToServer("[地图管理:另一位管理正在编辑\n若遇到特殊情况可以使用!resetma]");
	}
	char arg[PLATFORM_MAX_PATH];
	GetCmdArg(1, arg, sizeof(arg));
	MapAdminMenu(client,arg);
	return Plugin_Handled;
}

Action MapAdminResetCommand(int client, int args) {
	if (!IsClientInGame(client)) return Plugin_Handled;
	g_Admin_Editing = false;
	return Plugin_Handled;
}

void MapAdminMenu(int client,char trie_search[PLATFORM_MAX_PATH]="")
{
	g_Admin_Editing = false;
	Menu menu = CreateMenu(MapAdminMenuHandler);
	Map_Log map;
	menu.SetTitle("地图管理菜单");
	for(int i = 0 ; i < Map_List.Length ; i++)
	{
		GetArrayArray(Map_List,i,map,sizeof(map));
		if(strlen(trie_search) && StrContains(map.name,trie_search)==-1 && StrContains(map.name_cn,trie_search) == -1)	continue;
		menu.AddItem(map.name,map.name);
	}
	if(strlen(trie_search))
	{
		if(menu.ItemCount == 0)
		{
			g_Admin_Editing = false;
			menu.Close();
			PrintToChat(client,"\x05地图管理：找不到匹配地图");
			return;
		}
		if(menu.ItemCount == 1)
		{
			menu.GetItem(0, map.name, sizeof(map.name));
			menu.Close();
			Maps.GetArray(map.name,Cmap,sizeof(Cmap));
			g_Admin_Editing =true;
			MapAdminConfigMenu(client);
		}
		if(menu.ItemCount > 1)
		{
			g_Admin_Editing = true;
			menu.Display(client, MENU_TIME_FOREVER);			
		}
	}
	else
	{
		g_Admin_Editing = true;
		menu.Display(client, MENU_TIME_FOREVER);
	}
}

int MapAdminMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{                             
		char buffer[PLATFORM_MAX_PATH];                                            
		menu.GetItem(param,buffer,sizeof(buffer));
		Maps.GetArray(buffer,Cmap,sizeof(Cmap));
		MapAdminConfigMenu(client);
	}
}

void MapAdminConfigMenu(int client)
{
	Menu menu = CreateMenu(MapAdminCfgHandler);
	char buffer[PLATFORM_MAX_PATH];
	char ctime[64];
	FormatTime(ctime, 64, NULL_STRING,Cmap.last_run_time);
	Format(buffer,sizeof(buffer),"地图管理:\n地图名:%s\n上次运行时间%s",Cmap.name,ctime);
	menu.SetTitle(buffer);
	Format(buffer,sizeof(buffer),"地图译名:%s",Cmap.name_cn);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"地图CD(分钟):%d",Cmap.cooldown);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"地图订价(积分):%d",Cmap.cost);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"难度:%s",difficulty_name[Cmap.difficulty]);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"开放订阅:%s",Cmap.available?"是":"否");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"已添加下载:%s",Cmap.download?"是":"否");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"能否野生出现:%s",Cmap.random?"是":"否");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"延长次数:%d",Cmap.extend);
	menu.AddItem("",buffer);
	menu.Display(client, MENU_TIME_FOREVER);
}
int MapAdminCfgHandler(Menu menu, MenuAction action, int client, int param) 
{
	char buffer[PLATFORM_MAX_PATH];
	if (action == MenuAction_End)
	{
		g_Admin_Editing = false;
		menu.Close();
	}
	else if (action == MenuAction_Select)
	{                             
		if(param == 0)
		{
			PrintToChat(client,"译名修改参数已发送至控制台");
			Format(buffer,sizeof(buffer),"sm_mapnamecn_update \"%s\" \"%s\"",Cmap.name,Cmap.name_cn);
			PrintToConsole(client,buffer);
		}	
		if(param == 1)
		{
			MapCooldownMenu(client);
		}
		if(param == 2)
		{
			MapCostMenu(client);
		}
		if(param == 3)
		{
			Cmap.difficulty--;
			if(Cmap.difficulty<0)
			{
				Cmap.difficulty = 4;
			}
			MapCfgUpdate(Cmap);
			MapAdminConfigMenu(client);
		}
		if(param == 4)
		{
			if(Cmap.available == 1)	Cmap.available =0;
			else Cmap.available = 1;
			MapCfgUpdate(Cmap);
			MapAdminConfigMenu(client);
		}	
		if(param == 5)
		{
			if(Cmap.download == 1)	Cmap.download =0;
			else Cmap.download = 1;
			MapCfgUpdate(Cmap);
			MapAdminConfigMenu(client);
		}	
		if(param == 6)
		{
			if(Cmap.random == 1)	Cmap.random =0;
			else Cmap.random =1;
			MapCfgUpdate(Cmap);
			MapAdminConfigMenu(client);
		}
		if(param == 7)
		{
			Cmap.extend--;
			if(Cmap.extend<0)
			{
				Cmap.extend = 5;
			}
			MapCfgUpdate(Cmap);
			MapAdminConfigMenu(client);
		}
	}	
}
void MapCfgUpdate(Map_Info map)
{
	char query[256];
	Format(query,sizeof(query),"UPDATE ZEMAPS SET CN_NAME = '%s', COOLDOWN = %d, COST = %d, LAST_RUN_TIME = %d, ROUND = %d,AVAILABLE = %d,DOWNLOAD = %d,DIFFICULTY = %d, RANDOM = %d, EXTEND = %d WHERE ID = %d and NAME = '%s'",map.name_cn,map.cooldown,map.cost,map.last_run_time,map.round,map.available,map.download,map.difficulty,map.random,map.extend,map.id,map.name);
	PrintToServer(query);
	DbTQuery(DbQueryErrorCallback,query);	
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
}

Action MapNamecnCommand(int client,int args)
{
	if (args<2)		return Plugin_Handled;
	Map_Info map;
	GetCmdArg(1,map.name,sizeof(map.name));
	if(!Maps.GetArray(map.name,map,sizeof(map)))	return Plugin_Handled;
	GetCmdArg(2, map.name_cn, sizeof(map.name_cn));
	MapCfgUpdate(map);
	return Plugin_Handled;
}

void MapCooldownMenu(int client)
{
	char title[256];
	Menu menu = CreateMenu(MapCooldownCfgHandler);
	Format(title,sizeof(title),"地图冷却时间修改:\n%s\n当前冷却:%d分钟",Cmap.name,Cmap.cooldown);
	menu.SetTitle(title);
	menu.AddItem("","-10分钟",Cmap.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-1小时",Cmap.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-6小时",Cmap.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-1天",Cmap.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-7天",Cmap.cooldown>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+10分钟",Cmap.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+1小时",Cmap.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+6小时",Cmap.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+1天",Cmap.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+7天",Cmap.cooldown<20160?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","自定义");
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapCooldownCfgHandler(Menu menu, MenuAction action, int client, int param) 
{
	char buffer[256];
	if (action == MenuAction_End)
	{
		g_Admin_Editing = false;
		menu.Close();
	}
	else if (param == MenuCancel_ExitBack) MapAdminConfigMenu(client);
	else if (action == MenuAction_Select) {
		if (param == 0) Cmap.cooldown = Max(0, Cmap.cooldown-10);
		else if (param == 1) Cmap.cooldown = Max(0, Cmap.cooldown-60);
		else if (param == 2) Cmap.cooldown = Max(0, Cmap.cooldown-360);
		else if (param == 3) Cmap.cooldown = Max(0, Cmap.cooldown-1440);
		else if (param == 4) Cmap.cooldown = Max(0, Cmap.cooldown-10080);
		else if (param == 5) Cmap.cooldown = Min(20160, Cmap.cooldown+10);
		else if (param == 6) Cmap.cooldown = Min(20160, Cmap.cooldown+60);
		else if (param == 7) Cmap.cooldown = Min(20160, Cmap.cooldown+360);
		else if (param == 8) Cmap.cooldown = Min(20160, Cmap.cooldown+1440);
		else if (param == 9) Cmap.cooldown = Min(20160, Cmap.cooldown+10080);
		else if (param == 10)
		{
			PrintToChat(client,"更改时间参数已发送至控制台");
			Format(buffer,sizeof(buffer),"sm_mapcd_update \"%s\" %d",Cmap.name,Cmap.cooldown);
			PrintToServer(buffer);
		}
		MapCfgUpdate(Cmap);
		MapCooldownMenu(client);
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
	MapCfgUpdate(map);
	return Plugin_Handled;
}

void MapCostMenu(int client)
{
	char title[256];
	Menu menu = CreateMenu(MapCostCfgHandler);
	Format(title,sizeof(title),"地图订价修改:\n%s\n当前订价:%d积分",Cmap.name,Cmap.cost);
	menu.SetTitle(title);
	menu.AddItem("","-10积分",Cmap.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-50积分",Cmap.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-200积分",Cmap.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-1000积分",Cmap.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","-5000积分",Cmap.cost>0?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+10积分",Cmap.cost<10000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+50积分",Cmap.cost<10000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+200积分",Cmap.cost<10000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+1000积分",Cmap.cost<10000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","+5000积分",Cmap.cost<10000?ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED);
	menu.AddItem("","自定义");
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapCostCfgHandler(Menu menu, MenuAction action, int client, int param) 
{
	char buffer[256];
	if (action == MenuAction_End)
	{
		g_Admin_Editing = false;
		menu.Close();
	}
	else if (param == MenuCancel_ExitBack) MapAdminConfigMenu(client);
	else if (action == MenuAction_Select) {
		if (param == 0) Cmap.cost = Max(0, Cmap.cost-10);
		else if (param == 1) Cmap.cost = Max(0, Cmap.cost-50);
		else if (param == 2) Cmap.cost = Max(0, Cmap.cost-200);
		else if (param == 3) Cmap.cost = Max(0, Cmap.cost-1000);
		else if (param == 4) Cmap.cost = Max(0, Cmap.cost-5000);
		else if (param == 5) Cmap.cost = Min(10000, Cmap.cost+10);
		else if (param == 6) Cmap.cost = Min(10000, Cmap.cost+50);
		else if (param == 7) Cmap.cost = Min(10000, Cmap.cost+200);
		else if (param == 8) Cmap.cost = Min(10000, Cmap.cost+1000);
		else if (param == 9) Cmap.cost = Min(10000, Cmap.cost+5000);
		else if (param == 10)
		{
			PrintToChat(client,"更改订价参数已发送至控制台");
			Format(buffer,sizeof(buffer),"sm_mapcost_update \"%s\" %d",Cmap.name,Cmap.cost);
			PrintToServer(buffer);
		}
		MapCfgUpdate(Cmap);
		MapCostMenu(client);
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
	if(n_cost<0||n_cost>10000)
	{
		PrintToChat(client,"参数不合法，请输入0~10000间的数字");
		return Plugin_Handled;
	}
	map.cost = n_cost;
	MapCfgUpdate(map);
	return Plugin_Handled;
}