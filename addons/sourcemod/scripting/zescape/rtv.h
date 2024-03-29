ConVar g_Cvar_RTV_MaxRounds;
ConVar g_Cvar_RTV_PlayerNeededRatio;
bool current_map_logged =false;
int g_RTV_VotesNum;
int g_RTV_Rounds;
bool g_RTV_PlyVoted[65];
bool g_Allow_RTV;
bool g_Nextmap_Selected;
int g_WarningTime_BeforeVote;
int g_VoteTime;
int g_Extend_Times;
bool g_Instant_RTV;
bool g_Extend_Vote;
int g_Extend_Vote_Yes;
int g_Extend_Vote_No;
int g_Extend_Vote_All;
Handle g_WTimer_BeforeVote;
Handle g_WTimer_BeforeMapChange;
Handle g_Timer_Timeleft;
Handle g_Timer_MapVote_CenterText;
Handle g_RTV_Forward_StartMapVote;
Handle g_RTV_Forward_ExtendSelect;
Handle g_RTV_Forward_NoChangeSelect;
Handle g_RTV_Forward_NextMapSelected;
char nextmap_name[64];
enum struct Maps_VoteInfo
{
	char name[PLATFORM_MAX_PATH];
	char name_cn[PLATFORM_MAX_PATH];
	bool nominated;
	char nominator_name[PLATFORM_MAX_PATH];
	int nominator_uid;
	char nominator_steamauth[PLATFORM_MAX_PATH];
	char nominator_steampage[PLATFORM_MAX_PATH];
	int difficulty;
	int nominate_cost;
}
enum MapChangeTime
{
	MapChangeTime_Instant,
	MapChangeTime_RoundEnd,
	MapChangeTime_MapEnd
};
enum struct MapVote_Log
{
	int map_param;
	char mapvote_name[64];
	int votes;
}
MapVote_Log mapvote_log_array[6];

MapChangeTime g_ChangeMap_Time;
ArrayList MapVote_List;
ArrayList RandomMap_Candidate;
Maps_VoteInfo g_Nextmap_Result,g_LastRound_MapVoteSave;
int rtv_totalvotes;
static int server_ip;
static int server_port;
static SERVER_LOG current_server;
void RTVOnPluginStart()
{
	server_ip = FindConVar("hostip").IntValue;
	server_port = FindConVar("hostport").IntValue;
	EXGUSERS_GetServerByPort(server_ip,server_port,current_server);
	g_MapVote_Initiated = false;
	g_MapVote_Proceeding = false;
	g_WTimer_BeforeVote = INVALID_HANDLE;
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	g_Timer_Timeleft = INVALID_HANDLE;
	g_Timer_MapVote_CenterText = INVALID_HANDLE;
	MapVote_List = CreateArray(sizeof(Maps_VoteInfo));
	RandomMap_Candidate = CreateArray(sizeof(Maps_VoteInfo));
	ResetRTV();
	RegConsoleCmd("sm_rtv",RTVCommand);
	RegConsoleCmd("sm_ht",RTVCommand);
	RegConsoleCmd("sm_mct",MapChangeTimeCommand);
	RegConsoleCmd("sm_ext",ExtendCommand);
	RegAdminCmd("sm_forcertv",ForceRTVCommand,ADMFLAG_GENERIC);
	g_Cvar_RTV_PlayerNeededRatio = CreateConVar("emc_rtv_needed","0.4","Percentage of RTV Votes needed to start(def:0.45)",0, true, 0.05, true, 1.0);
	g_Cvar_RTV_MaxRounds = CreateConVar("emc_max_rounds","50","Percentage of RTV Votes needed to start(def:0.45)",0, true, 0.05, true, 1.0);
	if(g_RTV_Forward_StartMapVote == null)
	{
		g_RTV_Forward_StartMapVote = CreateGlobalForward("EMC_Forward_RTV_StartMapVote",ET_Ignore);
	}
	if(g_RTV_Forward_ExtendSelect == null)
	{
		g_RTV_Forward_ExtendSelect = CreateGlobalForward("EMC_Forward_RTV_Extended",ET_Ignore);
	}
	if(g_RTV_Forward_NoChangeSelect == null)
	{
		g_RTV_Forward_NoChangeSelect = CreateGlobalForward("EMC_Forward_RTV_Nochange",ET_Ignore);
	}
	if(g_RTV_Forward_NextMapSelected == null)
	{
		g_RTV_Forward_NextMapSelected = CreateGlobalForward("EMC_Forward_NextmapSelected",ET_Ignore,Param_String);
	}
	current_map_logged = false;
}

void MapVoteLogClear()
{
	for(int i=0; i<=5; i++)
	{
		mapvote_log_array[i].map_param = i+3;
		mapvote_log_array[i].mapvote_name = "";
		mapvote_log_array[i].votes = 0;
	}
}

void MapVoteLogSort()
{
	MapVote_Log mapvote_log_temp;
	for(int i=0;i<=4;i++)
	{
		for(int j=i+1;j<=5;j++)
		{
			if(mapvote_log_array[i].votes<=mapvote_log_array[j].votes)
			{
				mapvote_log_temp.map_param = mapvote_log_array[i].map_param;
				strcopy(mapvote_log_temp.mapvote_name,sizeof(mapvote_log_temp.mapvote_name),mapvote_log_array[i].mapvote_name);
				mapvote_log_temp.votes = mapvote_log_array[i].votes;

				mapvote_log_array[i].map_param = mapvote_log_array[j].map_param;
				strcopy(mapvote_log_array[i].mapvote_name,sizeof(mapvote_log_temp.mapvote_name),mapvote_log_array[j].mapvote_name);
				mapvote_log_array[i].votes=mapvote_log_array[j].votes;

				mapvote_log_array[j].map_param = mapvote_log_temp.map_param;
				strcopy(mapvote_log_array[j].mapvote_name,sizeof(mapvote_log_temp.mapvote_name),mapvote_log_temp.mapvote_name);
				mapvote_log_array[j].votes = mapvote_log_temp.votes;
			}
		}
	}
}

Action MapChangeTimeCommand(int client,int args)
{
	if(!IsClientInGame(client))	return Plugin_Handled;
	if(g_ChangeMap_Time == MapChangeTime_MapEnd)
	{
		PrintToChatAll("地图结束后换图");
	}
	if(g_ChangeMap_Time == MapChangeTime_Instant)
	{
		PrintToChatAll("立即换图");
	}
	if(g_ChangeMap_Time == MapChangeTime_RoundEnd)
	{
		PrintToChatAll("回合结束后换图");
	}
	return Plugin_Handled;
}
Action ForceRTVCommand(int client,int args)
{
	if(!IsClientInGame(client))	return Plugin_Handled;
	char buffer[256];
	GetClientName(client,buffer,sizeof(buffer));
	PrintToChatAll(" \x05[EMC] \x09%s\x01发起了强制换图投票",buffer);
	g_Allow_RTV = false;
	if(g_Nextmap_Selected)
	{
		g_ChangeMap_Time = MapChangeTime_Instant;
		ChangeMap();
	}
	else
	{
		StartMapVote(MapChangeTime_RoundEnd);
	}
	return Plugin_Handled;
}
void RTVOnMapStart()
{
	server_ip = FindConVar("hostip").IntValue;
	server_port = FindConVar("hostport").IntValue;
	EXGUSERS_GetServerByPort(server_ip,server_port,current_server);
	g_WTimer_BeforeVote = INVALID_HANDLE;
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	g_Timer_Timeleft = INVALID_HANDLE;
	g_Timer_MapVote_CenterText = INVALID_HANDLE;
	ResetRTV();
	g_RTV_Rounds = 0;
	g_Extend_Vote = true;
	current_map_logged = false;
}
public bool isMapCoolDownOver(Map_Info map)
{
	int current_time;
	char cooldown_state[PLATFORM_MAX_PATH];
	current_time = GetTime();
	if(!DiffTimeTransform1(cooldown_state,current_time,map.last_run_time,map.cooldown*60))
	{
		return true;
	}

	return false;
}
public bool GetCurrentMapNominatorName(char nominator_name[PLATFORM_MAX_PATH],char nominator_steamauth[PLATFORM_MAX_PATH])
{
	if(g_LastRound_MapVoteSave.nominated)
	{
		PrintToServer(g_LastRound_MapVoteSave.nominator_name);
		strcopy(nominator_name,sizeof(nominator_name),g_LastRound_MapVoteSave.nominator_name);
		strcopy(nominator_steamauth,sizeof(nominator_steamauth),g_LastRound_MapVoteSave.nominator_steamauth);
		return true;
	}
	return false;
}

void RTVOnMapEnd()
{
	PrintToChatAll("[RTVOnMapEnd]");
	ResetRTV();
	CloseHandleSafe(MapVote_List);
	CloseHandleSafe(RandomMap_Candidate);
	MapVote_List = CreateArray(sizeof(Maps_VoteInfo));
	RandomMap_Candidate = CreateArray(sizeof(Maps_VoteInfo));
	g_ChangeMap_Time = MapChangeTime_MapEnd;
}
void ResetRTV()
{
	g_WTimer_BeforeVote = INVALID_HANDLE;
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	g_Timer_Timeleft = INVALID_HANDLE;
	g_Timer_MapVote_CenterText = INVALID_HANDLE;
	g_Allow_RTV = true;
	g_Instant_RTV = false;
	g_ChangeMap_Time = MapChangeTime_MapEnd;
	MapVote_List.Clear();
	RandomMap_Candidate.Clear();
	g_WarningTime_BeforeVote = 10;
	KillTimerSafe(g_WTimer_BeforeVote);
	KillTimerSafe(g_Timer_MapVote_CenterText);
	for(int i = 1 ; i<=MaxClients ; i++)
	{
		g_RTV_PlyVoted[i] = false;
	}
	g_RTV_VotesNum=0;
	g_Nextmap_Selected = false;
	g_MapVote_Initiated = false;
	g_MapVote_Proceeding = false;
	Nominate_ALLOW = true;
	g_Nextmap_Result.name = "";
	g_Nextmap_Result.nominated = false;
	g_Nextmap_Result.nominator_name = "";
	g_Nextmap_Result.nominator_uid = 0;
	g_Nextmap_Result.difficulty = 0;
	g_Extend_Times = 0;
}
void RTVOnRoundStart()
{
	PrintToServer("[RTVOnRoundStart]");
	g_RTV_Rounds++;
	char buffer[256];
	char map_name[64];
	GetCurrentMap(map_name,sizeof(map_name));
	if(g_RTV_Rounds<=2&&(strcmp(map_name,g_LastRound_MapVoteSave.name)==0))
	{
		Format(buffer,sizeof(buffer)," \x05当前地图: \x09%s%s%s",map_name,g_LastRound_MapVoteSave.nominated?"\x05预定者":"",g_LastRound_MapVoteSave.nominated?g_LastRound_MapVoteSave.nominator_name:"");
		PrintToChatAll(buffer);
		PrintToChatAll(buffer);
		PrintToChatAll(buffer);
		Format(buffer,sizeof(buffer),"[EMC]%s%s(UID:%d)",g_LastRound_MapVoteSave.nominated?"预定者":"当前地图为野生",g_LastRound_MapVoteSave.nominated?g_LastRound_MapVoteSave.nominator_name:"",g_LastRound_MapVoteSave.nominator_uid);
		PrintToServer(buffer);
	}
	char query[512];
	if(!current_map_logged)
	{
		if(isDbConnected())
		{
			Format(query,sizeof(query),"INSERT INTO exgze_maphistory (SVNAME,NOM,NOMNAME,NOMUID,TIMESTAMP,MAPNAME) VALUES('%s',%d,'%s',%d,%d,'%s')",current_server.shortname,g_LastRound_MapVoteSave.nominated,g_LastRound_MapVoteSave.nominator_name,g_LastRound_MapVoteSave.nominator_uid,GetTime(),map_name);
			DbTQuery(DbQueryErrorCallback,query);
		}
		current_map_logged = true;
	}
}
void RTVOnRoundEnd()
{
	if(g_ChangeMap_Time == MapChangeTime_RoundEnd&&g_Nextmap_Selected)
	{
		KillTimerSafe(g_WTimer_BeforeMapChange);
		g_WTimer_BeforeMapChange = CreateTimer(8.0,ChangeMap_RoundEnd_Hndl, _,TIMER_FLAG_NO_MAPCHANGE);
	}
	else
	{
		if(!g_Nextmap_Selected)
		{
			int timeleft = 0;
			GetMapTimeLeft(timeleft);
			if(timeleft<0)
			{
				RandomMapChange(1);
			}
		}
	}
}
Action ChangeMap_RoundEnd_Hndl(Handle timer)
{
	char nextmap[64];
	GetNextMap(nextmap,sizeof(nextmap));
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	char buffer[256];
	Format(buffer,sizeof(buffer),"map %s",nextmap);
	ServerCommand(buffer);
}
public void OnClientSayCommand_Post(int client, const char[] command, const char[] sArgs)
{
	if(!IsClientInGame(client)||!client||IsChatTrigger())	return;
	if(strcmp(sArgs,"rtv",false)==0||strcmp(sArgs,"rockthevote",false)==0||strcmp(sArgs,"ht",false)==0)
	{
		AttemptRTV(client);
	}
}
void RandomMapChange(int instant = 0)
{
	g_RTV_VotesNum = 0;
	for(int i = 1; i <= 64; i++)
	{
		g_RTV_PlyVoted[i] = false;
	}
	Map_Info map;
	StringMapSnapshot snap=Maps.Snapshot();
	int player_num = GetClientCount(true);
	ArrayList RandomMapChange_List = CreateArray(sizeof(Map_Info));
	char current_mapname[64];
	GetCurrentMap(current_mapname,sizeof(current_mapname));
	for(int i =0;i < snap.Length ; i++)
	{
		snap.GetKey(i, map.name, sizeof(map.name));
		Maps.GetArray(map.name, map, sizeof(map));
		bool interval_status = true;
		bool playernum_limit = true;
		if(map.available&&map.exist&&map.download&&map.random)
		{

			if(map.difficulty>=2)
			{
				if(g_Map_Interval_Count>0&&current_server.ze_fatigue)
				{
					interval_status = false;
					//PrintToConsoleAll("地图%s由于疲劳机制被剔出了随机队列",map.name);
				}
			}
			if(player_num>map.plupper&&map.plupper!=0){
				playernum_limit = false;
			}
			if(player_num<map.pllower&&map.pllower!=0)
			{
				playernum_limit = false;
			}
			if(!isNominated(map)&&isMapCoolDownOver(map)&&interval_status&&playernum_limit&&strcmp(current_mapname,map.name)!=0)
			{
				RandomMapChange_List.PushArray(map,sizeof(map));
			}
		}
	}
	int random_order = GetURandomInt()%(RandomMapChange_List.Length);
	Map_Info random_nextmap;
	GetArrayArray(RandomMapChange_List,random_order,random_nextmap,sizeof(random_nextmap));
	g_Nextmap_Result.name = random_nextmap.name;
	g_Nextmap_Result.nominated = 0;
	g_Nextmap_Result.nominator_name = "";
	Cvar_SM_NEXTMAP.SetString(g_Nextmap_Result.name,true,false);
	delete RandomMapChange_List;
	PrintToChatAll(" \x05[地图系统]\x01由于管理员设置或者地图系统出现问题，随机选择并更换地图为%s",g_Nextmap_Result.name);
	if(instant==0)
	{
		return;
	}
	CreateTimer(5.0,Timer_RandomChangeMap_Hndl,_,TIMER_FLAG_NO_MAPCHANGE);
}
Action Timer_RandomChangeMap_Hndl(Handle timer)
{
	char buffer[256];
	Format(buffer,sizeof(buffer),"map %s",g_Nextmap_Result.name);
	ServerCommand(buffer);
	return Plugin_Handled;
}
public void RTVOnClientDisconnect(int client)
{
	if(!IsFakeClient(client))
	{

		if(g_RTV_PlyVoted[client])
		{
			g_RTV_PlyVoted[client]=false;
			g_RTV_VotesNum--;
		}
	}
	if(GetClientCount(true) <=5 )	return;
	if(!g_Allow_RTV)	return;
	int d_RTV_PlayerNeeded;
	d_RTV_PlayerNeeded=Max(1,RoundToFloor(GetClientCount(true)*GetConVarFloat(g_Cvar_RTV_PlayerNeededRatio)));
	if(g_RTV_VotesNum>=d_RTV_PlayerNeeded)
	{
		g_Allow_RTV = false;
		if(g_Instant_RTV)
		{
			StartMapVote(MapChangeTime_Instant);
		}
		else
		{
			StartMapVote(MapChangeTime_RoundEnd);
		}
	}

}
public void OnMapTimeLeftChanged()
{
	int timeleft;
	char buffer[256];
	if(g_Nextmap_Selected)	return;
	GetMapTimeLeft(timeleft);
	Format(buffer,sizeof(buffer),"地图时间:%d秒",timeleft);
	PrintToServer(buffer);
	//PrintToConsoleAll(buffer);
	if(timeleft<0)	return;
	if(timeleft-180<=0)
	{
		g_ChangeMap_Time = MapChangeTime_MapEnd;
		g_Allow_RTV = false;
		StartMapVote(MapChangeTime_MapEnd);
	}
	else
	{
		KillTimerSafe(g_Timer_Timeleft);
		g_Timer_Timeleft = CreateTimer(float(timeleft)-180.0,TIMELEFT_AUTOVOTEMAP_HNDL, _,TIMER_FLAG_NO_MAPCHANGE);
	}
}
Action TIMELEFT_AUTOVOTEMAP_HNDL(Handle timer)
{
	if(!g_Nextmap_Selected&&!g_MapVote_Initiated&&!g_MapVote_Proceeding)
	{
		g_ChangeMap_Time = MapChangeTime_MapEnd;
		g_Allow_RTV = false;
		StartMapVote(MapChangeTime_MapEnd);
	}
	g_Timer_Timeleft = INVALID_HANDLE;
	return Plugin_Stop;
}
Action RTVCommand(int client,int args)
{
	if(!IsClientInGame(client))	return Plugin_Handled;
	AttemptRTV(client);
	return Plugin_Handled;
}
void AttemptRTV(int client)
{
	char buffer[256];
	if(!IsClientInGame(client))	return;
	if(!isDbConnected())	return;
	int timeleft;
	GetMapTimeLeft(timeleft);
	if(g_Nextmap_Selected||g_Instant_RTV||timeleft<0)
	{
		AttemptInstantRTV(client);
		return;
	}
	if(!g_Allow_RTV)
	{
		PrintToChat(client," \x05[EMC] \x01当前无法发起换图投票");
		return;
	}
	if(g_MapVote_Proceeding||g_MapVote_Initiated)
	{
		PrintToChat(client," \x05[EMC] \x01地图投票正在进行中");
		return;
	}
	int RTV_PlayerNeeded;

	RTV_PlayerNeeded = RoundToFloor(GetClientCount(true) * GetConVarFloat(g_Cvar_RTV_PlayerNeededRatio));
	RTV_PlayerNeeded = Max(RTV_PlayerNeeded,1);
	if(g_RTV_PlyVoted[client])
	{
		Format(buffer,sizeof(buffer)," \x05[EMC] \x01您已要求发起换图投票(当前\x08 %d票，还需\x09 %d票)",g_RTV_VotesNum,RTV_PlayerNeeded-g_RTV_VotesNum);
		PrintToChat(client,buffer);
		return;
	}
	g_RTV_VotesNum++;
	char clientname[256];
	GetClientName(client,clientname,sizeof(clientname));

	Format(buffer,sizeof(buffer)," \x05[EMC] \x09%s \x01要求发起换图投票:\x08 %d票/\x09 %d票(仍需\x09 %d票)",clientname,g_RTV_VotesNum,RTV_PlayerNeeded,RTV_PlayerNeeded-g_RTV_VotesNum);
	PrintToChatAll(buffer);
	g_RTV_PlyVoted[client]=true;
	if(g_RTV_VotesNum>=RTV_PlayerNeeded)
	{
		g_Allow_RTV = false;
		StartMapVote(MapChangeTime_RoundEnd);
	}
}
void AttemptInstantRTV(int client)
{
	char buffer[256];
	int RTV_PlayerNeeded;
	RTV_PlayerNeeded = RoundToFloor(GetClientCount(true) * GetConVarFloat(g_Cvar_RTV_PlayerNeededRatio));
	RTV_PlayerNeeded = Max(RTV_PlayerNeeded,1);
	if(g_RTV_PlyVoted[client])
	{
		Format(buffer,sizeof(buffer)," \x05[EMC] \x01您已要求立即换图(当前\x08 %d票，还需\x09 %d票)",g_RTV_VotesNum,RTV_PlayerNeeded-g_RTV_VotesNum);
		PrintToChat(client,buffer);
		return;
	}
	g_RTV_PlyVoted[client]=true;
	g_RTV_VotesNum++;
	char clientname[256];
	GetClientName(client,clientname,sizeof(clientname));
	Format(buffer,sizeof(buffer)," \x05[EMC] \x09%s \x01要求立即换图:\x08 %d票 /\x09 %d票(仍需\x09 %d票)",clientname,g_RTV_VotesNum,RTV_PlayerNeeded,RTV_PlayerNeeded-g_RTV_VotesNum);
	PrintToChatAll(buffer);
	if(g_RTV_VotesNum>=RTV_PlayerNeeded)
	{
		g_ChangeMap_Time = MapChangeTime_Instant;
		ChangeMap();
	}
}
void StartMapVote(MapChangeTime when)
{
	if(GetClientCount(true) == 0)	return;
	Call_StartForward(g_RTV_Forward_StartMapVote);
	Call_Finish();
	Nominate_ALLOW = false;
	g_MapVote_Initiated = true;
	g_RTV_VotesNum = 0;
	for(int i = 1; i <= 64; i++)
	{
		g_RTV_PlyVoted[i] = false;
	}
	PrintToChatAll(" \x05[EMC]\x01 10秒后开启下张地图投票");
	g_ChangeMap_Time = when;
	KillTimerSafe(g_WTimer_BeforeVote);
	g_WTimer_BeforeVote = CreateTimer(1.0,g_Wtimer_Hndl, when, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

Action g_Wtimer_Hndl(Handle timer,MapChangeTime when)
{
	char buffer[256];

	if(g_WarningTime_BeforeVote>=0)
	{
		Format(buffer,sizeof(buffer),"[EMC]下张地图投票倒计时:%d秒",g_WarningTime_BeforeVote);
		PrintCenterTextAll(buffer);
		g_WarningTime_BeforeVote--;
		return Plugin_Handled;
	}
	g_MapVote_Initiated = false;
	g_MapVote_Proceeding = true;
	g_WarningTime_BeforeVote = 10;
	CreateNextMapVote();
	g_WTimer_BeforeVote = INVALID_HANDLE;
	return Plugin_Stop;
}

void CreateNextMapVote()
{
	int Random_Num;
	char buffer[256];
	if(GetClientCount(true) == 0)	return;
	Random_Num = Min(Nominate_Max_Num-Nom_Map_List.Length,5);
	Nominate_ALLOW=false;
	Menu menu = CreateMenu(NextMapVoteHandler, MenuAction_End | MenuAction_Display | MenuAction_DisplayItem | MenuAction_VoteCancel);
	SetMenuOptionFlags(menu, MENUFLAG_BUTTON_NOVOTE);
	SetMenuPagination(menu, MENU_NO_PAGINATION);
	Nomlist_Log nomlog;
	Map_Info map;
	Maps_VoteInfo mapv;
	MapVote_List.Clear();
	menu.SetTitle("地图投票");
	menu.AddItem("","选择下一张地图",ITEMDRAW_DISABLED);
	menu.AddItem("","请慎重投票，不要随意选图",ITEMDRAW_DISABLED);
	int Nom_Invalid = 0;
	for(int i = 0 ; i < Nom_Map_List.Length; i++)
	{
		GetArrayArray(Nom_Map_List,i,nomlog,sizeof(nomlog));
		Maps.GetArray(nomlog.name,map,sizeof(map));
		mapv.name = map.name;
		mapv.nominated = true;
		mapv.nominator_name = nomlog.nominator_name;
		mapv.nominator_uid = nomlog.nominator_uid;
		strcopy(mapv.nominator_steamauth,sizeof(mapv.nominator_steamauth),nomlog.nominator_steamauth);
		/*if(GetTime()<=map.last_run_time+map.cooldown*60&&map.temp_cd==0)
		{
			PrintToChatAll(" \x05[地图系统]\x01玩家%s预定的%s地图由于CD不合法")
			continue;
		}*/
		mapv.name_cn = map.name_cn;
		mapv.difficulty = map.difficulty;
		mapv.nominate_cost = map.cost;
		MapVote_List.PushArray(mapv,sizeof(mapv));
	}
	StringMapSnapshot snap=Maps.Snapshot();
	int RandomMap_Picked,RandomMap_Order;
	RandomMap_Picked = 0;
	if(Random_Num>0)
	{
		for(int i =0;i < snap.Length ; i++)
		{
			snap.GetKey(i, map.name, sizeof(map.name));
			Maps.GetArray(map.name, map, sizeof(map));
			bool interval_status = true;
			bool playernum_limit = true;
			if(map.available&&map.exist&&map.download&&map.random)
			{

				if(map.difficulty>=2)
				{
					if(g_Map_Interval_Count>0&&current_server.ze_fatigue)
					{
						interval_status = false;
						//PrintToConsoleAll("地图%s由于疲劳机制被剔出了随机队列",map.name);
					}
				}
				//待实现:人数上下限满足检测
				if(!isNominated(map)&&isMapCoolDownOver(map)&&interval_status&&playernum_limit)
				{
					//PrintToConsoleAll("Random+ %s",map.name);
					mapv.name = map.name;
					mapv.nominated = false;
					mapv.nominator_name = "\0";
					mapv.name_cn = map.name_cn;
					mapv.difficulty = map.difficulty;
					mapv.nominate_cost = 0;
					RandomMap_Candidate.PushArray(mapv,sizeof(mapv));
				}
			}
		}
		while(RandomMap_Picked<Random_Num||(!RandomMap_Candidate.Length))
		{
			RandomMap_Order = GetURandomInt()%(RandomMap_Candidate.Length);	//随机序号0~Length-1,索引同理
			GetArrayArray(RandomMap_Candidate,RandomMap_Order,mapv,sizeof(mapv));
			RandomMap_Candidate.Erase(RandomMap_Order);
			MapVote_List.PushArray(mapv);

			RandomMap_Picked++;
		}
	}
	delete snap;
	ArrayList Temp_MapVote_List;
	Temp_MapVote_List = CreateArray(sizeof(Maps_VoteInfo));
	int select_random;
	for(int i = 0; i<MapVote_List.Length ; i++)
	{
		GetArrayArray(MapVote_List,i,mapv,sizeof(mapv));
		Temp_MapVote_List.PushArray(mapv);
	}
	while(Temp_MapVote_List.Length>0)
	{
		select_random = GetURandomInt()%Temp_MapVote_List.Length;
		GetArrayArray(Temp_MapVote_List,select_random,mapv,sizeof(mapv));
		Temp_MapVote_List.Erase(select_random);
		Format(buffer,sizeof(buffer),"[%s]%s\n%s%s",difficulty_name[mapv.difficulty],mapv.name,mapv.name_cn,mapv.nominated?"[订]":"");
		menu.AddItem(mapv.name,buffer,ITEMDRAW_DEFAULT);
	}
	CloseHandleSafe(Temp_MapVote_List);
	if(g_ChangeMap_Time == MapChangeTime_MapEnd)
	{
		if(g_Extend_Times<Pmap.extend)
		{
			menu.AddItem("Extend","延长地图",ITEMDRAW_DEFAULT);
		}
	}
	else if(g_ChangeMap_Time == MapChangeTime_RoundEnd)
	{
		menu.AddItem("NoChange","不要更换",ITEMDRAW_DEFAULT);
	}


	MapVoteLogClear();
	for(int i=0;i<=5;i++)
	{
		GetMenuItem(menu,i+2,buffer,sizeof(buffer));
		mapvote_log_array[i].map_param=i+2;
		strcopy(mapvote_log_array[i].mapvote_name,64,buffer);
		if(strcmp(mapvote_log_array[i].mapvote_name,"NoChange")==0)		mapvote_log_array[i].mapvote_name = "不要更换";
		if(strcmp(mapvote_log_array[i].mapvote_name,"Extend")==0)		mapvote_log_array[i].mapvote_name = "延长地图";
	}
	rtv_totalvotes=0;
	SetVoteResultCallback(menu,MapVoteHandler);
	VoteMenuToAll(menu,15);
	g_VoteTime = 15;
	KillTimerSafe(g_Timer_MapVote_CenterText);
	g_Timer_MapVote_CenterText = CreateTimer(1.0,MapVote_CenterText_ShowTimer,_,TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

Action MapVote_CenterText_ShowTimer(Handle timer)
{
	char buffer[256];
	int client_num;
	client_num = GetClientCount(true);
	if(g_VoteTime>=0)
	{
		MapVoteLogSort();
		Format(buffer,sizeof(buffer),"投票数:%d/%d 剩余:%d秒\n%s(%d)\n%s(%d)\n%s(%d)",rtv_totalvotes,client_num,g_VoteTime,mapvote_log_array[0].mapvote_name,mapvote_log_array[0].votes,mapvote_log_array[1].mapvote_name,mapvote_log_array[1].votes,mapvote_log_array[2].mapvote_name,mapvote_log_array[2].votes);
		PrintCenterTextAll(buffer);
		g_VoteTime--;
		return Plugin_Handled;
	}
	g_Timer_MapVote_CenterText = INVALID_HANDLE;
	return Plugin_Stop;
}

int NextMapVoteHandler(Menu menu, MenuAction action, int param1, int param2)
{
	Maps_VoteInfo mapv;
	int nominator_credits;
	int credits_return;
	char buffer[256];
	char votemap_name[64];
	char voter_name[64];
	if (action == MenuAction_End)
	{
		menu = null;
		delete menu;
	}
	else if (action == MenuAction_VoteCancel)
	{
		if(param1 == VoteCancel_NoVotes)
		{
			int RandomMap_Num,RandomMap_Select;
			RandomMap_Num = MapVote_List.Length;
			RandomMap_Select = GetURandomInt()%RandomMap_Num;
			GetArrayArray(MapVote_List,RandomMap_Select,g_Nextmap_Result,sizeof(g_Nextmap_Result));
			g_LastRound_MapVoteSave = g_Nextmap_Result;
			g_MapVote_Proceeding = false;
			Format(buffer,sizeof(buffer)," \x05[EMC] \x01随机选择了地图:%s",g_Nextmap_Result.name);
			PrintToChatAll(buffer);
			g_Nextmap_Selected = true;
			for(int i=0;i<MapVote_List.Length;i++)
			{
				GetArrayArray(MapVote_List,i,mapv,sizeof(mapv));
				if(strcmp(mapv.name,g_Nextmap_Result.name)) continue;
			 	if(mapv.nominated)
			 	{
			 		for(int j = 1; j <= 64 ; j++)
			 		{
			 			if(!IsClientInGame(j) || IsFakeClient(j))
							continue;

			 			if(EXGUSERS_GetUserUID(j)==mapv.nominator_uid)
			 			{
							if (g_pStore)
							{
								nominator_credits = Store_GetClientCredits(j);
								credits_return = RoundToFloor(mapv.nominate_cost * 0.5);
								Store_SetClientCredits(j,nominator_credits+credits_return,"预定地图");
							}

							PrintToChatAll(" \x05[EMC] \x09%s \x01预定的 \x07%s \x01未选上，\x07%d \x01积分已退还",mapv.nominator_name,mapv.name,credits_return);
			 			}	
			 		}
			 	}
			}
			g_ChangeMap_Time = MapChangeTime_RoundEnd;
			ChangeMap();
		}
	}
	else if (action == MenuAction_Select)
	{
		rtv_totalvotes++;
		if(param2>=2)
		{
			GetClientName(param1,voter_name,sizeof(voter_name));
			GetMenuItem(menu,param2,votemap_name,sizeof(votemap_name));
			PrintToConsoleAll("[EMC]%s选择了%s",voter_name,votemap_name);
			for(int i=0;i<=5;i++)
			{
				if(mapvote_log_array[i].map_param==param2)
				{
					mapvote_log_array[i].votes++;
					break;
				}
			}
		}
	}
}

void ClearNomMapList()
{
	Nomlist_Log mapv;
	for(int i = 0; i < Nom_Map_List.Length; i++)
	{
		GetArrayArray(Nom_Map_List,i,mapv,sizeof(mapv));
		for(int j = 1; j <=64 ;j++)
		{
		 	if(!IsClientInGame(j)||IsFakeClient(j))	continue;
		 	if(EXGUSERS_GetUserUID(j)==mapv.nominator_uid)
		 	{
				if (g_pStore)
				{
					int nominator_credits = Store_GetClientCredits(j);
					int credits_return = RoundToFloor(mapv.nom_cost * 0.75);
					Store_SetClientCredits(j,nominator_credits+credits_return,"预定地图");
					PrintToChatAll(" \x05[EMC] \x09%s \x01预定的 \x07%s \x01未选上，\x07%d \x01积分已退还",mapv.nominator_name,mapv.name,credits_return);
				}
				else
				{
					PrintToChatAll(" \x05[EMC] \x09%s \x01预定的 \x07%s \x01未选上",mapv.nominator_name,mapv.name);
				}
			}
		}
	}
	Nom_Map_List.Clear();
}

public void MapVoteHandler(Menu menu, int num_votes, int num_clients, const int[][] client_info, int num_items, const int[][] item_info)
{
	char result[64];
	char buffer[256];
	GetMenuItem(menu,item_info[0][VOTEINFO_ITEM_INDEX],result,sizeof(result));
	Maps_VoteInfo mapv;
	int nominator_credits;
	int credits_return;
	if(strcmp(result,"Extend")==0)
	{
		ExtendMapTimeLimit(900);
		g_Extend_Times++;
		Format(buffer,sizeof(buffer)," \x05[EMC] \x01地图已延长15分钟，延长次数: \x05%d \x01/ \x07%d",g_Extend_Times,Pmap.extend);
		PrintToChatAll(buffer);
		g_ChangeMap_Time = MapChangeTime_MapEnd;
		g_Allow_RTV = true;
		g_Nextmap_Selected = false;
		g_MapVote_Initiated = false;
		g_MapVote_Proceeding = false;
		Nominate_ALLOW = true;
		Call_StartForward(g_RTV_Forward_ExtendSelect);
		Call_Finish();
		ClearNomMapList();
		delete menu;
	}
	else if(strcmp(result,"NoChange")==0)
	{
		Format(buffer,sizeof(buffer)," \x05[EMC] \x07地图未更换");
		PrintToChatAll(buffer);
		g_ChangeMap_Time = MapChangeTime_MapEnd;
		g_Allow_RTV = true;
		g_Nextmap_Selected = false;
		g_MapVote_Initiated = false;
		g_MapVote_Proceeding = false;
		Nominate_ALLOW = true;
		Call_StartForward(g_RTV_Forward_NoChangeSelect);
		Call_Finish();
		ClearNomMapList();
		int timeleft_check;
		GetMapTimeLeft(timeleft_check);
		if(timeleft_check<180)
		{
			PrintToChatAll(" \x05[EMC]地图剩余时间不足，补偿至4分钟");
			ExtendMapTimeLimit(240-timeleft_check);
		}
		delete menu;
	}
	else
	{
		for(int i=0;i<MapVote_List.Length;i++)
		{
			GetArrayArray(MapVote_List,i,mapv,sizeof(mapv));
			if(strcmp(result,mapv.name)==0)
			{
				g_Nextmap_Result = mapv;
				g_LastRound_MapVoteSave = g_Nextmap_Result;
				g_Nextmap_Selected = true;
				char vote_result[256];
				int vote_result_num = item_info[0][VOTEINFO_ITEM_VOTES];
				int pctg = 100*vote_result_num/num_clients;
				Format(vote_result,sizeof(vote_result)," \x05[EMC]投票结果:地图\x07%s\x05得票最高，为\x07%d票(%d",mapv.name,vote_result_num,pctg);
				StrCat(vote_result,sizeof(vote_result),"%%)");
				PrintToChatAll(vote_result);
				Call_StartForward(g_RTV_Forward_NextMapSelected);
				Call_PushString(mapv.name);
				Call_Finish();
			}
		 	else if(mapv.nominated)
		 	{
		 		for(int j = 1; j <= 64 ; j++)
		 		{
		 			if(!IsClientInGame(j)||IsFakeClient(j))	continue;
		 			if(EXGUSERS_GetUserUID(j)==mapv.nominator_uid)
		 			{
						if (g_pStore)
						{
							nominator_credits = Store_GetClientCredits(j);
							credits_return = RoundToFloor(mapv.nominate_cost * 0.5);
							Store_SetClientCredits(j,nominator_credits+credits_return,"预定地图");
						}
						PrintToChatAll(" \x05[EMC] \x09%s \x01预定的 \x07%s \x01未选上，\x07%d \x01积分已退还",mapv.nominator_name,mapv.name,credits_return);
		 			}
		 		}
		 	}
		}
		delete menu;
		Nom_Map_List.Clear();
		ChangeMap();
	}
}

void ChangeMap()
{
	char buffer[256];
	g_Timer_Timeleft = INVALID_HANDLE;
	KillTimerSafe(g_Timer_Timeleft);
	Cvar_SM_NEXTMAP.SetString(g_Nextmap_Result.name,true,false);
	g_Nextmap_Selected = true;
	g_MapVote_Initiated = false;
	g_MapVote_Proceeding = false;
	char current_mapname[64];
	GetCurrentMap(current_mapname,sizeof(current_mapname))
	if(strcmp(g_Nextmap_Result.name,"",false)==0||strcmp(g_Nextmap_Result.name,current_mapname,false)==0)
	{
		RandomMapChange();
		return;
	}
	Format(buffer,sizeof(buffer)," \x05下一张地图为: \09%s %s \x07%s",g_Nextmap_Result.name,g_Nextmap_Result.nominated?" \x01预定者:":"",g_Nextmap_Result.nominated?g_Nextmap_Result.nominator_name:"");
	PrintToChatAll(buffer);
	g_RTV_VotesNum = 0;
	for(int i = 1; i <= 64; i++)
	{
		g_RTV_PlyVoted[i] = false;
	}
	if(g_ChangeMap_Time == MapChangeTime_RoundEnd)
	{
		PrintToChatAll(" \x05[EMC]\x01地图将在本回合结束后更换，再发起rtv可以立刻换图");
		g_Instant_RTV = true;
	}
	else if(g_ChangeMap_Time == MapChangeTime_Instant)
	{
		g_Instant_RTV = false;
		KillTimerSafe(g_WTimer_BeforeMapChange);
		g_WTimer_BeforeMapChange = CreateTimer(4.0,Instant_ChangeMap_Hndl, _,TIMER_FLAG_NO_MAPCHANGE);
	}
	else if(g_ChangeMap_Time == MapChangeTime_MapEnd)
	{
		PrintToChatAll(" \x05[EMC]\x01地图将在时间结束后更换，再发起rtv可以立刻换图");
		g_Instant_RTV = true;
	}
}

Action Instant_ChangeMap_Hndl(Handle timer)
{
	char nextmap[64];
	GetNextMap(nextmap,sizeof(nextmap));
	g_WTimer_BeforeMapChange  = INVALID_HANDLE;
	char buffer[256];
	Format(buffer,sizeof(buffer),"map %s",nextmap);
	ServerCommand(buffer);
}

Action ExtendCommand(int client,int args)
{
	if(!IsClientVIP(client))
	{
		PrintToChat(client," \x05[EMC]\x01您不是VIP，无法使用延长功能!");
		return Plugin_Handled;
	}
	if(!g_Extend_Vote)
	{
		PrintToChat(client," \x05[EMC]\x01已经发起过延长投票!");
		return Plugin_Handled;
	}
	ExtendMapVote(client);
	return Plugin_Handled;
}
void ExtendMapVote(int client)
{
	char buffer[256];
	if(g_MapVote_Proceeding||g_MapVote_Initiated||g_ChangeMap_Time == MapChangeTime_Instant)
	{
		PrintToChat(client," \x05[EMC]\x01当前正在进行地图投票或即将换图，无法使用延长功能");
		return;
	}
	g_Extend_Vote_Yes = 0;
	g_Extend_Vote_No = 0;
	g_Extend_Vote_All = 0;
	if(g_Nextmap_Selected)
	{
		if(g_ChangeMap_Time == MapChangeTime_RoundEnd)
		{
			g_Extend_Vote = false;
			g_Allow_RTV = false;
			g_ChangeMap_Time = MapChangeTime_MapEnd;
			ExtendMapTimeLimit(60);
			Menu menu = CreateMenu(ExtendMapVoteHandler, MenuAction_End | MenuAction_Display | MenuAction_DisplayItem);
			SetMenuPagination(menu, MENU_NO_PAGINATION);
			char client_name[64];
			GetClientName(client,client_name,sizeof(client_name));
			Format(buffer,sizeof(buffer)," \x05[EMC]\x01玩家\x09%s\x01发起了延长地图投票",client_name);
			PrintToChatAll(buffer);
			menu.SetTitle("地图延长投票");
			menu.AddItem("","若同意票数超过总人数2/3",ITEMDRAW_DISABLED);
			menu.AddItem("","地图将延长15分钟",ITEMDRAW_DISABLED);
			menu.AddItem("","同意");
			menu.AddItem("","反对");
			SetVoteResultCallback(menu,ExtendResultHandler1);
			VoteMenuToAll(menu,15);
		}
		else
		{
			if(g_ChangeMap_Time == MapChangeTime_MapEnd)
			{
				g_Extend_Vote = false;
				g_Allow_RTV = false;
				ExtendMapTimeLimit(60);
				Menu menu = CreateMenu(ExtendMapVoteHandler, MenuAction_End | MenuAction_Display | MenuAction_DisplayItem);
				SetMenuPagination(menu, MENU_NO_PAGINATION);
				char client_name[64];
				GetClientName(client,client_name,sizeof(client_name));
				Format(buffer,sizeof(buffer)," \x05[EMC]\x01玩家\x09%s\x01发起了延长地图投票",client_name);
				PrintToChatAll(buffer);
				menu.SetTitle("地图延长投票");
				menu.AddItem("","若同意票数超过总人数2/3",ITEMDRAW_DISABLED);
				menu.AddItem("","地图将延长15分钟",ITEMDRAW_DISABLED);
				menu.AddItem("Yes","同意");
				menu.AddItem("No","反对");
				SetVoteResultCallback(menu,ExtendResultHandler2);
				VoteMenuToAll(menu,15);
			}
		}
	}
}

int ExtendMapVoteHandler(Menu menu, MenuAction action, int param1, int param2)
{
	if(action == MenuAction_End)
	{
		menu = null;
		delete menu;
	}
	if(action == MenuAction_Select)
	{
		char voter_name[64];
		GetClientName(param1,voter_name,sizeof(voter_name));
		if(param2==2)
		{
			g_Extend_Vote_Yes++;
			g_Extend_Vote_All++;
			PrintToConsoleAll("[EMC]延长投票:%s选择了同意",voter_name);
		}
		if(param2==3)
		{
			g_Extend_Vote_No++;
			g_Extend_Vote_All++;
			PrintToConsoleAll("[EMC]延长投票:%s选择了反对",voter_name);
		}
	}
}

public void ExtendResultHandler1(Menu menu, int num_votes, int num_clients, const int[][] client_info, int num_items, const int[][] item_info)
{
	g_Allow_RTV = true;
	PrintToChatAll(" \x05[EMC]\x01延长投票结果:\x07%d\x01/\x09%d\x01,通过率:\x07%.4f%",g_Extend_Vote_Yes,g_Extend_Vote_All,100.0*float(g_Extend_Vote_Yes)/float(g_Extend_Vote_All));
	if(g_Extend_Vote_Yes*3>=g_Extend_Vote_All*2)
	{
		PrintToChatAll(" \x05[EMC]\x01投票延长成功!地图延长15分钟");
		ExtendMapTimeLimit(900);
		g_ChangeMap_Time = MapChangeTime_MapEnd;
	}
	else
	{
		PrintToChatAll(" \x05[EMC]\x01投票延长失败!地图将在回合结束后更换");
		g_ChangeMap_Time = MapChangeTime_RoundEnd;
	}
	delete menu;
}

public void ExtendResultHandler2(Menu menu, int num_votes, int num_clients, const int[][] client_info, int num_items, const int[][] item_info)
{
	g_Allow_RTV = true;
	PrintToChatAll(" \x05[EMC]\x01延长投票结果:\x07%d\x01/\x09%d\x01,通过率:\x07%.4f%",g_Extend_Vote_Yes,g_Extend_Vote_All,float(g_Extend_Vote_Yes)/float(g_Extend_Vote_All));
	if(g_Extend_Vote_Yes*3>=g_Extend_Vote_All*2)
	{
		PrintToChatAll(" \x05[EMC]\x01投票延长成功!地图延长15分钟");
		ExtendMapTimeLimit(900);
		g_ChangeMap_Time = MapChangeTime_MapEnd;
	}
	else
	{
		PrintToChatAll(" \x05[EMC]\x01投票延长失败!地图将在剩余时间结束后更换");
		g_ChangeMap_Time = MapChangeTime_MapEnd;
	}
}