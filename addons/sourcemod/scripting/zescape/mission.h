enum TASK_TYPE
{
	ZM_INFECT,
	HM_KILLZM,
	ZM_DMGTAKE,
	HM_DMGMAKE,
	HM_HIT,
	HM_PASS,
	HM_LEAD,
	HM_NADE,
}
enum struct TASK
{
	int stage;
	int num[3];
	int exp_base;
	int period;
	char name[64];
	TASK_TYPE type;
	int difficulty;	//PASS
	int tag;	//PASS
}
enum struct Missions
{
	int id;
	int start_timestamp;
	int end_timestamp;
	char shortname[64];
	char name[64];
	char cnname[128];
	char playerdbname[64];
	int max_level;
	int level_exp;
	int daily_timestamp;
	int weekly_timestamp;
}
enum struct PlayerMissionInfo
{
	int uid;
	int exp;
	int lvl;
	int taskdata[16];
	int taskstage[16];
	int dataupdate_time;
	int loaded;	//temp
	int sp;
	int vip;
	int emoney;
	int nt[2]; //WIP
	int challenge[5];
}
enum struct PlayerMissionRoundBuffer
{
	int bosshits;
	int zminfect;
	int zmtake;
	int motherzm;
	int hmdmg;
	int hmkill;
	int pay;
}
PlayerMissionRoundBuffer playermission_rounddata[65];
PlayerMissionRoundBuffer nullrounddata;
ArrayList Current_Mission_Tasklist;
bool g_ValidMission_Exist;
PlayerMissionInfo playermission_list[65];
PlayerMissionInfo nullpmi;
Missions Current_Mission;
LEVEL_LOG Mission_Current_Level;
int g_Daily_Task_Exp_Factor = 2;
int g_Weekly_Task_Exp_Factor = 2;
int round_starttime;
int round_endtime;
const int mission_lowest_playernum = 30;
void MissionOnPluginStart()
{
	round_starttime = GetTime();
	Mission_Current_Level.id=-1;
	g_ValidMission_Exist = false;
	Current_Mission.id = 0;
	nullrounddata.bosshits = 0;
	nullrounddata.zminfect = 0;
	nullrounddata.motherzm = 0;
	nullrounddata.hmdmg = 0;
	nullrounddata.hmkill = 0;
	nullrounddata.pay = 0;
	nullrounddata.zmtake = 0;
	RegConsoleCmd("sm_mission",MissionMenuCommand);
	RegConsoleCmd("sm_ms",MissionMenuCommand);
	RegConsoleCmd("sm_dxd",MissionMenuCommand);	//Just For Fun!
	RegConsoleCmd("sm_msr",MissionInfoReloadCommand);
	RegAdminCmd("sm_clearch",ClearChCommand,ADMFLAG_GENERIC);
	if(!isDbConnected())	return;			//未连接，return，通过Db连接函数的函数执行Post，已连接则通直接Post使得换图后重载各插件数据
	CheckValidMission();
	ReloadAllPlayerMissionInfo();
}
Action ClearChCommand(int client,int args)
{
	for(int i=1;i<=64;i++)
	{
		if(!IsClientInGame(i))	continue;
		if(IsFakeClient(i))		continue;
		playermission_list[i].challenge[0]=0;
		playermission_list[i].challenge[1]=0;
		playermission_list[i].challenge[2]=0;
		playermission_list[i].challenge[3]=0;
		playermission_list[i].challenge[4]=0;
		playermission_rounddata[i]=nullrounddata;
		UpdatePlayerMissionInfo(i);
	}
}
void MissionTimeCheck()
{
	int current_time = GetTime();
	CheckValidMission();
	if(Current_Mission.id<=0)	return;
	bool Timestampdailychanged=false;
	bool Timestampweeklychanged=false;
	PrintToConsoleAll(" \x05[调试]检测当前时间戳...%d，当前活动每日时间戳%d,每周时间戳%d",current_time,Current_Mission.daily_timestamp,Current_Mission.weekly_timestamp);
	if(current_time - Current_Mission.daily_timestamp >=86400)
	{
		int days_delta = (current_time - Current_Mission.daily_timestamp)/86400;
		PrintToChatAll(" [任务系统]新活动日开启-旧时间戳:%d 新时间戳%d 经过:%d天",Current_Mission.daily_timestamp,current_time,days_delta);
		Current_Mission.daily_timestamp += days_delta*86400;
		Timestampdailychanged = true;
	}
	if(current_time - Current_Mission.weekly_timestamp >= 604800)
	{
		int weeks_delta = (current_time - Current_Mission.weekly_timestamp)/604800;
		PrintToChatAll(" [任务系统]新活动周开启-旧时间戳:%d 新时间戳%d 经过:%d周",Current_Mission.weekly_timestamp,current_time,weeks_delta);
		Current_Mission.weekly_timestamp += weeks_delta*604800;
		Timestampweeklychanged = true;
	}
	if(Timestampweeklychanged)
	{
		ClearPlayerMissionInfo(1);
	}
	else
	{
		if(Timestampdailychanged)
		{
			ClearPlayerMissionInfo(0);
		}
	}
	if(Timestampdailychanged||Timestampweeklychanged)
	{
		UpdateMissionTimeStamp();
	}
}
void UpdateMissionTimeStamp()
{
	char query[512];
	Format(query,sizeof(query),"UPDATE ZEMISSION SET DAILY = %d, WEEKLY =%d WHERE ID = %d",Current_Mission.daily_timestamp,Current_Mission.weekly_timestamp,Current_Mission.id);
	DbTQuery(DbQueryErrorCallback,query);
}
void PlayerMissionDailyUpdate(int client)
{
	if(client<=0||client>=65)	return;
	PrintToServer(" \x05[调试]玩家%d的每日任务数据过期，清空中...",client);
	for(int i=0;i<=8;i++)
	{
		playermission_list[client].taskdata[i]=0;
		playermission_list[client].taskstage[i]=0;
	}
	playermission_list[client].loaded = 1;
	UpdatePlayerMissionInfo(client);
}
void PlayerMissionWeeklyUpdate(int client)
{
	if(client<=0||client>=65)	return;
	PrintToServer(" \x05[调试]玩家%d的每周任务数据过期，清空中...",client);
	for(int i=9;i<=14;i++)
	{
		playermission_list[client].taskdata[i]=0;
		playermission_list[client].taskstage[i]=0;
	}
	playermission_list[client].challenge[0]=0;
	playermission_list[client].challenge[1]=0;
	playermission_list[client].challenge[2]=0;
	playermission_list[client].challenge[3]=0;
	playermission_list[client].challenge[4]=0;
	playermission_list[client].loaded = 1;
	UpdatePlayerMissionInfo(client);
}

Action MissionInfoReloadCommand(int client,int args)
{
	if(client<=0||client>64)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	LoadPlayerMissionInfo(client);
	return Plugin_Handled;
}
void LoadPlayerMissionInfo(int client)
{
	char query[256];
	int uid;
	if(client<=0||client>=65)	return;
	if(Current_Mission.id<=0)	return;
	if(IsFakeClient(client))	return;
	if(!isDbConnected())
	{
		playermission_list[client].loaded=0;
		PrintToChat(client,"\x05[任务系统]\x01加载玩家%d的[用户数据]时出错，数据库未载入（请等待下一回合，\x07或输入!msr再试\x01)",client);
		return;
	}
	playermission_list[client]=nullpmi;
	if(!IsClientInGame(client))	return;
	USER_LOG user;
	EXGUSERS_GetUserInfo(client,user);
	if(user.loaded==0||user.uid==0)
	{
		playermission_list[client].loaded=0;
		PrintToChat(client,"\x05[任务系统]\x01加载玩家%d的[用户数据]时出错，用户系统未载入（请等待下一回合，\x07或输入!msr再试\x01)",client);
		return;
	}
	uid = user.uid;
	Format(query,sizeof(query),"SELECT * FROM %s WHERE UID = %d",Current_Mission.playerdbname,uid);
	DataPack dp = new DataPack();
	dp.WriteCell(client);
	dp.WriteCell(uid);
	DbTQuery(LoadPlayerMissionInfoCallBack,query,dp);
}

void LoadPlayerMissionInfoCallBack(Handle owner, Handle hndl, char[] error, DataPack dp)
{
	char query[512];
	dp.Reset();
	int client = dp.ReadCell();
	int uid = dp.ReadCell();
	delete dp;
	playermission_list[client].uid=uid;
	int current_time = GetTime();
	playermission_list[client].loaded=0;
	if(!SQL_FetchRow(hndl))
	{
		PrintToConsoleAll("[调试]未检测到玩家%d赛季活动数据，注册新玩家%d 当前时间:%d",client,uid,current_time);
		Format(query,sizeof(query),"INSERT INTO %s (UID,TIMESTAMP,VIP) VALUES(%d,%d,0)",Current_Mission.playerdbname,uid,current_time);
	}
	else
	{
		playermission_list[client].dataupdate_time = DbFetchInt(hndl,"TIMESTAMP");
		PrintToConsoleAll("[调试]检测到玩家%d赛季活动数据，载入[UID:%d]数据 上一次数据更新时间戳:%d 当前时间戳:%d 当前赛季活动日时间戳:%d",client,uid,playermission_list[client].dataupdate_time,current_time,Current_Mission.daily_timestamp);
		playermission_list[client].exp = DbFetchInt(hndl,"EXP");
		playermission_list[client].lvl = DbFetchInt(hndl,"LVL");
		playermission_list[client].taskstage[0]=DbFetchInt(hndl,"DT1ST");
		playermission_list[client].taskstage[1]=DbFetchInt(hndl,"DT2ST");
		playermission_list[client].taskstage[2]=DbFetchInt(hndl,"DT3ST");
		playermission_list[client].taskstage[3]=DbFetchInt(hndl,"DT4ST");
		playermission_list[client].taskstage[4]=DbFetchInt(hndl,"DT5ST");
		playermission_list[client].taskstage[5]=DbFetchInt(hndl,"DT6ST");
		playermission_list[client].taskstage[6]=DbFetchInt(hndl,"DT7ST");
		playermission_list[client].taskstage[7]=DbFetchInt(hndl,"DT8ST");
		playermission_list[client].taskstage[8]=DbFetchInt(hndl,"DT9ST");
		playermission_list[client].taskstage[9]=DbFetchInt(hndl,"WT1ST");
		playermission_list[client].taskstage[10]=DbFetchInt(hndl,"WT2ST");
		playermission_list[client].taskstage[11]=DbFetchInt(hndl,"WT3ST");
		playermission_list[client].taskstage[12]=DbFetchInt(hndl,"WT4ST");
		playermission_list[client].taskstage[13]=DbFetchInt(hndl,"WT5ST");
		playermission_list[client].taskstage[14]=DbFetchInt(hndl,"WT6ST");
		playermission_list[client].taskdata[0]=DbFetchInt(hndl,"DINFECT");
		playermission_list[client].taskdata[1]=DbFetchInt(hndl,"DKILLZM");
		playermission_list[client].taskdata[2]=DbFetchInt(hndl,"DDMGTAKE");
		playermission_list[client].taskdata[3]=DbFetchInt(hndl,"DDMGMAKE");
		playermission_list[client].taskdata[4]=DbFetchInt(hndl,"DHIT");
		playermission_list[client].taskdata[5]=DbFetchInt(hndl,"DPASS");
		playermission_list[client].taskdata[6]=DbFetchInt(hndl,"DPASS2");
		playermission_list[client].taskdata[7]=DbFetchInt(hndl,"DPASS3");
		playermission_list[client].taskdata[8]=DbFetchInt(hndl,"DNADE");
		playermission_list[client].taskdata[9]=DbFetchInt(hndl,"WINFECT");
		playermission_list[client].taskdata[10]=DbFetchInt(hndl,"WKILLZM");
		playermission_list[client].taskdata[11]=DbFetchInt(hndl,"WDMGTAKE");
		playermission_list[client].taskdata[12]=DbFetchInt(hndl,"WDMGMAKE");
		playermission_list[client].taskdata[13]=DbFetchInt(hndl,"WNADE");
		playermission_list[client].taskdata[14]=DbFetchInt(hndl,"WPASS");
		playermission_list[client].emoney = DbFetchInt(hndl,"EMONEY");
		playermission_list[client].sp = DbFetchInt(hndl,"SP");
		playermission_list[client].vip = DbFetchInt(hndl,"VIP");
		playermission_list[client].challenge[1] = DbFetchInt(hndl,"CH1");
		playermission_list[client].challenge[2] = DbFetchInt(hndl,"CH2");
		playermission_list[client].challenge[3] = DbFetchInt(hndl,"CH3");
		playermission_list[client].challenge[4] = DbFetchInt(hndl,"CH4");
		playermission_list[client].challenge[0] = DbFetchInt(hndl,"CH");
		if(playermission_list[client].dataupdate_time<Current_Mission.daily_timestamp)
		{
			PlayerMissionDailyUpdate(client);
		}
		if(playermission_list[client].dataupdate_time<Current_Mission.weekly_timestamp)
		{
			PlayerMissionWeeklyUpdate(client);
		}
		Format(query,sizeof(query),"UPDATE %s SET TIMESTAMP = %d WHERE UID = %d",Current_Mission.playerdbname,current_time,uid);
	}
	playermission_list[client].dataupdate_time = current_time;
	PrintToServer(query);
	PrintToConsoleAll(query);
	DbTQuery(DbQueryErrorCallback,query);
	playermission_list[client].loaded = 1;
}

void ReloadAllPlayerMissionInfo()
{
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				PrintToServer("[调试]全体重载-玩家%d的大行动数据",i);
				LoadPlayerMissionInfo(i);
			}
		}
	}
}

void ClearPlayerMissionInfo(int weekly=0)
{
	char query[512];
	if(weekly)
	{
	}
	PrintToChatAll(" \x05[任务系统]清空大行动每日任务数据....");
	Format(query,sizeof(query),"UPDATE %s SET DINFECT = 0, DKILLZM = 0, DDMGTAKE=0, DDMGMAKE=0, DHIT=0, DPASS=0, DPASS2=0, DPASS3=0, DNADE=0, DT1ST=0, DT2ST=0, DT3ST=0, DT4ST=0, DT5ST=0, DT6ST=0, DT7ST=0, DT8ST=0, DT9ST=0",Current_Mission.playerdbname);
	DbTQuery(DbClearPlayerMissionInfoCallback,query);
}

void DbClearPlayerMissionInfoCallback(Handle owner, Handle hndl, char[] error, any data)
{
	ReloadAllPlayerMissionInfo();
}

void UpdatePlayerMissionInfo(int client,int force=0)
{
	if((!force)&&(playermission_list[client].loaded==0||playermission_list[client].uid==0))
	{
		PrintToServer("[任务系统]重载因数据库出错而未载入的玩家%d的任务数据",client);
		LoadPlayerMissionInfo(client);
		return;
	}

	char query[2048];
	int uid = playermission_list[client].uid;
	int lvl = playermission_list[client].lvl;
	int exp = playermission_list[client].exp;
	int taskdata[16];
	int taskstage[16];
	int sp = playermission_list[client].sp;
	int vip = playermission_list[client].vip;
	int emoney = playermission_list[client].emoney;
	for(int i =0;i<=14;i++)
	{
		taskdata[i]=playermission_list[client].taskdata[i];
		taskstage[i]=playermission_list[client].taskstage[i];
	}

	int current_time = GetTime();
	PrintToServer("[任务系统][UID:%d]保存玩家%d数据",uid,client);
	Format(query,sizeof(query),"UPDATE %s SET LVL = %d, EXP = %d, DINFECT = %d,DKILLZM = %d,DDMGTAKE = %d,DDMGMAKE = %d,DHIT = %d, DPASS = %d, DPASS2 = %d, DPASS3 = %d, DNADE = %d, WINFECT = %d, WKILLZM = %d, WDMGTAKE = %d, WDMGMAKE = %d, WNADE = %d, WPASS = %d, DT1ST = %d, DT2ST = %d, DT3ST = %d, DT4ST = %d, DT5ST =%d, DT6ST = %d, DT7ST = %d, DT8ST = %d, DT9ST = %d, WT1ST = %d, WT2ST = %d, WT3ST = %d, WT4ST = %d, WT5ST = %d, WT6ST = %d, TIMESTAMP = %d, SP = %d, VIP = %d, EMONEY = %d, CH1 = %d, CH2 = %d, CH3 = %d, CH4 = %d, CH = %d WHERE UID = %d",Current_Mission.playerdbname,lvl,exp,taskdata[0],taskdata[1],taskdata[2],taskdata[3],taskdata[4],taskdata[5],taskdata[6],taskdata[7],taskdata[8],taskdata[9],taskdata[10],taskdata[11],taskdata[12],taskdata[13],taskdata[14],taskstage[0],taskstage[1],taskstage[2],taskstage[3],taskstage[4],taskstage[5],taskstage[6],taskstage[7],taskstage[8],taskstage[9],taskstage[10],taskstage[11],taskstage[12],taskstage[13],taskstage[14],current_time,sp,vip,emoney,playermission_list[client].challenge[1],playermission_list[client].challenge[2],playermission_list[client].challenge[3],playermission_list[client].challenge[4],playermission_list[client].challenge[0],uid);
	DbTQuery(DbQueryErrorCallback,query);
	if(force==1)
	{
		playermission_list[client].uid=0;
		playermission_list[client].lvl=0;
		playermission_list[client].exp=0
		playermission_list[client].sp=0;
		playermission_list[client].vip=0;
		playermission_list[client].emoney=0;
		for(int i=0;i<=14;i++)
		{
			playermission_list[client].taskdata[i]=0;
			playermission_list[client].taskstage[i]=0;
		}
		for(int i=0;i<=4;i++)
		{
			playermission_list[client].challenge[i]=0;
		}
	}
}

void MissionOnClientConnected(int client)
{
	playermission_rounddata[client]=nullrounddata;
	playermission_list[client].loaded=0;
	playermission_list[client].uid = 0;
	LoadPlayerMissionInfo(client);
}

void MissionOnClientDisconnect(int client)
{
	playermission_rounddata[client]=nullrounddata;
	if(playermission_list[client].loaded)
	{
		UpdatePlayerMissionInfo(client,1);
	}
	playermission_list[client].loaded = 0;
}
void MissionOnEntitySpawned(int entity, const char[] classname)
{
	if(strcmp(classname,"func_physbox")==0||strcmp(classname,"func_physbox_multiplayer")==0||strcmp(classname,"trigger_multiple")==0)
	{
		char current_map[64];
		GetCurrentMap(current_map,sizeof(current_map));
		if(StrContains(current_map,"lotr_minas_tirith",false)!=-1||StrContains(current_map,"paramina_rift",false)!=-1)
		{
			char ent_name[64];
			GetEntPropString(entity,Prop_Data,"m_iName",ent_name,sizeof(ent_name));
			if(StrContains(ent_name,"ph_item_balrog_hp",false)!=-1)
			{
				HookSingleEntityOutput(entity,"OnHealthChanged",MissionOnHitSP);
			}
			if(StrContains(ent_name,"Crystal_Physbox",false)!=-1)
			{
				HookSingleEntityOutput(entity,"OnBreak",MissionOnHitSP2);
			}
		}
    }
}
void MissionOnHitSP(const char[] output, int caller, int activator, float delay)
{

	if(activator<=0||activator>64)	return;
	if(!IsClientInGame(activator))	return;
	if(IsFakeClient(activator))	return;
	if(ZR_IsClientHuman(activator))
	{
		playermission_rounddata[activator].bosshits++;
	}
}
void MissionOnHitSP2(const char[] output, int caller, int activator, float delay)
{

	if(activator<=0||activator>64)	return;
	if(!IsClientInGame(activator))	return;
	if(IsFakeClient(activator))	return;
	if(ZR_IsClientHuman(activator))
	{
		if(playermission_list[activator].challenge[4]!=1)
		{
			playermission_list[activator].challenge[4]==1;
			playermission_list[activator].emoney+=300;
			GrantExp(activator,300);
			PrintToChat(activator," \x05[任务系统]\x01恭喜你完成了当前悬赏任务的\x07额外任务\x01并获得\x07 300 \x01碎片和\x07 300\x01经验");
			UpdatePlayerMissionInfo(activator);
		}
	}
}
void MissionOnHit(int client)
{
	if(GetClientCount(true)<mission_lowest_playernum)	return;
	playermission_list[client].taskdata[4]++;
}
void MissionOnLevelSet(any level[sizeof(LEVEL_LOG)])
{
	Mission_Current_Level = level;
}
void MissionOnRoundBuffer_MotherZM(int client)
{
	playermission_rounddata[client].motherzm = 1;
}
void MissionOnRoundStart()
{
	for(int i=1;i<=64;i++)
	{
		playermission_rounddata[i]=nullrounddata;
	}
	round_starttime = GetTime();
}
void MissionOnRoundEnd(int winner)
{
	int player_num = GetClientCount(true);
	char map_name[64];
	int player_pass = 0;
	bool admin_online = false;
	GetCurrentMap(map_name,sizeof(map_name));
	round_endtime = GetTime();
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				if(winner==3&&IsPlayerAlive(i)&&ZR_IsClientHuman(i)&&player_num>=mission_lowest_playernum)
				{
					if(playermission_list[i].taskdata[5]<=10000)
					{
						playermission_list[i].taskdata[5]++;
					}
					if(playermission_list[i].taskdata[6]<=10000&&Pmap.difficulty>=2)
					{
						playermission_list[i].taskdata[6]++;
					}
					if(playermission_list[i].taskdata[7]<=10000&&Pmap.tag&label_code[3])
					{
						playermission_list[i].taskdata[7]++;
					}
					if(playermission_list[i].taskdata[14]<=10000&&Pmap.tag&label_code[10])
					{
						playermission_list[i].taskdata[14]++;
					}
					if(playermission_rounddata[i].hmkill >= 1)
					{
						if(playermission_list[i].challenge[1]!=1)
						{
							playermission_list[i].challenge[1]=1;
							PrintToChat(i," \x05[任务系统]\x01恭喜你完成了当前悬赏任务的\x07任务1\x01并获得\x07 100 \x01碎片和\x07 100\x01经验");
							playermission_list[i].emoney+=100;
							GrantExp(i,100);
						}
					}
				}
				if(StrContains(map_name,"obj",false)!=-1)
				{
					if(playermission_list[i].challenge[2]!=1)
					{
						if(playermission_rounddata[i].hmdmg>=200000)
						{
							playermission_list[i].challenge[2]=1;
							PrintToChat(i," \x05[任务系统]\x01恭喜你完成了当前悬赏任务的\x07任务2\x01并获得\x07 300 \x01碎片和\x07 300\x01经验");
							playermission_list[i].emoney+=300;
							GrantExp(i,300);
						}
					}
				}
				if(playermission_list[i].challenge[3]!=1&&playermission_rounddata[i].bosshits>=50&&StrContains(map_name,"lotr_minas_tirith",false)!=-1)
				{
					playermission_list[i].challenge[3]=1;
					PrintToChat(i," \x05[任务系统]\x01恭喜你完成了当前悬赏任务的\x07任务3\x01并获得\x07 300 \x01碎片和\x07 300\x01经验");
					playermission_list[i].emoney+=300;
					GrantExp(i,300);
				}
				/*if(IsPlayerAlive(i))
				{
					if(ZR_IsClientHuman(i))
					{
						player_pass++;
						if(player_num>=30&&(round_endtime - round_starttime >= 150))
						{
							if(StrContains(map_name,"mako_reactor_v5_3",false)!=-1)
							{
								if(playermission_list[i].challenge[1]!=1&&Mission_Current_Level.id==3)
								{
									playermission_list[i].challenge[1]=1;
									PrintToChat(i," \x05[任务系统]\x01恭喜你完成了当前挑战任务的\x07条件1\x01并获得\x07 200 \x01碎片和\x07 200\x01经验");
									playermission_list[i].emoney+=200;
									GrantExp(i,200);
								}
							}
							if(StrContains(map_name,"paramina_rift",false)!=-1)
							{
								if(playermission_list[i].challenge[2]!=1&&Mission_Current_Level.id==3)
								{
									playermission_list[i].challenge[2]=1;
									PrintToChat(i," \x05[任务系统]\x01恭喜你完成了当前挑战任务的\x07条件2\x01并获得\x07 200 \x01碎片和\x07 200\x01经验");
									playermission_list[i].emoney+=200;
									GrantExp(i,200);
								}
							}
							if(StrContains(map_name,"westersand_v8zeta1k",false)!=-1)
							{
								if(playermission_list[i].challenge[3]!=1&&(Mission_Current_Level.id==3||Mission_Current_Level.id==8))
								{
									playermission_list[i].challenge[3]=1;
									PrintToChat(i," \x05[任务系统]\x01恭喜你完成了当前挑战任务的\x07条件3\x01并获得\x07 200 \x01碎片和\x07 200\x01经验");
									playermission_list[i].emoney+=200;
									GrantExp(i,200);
								}
							}
							if(StrContains(map_name,"cosmo_canyon_v5k",false)!=-1)
							{
								if(playermission_list[i].challenge[4]!=1&&Mission_Current_Level.id==4)
								{
									playermission_list[i].challenge[4]=1;
									PrintToChat(i," \x05[任务系统]\x01恭喜你完成了当前挑战任务的\x07条件4\x01并获得\x07 200 \x01碎片和\x07 200\x01经验");
									playermission_list[i].emoney+=200;
									GrantExp(i,200);
								}
							}
							int credits = Store_GetClientCredits(i);
							int credits_added = credits;
							if(Mission_Current_Level.bounty>0)
							{
								credits_added += Mission_Current_Level.bounty;
							}
							if(Pmap.difficulty>=2)
							{
								credits_added += 30;
							}
							if(credits_added>credits)
							{
								Store_SetClientCredits(i,credits_added);
								PrintToChat(i," \x05[国庆活动]\x01恭喜你在活动关卡内获得\x07%d\x01积分",credits_added-credits);
							}
							if(Pmap.tag&(1<<10))
							{
								int lootbox_id = Store_GetItemIdbyUniqueId("uid_lootbox_freecase1");
								Store_GiveItem(i,lootbox_id,0,0,0);
								PrintCenterText(i,"你在活动地图上获得了一个特殊箱子，请及时打开");
								PrintToChat(i,"\x05[任务系统]\x01你在活动地图上获得了一个特殊箱子，请及时打开");
							}
						}
						else
						{
							PrintToChatAll(" \x05[任务系统]\x07人数过少或回合时间过短,不计算挑战任务与活动进度");
						}
					}
				}*/

				UpdatePlayerMissionInfo(i);
			}
		}
	}
}

void MissionHumanDmgCount(int attacker,int victim,int dmg)
{
	if(GetClientCount(true)<mission_lowest_playernum)	return;
	playermission_rounddata[attacker].hmdmg+=dmg;
	playermission_rounddata[victim].zmtake+=dmg;
	if(playermission_list[attacker].taskdata[3]<=10000000)
	{
		playermission_list[attacker].taskdata[3]+=dmg;
	}
	if(playermission_list[attacker].taskdata[12]<=10000000)
	{
		playermission_list[attacker].taskdata[12]+=dmg;
	}
	if(playermission_list[victim].taskdata[2]<=10000000)
	{
		playermission_list[victim].taskdata[2]+=dmg;
	}
	if(playermission_list[victim].taskdata[11]<=10000000)
	{
		playermission_list[victim].taskdata[11]+=dmg;
	}
}

void MissionHumanKillZombie(int attacker,int headshot=0)
{
	if(GetClientCount(true)<mission_lowest_playernum)	return;
	if(headshot==1)
	{
		playermission_rounddata[attacker].hmkill++;
	}
	if(playermission_list[attacker].taskdata[1]<=1000)
	{
		playermission_list[attacker].taskdata[1]++;
	}
	if(playermission_list[attacker].taskdata[10]<=1000)
	{
		playermission_list[attacker].taskdata[10]++;
	}
}

void MissionZombieInfectHuman(int attacker)
{
	if(GetClientCount(true)<mission_lowest_playernum)	return;
	playermission_rounddata[attacker].zminfect++;
	if(playermission_list[attacker].taskdata[0]<=1000)
	{
		playermission_list[attacker].taskdata[0]++;
	}
	if(playermission_list[attacker].taskdata[9]<=1000)
	{
		playermission_list[attacker].taskdata[9]++;
	}
}

void MissionHumanNadeCount(int client)
{
	if(GetClientCount(true)<mission_lowest_playernum)	return;
	if(playermission_list[client].taskdata[8]<=1000)
	{
		playermission_list[client].taskdata[8]++;
	}
	if(playermission_list[client].taskdata[13]<=1000)
	{
		playermission_list[client].taskdata[13]++;
	}
}
void MissionOnMapEnd()
{
	Mission_Current_Level.id=-1;
	Mission_Current_Level.bounty = 0;
	for(int i=1;i<=64;i++)
	{
		playermission_rounddata[i]=nullrounddata;
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				UpdatePlayerMissionInfo(i);
			}
		}
	}
}

void CheckValidMission()
{
	char query[512];
	Format(query,sizeof(query),"SELECT * FROM ZEMISSION");
	DbTQuery(CheckValidMissionCallBack,query);
}

void CheckValidMissionCallBack(Handle owner, Handle hndl, char[] error, any data)
{
	int current_time = GetTime();
	int start_time;
	int end_time;
	int id;
	g_ValidMission_Exist = false;
	while(SQL_FetchRow(hndl))
	{
		start_time = DbFetchInt(hndl,"STARTTIME");
		end_time = DbFetchInt(hndl,"ENDTIME");
		if(current_time>=start_time&&current_time<=end_time)
		{
			id = DbFetchInt(hndl,"ID");
			if(id == Current_Mission.id)
			{
				g_ValidMission_Exist = true;
				TEMP_OpHR_TasklistSet();
				return;
			}
			Current_Mission.id = id;
			Current_Mission.start_timestamp = start_time;
			Current_Mission.end_timestamp = end_time;
			DbFetchString(hndl,"SHORTNAME",Current_Mission.shortname,sizeof(Current_Mission.shortname));
			DbFetchString(hndl,"NAME",Current_Mission.name,sizeof(Current_Mission.name));
			DbFetchString(hndl,"CNNAME",Current_Mission.cnname,sizeof(Current_Mission.cnname));
			DbFetchString(hndl,"PLAYERDB",Current_Mission.playerdbname,sizeof(Current_Mission.playerdbname));
			Current_Mission.max_level = DbFetchInt(hndl,"MAXLEVEL");
			Current_Mission.level_exp = DbFetchInt(hndl,"LEVELEXP");
			Current_Mission.daily_timestamp = DbFetchInt(hndl,"DAILY");
			Current_Mission.weekly_timestamp = DbFetchInt(hndl,"WEEKLY");
			g_ValidMission_Exist = true;
			TEMP_OpHR_TasklistSet();
			break;
		}
	}
}

void TEMP_OpHR_TasklistSet()
{
	delete Current_Mission_Tasklist;
	Current_Mission_Tasklist = CreateArray(sizeof(TASK));
	char buffer[256];
	TASK task;
	task.stage = 3;
	task.num[0] = 3;task.num[1] = 6;task.num[2] = 9;
	task.exp_base = 60;
	task.period = 1;
	task.type = ZM_INFECT;
	task.name = "[僵尸]感染人类:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 2;task.num[1] = 4;task.num[2] = 6;
	task.exp_base = 60;
	task.period = 1;
	task.type = HM_KILLZM;
	task.name = "[人类]击杀僵尸:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 6;task.num[1] = 12;task.num[2] = 24;
	task.exp_base = 80;
	task.period = 1;
	task.type = ZM_DMGTAKE;
	task.name = "[僵尸]承受伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 4;task.num[1] = 8;task.num[2] = 12;
	task.exp_base = 80;
	task.period = 1;
	task.type = HM_DMGMAKE;
	task.name = "[人类]造成伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 400;task.num[1] = 800;task.num[2] = 1200;
	task.exp_base = 50;
	task.period = 1;
	task.type = HM_HIT;
	task.name = "[人类]攻击障碍/BOSS(次):";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 4;task.num[1] = 8;task.num[2] = 12;
	task.exp_base = 60;
	task.period = 1;
	task.type = HM_PASS;
	task.name = "[人类]通关回合:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 1;task.num[1] = 2;task.num[2] = 3;
	task.exp_base = 80;
	task.period = 1;
	task.type = HM_PASS;
	task.difficulty = 2;
	task.tag = -1;
	task.name = "[人类]通关[困难]以上地图:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 1;task.num[1] = 2;task.num[2] = 3;
	task.exp_base = 50;
	task.period = 1;
	task.type = HM_PASS;
	task.tag = 3;
	Format(buffer,sizeof(buffer),"[人类]通关[%s]地图:",label_name[task.tag]);
	task.difficulty = -1;
	strcopy(task.name,sizeof(task.name),buffer);
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 10;task.num[1] = 25;task.num[2] = 50;
	task.exp_base = 50;
	task.period = 1;
	task.type = HM_NADE;
	task.name = "[人类]投掷手雷:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 60;
	task.exp_base = 3000;
	task.period = 7;
	task.type = ZM_INFECT;
	task.name = "[僵尸]感染人类:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 30;
	task.exp_base = 3000;
	task.period = 7;
	task.type = HM_KILLZM;
	task.name = "[人类]击杀僵尸:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 250;
	task.exp_base = 3000;
	task.period = 7;
	task.type = ZM_DMGTAKE;
	task.name = "[僵尸]承受伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 125;
	task.exp_base = 3000;
	task.period = 7;
	task.type = HM_DMGMAKE;
	task.name = "[人类]造成伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 250;
	task.exp_base = 3000;
	task.period = 7;
	task.type = HM_NADE;
	task.name = "[人类]投掷手雷:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 10;
	task.exp_base = 3000;
	task.period = 7;
	task.type = HM_PASS;
	task.difficulty = -1;
	task.tag = 10;
	task.name = "[人类]通关[活动]地图:";
	Current_Mission_Tasklist.PushArray(task);
}

Action MissionMenuCommand(int client,int args)
{
	if(client<=0||client>=65)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	if(Current_Mission.id<=0)
	{
		PrintToChat(client," \x05[任务系统]\x01当前没有可用的赛季活动");
		return Plugin_Handled;
	}
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]\x01任务数据载入出错,等待下一回合或输入!msr");
		PrintToChat(client," \x05[任务系统]\x01尝试自动重载");
		LoadPlayerMissionInfo(client);
		return Plugin_Handled;
	}
	MissionMenuBuild(client);
	return Plugin_Handled;
}

void MissionMenuBuild(int client)
{
	if(Current_Mission.id<=0)
	{
		PrintToChat(client," \x05[任务系统]\x01当前没有可用的赛季活动");
		return;
	}
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]\x01任务数据载入出错,等待下一回合或输入!msr");
		PrintToChat(client," \x05[任务系统]\x01尝试自动重载");
		LoadPlayerMissionInfo(client);
		return;
	}
	Menu menu = CreateMenu(MissionMenuHandler);
	char buffer[512];
	Format(buffer,sizeof(buffer),"赛季活动\n%s\n%s\nLV.%d/%d\nEXP:%d/%d",Current_Mission.cnname,Current_Mission.name,playermission_list[client].lvl,Current_Mission.max_level,playermission_list[client].exp,Current_Mission.level_exp);
	menu.SetTitle(buffer);
	menu.AddItem("","每日任务[双倍经验]");
	menu.AddItem("","每周挑战[双倍经验]");
	menu.AddItem("","奖励兑换");
	menu.AddItem("","VIP奖励");
	menu.AddItem("","神秘商店[12/22新品上架]");
	menu.AddItem("","悬赏任务[12/22更新]");
	menu.AddItem("","成就称号[即将推出]",ITEMDRAW_DISABLED);
	menu.Display(client, MENU_TIME_FOREVER);
}

int MissionMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		if(param == 0)
		{
			DailyTaskMenu(client);
		}
		else if (param == 1)
		{
			WeeklyTaskMenu(client);
		}
		else if (param == 2)
		{
			AwardMenu(client);
		}
		else if (param == 3)
		{
			CheckVIPBonus(client);
		}
		else if (param == 4)
		{
			SecretShopMenu(client);
		}
		else if (param == 5)
		{
			ChallengeTask(client);
		}
		return 0;
	}
}

void DailyTaskMenu(int client)
{
	TASK task;
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]\x01任务数据载入出错,等待下一回合或输入!msr");
		return;
	}
	if(client<=0||client>=65)					return;
	Menu menu = CreateMenu(DailyTaskMenuHandler);
	char buffer[256],buffer2[256];
	menu.SetTitle("每日任务-完成后请点击任务提交\n注意每日任务均有3档");
	for(int i=0;i<Current_Mission_Tasklist.Length;i++)
	{
		GetArrayArray(Current_Mission_Tasklist,i,task,sizeof(task));
		if(task.period==1)
		{
			IntToString(i,buffer,sizeof(buffer));
			if(playermission_list[client].taskstage[i]>=task.stage)
			{
				Format(buffer2,sizeof(buffer2),"%s已完成",task.name);
				menu.AddItem(buffer,buffer2,ITEMDRAW_DISABLED);
			}
			else
			{
				if(task.type==ZM_DMGTAKE||task.type==HM_DMGMAKE)
				{
					Format(buffer2,sizeof(buffer2),"%s%d/%d",task.name,playermission_list[client].taskdata[i],task.num[playermission_list[client].taskstage[i]]*10000);
				}
				else
				{
					Format(buffer2,sizeof(buffer2),"%s%d/%d",task.name,playermission_list[client].taskdata[i],task.num[playermission_list[client].taskstage[i]]);
				}
				menu.AddItem(buffer,buffer2);
			}
		}
	}
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int DailyTaskMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char buffer[256];
	TASK task;
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		menu.GetItem(param,buffer,sizeof(buffer));
		int taskid=StringToInt(buffer);
		GetArrayArray(Current_Mission_Tasklist,taskid,task,sizeof(task));
		if(playermission_list[client].taskstage[taskid]>=task.stage)
		{
			PrintToChat(client," \x05[任务系统]你已完成该任务!");
		}
		if(task.type==ZM_DMGTAKE||task.type==HM_DMGMAKE)
		{
			if((playermission_list[client].taskdata[taskid]/10000)>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Daily_Task_Exp_Factor);
				playermission_list[client].emoney+=1;
				PrintToChat(client," \x05[任务系统]\x01完成日常任务，获得1碎片");
			}
			else
			{
				PrintToChat(client," \x05[任务系统]未达成任务条件");
			}
		}
		else
		{
			if((playermission_list[client].taskdata[taskid])>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Daily_Task_Exp_Factor);
				playermission_list[client].emoney+=1;
				PrintToChat(client," \x05[任务系统]\x01完成日常任务，获得1碎片");
			}
			else
			{
					PrintToChat(client," \x05[任务系统]未达成任务条件");
			}
		}
		UpdatePlayerMissionInfo(client);
		DailyTaskMenu(client);
	}
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
	return 0;
}

void WeeklyTaskMenu(int client)
{
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]\x01任务数据载入出错,等待下一回合或输入!msr");
		return;
	}
	if(client<=0||client>=65)					return;
	TASK task;
	Menu menu = CreateMenu(WeeklyTaskMenuHandler);
	char buffer[256],buffer2[256];
	menu.SetTitle("每周任务-完成后请点击任务提交");
	for(int i=0;i<Current_Mission_Tasklist.Length;i++)
	{
		GetArrayArray(Current_Mission_Tasklist,i,task,sizeof(task));
		if(task.period==7)
		{
			IntToString(i,buffer,sizeof(buffer));
			if(playermission_list[client].taskstage[i]>=task.stage)
			{
				Format(buffer2,sizeof(buffer2),"%s已完成",task.name);
				menu.AddItem(buffer,buffer2,ITEMDRAW_DISABLED);
			}
			else
			{
				if(task.type==ZM_DMGTAKE||task.type==HM_DMGMAKE)
				{
					Format(buffer2,sizeof(buffer2),"%s%d/%d",task.name,playermission_list[client].taskdata[i],task.num[playermission_list[client].taskstage[i]]*10000);
				}
				else
				{
					Format(buffer2,sizeof(buffer2),"%s%d/%d",task.name,playermission_list[client].taskdata[i],task.num[playermission_list[client].taskstage[i]]);
				}
				menu.AddItem(buffer,buffer2);
			}
		}
	}
	menu.ExitBackButton = true;
	menu.Display(client, MENU_TIME_FOREVER);
}

int WeeklyTaskMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	char buffer[256];
	TASK task;
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		menu.GetItem(param,buffer,sizeof(buffer));
		int taskid=StringToInt(buffer);
		GetArrayArray(Current_Mission_Tasklist,taskid,task,sizeof(task));
		if(playermission_list[client].taskstage[taskid]>=task.stage)
		{
			PrintToChat(client," \x05[任务系统]你已完成该任务!");
		}
		if(task.type==ZM_DMGTAKE||task.type==HM_DMGMAKE)
		{
			if((playermission_list[client].taskdata[taskid]/10000)>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Weekly_Task_Exp_Factor);
				playermission_list[client].emoney+=5;
				PrintToChat(client," \x05[任务系统]\x01完成周常任务，获得5碎片");
			}
			else
			{
				PrintToChat(client," \x05[任务系统]未达成任务条件");
			}
		}
		else
		{
			if((playermission_list[client].taskdata[taskid])>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Weekly_Task_Exp_Factor);
				playermission_list[client].emoney+=5;
				PrintToChat(client," \x05[任务系统]\x01完成周常任务，获得5碎片");
			}
			else
			{
				PrintToChat(client," \x05[任务系统]未达成任务条件");
			}
		}
		UpdatePlayerMissionInfo(client);
		WeeklyTaskMenu(client);
	}
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
	return 0;
}

void GrantExp(int client,int exp)
{
	if(client<=0||client>=65)	return;
	if(g_pStore)
	{
		int credits=Store_GetClientCredits(client);
	}
	playermission_list[client].exp+=exp;
	PrintToChat(client," \x05[任务系统]\x01您在\x09%s\x01中获得\x09%d\x01经验",Current_Mission.cnname,exp);
	int level_exp = Current_Mission.level_exp;
	int max_level = Current_Mission.max_level;
	int uplevel;
	/*
	if(playermission_list[client].exp>=level_exp)
	{
		if(playermission_list[client].lvl<max_level)
		{
			uplevel = playermission_list[client].exp/level_exp;
			playermission_list[client].lvl+=uplevel;
			playermission_list[client].exp%=level_exp;
			PrintToChat(client," \x05[任务系统] \x01您在\x09%s\x01的等级提升到了\x09%d\x01,并获得%d积分奖励(若购买等级无奖励)",Current_Mission.name,playermission_list[client].lvl,250*uplevel);
			if(g_pStore)
			{
				Store_SetClientCredits(client,credits+250*uplevel);
			}
			else
			{
				PrintToChat(client," \x05[任务系统]由于商店系统维护，暂时不给予积分，请保留截图");
			}
		}
		else
		{
			playermission_list[client].exp=level_exp;
		}
	}*/
	UpdatePlayerMissionInfo(client);
}

void AwardMenu(int client)
{
	Menu menu = CreateMenu(AwardMenuHandle);
	int iStyle = (g_pStore) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED;
	menu.SetTitle("特典奖励领取\n%s", (g_pStore) ? "点击对应选项领取奖励" : "商店插件未加载, 请稍后再试");
	menu.AddItem("uid_wepskin_dualg18","LV25:双枪-GLOCK[360天]", iStyle);
	menu.AddItem("uid_lootbox_freecase1","LV50:活动箱子+1000积分", iStyle);
	menu.AddItem("uid_wepskin_cso2axe","LV75:战斧-极寒冲击[360天]", iStyle);
	menu.AddItem("uid_model_lemalin","LV100:恶毒-1[永久]", iStyle);
	menu.AddItem("uid_wepskin_lvreji","LV125:手雷-铝热剂[360天]", iStyle);
	menu.AddItem("uid_wepskin_elcjb","LV150:匕首-恶灵苦无[360天]", iStyle);
	menu.AddItem("uid_wepskin_carbizon","LV175:野牛-CAR(浊心斯卡蒂)[360天]", iStyle);
	menu.AddItem("uid_wepskin_dualdyhj","LV200:双枪-双持沙鹰(印花集)[360天]", iStyle);
	menu.AddItem("uid_lootbox_freecase1","LV225:活动箱子+2000积分", iStyle);
	menu.AddItem("uid_model_lemalin2","LV250:恶毒-2[永久]", iStyle);
	menu.AddItem("uid_model_lemalin3","LV275:恶毒-3[永久]", iStyle);
	menu.AddItem("uid_wepskin_yhmachak","LV300:M249-旋风AK(野荷)[360天]", iStyle);
	menu.AddItem("uid_model_lemalin4","LV325:恶毒-4[永久]", iStyle);
	menu.AddItem("uid_model_xieshen","LV350:邪神[永久]", iStyle);
	menu.AddItem("uid_wepskin_ggneawp","LV375:MAG7-AWP(永恒)[360天]", iStyle);
	menu.AddItem("uid_model_xieshen2","LV400:邪神(泳装)[永久]", iStyle);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int AwardMenuHandle(Menu menu, MenuAction action, int client, int param)
{
	char item[256];
	int item_id;
	int current_time=GetTime();
	int expdate;
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		if(!g_pStore)
		{
			return 0;
		}
		int client_credits = Store_GetClientCredits(client);
		menu.GetItem(param,item,sizeof(item));
		if(playermission_list[client].sp&(1<<(param)))
		{
			PrintToChat(client," \x05[任务系统]\x01已经领取过该奖励！");
			AwardMenu(client);
			return 0;
		}
		if(playermission_list[client].lvl<25*(param+1))
		{
			PrintToChat(client," \x05[任务系统]\x01领取所需等级不足!需要赛季等级\x09%d\x01级，而你只有\x09%d\x01级",25*(param+1),playermission_list[client].lvl);
			AwardMenu(client);
			return 0;
		}
		item_id = Store_GetItemIdbyUniqueId(item);
		if(item_id!=-1)
		{
			if(StrContains(item,"uid_wepskin",false)!=-1)
			{
				expdate = current_time+360*86400;
				Store_GiveItem(client,item_id,0,expdate,0);
				playermission_list[client].sp=playermission_list[client].sp|(1<<(param));
			}
			else
			{
				if(StrContains(item,"uid_model",false)!=-1)
				{
					if(!Store_HasClientItem(client,item_id))
					{
						Store_GiveItem(client,item_id,0,0,0);
					}
					else
					{
						PrintToChat(client," \x05[任务系统]\x01您已经拥有该人物模型，退还500碎片");
						playermission_list[client].emoney+=500;
					}
					playermission_list[client].sp=playermission_list[client].sp|(1<<(param));
				}
				else
				{
					if(StrContains(item,"uid_lootbox",false)!=-1)
					{
						if(!Store_HasClientItem(client,item_id))
						{
							Store_SetClientCredits(client,client_credits+5000);
							Store_GiveItem(client,item_id,0,expdate,0);
							playermission_list[client].sp=playermission_list[client].sp|(1<<(param));
							UpdatePlayerMissionInfo(client);
						}
						else
						{
							PrintToChat(client," \x05[任务系统]\x01请打开你已有的活动箱子再领取!");
						}
					}
				}
			}
			UpdatePlayerMissionInfo(client);
		}
		AwardMenu(client);
		return 0;
	}
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
	return 0;
}

void CheckVIPBonus(int client)
{
	int vipstatus;
	vipstatus=IsClientVIP(client);
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]数据未载入，无法领取(请等待下一回合或输入!msr)");
		return;
	}
	if(vipstatus!=-1)
	{
		if(vipstatus)
		{
			if(playermission_list[client].vip==0)
			{
				PrintToChat(client," \x05[任务系统]\x01VIP奖励—15000点经验已发放");
				GrantExp(client,15000);
				playermission_list[client].vip=1;
				UpdatePlayerMissionInfo(client);
			}
			else
			{
				if(playermission_list[client].vip==2)
				{
						PrintToChat(client," \x05[任务系统]\x01VIP奖励补发—5000点经验已发放");
						GrantExp(client,5000);
						playermission_list[client].vip=1;
						UpdatePlayerMissionInfo(client);
				}
				else if(playermission_list[client].vip==1)
				{
					PrintToChat(client," \x05[任务系统]\x01你已经领取过VIP奖励!");
				}
			}
		}
		else
		{
			PrintToChat(client," \x05[任务系统]\x01你还不是VIP，无法领取奖励，快去氪金吧!");
		}
	}
}

void SecretShopMenu(int client)
{
	if(client<=0||client>=65)	return;
	if(!g_pStore)
	{
		PrintToChat(client," \x05[任务系统]\x01商店插件未载入或维护中");
		return;
	}
	Menu menu = CreateMenu(SecretShopHandler);
	int credits = Store_GetClientCredits(client);
	int emoney = playermission_list[client].emoney;
	menu.SetTitle("神秘商店\n您当前积分为:%d\n您当前持有的碎片为:%d",credits,playermission_list[client].emoney);
	menu.AddItem("","购买1大行动等级(3000积分)",credits>=3000?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("","购买10大行动等级(25000积分)",credits>=25000?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("","兑换积分(100碎片->300积分)",emoney>=100?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_model_kokuriruru","兑换人物模型:狐九里露露[永久](2500碎片)",emoney>=2500?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_wepskin_apexbs","兑换武器模型:R8-小帮手[360天](1500碎片)",emoney>=1500?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_wepskin_waterknifered","兑换武器模型:海豹短刀-波光潋滟[360天](1500碎片)",emoney>=1500?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_wepskin_aquatmp","兑换武器模型:TMP海洋之心[360天](1500碎片)",emoney>=1500?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_model_xinhai","兑换人物模型:心海[永久](3500碎片)",emoney>=3500?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int SecretShopHandler(Menu menu, MenuAction action, int client, int param)
{
	if(client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	int credits = Store_GetClientCredits(client);
	int emoney = playermission_list[client].emoney;
	int current_time = GetTime();
	int expdate;
	int item_id;
	char item[256];
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]数据未载入，无法购买(请等待下一回合或输入!msr)");
		menu.Close();
	}
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(!g_pStore)
		{
			PrintToChat(client," \x05[任务系统]\x01商店插件未载入或维护中");
			return 0;
		}
		menu.GetItem(param,item,sizeof(item));
		switch(param)
		{
			case 0:
			{
				if(credits>=3000&&playermission_list[client].lvl<Current_Mission.max_level)
				{
					GrantExp(client,Current_Mission.level_exp);
					Store_SetClientCredits(client,credits-3000);
					PrintToChat(client," \x05[任务系统]消费积分购买了1大行动等级");
				}
				else
				{
					PrintToChat(client," \x05[任务系统]你的积分不足或你已满级!");
				}
			}
			case 1:
			{
				if(credits>=25000&&playermission_list[client].lvl<Current_Mission.max_level)
				{
					GrantExp(client,10*(Current_Mission.level_exp));
					Store_SetClientCredits(client,credits-25000);
					PrintToChat(client," \x05[任务系统]消费积分购买了10大行动等级");
				}
				else
				{
					PrintToChat(client," \x05[任务系统]你的积分不足或你已满级!");
				}
			}
			case 2:
			{
				Store_SetClientCredits(client,credits+300);
				playermission_list[client].emoney-=100;
			}
			case 3:
			{
				item_id = Store_GetItemIdbyUniqueId(item);
				if(!Store_HasClientItem(client,item_id))
				{
					Store_GiveItem(client,item_id,0,0,0);
					PrintToChat(client," \x05[任务系统]\x01兑换成功")	;
					playermission_list[client].emoney -= 2500;
					UpdatePlayerMissionInfo(client);
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01您已经拥有该道具，请勿重复兑换")	;
				}
			}
			case 4:
			{
				item_id = Store_GetItemIdbyUniqueId(item);
				if(!Store_HasClientItem(client,item_id))
				{
					expdate = current_time+360*86400;
					Store_GiveItem(client,item_id,0,expdate,0);
					PrintToChat(client," \x05[任务系统]\x01兑换成功")	;
					playermission_list[client].emoney -= 1500;
					UpdatePlayerMissionInfo(client);
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01您已经拥有该道具，请勿重复兑换")	;
				}
			}
			case 5:
			{
				item_id = Store_GetItemIdbyUniqueId(item);
				if(!Store_HasClientItem(client,item_id))
				{
					expdate = current_time+360*86400;
					Store_GiveItem(client,item_id,0,expdate,0);
					PrintToChat(client," \x05[任务系统]\x01兑换成功")	;
					playermission_list[client].emoney -= 1500;
					UpdatePlayerMissionInfo(client);
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01您已经拥有该道具，请勿重复兑换")	;
				}
			}
			case 6:
			{
				item_id = Store_GetItemIdbyUniqueId(item);
				if(!Store_HasClientItem(client,item_id))
				{
					expdate = current_time+360*86400;
					Store_GiveItem(client,item_id,0,expdate,0);
					PrintToChat(client," \x05[任务系统]\x01兑换成功")	;
					playermission_list[client].emoney -= 1500;
					UpdatePlayerMissionInfo(client);
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01您已经拥有该道具，请勿重复兑换")	;
				}
			}
			case 7:
			{
				item_id = Store_GetItemIdbyUniqueId(item);
				if(!Store_HasClientItem(client,item_id))
				{
					Store_GiveItem(client,item_id,0,0,0);
					PrintToChat(client," \x05[任务系统]\x01兑换成功")	;
					playermission_list[client].emoney -= 3500;
					UpdatePlayerMissionInfo(client);
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01您已经拥有该道具，请勿重复兑换")	;
				}
			}
		}
		SecretShopMenu(client);
		return 0;
	}
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
	return 0;
}

public int Native_RY_GetClientMissionLevel(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	if(client<=0||client>=65)	return -1;
	if(!IsClientInGame(client))	return -1;
	if(IsFakeClient(client))	return -1;
	if(!playermission_list[client].loaded)	return -1;
	return playermission_list[client].lvl;
}

public int Native_RY_GiveClientMissionExp(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int exp = GetNativeCell(2);
	if(client<=0||client>=65)	return -1;
	if(exp<=0)	return -1;
	if(!IsClientInGame(client))	return -1;
	if(IsFakeClient(client))	return -1;
	if(!playermission_list[client].loaded)	return -1;
	GrantExp(client,exp);
	return 0;
}
void ChallengeTask(int client)
{
	char buffer[256];
	if(client<=0||client>=65)	return;
	if(playermission_list[client].loaded==0)
	{
		return;
	}
	Menu menu = CreateMenu(ChallengeTaskMenuHandler);
	menu.SetTitle("悬赏任务:20221222");
	menu.AddItem("",playermission_list[client].challenge[1]?"[已完成]":"任务1:致命打击\n爆头击杀一只僵尸并通关[100碎片+100经验]",ITEMDRAW_DISABLED);
	menu.AddItem("",playermission_list[client].challenge[2]?"[已完成]":"任务2:行动代号:炒股\n在任意一张obj系列地图中单局对僵尸造成20万点伤害并通关[300碎片+300经验]",ITEMDRAW_DISABLED);
	menu.AddItem("",playermission_list[client].challenge[3]?"[已完成]":"任务3:你必须挂科\n在米纳斯中单局攻击炎魔至少50次并通关[300碎片+300经验]\n[未完成]",ITEMDRAW_DISABLED);
	menu.AddItem("",playermission_list[client].challenge[3]?"[已完成]":"额外任务:破冰行动\n在帕拉米娜大峡谷的第二关的BOSS战中，击碎一块冰晶[300碎片+300经验]\n[未完成]",ITEMDRAW_DISABLED);
	menu.AddItem("","提交任务(只需完成任务1~3)[600碎片+1200经验]");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
	return;
}
int Native_RY_ZE_ZBUYCOUNT(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int price = GetNativeCell(2);
	playermission_rounddata[client].pay+=price;
	return 0;
}
int ChallengeTaskMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if(client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]数据未载入，无法购买(请等待下一回合或换图)");
		menu.Close();
		return 0;
	}
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		if(playermission_list[client].challenge[0]!=1)
		{
			if((playermission_list[client].challenge[1]==1)&&(playermission_list[client].challenge[2]==1)&&(playermission_list[client].challenge[3]==1))
			{
				playermission_list[client].challenge[0]=1;
				playermission_list[client].emoney+=600;
				GrantExp(client,1200);
				PrintToChat(client," \x05[任务系统]\x01完成悬赏任务，获得600碎片和1200经验!");
				UpdatePlayerMissionInfo(client);
			}
			else
			{
				PrintToChat(client," \x05[任务系统]\x01你还未达成所有条件!");
			}
		}
		else
		{
			PrintToChat(client," \x05[任务系统]\x01你已经提交过任务，请勿重复提交!");
		}
	}
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
	return 0;
}
