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
}
enum struct Map_Log
{
	int id;
	char name[PLATFORM_MAX_PATH];
}
int Map_Id_Max;
StringMap Maps;
ArrayList Map_List;
void MapAdmOnPluginStart() 
{
	Maps = CreateTrie();
	Map_List = CreateArray(sizeof(Map_Log));
	RegAdminCmd("sm_mdr",MapDataReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapdatareload",MapDataReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mfr",MapFileReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapfilereload",MapFileReloadCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_ma",MapAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_mapadmin",MapAdminCommand,ADMFLAG_GENERIC);
}
void MapAdmOnDbConnected()
{
	Maps.Clear();
	Maps.ClearTrie();
	MapList.ClearArray();
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
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE)	SetFailState("数据库错误: %s", error);
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
		map.exist = 0;
		Maps.SetArray(map.name, map, sizeof(map), true);
		Map_List.Push(mapl);
		if(map.id>Map_Id_Max)	Map_Id_Max = map.id;
		//PrintToServer("%d %s",map.id,map.name);
	}	
	MapFileReload();
}
void MapFileReload()
{
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
			//PrintToServer("New Map:%d %s",map.id,map.name);
			NewMapFileUpdate(map);
		}
		map.exist = 1;	
	}
	ResetFileExist();
}

void NewMapFileUpdate(Map_Info map)
{
	char query[256];
	Map_Log mapl;
	mapl.id = map.id;
	Format(mapl.name,sizeof(mapl.name),"%s",map.name);
	Map_List.Push(mapl);
	Format(query,sizeof(query),"INSERT INTO ZEMAPS (ID,NAME) VALUES(%d,'%s')",map.id,map.name);
	DbTQuery(DbQueryErrorCallback,query);
}

void ResetFileExist()
{
	StringMapSnapShot snap= Maps.snapshot();
	Map_Info map;
	for (int i =0;i < snap.Length ; i++)
	{
		snap.GetKey(i, map.name, sizeof(map.Name));
		Maps.GetArray(map.name, map, sizeof(map));	
		Format(query,sizeof(query),"UPDATE ZEMAPS SET EXIST = %d where ID = %d",map.exist,map.id);
		DbTQuery(DbQueryErrorCallback,query);		
	}
	snap.close();
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

void MapAdminMenu(int client,char trie_search[PLATFORM_MAX_PATH]="")
{
	Menu menu = CreateMenu(MapAdminMenuHandler);
	Map_Log map;
	menu.SetTitle("地图管理菜单");
	for(int i = 0 ; i < snap.Length ; i++)
	{
		snap.GetKey(i,map.name,sizeof(map.name));
		Maps.GetArray(map.name,map,sizeof(map));
		if(strlen(trie_search) && StrContains(map.name,trie_search) && StrContains(map.name_cn,trie_search))	continue;
		menu.AddItem(map.name,map.name);
	}
	snap.Close();
	if(strlen(trie_search))
	{
		if(menu.ItemCount == 0)
		{
			menu.Close();
			PrintToChat(client,"\x04地图管理：找不到匹配地图");
			return;
		}
		if(menu.ItemCount == 1)
		{
			menu.GetItem(0, map.name, sizeof(map.name));
			menu.Close();
			MapAdminConfigMenu(client,map.name);
		}
	}
	else
	{
		menu.Display(client, MENU_TIME_FOREVER);
	}
}

int MapAdminMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{                             
		char buffer[PLATFORM_MAX_PATH];                                            
		menu.GetItem(param,buffer,sizeof(buffer));
		PrintToChatAll(buffer);
		MapAdminConfigMenu(client,buffer);
	}
}

void MapAdminConfigMenu(int client,char map_name[PLATFORM_MAX_PATH])
{
	Map_Info map;
	Maps.GetArray(map_name,map,sizeof(map));
	Menu menu = CreateMenu(MapAdminCfgHandler);
	char buffer[PLATFORM_MAX_PATH];
	Format(buffer,sizeof(buffer),"地图管理:\n地图名:%s",map_name);
	menu.SetTitle(buffer);
	Format(buffer,sizeof(buffer),"地图译名:%s",map.name_cn);
	menu.AddItem("1",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"地图CD(分钟):%d",map.cooldown);
	menu.AddItem("2",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"地图订价(积分):%d",map.cost);
	menu.AddItem("3",buffer,ITEMDRAW_DISABLED);
	menu.Display(client, MENU_TIME_FOREVER);
}
int MapAdminCfgHandler(Menu menu, MenuAction action, int client, int param) 
{
	
}