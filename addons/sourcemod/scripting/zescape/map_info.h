//Maps:Global Map StringMap in map_adm.h
Map_Info Pmap;
bool Current_Map_Loaded = false;
ConVar Cvar_MP_TIMELIMIT;
void MapInfoOnPluginStart(){
	RegConsoleCmd("sm_mi", ActionMapInfoMenu);
	RegConsoleCmd("sm_mapinfo", ActionMapInfoMenu);
	Current_Map_Loaded = false;
	Cvar_MP_TIMELIMIT = FindConVar("mp_timelimit");
}
void MapInfoOnMapStart(){
	Current_Map_Loaded = false;
}
void MapInfoOnDbConnected_MapStartPost(){
	char map_name[64];
	int Time_MapStart = 0;
	if(Current_Map_Loaded)	return;
	GetCurrentMap(map_name,sizeof(map_name));
	if(Maps.GetArray(map_name,Pmap,sizeof(Pmap)))
	{
		PrintToServer("[当前地图信息读取完毕]");
		Time_MapStart = GetTime();
		if(!Current_Map_Loaded)
		{
			Pmap.round += 1;
			Pmap.last_run_time = Time_MapStart;
		}
		Pmap.temp_cooldown = false;
		MapCfgUpdate(Pmap);
		Cvar_MP_TIMELIMIT.SetFloat(float(Pmap.timelimit),true,false);
		Current_Map_Loaded = true;
	}
	else
	{
		PrintToServer("[地图信息读取失败:地图信息不存在]");		
		Current_Map_Loaded = false;
	}
}

Action ActionMapInfoMenu(int client,int args)
{
	if(!Current_Map_Loaded)
	{
		PrintToServer("[地图信息读取失败:尚未加载 重试中...]");
		MapInfoOnDbConnected_MapStartPost();
		return Plugin_Handled;
	}
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
		地图订价:%d",Pmap.name,Pmap.name_cn,difficulty_name[Pmap.difficulty],Pmap.wins,Pmap.round-Pmap.wins,Pmap.heat,Pmap.cost);
	Menu menu = CreateMenu(MapInfoMenuHandler);
	menu.SetTitle(title);
	Format(title,sizeof(title),"回合时长:%d",Pmap.roundtime);
	menu.AddItem("",title,ITEMDRAW_DISABLED);
	Format(title,sizeof(title),"地图时长:%d",Pmap.timelimit);
	menu.AddItem("",title,ITEMDRAW_DISABLED);
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapInfoMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) menu.Close();
}
