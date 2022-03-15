ConVar g_Cvar_RTV_MaxRounds;
ConVar g_Cvar_RTV_PlayerNeededRatio;
int g_RTV_TotalPlayer;
int g_RTV_VotesNum;
int g_RTV_Rounds;
bool g_RTV_PlyaerVoted[65];
bool g_Allow_RTV;
bool g_RTV_Confirmed;
bool g_Nextmap_Selected;
bool g_MapVote_Initiated;
bool g_MapVote_Proceeding;
int g_WarningTime_BeforeVote;
int g_Extend_Times;
bool g_MapVote_PlayerVoted[65];
bool g_Instant_RTV;
Handle g_WTimer_BeforeVote;
Handle g_WTimer_BeforeMapChange;
Handle g_Timer_Timeleft;
char nextmap_name[64];
enum struct Maps_VoteInfo
{
	char name[PLATFORM_MAX_PATH];
	char name_cn[PLATFORM_MAX_PATH];
	bool nominated;
	char nominator_name[PLATFORM_MAX_PATH];
	int nominator_steamid;
	int difficulty;
	int nominate_cost;
}
enum MapChangeTime
{
	MapChangeTime_Instant,
	MapChangeTime_RoundEnd,
	MapChangeTime_MapEnd
};
MapChangeTime g_ChangeMap_Time;
ArrayList MapVote_List;
ArrayList RandomMap_Candidate;
Maps_VoteInfo g_Nextmap_Result,g_LastRound_MapVoteSave;
void RTVOnPluginStart()
{
	g_WTimer_BeforeVote = INVALID_HANDLE;
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	g_Timer_Timeleft = INVALID_HANDLE;
	MapVote_List = CreateArray(sizeof(Maps_VoteInfo));
	RandomMap_Candidate = CreateArray(sizeof(Maps_VoteInfo));
	ResetRTV();
	RegConsoleCmd("sm_rtv",RTVCommand);
	RegConsoleCmd("sm_ht",RTVCommand);
	RegConsoleCmd("sm_mct",MapChangeTimeCommand);
	g_Cvar_RTV_PlayerNeededRatio = CreateConVar("emc_rtv_needed","0.5","Percentage of RTV Votes needed to start(def:0.45)",0, true, 0.05, true, 1.0);
	g_Cvar_RTV_MaxRounds = CreateConVar("emc_max_rounds","50","Percentage of RTV Votes needed to start(def:0.45)",0, true, 0.05, true, 1.0);
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
void RTVOnMapStart()
{
	g_WTimer_BeforeVote = INVALID_HANDLE;
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	ResetRTV();
	g_RTV_Rounds = 0;
}
void RTVOnMapEnd()
{
	PrintToChatAll("[RTVOnMapEnd]");
	g_RTV_TotalPlayer = 0;
	g_ChangeMap_Time = MapChangeTime_MapEnd;
}
void ResetRTV()
{
	g_WTimer_BeforeVote = INVALID_HANDLE;
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	g_Allow_RTV = true;
	g_Instant_RTV = false;
	g_ChangeMap_Time = MapChangeTime_MapEnd;
	MapVote_List.Clear();
	RandomMap_Candidate.Clear();
	g_WarningTime_BeforeVote = 10;
	KillTimerSafe(g_WTimer_BeforeVote);
	for(int i = 1 ; i<=MaxClients ; i++)
	{
		g_RTV_PlyaerVoted[i] = false;
	}
	g_RTV_VotesNum=0;
	g_RTV_Confirmed = false;
	g_Nextmap_Selected = false;
	g_MapVote_Initiated = false;
	g_MapVote_Proceeding = false;
	Nominate_ALLOW = true;
	g_Nextmap_Result.name = "";
	g_Nextmap_Result.nominated = false;
	g_Nextmap_Result.nominator_name = "";
	g_Nextmap_Result.nominator_steamid = 0;
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
		Format(buffer,sizeof(buffer)," \x05当前地图:\x09%s%s%s",map_name,g_LastRound_MapVoteSave.nominated?"\x05预定者":"",g_LastRound_MapVoteSave.nominated?g_LastRound_MapVoteSave.nominator_name:"");
		PrintToChatAll(buffer);
		Format(buffer,sizeof(buffer),"[EMC]%s%s(STEAMID:%d)",g_LastRound_MapVoteSave.nominated?"预定者":"当前地图为野生",g_LastRound_MapVoteSave.nominated?g_LastRound_MapVoteSave.nominator_name:"",g_LastRound_MapVoteSave.nominator_steamid);
		PrintToServer(buffer);
	}
}
void RTVOnRoundEnd()
{
	if(g_ChangeMap_Time == MapChangeTime_RoundEnd)
	{
		KillTimerSafe(g_WTimer_BeforeMapChange);
		g_WTimer_BeforeMapChange = CreateTimer(4.0,ChangeMap_RoundEnd_Hndl, _,TIMER_FLAG_NO_MAPCHANGE);
	}
}
Action ChangeMap_RoundEnd_Hndl(Handle timer)
{
	char nextmap[64];
	GetNextMap(nextmap,sizeof(nextmap));
	g_WTimer_BeforeMapChange = INVALID_HANDLE;
	ForceChangeLevel(nextmap,"[EMC]RoundEnd Change Map");
}
public void OnClientSayCommand_Post(int client, const char[] command, const char[] sArgs)
{
	if(!IsClientInGame(client)||!client||IsChatTrigger())	return;
	if(strcmp(sArgs,"rtv",false)==0||strcmp(sArgs,"rockthevote",false)==0||strcmp(sArgs,"ht",false)==0)
	{
		AttemptRTV(client);
	}

}
public void RTVOnClientConnected(int client)
{
	if(!IsFakeClient(client))
	{
		g_RTV_TotalPlayer++;
	}
}
public void RTVOnClientDisconnect(int client)
{
	if(!IsFakeClient(client))
	{
		g_RTV_TotalPlayer--;
		if(g_RTV_PlyaerVoted[client])
		{
			g_RTV_PlyaerVoted[client]=false;
			g_RTV_VotesNum--;
		}
	}
	if(g_RTV_TotalPlayer == 0)	return;
	if(g_RTV_VotesNum>=g_RTV_TotalPlayer)
	{
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
	GetMapTimeLeft(timeleft);
	g_Timer_Timeleft = INVALID_HANDLE;
	KillTimerSafe(g_Timer_Timeleft);
	char buffer[256];
	Format(buffer,sizeof(buffer)," \x05地图时间:\x01%d秒",timeleft);
	PrintToServer(buffer);
	if(timeleft<0)	return;
	g_Timer_Timeleft = CreateTimer(float(timeleft)-300.0,TIMELEFT_AUTOVOTEMAP_HNDL, _,TIMER_FLAG_NO_MAPCHANGE);
}
Action TIMELEFT_AUTOVOTEMAP_HNDL(Handle timer)
{
	if(g_Nextmap_Selected||g_MapVote_Initiated||g_MapVote_Proceeding)
	{
		return Plugin_Handled;
	}
	PrintToServer(" \x05[EMC]\x01地图剩余时间已到5分钟，准备换图");
	g_ChangeMap_Time = MapChangeTime_MapEnd;
	g_Allow_RTV = false;
	StartMapVote(MapChangeTime_MapEnd);
	g_Timer_Timeleft = INVALID_HANDLE;
	return Plugin_Handled;
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
	if(g_Nextmap_Selected||g_Instant_RTV)
	{
		AttemptInstantRTV(client);
		return;
	}
	if(!g_Allow_RTV)
	{
		PrintToChat(client," \x05[EMC]\x01当前无法发起换图投票");
		return;
	}
	if(g_MapVote_Proceeding||g_MapVote_Initiated)
	{
		PrintToChat(client," \x05[EMC]\x01地图投票正在进行中");
		return;		
	}
	int RTV_PlayerNeeded;

	RTV_PlayerNeeded = RoundToFloor(g_RTV_TotalPlayer * GetConVarFloat(g_Cvar_RTV_PlayerNeededRatio)); 
	RTV_PlayerNeeded = Max(RTV_PlayerNeeded,1);
	if(g_RTV_PlyaerVoted[client])
	{
		Format(buffer,sizeof(buffer)," \x05[EMC]\x01您已要求发起换图投票(当前%d票，还需%d票",g_RTV_VotesNum,RTV_PlayerNeeded);
		PrintToChat(client,buffer);
		return;
	}
	g_RTV_VotesNum++;
	char clientname[256];
	GetClientName(client,clientname,sizeof(clientname));
	Format(buffer,sizeof(buffer)," \x05[EMC]\x01%s要求发起换图投票:%d票/%d票(仍需%d票)",clientname,g_RTV_VotesNum,RTV_PlayerNeeded,RTV_PlayerNeeded-g_RTV_VotesNum);
	PrintToChatAll(buffer);
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
	RTV_PlayerNeeded = RoundToFloor(g_RTV_TotalPlayer * GetConVarFloat(g_Cvar_RTV_PlayerNeededRatio)); 
	RTV_PlayerNeeded = Max(RTV_PlayerNeeded,1);	
	if(g_RTV_PlyaerVoted[client])
	{
		Format(buffer,sizeof(buffer)," \x05[EMC]\x01您已要求立即换图(当前%d票，还需%d票",g_RTV_VotesNum,RTV_PlayerNeeded);
		PrintToChat(client,buffer);
		return;
	}
	g_RTV_VotesNum++;
	char clientname[256];
	GetClientName(client,clientname,sizeof(clientname));
	Format(buffer,sizeof(buffer)," \x05[EMC]\x01%s要求立即换图:%d票/%d票(仍需%d票)",clientname,g_RTV_VotesNum,RTV_PlayerNeeded,RTV_PlayerNeeded-g_RTV_VotesNum);
	PrintToChatAll(buffer);
	if(g_RTV_VotesNum>=RTV_PlayerNeeded)
	{
		g_ChangeMap_Time = MapChangeTime_Instant;
		ChangeMap();
	}
}
void StartMapVote(MapChangeTime when)
{
	if(g_RTV_TotalPlayer == 0)	return;
	Nominate_ALLOW = false;
	g_MapVote_Initiated = true;
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
	g_WarningTime_BeforeVote = 10;
	CreateNextMapVote();
	g_WTimer_BeforeVote = INVALID_HANDLE;
	return Plugin_Stop;
}

void CreateNextMapVote()
{
	int Random_Num;
	char buffer[256];
	if(g_RTV_TotalPlayer == 0)	return;
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
	for(int i = 0 ; i < Nom_Map_List.Length; i++)
	{
		GetArrayArray(Nom_Map_List,i,nomlog,sizeof(nomlog));
		Maps.GetArray(nomlog.name,map,sizeof(map));
		mapv.name = map.name;
		mapv.nominated = true;
		mapv.nominator_name = nomlog.nominator_name;
		mapv.nominator_steamid = nomlog.nominator_steamid;
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
			if(map.available&&map.exist&&map.download&&map.random)
			{	
				if(!isNominated(map))
				{
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
		char test[64];
		Format(test,sizeof(test),"随机地图池总数:%d",RandomMap_Candidate.Length);
		PrintToServer(test);
		while(RandomMap_Picked<Random_Num||(!RandomMap_Candidate.Length))
		{
			RandomMap_Order = GetURandomInt()%(RandomMap_Candidate.Length);	//随机序号0~Length-1,索引同理
			GetArrayArray(RandomMap_Candidate,RandomMap_Order,mapv,sizeof(mapv));
			RandomMap_Candidate.Erase(RandomMap_Order);
			MapVote_List.PushArray(mapv);

			RandomMap_Picked++;
		}
	}
	for(int i = 0; i<MapVote_List.Length ; i++)
	{
		GetArrayArray(MapVote_List,i,mapv,sizeof(mapv));
		Format(buffer,sizeof(buffer),"[%s]%s\n%s%s",difficulty_name[mapv.difficulty],mapv.name,mapv.name_cn,mapv.nominated?"[订]":"");
		menu.AddItem(mapv.name,buffer,ITEMDRAW_DEFAULT);
	}
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
	SetVoteResultCallback(menu,MapVoteHandler);
	VoteMenuToAll(menu,15);
}
int NextMapVoteHandler(Menu menu, MenuAction action, int param1, int param2) 
{
	char buffer[256];
	if (action == MenuAction_End)
	{
		menu.Close();
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
			Format(buffer,sizeof(buffer)," \x05[EMC]\x01随机选择了地图:%s",g_Nextmap_Result.name);
			PrintToChatAll(buffer);
			for(int i=0;i<MapVote_List.Length;i++)
			{
				GetArrayArray(MapVote_List,i,mapv,sizeof(mapv));
				if(strcmp(mapv.name,g_Nextmap_Result.name)) continue;
			 	if(mapv.nominated)
			 	{
			 		for(int j = 1; j <= 64 ; j++)
			 		{
			 			if(!IsClientInGame(j)||IsFakeClient(j))	continue;
			 			if(GetSteamAccountID(j,true)==mapv.nominator_steamid)
			 			{
			 				nominator_credits = Store_GetClientCredits(j);
			 				credits_return = RoundToFloor(mapv.nominate_cost * 0.5);
							Store_SetClientCredits(j,nominator_credits+credits_return);
							PrintToChatAll(" \x05[EMC]\x09%s\x01预定的\x07%s\x01未选上，\x07%d\x01积分已退还",mapv.nominator_name,mapv.name,credits_return);	 				
			 			}
			 		}
			 	}
			}
			g_ChangeMap_Time = MapChangeTime_RoundEnd;
			ChangeMap();
		}
	}
}

public void MapVoteHandler(Menu menu, int num_votes, int num_clients, const int[][] client_info, int num_items, const int[][] item_info)
{
	char result[64];
	char buffer[256];
	GetMenuItem(menu,item_info[0][VOTEINFO_ITEM_INDEX],result,sizeof(result));
	PrintToChatAll(buffer);
	Maps_VoteInfo mapv;
	int nominator_credits;
	int credits_return;
	if(strcmp(result,"Extend")==0)
	{
		ExtendMapTimeLimit(900);
		g_Extend_Times++;
		Format(buffer,sizeof(buffer)," \x05[EMC]\x01地图已延长15分钟，延长次数:\x05%d\x01/\x07%d",g_Extend_Times,Pmap.extend);
		PrintToChatAll(buffer);
		g_ChangeMap_Time = MapChangeTime_MapEnd;
		g_Allow_RTV = true;
		g_Nextmap_Selected = false;
		g_MapVote_Initiated = false;
		g_MapVote_Proceeding = false;
		Nominate_ALLOW = true;
		g_RTV_VotesNum = 0;
	}
	else if(strcmp(result,"NoChange")==0)
	{
		Format(buffer,sizeof(buffer),"地图未更换");
		PrintToChatAll(buffer);
		g_ChangeMap_Time = MapChangeTime_MapEnd;
		g_Allow_RTV = true;
		g_Nextmap_Selected = false;
		g_MapVote_Initiated = false;
		g_MapVote_Proceeding = false;
		Nominate_ALLOW = true;
		g_RTV_VotesNum = 0;
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
			}
		 	else if(mapv.nominated)
		 	{
		 		for(int j = 1; j <= 64 ; j++)
		 		{
		 			if(!IsClientInGame(j)||IsFakeClient(j))	continue;
		 			if(GetSteamAccountID(j,true)==mapv.nominator_steamid)
		 			{
		 				nominator_credits = Store_GetClientCredits(j);
		 				credits_return = RoundToFloor(mapv.nominate_cost * 0.5);
						Store_SetClientCredits(j,nominator_credits+credits_return);
						PrintToChatAll(" \x05[EMC]\x09%s\x01预定的\x07%s\x01未选上，\x07%d\x01积分已退还",mapv.nominator_name,mapv.name,credits_return);	 				
		 			}
		 		}
		 	}
		}
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
	Format(buffer,sizeof(buffer)," \x05下一张地图为:%s %s%s",g_Nextmap_Result.name,g_Nextmap_Result.nominated?"预定者:":"",g_Nextmap_Result.nominated?g_Nextmap_Result.nominator_name:"");
	PrintToChatAll(buffer);
	for(int i = 1 ;i <= 64; i++)
	{
		g_MapVote_PlayerVoted[i]=false;
	}
	if(g_ChangeMap_Time == MapChangeTime_RoundEnd)
	{ 
		PrintToChatAll(" \x05[EMC]\x01地图将在本回合结束后更换，再发起rtv可以立刻换图");
		g_Instant_RTV = true;
		g_RTV_VotesNum = 0;
	}
	else if(g_ChangeMap_Time == MapChangeTime_Instant)
	{	
		g_Instant_RTV = false;
		KillTimerSafe(g_WTimer_BeforeMapChange);
		g_RTV_VotesNum = 0;
		g_WTimer_BeforeMapChange = CreateTimer(4.0,Instant_ChangeMap_Hndl, _,TIMER_FLAG_NO_MAPCHANGE);
	}
	else if(g_ChangeMap_Time == MapChangeTime_MapEnd)
	{
		PrintToChatAll(" x05[EMC]\x01地图将在时间结束后更换，再发起rtv可以立刻换图");	
		g_Instant_RTV = true;	
		g_RTV_VotesNum = 0;
	}
}

Action Instant_ChangeMap_Hndl(Handle timer)
{
	char nextmap[64];
	GetNextMap(nextmap,sizeof(nextmap));
	g_WTimer_BeforeMapChange  = INVALID_HANDLE;
	ForceChangeLevel(nextmap,"[EMC]Instant Change Map");
}