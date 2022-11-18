//Maps:Global Map StringMap in map_adm.h
Map_Info Pmap;
bool Current_Map_Loaded = false;
bool Current_Map_Fatigue = false;
ConVar Cvar_MP_TIMELIMIT;
ConVar Cvar_InfectSpawnTimeMax;
ConVar Cvar_InfectSpawnTimeMin;
ConVar Cvar_DamageScale_1;
ConVar Cvar_DamageScale_2;

char g_current_nominator_name[PLATFORM_MAX_PATH];
char g_current_nominator_steamauth[PLATFORM_MAX_PATH];
bool g_current_nominated;
int g_Map_Interval_Count;
bool g_Map_PlayerNum_Check=false;
int g_Map_Round=0;
bool g_Map_RuntimeUpdate_Checked;


void MapInfoOnPluginStart()
{
	g_Map_Interval_Count = 0;
	g_Map_PlayerNum_Check = false;
	RegConsoleCmd("sm_mi", ActionMapInfoMenu);
	RegConsoleCmd("sm_mapinfo", ActionMapInfoMenu);
	Current_Map_Loaded = false;
	Cvar_MP_TIMELIMIT = FindConVar("mp_timelimit");
	Cvar_InfectSpawnTimeMin = FindConVar("zr_infect_spawntime_min");
	Cvar_InfectSpawnTimeMax = FindConVar("zr_infect_spawntime_max");
	Cvar_DamageScale_1 = FindConVar("mp_damage_scale_t_body");
	Cvar_DamageScale_2 = FindConVar("mp_damage_scale_t_head");
	g_Map_Round = 0;
	g_Map_RuntimeUpdate_Checked = false;
}

void MapInfoOnMapStart()
{
	g_Map_Round = 0;
	g_Map_RuntimeUpdate_Checked = false;
	Current_Map_Loaded = false;
	Current_Map_Fatigue = false;
	g_current_nominated = GetCurrentMapNominatorName(g_current_nominator_name,g_current_nominator_steamauth);
}

void MapInfoOnDbConnected_MapStartPost()
{
	char map_name[64];
	if(Current_Map_Loaded)	return;
	Current_Map_Fatigue = false;
	GetCurrentMap(map_name,sizeof(map_name));
	int server_port = FindConVar("hostport").IntValue;
	SERVER_LOG current_server;
	EXGUSERS_GetServerByPort(server_port,current_server);
	if(Maps.GetArray(map_name,Pmap,sizeof(Pmap)))
	{
		PrintToServer("[当前地图信息读取完毕]");

		if(!Current_Map_Loaded&&current_server.sid!=0)
		{
			Current_Map_Fatigue = true;
			if(current_server.ze_fatigue)
			{

			//PrintToServer("疲劳计数:%d",g_Map_Interval_Count)
				if(Pmap.difficulty>=2)
				{
					g_Map_Interval_Count = Min(2,g_Map_Interval_Count+Pmap.interval);				
				}
				else
				{
					g_Map_Interval_Count= Max(0,g_Map_Interval_Count-1);				
				}
				PrintToChatAll(" \x05[地图系统]\x01服务器启用神图疲劳");
			}
			else
			{
				PrintToChatAll(" \x05[地图系统]\x01服务器无神图疲劳，已清零");
				g_Map_Interval_Count = 0;
			}
		}
		Pmap.temp_cooldown = false;
		MapCfgUpdate(Pmap);
		Cvar_MP_TIMELIMIT.SetFloat(float(Pmap.timelimit),true,false);
		if(Cvar_InfectSpawnTimeMin&&Cvar_InfectSpawnTimeMax)
		{
			if(Pmap.infecttime >0 )
			{
				Cvar_InfectSpawnTimeMin.SetFloat(Pmap.infecttime,true,false);
				Cvar_InfectSpawnTimeMax.SetFloat(Pmap.infecttime,true,false);
			}
		}
		Current_Map_Loaded = true;
	}
	else
	{
		PrintToServer("[地图信息读取失败:地图信息不存在]");		
		Current_Map_Loaded = false;
	}

	Call_StartForward(g_MapConfigLoaded);
	Call_Finish();
}
void Pmap_Reload()
{
	if(!Current_Map_Loaded)	return;
	char map_name[64];
	GetCurrentMap(map_name,sizeof(map_name));
	Maps.GetArray(map_name,Pmap,sizeof(Pmap));
}
public int Native_RY_MapProperty_BanHumanSkills(Handle plugin, int numParams)
{
	Pmap_Reload();
	return Pmap.nohmskill;
}
public int Native_RY_MapProperty_BanZombieSkills(Handle plugin, int numParams)
{
	Pmap_Reload();
	return Pmap.nozmskill;
}
public int Native_RY_MapProperty_BanJK(Handle plugin, int numParams)
{
	Pmap_Reload();
	return Pmap.nojk;
}
public int Native_RY_MapProperty_NoBhopLimit(Handle plugin, int numParams)
{
	Pmap_Reload();
	return Pmap.nobhoplimit;
}
public int Native_RY_Map_GetMapInfo(Handle plugin, int numParams)
{
	char map_name[64];
	Map_Info Map_Return;
	GetNativeString(1, map_name, sizeof(map_name));
	if(Maps.GetArray(map_name,Map_Return,sizeof(Map_Return)))
	{
		SetNativeArray(2, view_as<int>(Map_Return), sizeof(Map_Info)); 
		return 1;
	}
	else
	{
		return 0;
	}
}

public int Native_RY_Map_GetCurrentMapInfo(Handle plugin, int numParams)
{
	Map_Info Map_Return;
	Maps.GetArray(Pmap.name,Map_Return,sizeof(Map_Return));
	SetNativeArray(1, view_as<int>(Map_Return), sizeof(Map_Info)); 
}
public int Native_RY_Map_GetCurrentMapNominator(Handle plugin,int numParams)
{
	SetNativeString(1,g_current_nominator_steamauth,sizeof(g_current_nominator_steamauth));
}
Action ActionMapInfoMenu(int client,int args)
{
	if (!IsClientInGame(client)) return Plugin_Handled;
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
	if(g_current_nominated)
	{
		Format(title,sizeof(title),"预定者:%s",g_current_nominator_name);
		menu.AddItem("",title,ITEMDRAW_DISABLED);
		menu.AddItem("",g_current_nominator_steamauth);
	}
	else
	{
		menu.AddItem("","野生",ITEMDRAW_DISABLED);
	}
	menu.Display(client, MENU_TIME_FOREVER);
}

int MapInfoMenuHandler(Menu menu, MenuAction action, int client, int param) {
	if (action == MenuAction_End) menu.Close();
	else if(action == MenuAction_Select)
	{
		PrintToChat(client,g_current_nominator_steamauth);
	}
}
void MapInfoOnRoundStart()
{
	if(!Current_Map_Fatigue)
	{
		int server_port = FindConVar("hostport").IntValue;
		SERVER_LOG current_server;
		EXGUSERS_GetServerByPort(server_port,current_server);		
		if(current_server.sid!=0)
		{
			Current_Map_Fatigue = true;
			if(current_server.ze_fatigue)
			{

				if(Pmap.difficulty>=2)
				{
					g_Map_Interval_Count = Min(2,g_Map_Interval_Count+Pmap.interval);				
				}
				else
				{
					g_Map_Interval_Count= Max(0,g_Map_Interval_Count-1);				
				}
				PrintToChatAll(" \x05[地图系统]\x01服务器启用神图疲劳");
			}
			else
			{
				PrintToChatAll(" \x05[地图系统]\x01服务器无神图疲劳，已清零");
				g_Map_Interval_Count = 0;
			}			
		}
	}
	Cvar_DamageScale_1.SetFloat(Pmap.dmgscale,true,false);
	Cvar_DamageScale_2.SetFloat(Pmap.dmgscale,true,false);
}
void MapInfoOnRoundEnd(int winner)
{
	Pmap_Reload();
	if(winner==2)
	{
		Pmap.round++;	
	}
	if(winner==3)
	{
		Pmap.round++;
		Pmap.wins++;
	}
	if(Cvar_InfectSpawnTimeMin&&Cvar_InfectSpawnTimeMax)
	{
		if(Pmap.infecttime >0 )
		{
			Cvar_InfectSpawnTimeMin.SetFloat(Pmap.infecttime,true,false);
			Cvar_InfectSpawnTimeMax.SetFloat(Pmap.infecttime,true,false);
		}
	}
	else
	{
		Cvar_InfectSpawnTimeMin = FindConVar("zr_infect_spawntime_min");
		Cvar_InfectSpawnTimeMax = FindConVar("zr_infect_spawntime_max");		
	}
	MapCfgUpdate(Pmap);
	CheckMapCoolDown();
	return;
}
void MapInfoOnWarmUpEnd()
{
	CheckMapCoolDown();
}

void CheckMapCoolDown()
{
	if(g_Map_RuntimeUpdate_Checked)	return;
	if(Current_Map_Loaded)
	{
			Pmap_Reload();
			if(GetClientCount(true)>=20)
			{
				g_Map_RuntimeUpdate_Checked = true;
				int Time_MapStart = GetTime();
				PrintToChatAll(" \x05[地图系统]\x01玩家人数超过20，计入地图CD");
				Pmap.last_run_time = Time_MapStart;			//Check PlayerNum When MapEnd and if PlayerNum>20,Count Last Run Time
			}
			else 
			{
				PrintToChatAll(" \x05[地图系统]\x01玩家人数不足20, 不计入地图CD");				
			}
			MapCfgUpdate(Pmap);		
	}
}