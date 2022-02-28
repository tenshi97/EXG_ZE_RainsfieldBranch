Map_Info MapInfo;
char difficulty_name[5][10]={"简单","普通","困难","高难","极难"};
bool Current_Map_Loaded = false;
void MapInfoOnPluginStart(){
	RegConsoleCmd("sm_mi", ActionMapInfoMenu);
	RegConsoleCmd("sm_mapinfo", ActionMapInfoMenu);
	Current_Map_Loaded = false;
}
void MapInfoOnMapStart(){
	Current_Map_Loaded = false;
}
void MapInfoOnDbConnected(){
	char map_name[64];
	char query[256];
	GetCurrentMap(map_name,sizeof(map_name));
	Format(query,sizeof(query),"SELECT * FROM ZEMAPS WHERE NAME='%s'",map_name);
	DbTQuery(CurrentMapInfoCallback,query);
	Current_Map_Loaded = true;
}

Action ActionMapInfoMenu(int client,int args)
{
	if(!Current_Map_Loaded)
	{
		PrintCenterText(client,"[地图信息读取失败:尚未加载]");
		return Plugin_Handled;
	}
	PrintToChatAll("test");
	MakeMapInfoMenu(client);
	return Plugin_Handled;
}

void MakeMapInfoMenu(int client){
	char title[256];
	Format(title,sizeof(title),"当前地图:%s\n\
		地图译名:%s\n\
		地图难度:%s\n\
		地图胜负:%d胜%d负\n\
		地图热度:%d\n\
		地图订价:%d",MapInfo.name,MapInfo.name_cn,difficulty_name[MapInfo.difficulty],MapInfo.wins,MapInfo.round-MapInfo.wins,MapInfo.heat,MapInfo.cost);
	Menu menu = CreateMenu(MapInfoMenuHandler);
	menu.SetTitle(title);
	menu.AddItem("","关闭");
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapInfoMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)	menu.Close();
}

void CurrentMapInfoCallback(Handle owner, Handle hndl, char[] error, any data) {
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE)	SetFailState("数据库错误: %s", error);
	if (!SQL_GetRowCount(hndl)) {
		PrintToChatAll("当前地图信息未录入");
		return;
	}
	int field;
	if (!SQL_FieldNameToNum(hndl, "ID", field)) SetFailState("数据库错误: 未知列: id");
	MapInfo.id = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "NAME", field)) SetFailState("数据库错误: 未知列: name");
	SQL_FetchString(hndl, field,MapInfo.name,sizeof(MapInfo.name));
	if (!SQL_FieldNameToNum(hndl, "CN_NAME", field)) SetFailState("数据库错误: 未知列: cn_name");
	SQL_FetchString(hndl, field,MapInfo.name_cn,sizeof(MapInfo.name_cn));
	if (!SQL_FieldNameToNum(hndl, "COOLDOWN", field)) SetFailState("数据库错误: 未知列: cooldown");
	MapInfo.cooldown = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "LAST_RUN_TIME", field)) SetFailState("数据库错误: 未知列: last_run_time");
	MapInfo.last_run_time = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "AVAILABLE", field)) SetFailState("数据库错误: 未知列: available");
	MapInfo.available = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "COST", field)) SetFailState("数据库错误: 未知列: cost");
	MapInfo.cost = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "HEAT", field)) SetFailState("数据库错误: 未知列: heat");
	MapInfo.heat = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "DIFFICULTY", field)) SetFailState("数据库错误: 未知列: difficulty");
	MapInfo.difficulty = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "TAG", field)) SetFailState("数据库错误: 未知列: tag");
	MapInfo.tag = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "ROUND", field)) SetFailState("数据库错误: 未知列: round");
	MapInfo.round = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "WINS", field)) SetFailState("数据库错误: 未知列: wins");
	MapInfo.wins = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "TRANSLATED", field)) SetFailState("数据库错误: 未知列: translated");
	MapInfo.translated = SQL_FetchInt(hndl, field);
	if (!SQL_FieldNameToNum(hndl, "DOWNLOAD", field)) SetFailState("数据库错误: 未知列: download");
	MapInfo.download = SQL_FetchInt(hndl, field);	
	Current_Map_Loaded = true;
	return;
}