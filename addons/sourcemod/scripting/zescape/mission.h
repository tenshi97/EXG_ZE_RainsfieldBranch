enum TASK_TYPE
{
	ZM_INFECT,
	HM_KILLZM,
	ZM_DMGTAKE,
	HM_DMGMAKE,
	ALL_ONLINE,
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
	int sp[5];
	int vip;
	int emoney;
	int nt[2]; //WIP
	int challenge[5];
}
ArrayList Current_Mission_Tasklist;
bool g_ValidMission_Exist;
PlayerMissionInfo playermission_list[65];
PlayerMissionInfo nullpmi;
Missions Current_Mission;
void MissionOnPluginStart()
{
	g_ValidMission_Exist = false;
	Current_Mission.id = 0;
	RegConsoleCmd("sm_mission",MissionMenuCommand);
	RegConsoleCmd("sm_ms",MissionMenuCommand);
	RegConsoleCmd("sm_dxd",MissionMenuCommand);	//Just For Fun!
	if(!isDbConnected())	return;			//未连接，return，通过Db连接函数的函数执行Post，已连接则通直接Post使得换图后重载各插件数据
	CheckValidMission();
	ReloadAllPlayerMissionInfo();
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
	for(int i=0;i<=7;i++)
	{
		playermission_list[client].taskdata[i]=0;
		playermission_list[client].taskstage[i]=0;		
	}
	playermission_list[client].taskdata[4]=MostActive_GetPlayTimeTotal(client);
	playermission_list[client].loaded = 1;
	UpdatePlayerMissionInfo(client);
}
void PlayerMissionWeeklyUpdate(int client)
{
	if(client<=0||client>=65)	return;
	PrintToServer(" \x05[调试]玩家%d的每周任务数据过期，清空中...",client);
	for(int i=8;i<=12;i++)
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
		PrintToServer("[任务系统]加载玩家%d数据时出错，数据库未载入（请等待下一回合)",client);
		return;
	}
	playermission_list[client]=nullpmi;
	if(!IsClientInGame(client))	return;
	uid = GetSteamAccountID(client);
	int playtime = MostActive_GetPlayTimeTotal(client);	
	PrintToServer("[调试]进服玩家%d[UID:%d],总游玩时长:%d",client,uid,playtime);
	Format(query,sizeof(query),"SELECT * FROM %s WHERE UID = %d",Current_Mission.playerdbname,uid);
	DbTQuery(LoadPlayerMissionInfoCallBack,query,client);
}
void LoadPlayerMissionInfoCallBack(Handle owner, Handle hndl, char[] error, any data)
{	
	char query[512];
	int client = data;
	int uid = GetSteamAccountID(client);
	playermission_list[client].uid=uid;
	int current_time = GetTime();
	playermission_list[client].loaded=0;
	if(!SQL_FetchRow(hndl))
	{
		int playtime = MostActive_GetPlayTimeTotal(client);
		PrintToConsoleAll("[调试]未检测到玩家%d赛季活动数据，注册新玩家%d 当前时间:%d,载入时总游玩时间%d",client,uid,current_time,playtime);
		Format(query,sizeof(query),"INSERT INTO %s (UID,TIMESTAMP,DONLINE,VIP) VALUES(%d,%d,%d,0)",Current_Mission.playerdbname,uid,current_time,playtime);
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
		playermission_list[client].taskdata[4]=DbFetchInt(hndl,"DONLINE");	
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
		playermission_list[client].sp[0]=DbFetchInt(hndl,"SP1");
		playermission_list[client].sp[1]=DbFetchInt(hndl,"SP2");
		playermission_list[client].sp[2]=DbFetchInt(hndl,"SP3");
		playermission_list[client].sp[3]=DbFetchInt(hndl,"SP4");
		playermission_list[client].sp[4]=DbFetchInt(hndl,"SP5");
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
		PrintToChatAll(" \x05[任务系统]清空大行动每周任务数据....");
		Format(query,sizeof(query),"UPDATE %s SET WINFECT = 0,WKILLZM = 0,WDMGMAKE = 0,WDMGTAKE = 0,WNADE = 0,WPASS = 0,WT1ST = 0,WT2ST = 0,WT3ST =0,WT4ST = 0,WT5ST =0,WT6ST = 0,CH1 = 0,CH2 = 0,CH3 = 0,CH4 = 0,CH = 0",Current_Mission.playerdbname);
		DbTQuery(DbQueryErrorCallback,query);
	}
	PrintToChatAll(" \x05[任务系统]清空大行动每日任务数据....");
	Format(query,sizeof(query),"UPDATE %s SET DINFECT = 0, DKILLZM = 0, DDMGTAKE=0, DDMGMAKE=0, DONLINE=0, DPASS=0, DPASS2=0, DPASS3=0, DNADE=0, DT1ST=0, DT2ST=0, DT3ST=0, DT4ST=0, DT5ST=0, DT6ST=0, DT7ST=0, DT8ST=0, DT9ST=0",Current_Mission.playerdbname);
	DbTQuery(DbClearPlayerMissionInfoCallback,query);
}

void DbClearPlayerMissionInfoCallback(Handle owner, Handle hndl, char[] error, any data)
{
	ReloadAllPlayerMissionInfo();
}
void UpdatePlayerMissionInfo(int client)
{
	if(playermission_list[client].loaded==0)
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
	int sp1=playermission_list[client].sp[0];
	int sp2=playermission_list[client].sp[1];
	int sp3=playermission_list[client].sp[2];
	int sp4=playermission_list[client].sp[3];
	int sp5=playermission_list[client].sp[4];
	for(int i =0;i<=14;i++)
	{
		taskdata[i]=playermission_list[client].taskdata[i];
		taskstage[i]=playermission_list[client].taskstage[i];
	}
	int current_time = GetTime();
	PrintToServer("[任务系统][UID:%d]保存玩家%d数据",uid,client);
	Format(query,sizeof(query),"UPDATE %s SET LVL = %d, EXP = %d, DINFECT = %d,DKILLZM = %d,DDMGTAKE = %d,DDMGMAKE = %d,DONLINE = %d, DPASS = %d, DPASS2 = %d, DPASS3 = %d, DNADE = %d, WINFECT = %d, WKILLZM = %d, WDMGTAKE = %d, WDMGMAKE = %d, WNADE = %d, WPASS = %d, DT1ST = %d, DT2ST = %d, DT3ST = %d, DT4ST = %d, DT5ST =%d, DT6ST = %d, DT7ST = %d, DT8ST = %d, DT9ST = %d, WT1ST = %d, WT2ST = %d, WT3ST = %d, WT4ST = %d, WT5ST = %d, WT6ST = %d, TIMESTAMP = %d, SP1 = %d, SP2 = %d, SP3 = %d, SP4 = %d, SP5 = %d, VIP = %d, EMONEY = %d, CH1 = %d, CH2 = %d, CH3 = %d, CH4 = %d, CH = %d WHERE UID = %d",Current_Mission.playerdbname,lvl,exp,taskdata[0],taskdata[1],taskdata[2],taskdata[3],taskdata[4],taskdata[5],taskdata[6],taskdata[7],taskdata[8],taskdata[9],taskdata[10],taskdata[11],taskdata[12],taskdata[13],taskdata[14],taskstage[0],taskstage[1],taskstage[2],taskstage[3],taskstage[4],taskstage[5],taskstage[6],taskstage[7],taskstage[8],taskstage[9],taskstage[10],taskstage[11],taskstage[12],taskstage[13],taskstage[14],current_time,sp1,sp2,sp3,sp4,sp5,playermission_list[client].vip,playermission_list[client].emoney,playermission_list[client].challenge[1],playermission_list[client].challenge[2],playermission_list[client].challenge[3],playermission_list[client].challenge[4],playermission_list[client].challenge[0],uid);
	DbTQuery(DbQueryErrorCallback,query);
}
void MissionOnClientConnected(int client)
{
	LoadPlayerMissionInfo(client);
}
void MissionOnClientDisconnect(int client)
{
	if(playermission_list[client].loaded)
	{
		UpdatePlayerMissionInfo(client);
	}
	playermission_list[client].loaded=0;
}
void MissionOnRoundEnd(int winner)
{
	int player_num = GetClientCount(true);
	char map_name[64];
	GetCurrentMap(map_name,sizeof(map_name));
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				if(winner==3&&IsPlayerAlive(i)&&ZR_IsClientHuman(i)&&player_num>=10)
				{
					if(playermission_list[i].taskdata[5]<=10000)
					{
						playermission_list[i].taskdata[5]++;
					}
					if(playermission_list[i].taskdata[6]<=10000&&Pmap.difficulty>=2)
					{
						playermission_list[i].taskdata[6]++;
					}
					if(playermission_list[i].taskdata[7]<=10000&&Pmap.tag&label_code[2])
					{
						playermission_list[i].taskdata[7]++;
					}
					if(playermission_list[i].taskdata[14]<=10000&&Pmap.tag&label_code[10])
					{
						playermission_list[i].taskdata[14]++;
					}
					if((strcmp(map_name,"ze_lotr_minas_tirith_p5",false)==0)&&playermission_list[i].challenge[1]!=1)
					{
						PrintToChat(i," \x05[任务系统]\x01恭喜你完成了本周挑战任务的\x07条件1");
						playermission_list[i].challenge[1]=1;
					}
					if(strcmp(map_name,"ze_tesv_skyrim_v5_6",false)==0&&playermission_list[i].challenge[2]!=1)
					{
						PrintToChat(i," \x05[任务系统]\x01恭喜你完成了本周挑战任务的\x07条件2");
						playermission_list[i].challenge[2]=1;
					}
					if(strcmp(map_name,"ze_last_man_standing_v6_t2",false)==0&&playermission_list[i].challenge[3]!=1)
					{
						PrintToChat(i," \x05[任务系统]\x01恭喜你完成了本周挑战任务的\x07条件3");
						playermission_list[i].challenge[3]=1;
					}					
				}
				UpdatePlayerMissionInfo(i);
			}
		}
	}
}

void MissionOnMapEnd()
{
	for(int i=1;i<=64;i++)
	{
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
	CloseHandleSafe(Current_Mission_Tasklist);
	Current_Mission_Tasklist = CreateArray(sizeof(TASK));
	g_ValidMission_Exist = false;
	Format(query,sizeof(query),"SELECT * FROM ZEMISSION");
	DbTQuery(CheckValidMissionCallBack,query);
}
void MissionHumanDmgCount(int attacker,int victim,int dmg)
{
	if(GetClientCount(true)<10)	return;
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
void MissionHumanKillZombie(int attacker)
{
	if(GetClientCount(true)<10)	return;
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
	if(GetClientCount(true)<10)	return;
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
	if(GetClientCount(true)<10)	return;
	if(playermission_list[client].taskdata[8]<=1000)
	{
		playermission_list[client].taskdata[8]++;
	}
	if(playermission_list[client].taskdata[13]<=1000)
	{
		playermission_list[client].taskdata[13]++;
	}
}
void CheckValidMissionCallBack(Handle owner, Handle hndl, char[] error, any data)
{
	int current_time = GetTime();
	int start_time;
	int end_time;
	Current_Mission.id = 0;
	while(SQL_FetchRow(hndl))
	{
		start_time = DbFetchInt(hndl,"STARTTIME");
		end_time = DbFetchInt(hndl,"ENDTIME");
		if(current_time>=start_time&&current_time<=end_time)
		{
			Current_Mission.id = DbFetchInt(hndl,"ID");
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
	char buffer[256];
	TASK task;
	task.stage = 3;
	task.num[0] = 4;task.num[1] = 8;task.num[2] = 12;
	task.exp_base = 80;
	task.period = 1;
	task.type = ZM_INFECT;
	task.name = "[僵尸]感染人类:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 3;task.num[1] = 6;task.num[2] = 9;
	task.exp_base = 80;
	task.period = 1;
	task.type = HM_KILLZM;
	task.name = "[人类]击杀僵尸:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 6;task.num[1] = 12;task.num[2] = 24;
	task.exp_base = 120;
	task.period = 1;
	task.type = ZM_DMGTAKE;
	task.name = "[僵尸]承受伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 4;task.num[1] = 8;task.num[2] = 12;
	task.exp_base = 120;
	task.period = 1;
	task.type = HM_DMGMAKE;
	task.name = "[人类]造成伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 60;task.num[1] = 120;task.num[2] = 240;
	task.exp_base = 80;
	task.period = 1;
	task.type = ALL_ONLINE;
	task.name = "[公共]在线时间(分钟):";
	Current_Mission_Tasklist.PushArray(task);	

	task.stage = 3;
	task.num[0] = 3;task.num[1] = 6;task.num[2] = 9;
	task.exp_base = 70;
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
	task.exp_base = 80;
	task.period = 1;
	task.type = HM_PASS;
	task.tag = 2;
	Format(buffer,sizeof(buffer),"[人类]通关[%s]地图:",label_name[task.tag]);
	task.difficulty = -1;
	strcopy(task.name,sizeof(task.name),buffer);
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 10;task.num[1] = 25;task.num[2] = 50;
	task.exp_base = 100;
	task.period = 1;
	task.type = HM_NADE;
	task.name = "[人类]投掷手雷:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 100;
	task.exp_base = 2000;
	task.period = 7;
	task.type = ZM_INFECT;
	task.name = "[僵尸]感染人类:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 50;
	task.exp_base = 2000;
	task.period = 7;
	task.type = HM_KILLZM;
	task.name = "[人类]击杀僵尸:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 300;
	task.exp_base = 2000;
	task.period = 7;
	task.type = ZM_DMGTAKE;
	task.name = "[僵尸]承受伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 200;
	task.exp_base = 2000;
	task.period = 7;
	task.type = HM_DMGMAKE;
	task.name = "[人类]造成伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 250;
	task.exp_base = 2000;
	task.period = 7;
	task.type = HM_NADE;
	task.name = "[人类]投掷手雷:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 15;
	task.exp_base = 2000;
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
	if(Current_Mission.id<=0)	return Plugin_Handled;
	MissionMenuBuild(client);
	return Plugin_Handled;
}

void MissionMenuBuild(int client)
{
	if(Current_Mission.id<0)
	{
		PrintToChat(client," \x05[任务系统]\x01当前没有可用的赛季活动");
		return;
	}
	Menu menu = CreateMenu(MissionMenuHandler);
	char buffer[512];
	Format(buffer,sizeof(buffer),"赛季活动\n%s\n%s\nLV.%d/%d\nEXP:%d/%d",Current_Mission.cnname,Current_Mission.name,playermission_list[client].lvl,Current_Mission.max_level,playermission_list[client].exp,Current_Mission.level_exp);
	menu.SetTitle(buffer);
	menu.AddItem("","每日任务");
	menu.AddItem("","每周挑战");
	menu.AddItem("","奖励兑换");
	menu.AddItem("","VIP奖励");    
	menu.AddItem("","神秘商店");          
	menu.AddItem("","挑战任务");      
	menu.Display(client, MENU_TIME_FOREVER);
}

int MissionMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
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
	}
}

void DailyTaskMenu(int client)
{
	TASK task;
	if(!playermission_list[client].loaded)		return;
	if(client<=0||client>=65)					return;
	Menu menu = CreateMenu(DailyTaskMenuHandler);
	char buffer[256],buffer2[256];
	int onlinetime;
	menu.SetTitle("每日任务-完成后请点击任务提交\n注意每日任务均有3档");
	onlinetime = (MostActive_GetPlayTimeTotal(client) - playermission_list[client].taskdata[4])/60;
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
					if(task.type==ALL_ONLINE)
					{
						Format(buffer2,sizeof(buffer2),"%s%d/%d",task.name,onlinetime,task.num[playermission_list[client].taskstage[i]]);
					}
					else
					{
						Format(buffer2,sizeof(buffer2),"%s%d/%d",task.name,playermission_list[client].taskdata[i],task.num[playermission_list[client].taskstage[i]]);

					}				
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
	int onlinetime;
	if(client<=0||client>=65)	return;
	onlinetime = (MostActive_GetPlayTimeTotal(client) - playermission_list[client].taskdata[4])/60;
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
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
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid]));
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
			if(task.type==ALL_ONLINE)
			{
				if((onlinetime)>=task.num[playermission_list[client].taskstage[taskid]])
				{
					playermission_list[client].taskstage[taskid]++;
					GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid]));
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
					GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid]));
					playermission_list[client].emoney+=1;
					PrintToChat(client," \x05[任务系统]\x01完成日常任务，获得1碎片");
				}
				else
				{
					PrintToChat(client," \x05[任务系统]未达成任务条件");
				}
			}
		}
		UpdatePlayerMissionInfo(client);
		DailyTaskMenu(client);
	}	
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
}

void WeeklyTaskMenu(int client)
{
	if(!playermission_list[client].loaded)		return;
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
	if(client<=0||client>=65)	return;	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
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
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid]));
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
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid]));
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
}

void GrantExp(int client,int exp)
{
	if(client<=0||client>=65)	return;
	int credits=Store_GetClientCredits(client);
	playermission_list[client].exp+=exp;
	PrintToChat(client," \x05[任务系统]\x01您在\x09%s\x01中获得\x09%d\x01经验",Current_Mission.cnname,exp);
	int level_exp = Current_Mission.level_exp;
	int max_level = Current_Mission.max_level;
	int uplevel;
	if(playermission_list[client].exp>=level_exp)
	{
		if(playermission_list[client].lvl<max_level)
		{
			uplevel = playermission_list[client].exp/level_exp;
			playermission_list[client].lvl+=uplevel;
			playermission_list[client].exp%=level_exp;
			PrintToChat(client," \x05[任务系统] \x01您在\x09%s\x01的等级提升到了\x09%d\x01,并获得%d积分奖励",Current_Mission.name,playermission_list[client].lvl,500*uplevel);
			Store_SetClientCredits(client,credits+500*uplevel);
		}
		else
		{
			playermission_list[client].exp=level_exp;
		}
	}
	UpdatePlayerMissionInfo(client);
}

void AwardMenu(int client)
{
	Menu menu = CreateMenu(AwardMenuHandle);
	int iStyle = (g_pStore) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED;
	menu.SetTitle("特典奖励领取\n%s", (g_pStore) ? "点击对应选项领取奖励" : "商店插件未加载, 请稍后再试");

	menu.AddItem("uid_lootbox_freecase1","活动箱子+5000积分[LV20]", iStyle);
	menu.AddItem("uid_wepskin_aquatmp","TMP-海洋之心[LV40]", iStyle);
	menu.AddItem("uid_wepskin_waterknifered","海豹短刀-浮翠流丹[LV60]", iStyle);
	menu.AddItem("uid_wepskin_apexbs","小帮手[LV80]", iStyle);
	menu.AddItem("uid_model_kokuriruru","人物模型-狐九里露露[LV100]", iStyle);

	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int AwardMenuHandle(Menu menu, MenuAction action, int client, int param)
{
	char item[256];
	int item_id;
	int current_time=GetTime();
	int expdate;
	if(client<=0||client>=65)	return;	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		int client_credits = Store_GetClientCredits(client);
		menu.GetItem(param,item,sizeof(item));
		if(playermission_list[client].sp[param])
		{
			PrintToChat(client," \x05[任务系统]\x01已经领取过该奖励！");
			return;
		}
		if(playermission_list[client].lvl<20*(param+1))
		{
			PrintToChat(client," \x05[任务系统]\x01领取所需等级不足!需要赛季等级\x09%d\x01级，而你只有\x09%d\x01级",20*(param+1),playermission_list[client].lvl);
			return;
		}
		item_id = Store_GetItemIdbyUniqueId(item);
		if(item_id!=-1)
		{
			if(param==4)
			{
				Store_GiveItem(client,item_id,0,0,0);
				playermission_list[client].sp[param]=1;
				UpdatePlayerMissionInfo(client);
			}
			else
			{
				if(param==0)
				{
					if(!Store_HasClientItem(client,item_id))
					{
						Store_SetClientCredits(client,client_credits+5000);
						Store_GiveItem(client,item_id,0,expdate,0);		
						playermission_list[client].sp[param]=1;
						UpdatePlayerMissionInfo(client);
					}
					else
					{
						PrintToChat(client," \x05[任务系统]\x01请打开你已有的活动箱子再领取!");
					}
				}	
				else
				{
					expdate = current_time+180*86400;
					Store_GiveItem(client,item_id,0,expdate,0);
					playermission_list[client].sp[param]=1;
					UpdatePlayerMissionInfo(client);
				}
			}
		}
		AwardMenu(client);
	}		
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
}

void CheckVIPBonus(int client)
{
	int vipstatus;
	vipstatus=IsClientVIP(client);
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]数据未载入，无法领取(请等待下一回合或换图)");
		return;
	}
	if(vipstatus!=-1)
	{
		if(vipstatus)
		{
			if(playermission_list[client].vip==0)
			{
				PrintToChat(client," \x05[任务系统]\x01VIP奖励—10000点经验已发放");
				GrantExp(client,10000);
				playermission_list[client].vip=1;
				UpdatePlayerMissionInfo(client);
			}
			else
			{
				PrintToChat(client," \x05[任务系统]\x01你已经领取过VIP奖励!");
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
	Menu menu = CreateMenu(SecretShopHandler);
	int credits = Store_GetClientCredits(client);
	int emoney = playermission_list[client].emoney;
	menu.SetTitle("神秘商店\n您当前积分为:%d\n您当前持有的碎片为:%d",credits,playermission_list[client].emoney);
	menu.AddItem("","购买1大行动等级(3000积分)",credits>=3000?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("","购买10大行动等级(25000积分)",credits>=25000?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_wepskin_cso2axe","兑换[第一赛季-极寒咆哮]\n期限:120天 价格:200碎片",emoney>=200?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_wepskin_carbizon","兑换[第一赛季-CAR 浊心斯卡蒂]\n期限:120天 价格:250碎片",emoney>=250?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_wepskin_dualg18","兑换[第一赛季-双持GLOCK]\n期限:120天 价格:300碎片",emoney>=300?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_model_xinhai","兑换[第一赛季-心海]\n期限:永久 价格:600碎片",emoney>=600?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("uid_model_lemalin","兑换[第一赛季-恶毒]\n期限：永久 价格:1000碎片",emoney>=1000?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("","兑换积分:100碎片=500积分",emoney>=100?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int SecretShopHandler(Menu menu, MenuAction action, int client, int param)
{
	if(client<=0||client>=65) return;
	int credits = Store_GetClientCredits(client);
	int emoney = playermission_list[client].emoney;
	int current_time = GetTime();
	int expdate;
	int item_id;
	char item[256];
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]数据未载入，无法购买(请等待下一回合或换图)");
		menu.Close();
	}
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(param == 0)
		{
			if(credits>=3000&&playermission_list[client].lvl<100)
			{
				GrantExp(client,1000);	
				Store_SetClientCredits(client,credits-3000);
				PrintToChat(client," \x05[任务系统]消费积分购买了1大行动等级");
			}
			else
			{
				PrintToChat(client," \x05[任务系统]你的积分不足或你已满级!");
			}
		}
		else if(param == 1)
		{
			if(credits>=25000&&playermission_list[client].lvl<100)
			{
				GrantExp(client,10000);	
				Store_SetClientCredits(client,credits-25000);
				PrintToChat(client," \x05[任务系统]消费积分购买了10大行动等级");
			}
			else
			{
				PrintToChat(client," \x05[任务系统]你的积分不足或你已满级!");
			}
		}
		else if(param>=2&&param<=6)
		{
			menu.GetItem(param,item,sizeof(item));
			item_id = Store_GetItemIdbyUniqueId(item);
			if(param==2)
			{
				if(!Store_HasClientItem(client,item_id))
				{
					if(emoney<200)
					{
						PrintToChat(client," \x05[任务系统]\x01你持有的碎片不足!");
					}
					else
					{
						expdate = current_time+120*86400;
						Store_GiveItem(client,item_id,0,expdate,0);
						playermission_list[client].emoney-=200;						
					}
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你已拥有该道具，请勿重复兑换");
				}
			}
			if(param==3)
			{
				if(!Store_HasClientItem(client,item_id))
				{
					if(emoney<250)
					{
						PrintToChat(client," \x05[任务系统]\x01你持有的碎片不足!");
					}
					else
					{
						expdate = current_time+120*86400;
						Store_GiveItem(client,item_id,0,expdate,0);
						playermission_list[client].emoney-=250;						
					}
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你已拥有该道具，请勿重复兑换");
				}
			}
			if(param==4)
			{
				if(!Store_HasClientItem(client,item_id))
				{
					if(emoney<300)
					{
						PrintToChat(client," \x05[任务系统]\x01你持有的碎片不足!");
					}
					else
					{
						expdate = current_time+120*86400;
						Store_GiveItem(client,item_id,0,expdate,0);
						playermission_list[client].emoney-=300;
					}
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你已拥有该道具，请勿重复兑换");
				}
			}
			if(param==5)
			{
				if(!Store_HasClientItem(client,item_id))
				{
					if(emoney<600)
					{
						PrintToChat(client," \x05[任务系统]\x01你持有的碎片不足!");
					}
					else
					{
						Store_GiveItem(client,item_id,0,0,0);
						playermission_list[client].emoney-=600;						
					}
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你已拥有该道具，请勿重复兑换");
				}
			}
			if(param==6)
			{
				if(!Store_HasClientItem(client,item_id))
				{
					if(emoney<1000)
					{
						PrintToChat(client," \x05[任务系统]\x01你持有的碎片不足!");
					}
					else
					{
						Store_GiveItem(client,item_id,0,0,0);
						playermission_list[client].emoney-=1000;
					}
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你已拥有该道具，请勿重复兑换");
				}
			}
		}
		else if(param==7)
		{
			Store_SetClientCredits(client,credits+500);
			playermission_list[client].emoney-=100;
		}
		SecretShopMenu(client);
	}		
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
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
	Menu menu = CreateMenu(ChallengeTaskMenuHandler);
	menu.SetTitle("挑战任务");
	menu.AddItem("","本周挑战任务:英雄之诗~成为英雄拯救世界吧\n",ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"条件1:%s",(playermission_list[client].challenge[1]==1)?"通关地图魔戒:米纳斯提力斯\n已完成":"用剑刃切开烈火，用光明驱散暗影\n未完成");
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"条件2:%s",(playermission_list[client].challenge[2]==1)?"通关地图上古卷轴5：天际\n已完成":"黑龙的咆哮在山谷回响，当那流淌龙血的英雄降临时\n未完成");
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"条件3:%s",(playermission_list[client].challenge[3]==1)?"通关地图一夫当关\n已完成":"成为孤胆英雄，拯救世界于末日之中\n未完成");
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	menu.AddItem("","提交任务[奖励:400碎片]",(playermission_list[client].challenge[0]==1)?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}	

int ChallengeTaskMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if(client<=0||client>=65) return;
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]数据未载入，无法购买(请等待下一回合或换图)");
		menu.Close();
	}
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
	else if(action == MenuAction_Select)
	{
		if(playermission_list[client].challenge[0]!=1)
		{
			if((playermission_list[client].challenge[1]==1)&&(playermission_list[client].challenge[2]==1)&&(playermission_list[client].challenge[3]==1))
			{
				playermission_list[client].challenge[0]=1;
				playermission_list[client].emoney+=500;
				PrintToChat(client," \x05[任务系统]\x01完成挑战任务，获得500碎片!");
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
}