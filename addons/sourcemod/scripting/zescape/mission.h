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
	int dexpmax;
	int wexpmax;
}
enum struct PlayerMissionInfo
{
	int uid;
	int exp;
	int lvl;
	int dexp;
	int taskdata[11];
	int taskstage[11];
	int dataupdate_time;
	int loaded;	//temp
	int sp;
	int vip;
	int crate;
	int csave;
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
char CrateRewardInfo[][]=
{
	"人物模型-莱莎[永久]",//金勋 2%
	"人物模型-Padoru[永久]",//金勋 2%
 	"人物模型-心海[永久]",//金勋 1%
	"MVP音效-萨菲罗斯长号[永久]",
	"MVP音效-康康舞曲[永久]",
	"MVP音效-Gimme Da Blood[永久]",
	"MVP音效-莫扎特D小调安魂曲 继抒咏 神怒之日[永久]",
	"人物模型-普罗旺斯[30天]",
	"人物模型-狐九里露露[30天]",
	"人物模型-八重樱-旗袍[30天]",
	"MVP音效-88[永久]",
	"人物模型-心海[30天]",
	"MVP音效-没有名字的怪物[永久]",
	"3000积分",
	"100积分",
	"500积分",
	"MVP音效-[88][30天]",
	"人物模型-普罗旺斯[1天]",
	"人物模型-狐九里露露[1天]",
	"人物模型-八重樱-旗袍[1天]",
	"人物模型-心海[1天]"
}
int CrateRewardPoint[] =
{
	2,4,5,8,11,14,17,20,22,24,26,28,30,35,60,75,80,85,90,95,101
};
char CrateRewardList[][]=
{
	"uid_model_reisalin",
	"uid_model_Padoru",
	"uid_model_xinhai",
	"uid_mvp_sound_sephi_trombone",
	"uid_mvp_sound_cancan",
	"uid_mvp_sound_gimme",
	"uid_mvp_requiem",
	"uid_model_provence",
	"uid_model_kokuriruru",
	"uid_model_qpbcy",
	"uid_mvp_sound_bababa",
	"uid_model_xinhai",
	"uid_mvp_sound_namae_kaibutsu",
	"credits",
	"credits",
	"credits",
	"uid_mvp_sound_bababa",
	"uid_model_provence",
	"uid_model_kokuriruru",
	"uid_model_qpbcy",
	"uid_model_xinhai"
}
int CrateRewardNum[]=
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	30,
	30,
	30,
	0,
	30,
	0,
	3000,
	100,
	500,
	30,
	1,
	1,
	1,
	1
}
int MissionRewardLevels[] =
{
	10,25,40,50,60,75,90,100,110,125,140,150,160,175,190,200
};
int MissionRewardCredits[] =
{
	1000,2000,2000,2000,2000,3000,3000,3000,3000,4000,4000,4000,4000,5000,5000,10000
};
int MissionRewardCrates[] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
char MissionRewardItems[][] =
{
	"none",//10
	"uid_mvp_s5mvp1",//25
	"none",//40
	"none",//50
	"none",//60
	"uid_mvp_s5mvp2",//75
	"none",//90
	"uid_model_herloher",//100
	"none",//110
	"uid_mvp_s5mvp3",//75
	"none",//140
	"none",//150
	"none",//160
	"uid_mvp_s5mvp4",//75
	"none",//190
	"uid_model_machit"//200
};
char MissionRewardInfos[][] =
{
	"LV10:积分*1000",
	"LV25:MVP音乐:紅蓮の弓矢/积分*2000",
	"LV40:积分*2000",
	"LV50:积分*2000/两倍勋章(赛季内自动开启)",
	"LV60:积分*2000",
	"LV75:MVP音乐:the monster within/积分*3000",
	"LV90:积分*3000",
	"LV100:人物模型-赫洛赫尔 華折[永久]/积分*3000",
	"LV110:积分*3000",
	"LV125:MVP音乐:心臓を捧げよ!/积分*4000",
	"LV140:积分*4000",
	"LV150:积分*4000/三倍勋章(赛季内自动开启)",
	"LV160:积分*4000",
	"LV175:MVP音乐:Barricades/积分*5000",
	"LV190:积分*5000",
	"LV200:人物模型:诗歌剧[永久]/积分*10000"
};

PlayerMissionRoundBuffer playermission_rounddata[65];
PlayerMissionRoundBuffer nullrounddata;
ArrayList Current_Mission_Tasklist;
bool g_ValidMission_Exist;
PlayerMissionInfo playermission_list[65];
PlayerMissionInfo nullpmi;
Missions Current_Mission;
LEVEL_LOG Mission_Current_Level;
int g_Daily_Task_Exp_Factor = 1;
int g_Weekly_Task_Exp_Factor = 1;
int round_starttime;
int round_endtime;
const int mission_lowest_playernum = 20;
void MissionOnPluginStart()
{
	round_starttime = GetTime();
	Mission_Current_Level.id=-1;
	Mission_Current_Level.diff = 0;
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
	for(int i=0;i<=5;i++)
	{
		playermission_list[client].taskdata[i]=0;
		playermission_list[client].taskstage[i]=0;
	}
	playermission_list[client].loaded = 1;
	playermission_list[client].dexp = 0;
	UpdatePlayerMissionInfo(client);
}
void PlayerMissionWeeklyUpdate(int client)
{
	if(client<=0||client>=65)	return;
	PrintToServer(" \x05[调试]玩家%d的每周任务数据过期，清空中...",client);
	for(int i=6;i<=10;i++)
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
	if(!IsClientInGame(client))	return;
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
		playermission_list[client].dexp = DbFetchInt(hndl,"DEXP");
		playermission_list[client].taskstage[0]=DbFetchInt(hndl,"DT1ST");
		playermission_list[client].taskstage[1]=DbFetchInt(hndl,"DT2ST");
		playermission_list[client].taskstage[2]=DbFetchInt(hndl,"DT3ST");
		playermission_list[client].taskstage[3]=DbFetchInt(hndl,"DT4ST");
		playermission_list[client].taskstage[4]=DbFetchInt(hndl,"DT5ST");
		playermission_list[client].taskstage[5]=DbFetchInt(hndl,"DT6ST");
		playermission_list[client].taskstage[6]=DbFetchInt(hndl,"WT1ST");
		playermission_list[client].taskstage[7]=DbFetchInt(hndl,"WT2ST");
		playermission_list[client].taskstage[8]=DbFetchInt(hndl,"WT3ST");
		playermission_list[client].taskstage[9]=DbFetchInt(hndl,"WT4ST");
		playermission_list[client].taskstage[10]=DbFetchInt(hndl,"WT5ST");
		playermission_list[client].taskdata[0]=DbFetchInt(hndl,"DINFECT");
		playermission_list[client].taskdata[1]=DbFetchInt(hndl,"DKILLZM");
		playermission_list[client].taskdata[2]=DbFetchInt(hndl,"DDMGTAKE");
		playermission_list[client].taskdata[3]=DbFetchInt(hndl,"DDMGMAKE");
		playermission_list[client].taskdata[4]=DbFetchInt(hndl,"DHIT");
		playermission_list[client].taskdata[5]=DbFetchInt(hndl,"DNADE");
		playermission_list[client].taskdata[6]=DbFetchInt(hndl,"WINFECT");
		playermission_list[client].taskdata[7]=DbFetchInt(hndl,"WKILLZM");
		playermission_list[client].taskdata[8]=DbFetchInt(hndl,"WDMGTAKE");
		playermission_list[client].taskdata[9]=DbFetchInt(hndl,"WDMGMAKE");
		playermission_list[client].taskdata[10]=DbFetchInt(hndl,"WNADE");
		playermission_list[client].crate = DbFetchInt(hndl,"CRATE");
		playermission_list[client].csave = DbFetchInt(hndl,"CSAVE");
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
	delete hndl;
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
	Format(query,sizeof(query),"UPDATE %s SET DINFECT = 0, DKILLZM = 0, DDMGTAKE=0, DDMGMAKE=0, DHIT=0, DNADE=0, DT1ST=0, DT2ST=0, DT3ST=0, DT4ST=0, DT5ST=0, DT6ST=0, DEXP = 0",Current_Mission.playerdbname);

	if(weekly)
	{
		Format(query,sizeof(query),"UPDATE %s SET DINFECT = 0, DKILLZM = 0, DDMGTAKE=0, DDMGMAKE=0, DHIT=0, DNADE=0, WINFECT = 0, WKILLZM = 0, WDMGTAKE = 0, WDMGMAKE = 0,WNADE = 0, DT1ST=0, DT2ST=0, DT3ST=0, DT4ST=0, DT5ST=0, DT6ST=0, WT1ST=0, WT2ST=0, WT3ST=0, WT4ST=0,WT5ST =0, DEXP =0",Current_Mission.playerdbname)
	}
	PrintToChatAll(" \x05[任务系统]清空大行动任务数据....");
	DbTQuery(DbClearPlayerMissionInfoCallback,query);
}

void DbClearPlayerMissionInfoCallback(Handle owner, Handle hndl, char[] error, any data)
{
	ReloadAllPlayerMissionInfo();
	delete hndl;
}

void UpdatePlayerMissionInfo(int client,int force=0)
{
	if(!g_ValidMission_Exist)
	{
		return;
	}
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
	int dexp = playermission_list[client].dexp;
	int taskdata[16];
	int taskstage[16];
	int sp = playermission_list[client].sp;
	int vip = playermission_list[client].vip;
	int crate = playermission_list[client].crate;
	int csave = playermission_list[client].csave;
	for(int i =0;i<=10;i++)
	{
		taskdata[i]=playermission_list[client].taskdata[i];
		taskstage[i]=playermission_list[client].taskstage[i];
	}

	int current_time = GetTime();
	PrintToServer("[任务系统][UID:%d]保存玩家%d数据",uid,client);
	Format(query,sizeof(query),"UPDATE %s SET LVL = %d, EXP = %d, DEXP = %d,\
	DINFECT = %d,DKILLZM = %d,DDMGTAKE = %d,DDMGMAKE = %d,DHIT = %d, DNADE = %d, \
	WINFECT = %d, WKILLZM = %d, WDMGTAKE = %d, WDMGMAKE = %d, WNADE = %d, \
	DT1ST = %d, DT2ST = %d, DT3ST = %d, DT4ST = %d, DT5ST =%d, DT6ST =%d, \
	WT1ST = %d, WT2ST = %d, WT3ST = %d, WT4ST = %d, WT5ST = %d, \
	TIMESTAMP = %d, SP = %d, VIP = %d, CRATE = %d, CSAVE = %d, CH1 = %d, CH2 = %d, CH3 = %d, CH4 = %d, CH = %d WHERE UID = %d",
	Current_Mission.playerdbname,lvl,exp,dexp,
	taskdata[0],taskdata[1],taskdata[2],taskdata[3],taskdata[4],taskdata[5],taskdata[6],taskdata[7],taskdata[8],taskdata[9],taskdata[10],
	taskstage[0],taskstage[1],taskstage[2],taskstage[3],taskstage[4],taskstage[5],taskstage[6],taskstage[7],taskstage[8],taskstage[9],taskstage[10],
	current_time,sp,vip,crate,csave,
	playermission_list[client].challenge[1],playermission_list[client].challenge[2],playermission_list[client].challenge[3],playermission_list[client].challenge[4],playermission_list[client].challenge[0],uid);
	DbTQuery(DbQueryErrorCallback,query);
	if(force==1)
	{
		playermission_list[client].uid=0;
		playermission_list[client].lvl=0;
		playermission_list[client].exp=0
		playermission_list[client].dexp=0;
		playermission_list[client].sp=0;
		playermission_list[client].vip=0;
		for(int i=0;i<=10;i++)
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
	if(!g_ValidMission_Exist)
	{
		return;
	}
	char map_name[64];
	int player_pass = 0;
	GetCurrentMap(map_name,sizeof(map_name));
	round_endtime = GetTime();
	int exp_bonus = 0;
	int round_time = round_endtime - round_starttime;
	if(round_time<=120||GetClientCount(true)<mission_lowest_playernum)
	{
		PrintToChatAll(" \x05[活动系统]\x01人数不足\x07%d\x01人，或回合时间<2分钟，不计入",mission_lowest_playernum);
		for(int i=1;i<=64;i++)	UpdatePlayerMissionInfo(i);
		return;
	}
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				if(winner==3&&IsPlayerAlive(i)&&ZR_IsClientHuman(i))
				{
					player_pass++;
				}
			}
		}
	}
	switch(Pmap.difficulty)
	{
		case 0:
		{
			exp_bonus = 50;
		}
		case 1:
		{
			exp_bonus = 80;
		}
		case 2:
		{
			exp_bonus = 100;
		}
		case 3:
		{
			exp_bonus = 120;
		}
		case 4:
		{
			exp_bonus = 150;
		}
		default:
		{
			exp_bonus = 50;
		}
	}
	if(Pmap.tag&label_code[8])
	{
		exp_bonus += 70;
	}
	if(Pmap.tag&label_code[10])
	{
		exp_bonus += 20;
	}
	if(Pmap.tag&label_code[9])
	{
		exp_bonus/=2;
	}
	if(player_pass>=30)
	{
		exp_bonus +=40;
	}
	if(Mission_Current_Level.diff>0&&Mission_Current_Level.id!=-1)
	{
		exp_bonus +=15*Mission_Current_Level.diff;
	}
	exp_bonus *= g_Daily_Task_Exp_Factor;
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				if(winner==3&&IsPlayerAlive(i)&&ZR_IsClientHuman(i))
				{

					PrintToChat(i," \x05[赛季活动]\x01计算地图通关经验为:%d",exp_bonus);
					int dexp_max = 1500;
					if(IsClientVIP(i))
					{
						dexp_max = 2000;
					}
					if(playermission_list[i].dexp+exp_bonus<=dexp_max)
					{
						playermission_list[i].dexp+=exp_bonus;
						GrantExp(i,exp_bonus);
					}
					else
					{
						GrantExp(i,dexp_max-playermission_list[i].dexp);
						playermission_list[i].dexp = dexp_max;

					}
				}
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
	if(playermission_list[attacker].taskdata[9]<=10000000)
	{
		playermission_list[attacker].taskdata[9]+=dmg;
	}
	if(playermission_list[victim].taskdata[2]<=10000000)
	{
		playermission_list[victim].taskdata[2]+=dmg;
	}
	if(playermission_list[victim].taskdata[8]<=10000000)
	{
		playermission_list[victim].taskdata[8]+=dmg;
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
	if(playermission_list[attacker].taskdata[7]<=1000)
	{
		playermission_list[attacker].taskdata[7]++;
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
	if(playermission_list[attacker].taskdata[6]<=1000)
	{
		playermission_list[attacker].taskdata[6]++;
	}
}

void MissionHumanNadeCount(int client)
{
	if(GetClientCount(true)<mission_lowest_playernum)	return;
	if(playermission_list[client].taskdata[5]<=1000)
	{
		playermission_list[client].taskdata[5]++;
	}
	if(playermission_list[client].taskdata[10]<=1000)
	{
		playermission_list[client].taskdata[10]++;
	}
}
void MissionOnMapEnd()
{
	Mission_Current_Level.id=-1;
	Mission_Current_Level.bounty = 0;
	Mission_Current_Level.diff = 0;
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
	delete hndl;
}

void TEMP_OpHR_TasklistSet()
{
	delete Current_Mission_Tasklist;
	Current_Mission_Tasklist = CreateArray(sizeof(TASK));
	TASK task;
	task.stage = 3;
	task.num[0] = 2;task.num[1] = 4;task.num[2] = 6;
	task.exp_base = 50;
	task.period = 1;
	task.type = ZM_INFECT;
	task.name = "[僵尸]感染人类:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 1;task.num[1] = 2;task.num[2] = 3;
	task.exp_base = 50;
	task.period = 1;
	task.type = HM_KILLZM;
	task.name = "[人类]击杀僵尸:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 5;task.num[1] = 10;task.num[2] = 15;
	task.exp_base = 100;
	task.period = 1;
	task.type = ZM_DMGTAKE;
	task.name = "[僵尸]承受伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 4;task.num[1] = 8;task.num[2] = 12;
	task.exp_base = 100;
	task.period = 1;
	task.type = HM_DMGMAKE;
	task.name = "[人类]造成伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 200;task.num[1] = 400;task.num[2] = 800;
	task.exp_base = 50;
	task.period = 1;
	task.type = HM_HIT;
	task.name = "[人类]攻击障碍/BOSS(次):";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 3;
	task.num[0] = 5;task.num[1] = 15;task.num[2] = 40;
	task.exp_base = 50;
	task.period = 1;
	task.type = HM_NADE;
	task.name = "[人类]投掷手雷:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 30;
	task.exp_base = 1000;
	task.period = 7;
	task.type = ZM_INFECT;
	task.name = "[僵尸]感染人类:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 15;
	task.exp_base = 1000;
	task.period = 7;
	task.type = HM_KILLZM;
	task.name = "[人类]击杀僵尸:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 120;
	task.exp_base = 1000;
	task.period = 7;
	task.type = ZM_DMGTAKE;
	task.name = "[僵尸]承受伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 80;
	task.exp_base = 1000;
	task.period = 7;
	task.type = HM_DMGMAKE;
	task.name = "[人类]造成伤害:";
	Current_Mission_Tasklist.PushArray(task);

	task.stage = 1;
	task.num[0] = 150;
	task.exp_base = 1000;
	task.period = 7;
	task.type = HM_NADE;
	task.name = "[人类]投掷手雷:";
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
	int dexp_max = 1500;
	if(IsClientVIP(client))
	{
		dexp_max = 2000;
	}
	Format(buffer,sizeof(buffer),"赛季活动\n%s\n%s\nLV.%d/%d\nEXP:%d/%d 今日通关经验:%d/%d",Current_Mission.cnname,Current_Mission.name,playermission_list[client].lvl,Current_Mission.max_level,playermission_list[client].exp,Current_Mission.level_exp,playermission_list[client].dexp,dexp_max);
	menu.SetTitle(buffer);
	menu.AddItem("","日常任务[8月4日2点作战结束]");
	menu.AddItem("","周常任务[8月4日2点作战结束]");
	menu.AddItem("","奖励兑换");
	menu.AddItem("","神秘商店");
	menu.AddItem("","悬赏任务[暂未开放]",ITEMDRAW_DISABLED);
	menu.Display(client, MENU_TIME_FOREVER);
}

int MissionMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		if (param == 0)
		{
			DailyTaskMenu(client);
		}
		if (param == 1)
		{
			WeeklyTaskMenu(client);
		}
		if (param == 2)
		{
			AwardMenu(client);
		}
		if (param == 3)
		{
			SecretShopMenu(client);
		}
		if (param == 4)
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
				Format(buffer2,sizeof(buffer2),"%s >已完成<",task.name);
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
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		menu.GetItem(param,buffer,sizeof(buffer));
		int taskid=StringToInt(buffer);
		GetArrayArray(Current_Mission_Tasklist,taskid,task,sizeof(task));
		if(playermission_list[client].taskstage[taskid]>=task.stage)
		{
			PrintToChat(client," \x05[任务系统]\x01你已完成该任务!");
		}
		if(task.type==ZM_DMGTAKE||task.type==HM_DMGMAKE)
		{
			if((playermission_list[client].taskdata[taskid]/10000)>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Daily_Task_Exp_Factor);
			}
			else
			{
				PrintToChat(client," \x05[任务系统]\x01未达成任务条件");
			}
		}
		else
		{
			if((playermission_list[client].taskdata[taskid])>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Daily_Task_Exp_Factor);
			}
			else
			{
					PrintToChat(client," \x05[任务系统]\x01未达成任务条件");
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
				Format(buffer2,sizeof(buffer2),"%s >已完成<",task.name);
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
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		menu.GetItem(param,buffer,sizeof(buffer));
		int taskid=StringToInt(buffer);
		GetArrayArray(Current_Mission_Tasklist,taskid,task,sizeof(task));
		if(playermission_list[client].taskstage[taskid]>=task.stage)
		{
			PrintToChat(client," \x05[任务系统]\x01你已完成该任务!");
		}
		if(task.type==ZM_DMGTAKE||task.type==HM_DMGMAKE)
		{
			if((playermission_list[client].taskdata[taskid]/10000)>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Weekly_Task_Exp_Factor);
			}
			else
			{
				PrintToChat(client," \x05[任务系统]\x01未达成任务条件");
			}
		}
		else
		{
			if((playermission_list[client].taskdata[taskid])>=task.num[playermission_list[client].taskstage[taskid]])
			{
				playermission_list[client].taskstage[taskid]++;
				GrantExp(client,task.exp_base*(playermission_list[client].taskstage[taskid])*g_Weekly_Task_Exp_Factor);
			}
			else
			{
				PrintToChat(client," \x05[任务系统]\x01未达成任务条件");
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
			PrintToChat(client," \x05[任务系统] \x01您在\x09%s\x01的等级提升到了\x09%d\x01)",Current_Mission.name,playermission_list[client].lvl);
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
	if(!Store_IsClientLoaded(client))
	{
		PrintToChat(client," \x05[赛季活动]\x01您的商店数据还未载入!");
		return;
	}
	Menu menu = CreateMenu(AwardMenuHandle);
	int iStyle = (g_pStore) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED;
	menu.SetTitle("特典奖励领取\n%s", (g_pStore) ? "点击对应选项领取奖励" : "商店插件未加载, 请稍后再试");
	for (int i=0; i<sizeof(MissionRewardInfos);i++)
	{
		iStyle = (!g_pStore) ? ITEMDRAW_DISABLED : (playermission_list[client].sp&(1<<(i))) ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT;
		menu.AddItem("", MissionRewardInfos[i], iStyle);
	}

	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int AwardMenuHandle(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		if(!g_pStore||!Store_IsClientLoaded(client))
		{
			PrintToChat(client," \x05[赛季活动]\x01您的商店数据还未载入!");
			return 0;
		}
		if(GetTime()<=Store_GetClientDataProtect(client))
		{
			PrintToChat(client," \x05[赛季活动]\x01数据保护中，无法操作!");
			return 0;
		}
		if(playermission_list[client].loaded==0)
		{
			PrintToChat(client," \x05[赛季活动]数据未载入，无法购买(请等待下一回合或输入!msr)");
			return 0;
		}
		int client_credits = Store_GetClientCredits(client);
		if(playermission_list[client].sp&(1<<(param)))
		{
			PrintToChat(client," \x05[赛季活动]\x01已经领取过该奖励！");
			AwardMenu(client);
			return 0;
		}
		if(playermission_list[client].lvl<MissionRewardLevels[param])
		{
			PrintToChat(client," \x05[赛季活动]\x01领取所需等级不足!需要赛季等级\x09%d\x01级，而你只有\x09%d\x01级",MissionRewardLevels[param],playermission_list[client].lvl);
			AwardMenu(client);
			return 0;
		}
		if(MissionRewardCredits[param]!=0)
		{
			Store_SetClientCredits(client,client_credits + MissionRewardCredits[param],"赛季活动 等级特典奖励");
		}
		if(MissionRewardCrates[param]!=0)
		{
			playermission_list[client].crate+= MissionRewardCrates[param];
		}
		if(strcmp(MissionRewardItems[param],"none",false)!=0)
		{
			int item_id = Store_GetItemId(MissionRewardItems[param]);
			Store_GiveItem(client,item_id,0,0,0);
			UpdatePlayerMissionInfo(client);
		}
		playermission_list[client].sp=playermission_list[client].sp|(1<<(param));
		PrintToChat(client," \x07[赛季活动]\x01恭喜你在第四赛季活动中获得特典物品:%s",MissionRewardInfos[param]);
		AwardMenu(client);
		return 0;
	}
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
	return 0;
}

void SecretShopMenu(int client)
{
	if(client<=0||client>=65)	return;
	if(!g_pStore)
	{
		PrintToChat(client," \x05[任务系统]\x01商店插件未载入或维护中");
		return;
	}
	if(!Store_IsClientLoaded(client))
	{
		PrintToChat(client," \x05[赛季活动]\x01您的商店数据还未载入!");
		return;
	}
	Menu menu = CreateMenu(SecretShopHandler);
	int credits = Store_GetClientCredits(client);
	int crate = playermission_list[client].crate;
	int csave = playermission_list[client].csave;
	menu.SetTitle("神秘商店\n您当前积分为:%d\n",credits);
	menu.AddItem("","购买1大行动等级(5000积分)",ITEMDRAW_DEFAULT);
	menu.AddItem("","购买10大行动等级(45000积分)",ITEMDRAW_DEFAULT);
	menu.AddItem("","购买赛季箱子(2000积分)[暂未开放]",ITEMDRAW_DISABLED);
	menu.AddItem("","购买10*赛季箱子(20000积分)[暂未开放]",ITEMDRAW_DISABLED);
	menu.AddItem("","打开赛季箱子[暂未开放]",ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int SecretShopHandler(Menu menu, MenuAction action, int client, int param)
{
	if(client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	int current_time = GetTime();
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]数据未载入，无法购买(请等待下一回合或输入!msr)");
		delete menu;
	}
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
	}
	else if(action == MenuAction_Select)
	{
		if(!g_pStore||!Store_IsClientLoaded(client))
		{
			PrintToChat(client," \x05[赛季活动]\x01您的商店数据还未载入!");
			return 0;
		}
		int credits = Store_GetClientCredits(client);
		int crate = playermission_list[client].crate;
		int csave = playermission_list[client].csave;
		if(GetTime()<=Store_GetClientDataProtect(client))
		{
			PrintToChat(client," \x05[赛季活动]\x01数据保护中，无法操作!");
			return 0;
		}
		switch(param)
		{

			case 0:
			{
				if(!g_pStore)
				{
					PrintToChat(client," \x05[赛季活动]\x01商店插件出错!");
					return 0;
				}
				if(credits>=5000&&playermission_list[client].lvl<Current_Mission.max_level)
				{
					GrantExp(client,Current_Mission.level_exp);
					Store_SetClientCredits(client,credits-5000,"活动");
					PrintToChat(client," \x05[任务系统]\x01消费积分购买了1大行动等级");
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你的积分不足或你已满级!");
				}
			}
			case 1:
			{
				if(!g_pStore)
				{
					PrintToChat(client," \x05[赛季活动]\x01商店插件出错!");
					return 0;
				}
				if(credits>=45000&&playermission_list[client].lvl<Current_Mission.max_level)
				{
					GrantExp(client,10*(Current_Mission.level_exp));
					Store_SetClientCredits(client,credits-45000,"活动");
					PrintToChat(client," \x05[任务系统]\x01消费积分购买了10大行动等级");
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你的积分不足或你已满级!");
				}
			}
/*			case 2:
			{
				if(!g_pStore)
				{
					PrintToChat(client," \x05[赛季活动]\x01商店插件出错!");
					return 0;
				}
				if(credits>=2000)
				{
					playermission_list[client].crate+=1;
					Store_SetClientCredits(client,credits-2000,"赛季活动-购买箱子");
					PrintToChat(client," \x05[任务系统]\x01消费积分购买了1*赛季箱子");
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你的积分不足!");
				}
			}
			case 3:
			{
				if(!g_pStore)
				{
					PrintToChat(client," \x05[赛季活动]\x01商店插件出错!");
					return 0;
				}
				if(credits>=20000)
				{
					playermission_list[client].crate+=10;
					Store_SetClientCredits(client,credits-20000,"赛季活动-购买箱子");
					PrintToChat(client," \x05[任务系统]\x01消费积分购买了10*赛季箱子");
				}
				else
				{
					PrintToChat(client," \x05[任务系统]\x01你的积分不足!");
				}
			}
			case 4:
			{
				if(!g_pStore)
				{
					PrintToChat(client," \x05[赛季活动]\x01商店插件出错!");
					return 0;
				}
				if(playermission_list[client].crate>=1)
				{
					playermission_list[client].crate-=1;
					MissionOpenCrate(client);
				}
			}
			case 5:
			{
				if(!g_pStore)
				{
					PrintToChat(client," \x05[赛季活动]\x01商店插件出错!");
					return 0;
				}
				int item_id = Store_GetItemId("uid_model_reisalin");
				if(csave>=120)
				{
					playermission_list[client].csave-=120;
					Store_GiveItem(client,item_id,0,0,0);
				}
				else
				{
					PrintToChat(client," \x05[赛季活动]\x01音符碎片不足!");
				}
			}
			case 6:
			{
				if(!g_pStore)
				{
					PrintToChat(client," \x05[赛季活动]\x01商店插件出错!");
					return 0;
				}
				int item_id = Store_GetItemId("uid_wepskin_wushibb");
				if(csave>=80)
				{
					playermission_list[client].csave-=80;
					Store_GiveItem(client,item_id,0,0,0);
				}
				else
				{
					PrintToChat(client," \x05[赛季活动]\x01音符碎片不足!");
				}
			} */
		}
		UpdatePlayerMissionInfo(client);
		SecretShopMenu(client);
		return 0;
	}
	else if (param == MenuCancel_ExitBack) MissionMenuBuild(client);
	return 0;
}
void MissionOpenCrate(int client)
{
	if(!g_pStore)
	{
		return;
	}
	if(GetTime()<=Store_GetClientDataProtect(client))
	{
		PrintToChat(client," \x05[赛季活动]\x01数据保护中，无法操作!");
	}
	int credits = Store_GetClientCredits(client);
	int result = GetURandomInt()%100+1;
	int item_id;
	char player_name[64];
	GetClientName(client,player_name,64);
	PrintToChat(client," \x05[赛季活动]\x01开启赛季箱子获得一个音符碎片");
	playermission_list[client].csave++;
	for(int i=0;i<=19;i++)
	{
		if(i==0)
		{
			if(result<=2)
			{
				item_id = Store_GetItemId(CrateRewardList[i]);
				PrintToChatAll(" \x05[赛季活动]\x01恭喜玩家%s开启赛季箱子获得了金勋奖>%s<，让我们恭喜这个B!",player_name,CrateRewardInfo[i]);
				Store_GiveItem(client,item_id,0,0,0);
			}
		}
		else
		{
			if(result>CrateRewardPoint[i-1]&&result<=CrateRewardPoint[i])
			{
				if(strcmp(CrateRewardList[i],"credits",false)!=0)
				{
					item_id = Store_GetItemId(CrateRewardList[i]);
					if(CrateRewardNum[i]!=0)
					{

						Store_GiveItem(client,item_id,0,GetTime()+CrateRewardNum[i]*86400,0);
					}
					else
					{
						Store_GiveItem(client,item_id,0,0,0);
					}
				}
				else
				{
					Store_SetClientCredits(client,credits+CrateRewardNum[i],"赛季开箱");
				}
				if(result<=5)
				{
					PrintToChatAll(" \x05[赛季活动]\x01恭喜玩家%s开启赛季箱子获得了金勋奖>%s<，让我们恭喜这个B!",player_name,CrateRewardInfo[i]);
				}
				if(result<=35&&result>5)
				{
					PrintToChat(client," \x05[赛季活动]\x01恭喜您开启赛季箱子获得了银勋奖>%s<",CrateRewardInfo[i]);
				}
				if(result>35)
				{
					PrintToChat(client," \x05[赛季活动]\x01恭喜您开启赛季箱子获得了铜勋奖>%s<",CrateRewardInfo[i]);
				}
			}
		}
	}
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
	return;
}
int Native_RY_ZE_ZBUYCOUNT(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int price = GetNativeCell(2);
	playermission_rounddata[client].pay+=price;
	return 0;
}
/*int ChallengeTaskMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if(client<=0||client>=65)
	{
		menu.Close();
		return 0;
	}
	if(!playermission_list[client].loaded)
	{
		PrintToChat(client," \x05[任务系统]\x01数据未载入，无法购买(请等待下一回合或换图)");
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
}*/
