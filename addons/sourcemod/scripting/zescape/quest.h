enum struct QUEST
{
	int id;
	TASK_TYPE type;
	int num;
	int award;
	int period;
	char name[64];
}
enum struct PlayerQuestInfo
{
	int taskdata[16];
	int taskcomplete[16];
	int loaded;
	int timestamp;
	int uid;
	int letter[7];
	int letter_daily[7];
	int crate;
	int crate_total;
	int frag;
	int frag_daily;
}
char festcrate_awardlist[][] = {
	"fest_letter",	//2%
	"100credits",	//30%
	"500credits",	//15%
	"uid_wepskin_naxitan",	//180d,5%
	"uid_ride_thomasthank",	//180d,3%
	"uid_wepskin_blthm",	//30d,10%
	"uid_wepskin_sfrif",	//30d,10%
	"uid_wepskin_akcfhd",	//30d,10%
	"uid_wepskin_thomson",	//30d,10%
	"uid_mvp_sound_gongxifacai" //180d,5%
}
char letter_map[][]={
	"mako_reactor_v5_3", 	//3
	"lotr_minas_tirith",	//7
	"lotr_minas_tirith",	//8
	"obj_npst_v2",			//-1
	"santa",				//3
	"santa",				//4
	"westersand_v8zeta1k",	//3
	"djinn",				//-1
	"uchiha_legacy",		//3
	"a_e_s",				//-1
	"slender",				//3
	"paramina",				//3
	"pirates_port_royal",	//5
	"sandstorm",			//5
	"tesv",					//4
	"cosmo_canyon_v5k_fix",	//4
	"ancient_wrath_v2_test27",	//4
	"deadcore",				//-1
	"grau",					//1
	"frozen_abyss",			//-1
	"predator_ultimate",	//3
	"lila_panic",			//7
	"ashen",				//1
	"ashen",				//2
	"cosmo_canyon_v5r",		//4
	"castlevania",			//2
	"castlevania",			//5
	"obj_insane",			//-1
	"shroomforest2",		//7
	"yueyingchuanshuo_10_9_f3",	//0
	"yueyingchuanshuo_10_9_f3",	//1
	"wanderers_palace_v6",	//3
	"wanderers_palace_v6",	//4
	"serpentis_temple_csgo5",//2
	"serpentis_temple_csgo5",//3
	"serpentis_temple_csgo5",//4
	"stalker_ultimate_v2_3_gg",	//4
	"fapescape_rote_v1_3",	//4
	"fapescape_rote_v1_3",	//5
};
int letter_map_level[39]=
{
	3,7,8,-1,3,4,3,-1,3,-1,3,3,5,5,4,4,4,-1,1,-1,3,7,1,2,4,2,5,-1,7,0,1,3,4,2,3,4,4,4,5
}
ArrayList QuestList;
PlayerQuestInfo playerquest_list[65];
LEVEL_LOG Quest_Current_Level;
const int quest_plimit = 20;
int quest_roundstart_time;
bool RollDice(int possibility)
{
	int result = GetURandomInt()%100;
	if(result<possibility)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void OpenCrate(int client)
{
	if(!g_pStore)
	{
		PrintToChat(client," \x05[活动系统]\x01商店系统未载入!");
		return;
	}
	playerquest_list[client].crate-=1;
	playerquest_list[client].crate_total+=1;
	PrintToChat(client," \x05[活动系统]\x01打开新春箱子获得\x07贺岁红包\x01一个")
	char player_name[64];
	GetClientName(client,player_name,sizeof(player_name));
	int credits = Store_GetClientCredits(client);
	int result = GetURandomInt()%100;
	int current_time = GetTime();
	int item_id;
	char unid[64];
	UpdatePlayerQuestInfo(client);
	if(result<2)
	{
		playerquest_list[client].letter[5]++;
		playerquest_list[client].letter_daily[5]++;
		PrintToChatAll(" \x05[活动系统]\x01玩家%s抽中了金奖-\x07[快]字",player_name);
		UpdatePlayerQuestInfo(client);
		return;
	}
	if(result<32)
	{
		Store_SetClientCredits(client,credits+100,"活动");
		PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖-100积分");
		return;
	}
	if(result<47)
	{
		Store_SetClientCredits(client,credits+500,"活动");
		PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖-500积分");
		return;
	}
	if(result<52)
	{
		strcopy(unid,sizeof(unid),festcrate_awardlist[3]);
		item_id = Store_GetItemId(unid);
		if(Store_HasClientItem(client,item_id))
		{
			Store_SetClientCredits(client,credits+300,"活动");
			PrintToChat(client," \x05[活动系统]\x01你抽中了银奖草神M249，但你已经有该道具了，返还300积分");
		}
		else
		{
			Store_GiveItem(client,item_id,0,current_time+180*86400,0);
			PrintToChat(client," \x05[活动系统]\x01你抽中了银奖-\x08草神M249180天");
		}
		return;
	}
	if(result<55)
	{
		strcopy(unid,sizeof(unid),festcrate_awardlist[4]);
		item_id = Store_GetItemId(unid);
		if(Store_HasClientItem(client,item_id))
		{
			Store_SetClientCredits(client,credits+300,"活动");
			PrintToChat(client," \x05[活动系统]\x01你抽中了银奖托马斯小火车，但你已经有该道具了，返还300积分");
		}
		else
		{
			Store_GiveItem(client,item_id,0,current_time+30*86400,0);
			PrintToChat(client," \x05[活动系统]\x01你抽中了银奖-\x08托马斯小火车30天");
		}
		return;
	}
	if(result<65)
	{
		strcopy(unid,sizeof(unid),festcrate_awardlist[5]);
		item_id = Store_GetItemId(unid);
		if(Store_HasClientItem(client,item_id))
		{
			Store_SetClientCredits(client,credits+100,"活动");
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖巴雷特毁灭，但你已经有该道具了，返还100积分");
		}
		else
		{
			Store_GiveItem(client,item_id,0,current_time+30*86400,0);
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖-\x08AWP-巴雷特毁灭-30天");
		}
		return;
	}
	if(result<75)
	{
		strcopy(unid,sizeof(unid),festcrate_awardlist[6]);
		item_id = Store_GetItemId(unid);
		if(Store_HasClientItem(client,item_id))
		{
			Store_SetClientCredits(client,credits+100,"活动");
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖星际重炮，但你已经有该道具了，返还100积分");
		}
		else
		{
			Store_GiveItem(client,item_id,0,current_time+30*86400,0);
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖-\x08M4A4-星际重炮-30天");
		}
		return;
	}
	if(result<85)
	{
		strcopy(unid,sizeof(unid),festcrate_awardlist[7]);
		item_id = Store_GetItemId(unid);
		if(Store_HasClientItem(client,item_id))
		{
			Store_SetClientCredits(client,credits+100,"活动");
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖末日审判，但你已经有该道具了，返还100积分");
		}
		else
		{
			Store_GiveItem(client,item_id,0,current_time+30*86400,0);
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖-\x08AK47-末日审判-30天");
		}
		return;
	}
	if(result<95)
	{
		strcopy(unid,sizeof(unid),festcrate_awardlist[8]);
		item_id = Store_GetItemId(unid);
		if(Store_HasClientItem(client,item_id))
		{
			Store_SetClientCredits(client,credits+100,"活动");
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖星标汤姆逊，但你已经有该道具了，返还100积分");
		}
		else
		{
			Store_GiveItem(client,item_id,0,current_time+30*86400,0);
			PrintToChat(client," \x05[活动系统]\x01你抽中了铜奖-\x08野牛-星标汤姆逊-30天");
		}
		return;
	}
	strcopy(unid,sizeof(unid),festcrate_awardlist[8]);
	item_id = Store_GetItemId(unid);
	if(Store_HasClientItem(client,item_id))
	{
		Store_SetClientCredits(client,credits+300,"活动");
		PrintToChat(client," \x05[活动系统]\x01你抽中了银奖恭喜发财，但你已经有该道具了，返还300积分");
	}
	else
	{
		Store_GiveItem(client,item_id,0,current_time+180*86400,0);
		PrintToChat(client," \x05[活动系统]\x01你抽中了银奖-\x08MVP音乐-恭喜发财-180天");
	}
	return;
}
void QuestOnPluginStart()
{
	RegConsoleCmd("sm_rw",QuestCommand);
	RegConsoleCmd("sm_quest",QuestCommand);
	ReloadAllPlayerQuestInfo();
	QuestSet();
	quest_roundstart_time = GetTime();
}
void QuestOnMapStart()
{
	Quest_Current_Level.id = -1;
	CloseHandleSafe(QuestList);
	QuestSet();
}
void QuestOnMapEnd()
{
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				UpdatePlayerQuestInfo(i);
			}
		}
	}
}
void QuestOnDailyUpdate()
{
	PrintToConsoleAll(" [任务系统]日常活动新活动日触发");
	ClearAllPlayerQuestInfo();
}
Action QuestCommand(int client,int args)
{
	if(client<=0)	return Plugin_Handled;
	if (!IsClientInGame(client)) return Plugin_Handled;
	if(playerquest_list[client].loaded==0)
	{
		PrintToChat(client," \x05[任务系统]\x01载入玩家数据出错，重载数据");
		LoadPlayerQuestInfo(client);
		return Plugin_Handled;
	}
	Menu menu = CreateMenu(QuestMenuHandler);
	menu.SetTitle("任务系统");
	menu.AddItem("","每日任务");
	menu.AddItem("","每周任务",ITEMDRAW_DISABLED);
	menu.AddItem("","赛季任务");
	menu.Display(client, MENU_TIME_FOREVER);
	return Plugin_Handled;
}
int QuestMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End)
	{
		delete menu;
		return 0;
	}
	else if (action == MenuAction_Select)
	{
		if(param == 0)
		{
			DailyQuestMenu(client);
		}
		if(param == 1)
		{
			WeelyQuestMenu(client);
		}
		if(param == 2)
		{
			MissionMenuBuild(client);
		}
		return 0;
	}
}
void DailyQuestMenu(int client)
{
	Menu menu = CreateMenu(DailyQuestMenuHandler);
	menu.SetTitle("每日任务\n%s", (g_pStore) ? "完成后请点击任务领取积分奖励" : "未检测到商店插件, 禁用完成功能");
	char buffer[256];
	char questid[64];
	QUEST quest;
	int iStyle = (g_pStore) ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED;
	for(int i=0;i<=5;i++)
	{
		IntToString(i,questid,sizeof(questid));
		GetArrayArray(QuestList,i,quest,sizeof(quest));
		if(playerquest_list[client].taskcomplete[i]==1)
		{
			Format(buffer,sizeof(buffer),"%s [已完成]",quest.name);
			menu.AddItem(questid,buffer,ITEMDRAW_DISABLED);
		}
		else
		{
			Format(buffer,sizeof(buffer),"%s:%d/%d",quest.name,playerquest_list[client].taskdata[i],quest.num);
			menu.AddItem(questid, buffer, iStyle);
		}
	}
	menu.Display(client, MENU_TIME_FOREVER);
	return;
}

int DailyQuestMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		char buffer[64];
		menu.GetItem(param,buffer,sizeof(buffer));
		int questid=StringToInt(buffer);
		int credits=Store_GetClientCredits(client);
		QUEST quest;
		GetArrayArray(QuestList,questid,quest,sizeof(quest));
		if(playerquest_list[client].taskcomplete[questid]==1)
		{
			PrintToChat(client," \x05[任务系统]\x01您已完成每日任务[%s]",quest.name);
		}
		else
		{
			if(playerquest_list[client].taskdata[questid]>=quest.num)
			{
				playerquest_list[client].taskcomplete[questid]=1;
				Store_SetClientCredits(client,credits+quest.award,"活动");
				PrintToChat(client," \x05[任务系统]\x01恭喜您完成每日任务[%s]，获得150积分奖励",quest.name);			
			}
			else
			{
				PrintToChat(client," \x05[任务系统]\x01您还未达成每日任务[%s]的条件",quest.name);
			}
		}
		UpdatePlayerQuestInfo(client);
		DailyQuestMenu(client);
		return 0;
	}
}

void WeelyQuestMenu(int client)
{
	return;
}
void QuestOnPlayerConnected(int client)
{
	LoadPlayerQuestInfo(client);
}
void QuestOnPlayerDisconnected(int client)
{
	if(playerquest_list[client].loaded)
	{
		UpdatePlayerQuestInfo(client);
	}
	playerquest_list[client].loaded = 0;
	playerquest_list[client].uid = 0;
}

void LoadPlayerQuestInfo(int client)
{
	char query[512];
	if(client<=0||client>=65)	return;
	if(!IsClientInGame(client)||!isDbConnected())
	{
		playerquest_list[client].loaded = 0;
		PrintToConsoleAll(" [任务系统]玩家%d的数据载入出错，等待重载",client);
		return;
	}
	USER_LOG user;
	EXGUSERS_GetUserInfo(client,user);
	if(user.loaded==0||user.uid==0)
	{
		playermission_list[client].loaded=0;
		PrintToChat(client,"\x05[任务系统]\x01加载玩家%d的[用户数据]时出错，用户系统未载入（请等待下一回合，\x07或输入!msr再试\x01)",client);
		return;
	}
	playerquest_list[client].uid = user.uid;
	Format(query,sizeof(query),"SELECT * FROM zequest WHERE UID = %d",playerquest_list[client].uid);
	DbTQuery(LoadPlayerQuestInfoCallBack,query,client);
}
void LoadPlayerQuestInfoCallBack(Handle owner, Handle hndl, char[] error, any data)
{

	char query[512];
	int client = data;
	if(!IsClientInGame(client))	return;
	int current_time = GetTime();
	if(!SQL_FetchRow(hndl))
	{
		Format(query,sizeof(query),"INSERT INTO zequest (UID,TIMESTAMP) VALUES(%d,%d)",playerquest_list[client].uid,current_time);
	}
	else
	{
		playerquest_list[client].timestamp = DbFetchInt(hndl,"TIMESTAMP");
		playerquest_list[client].taskdata[0]=DbFetchInt(hndl,"DINFECT");
		playerquest_list[client].taskdata[1]=DbFetchInt(hndl,"DKILL");
		playerquest_list[client].taskdata[2]=DbFetchInt(hndl,"DDMGTAKE");
		playerquest_list[client].taskdata[3]=DbFetchInt(hndl,"DDMGMAKE");
		playerquest_list[client].taskdata[4]=DbFetchInt(hndl,"DPASS");
		playerquest_list[client].taskdata[5]=DbFetchInt(hndl,"DNADE");
		playerquest_list[client].taskcomplete[0]=DbFetchInt(hndl,"DT1C");
		playerquest_list[client].taskcomplete[1]=DbFetchInt(hndl,"DT2C");
		playerquest_list[client].taskcomplete[2]=DbFetchInt(hndl,"DT3C");
		playerquest_list[client].taskcomplete[3]=DbFetchInt(hndl,"DT4C");
		playerquest_list[client].taskcomplete[4]=DbFetchInt(hndl,"DT5C");
		playerquest_list[client].taskcomplete[5]=DbFetchInt(hndl,"DT6C");
		char section[64];
		for(int i=0;i<=6;i++)
		{
			Format(section,sizeof(section),"LET%d",i+1);
			playerquest_list[client].letter[i]=DbFetchInt(hndl,section);
			Format(section,sizeof(section),"LETD%d",i+1);
			playerquest_list[client].letter_daily[i]=DbFetchInt(hndl,section);
		}
		playerquest_list[client].crate = DbFetchInt(hndl,"CRATE");
		playerquest_list[client].crate_total = DbFetchInt(hndl,"CRATET");
		playerquest_list[client].frag = DbFetchInt(hndl,"FRAG");
		playerquest_list[client].frag_daily = DbFetchInt(hndl,"FRAGD");
		Format(query,sizeof(query),"UPDATE zequest SET TIMESTAMP = %d WHERE UID = %d",current_time,playerquest_list[client].uid);
		if(playerquest_list[client].timestamp<g_daily_timestamp)
		{
			ClearPlayerDailyQuestInfo(client);
			playerquest_list[client].letter[0]+=1;
			playerquest_list[client].letter_daily[0]=1;
		}
	}
	playerquest_list[client].timestamp = current_time;
	PrintToServer(query);
	DbTQuery(DbQueryErrorCallback,query);
	playerquest_list[client].loaded = 1;
	delete hndl;
}

void ReloadAllPlayerQuestInfo()
{
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				PrintToServer("[调试]全体重载-玩家%d的日常任务数据",i);
				LoadPlayerQuestInfo(i);
			}
		}
	}
}
void ClearAllPlayerQuestInfo()
{
	char query[512];
	PrintToChatAll(" \x05[任务系统]\x01清理日常任务数据...");
	for(int i=1;i<=64;i++)
	{
		if(playerquest_list[i].loaded == 0 )	continue;
		playerquest_list[i].loaded = 0;
		for(int j=0;j<=5;j++)
		{
			playerquest_list[i].taskdata[j]=0;
			playerquest_list[i].taskcomplete[j]=0;
		}
	}
	Format(query,sizeof(query),"UPDATE zequest SET DINFECT = 0, DKILL = 0, DDMGTAKE = 0, DDMGMAKE = 0, DPASS = 0, DNADE = 0, DT1C = 0, DT2C = 0, DT3C = 0, DT4C = 0, DT5C = 0, DT6C = 0, LETD1 = 0, LETD2 = 0, LETD3 = 0, LETD4 =0, LETD5 = 0, LETD6 = 0, LETD7 = 0, FRAGD = 0");
	DbTQuery(DbClearPlayerQuestInfoCallback,query);
}

void DbClearPlayerQuestInfoCallback(Handle owner, Handle hndl, char[] error, any data)
{
	ReloadAllPlayerQuestInfo();
	delete hndl;
}

void UpdatePlayerQuestInfo(int client)
{
	if(client<=0||client>=65)	return;
	if(playerquest_list[client].loaded==0)
	{
		PrintToServer("[任务系统]重载因数据库出错而未载入的玩家%d的日常任务数据",client);
		LoadPlayerQuestInfo(client);
		return;
	}
	int taskdata[16];
	int taskcomplete[16];
	int letter[7];
	int letterd[7];
	int current_time = GetTime();
	char query[2048];
	for(int i=0;i<=5;i++)
	{
		taskdata[i]=playerquest_list[client].taskdata[i];
		taskcomplete[i]=playerquest_list[client].taskcomplete[i];
	}
	for(int i=0;i<=6;i++)
	{
		letter[i]=playerquest_list[client].letter[i];
		letterd[i]=playerquest_list[client].letter_daily[i];
	}
	Format(query,sizeof(query),"UPDATE zequest SET TIMESTAMP = %d, DINFECT = %d, DKILL = %d, DDMGTAKE = %d, DDMGMAKE = %d, DPASS = %d, DNADE = %d, DT1C = %d, DT2C = %d, DT3C = %d, DT4C = %d, DT5C = %d, DT6C = %d, LET1 = %d, LET2 = %d, LET3 = %d, LET4 = %d, LET5 = %d, LET6 = %d, LET7 = %d, LETD1 = %d, LETD2 = %d, LETD3 = %d, LETD4 = %d, LETD5 = %d, LETD6 = %d, LETD7 = %d, CRATE = %d, CRATET = %d, FRAG = %d, FRAGD = %d WHERE UID = %d",current_time,taskdata[0],taskdata[1],taskdata[2],taskdata[3],taskdata[4],taskdata[5],taskcomplete[0],taskcomplete[1],taskcomplete[2],taskcomplete[3],taskcomplete[4],taskcomplete[5],letter[0],letter[1],letter[2],letter[3],letter[4],letter[5],letter[6],letterd[0],letterd[1],letterd[2],letterd[3],letterd[4],letterd[5],letterd[6],playerquest_list[client].crate,playerquest_list[client].crate_total,playerquest_list[client].frag,playerquest_list[client].frag_daily,playerquest_list[client].uid);
	DbTQuery(DbQueryErrorCallback,query);
}

void ClearPlayerDailyQuestInfo(int client)
{
	if(client<=0||client>=65)	return;
	for(int i=0;i<=5;i++)
	{
		playerquest_list[client].taskdata[i] = 0;
		playerquest_list[client].taskcomplete[i] = 0;
	}
	for(int i=0;i<=6;i++)
	{
		playerquest_list[client].letter_daily[i]=0;
	}
	playerquest_list[client].frag_daily = 0;
	playerquest_list[client].loaded = 1;
	UpdatePlayerQuestInfo(client);
}
void QuestOnLevelSet(any level[sizeof(LEVEL_LOG)])
{

	Quest_Current_Level = level;
}
void QuestOnRoundStart()
{
	quest_roundstart_time = GetTime();
}
void QuestOnRoundEnd(int winner)
{
	int quest_round_time = GetTime()-quest_roundstart_time;
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				if(winner==3&&IsPlayerAlive(i)&&ZR_IsClientHuman(i)&&GetClientCount(true)>=quest_plimit&&quest_round_time>=120)
				{
					if(playerquest_list[i].taskdata[4]<=10000)
					{
						playerquest_list[i].taskdata[4]++;
					}
				}
				else
				{
					if(GetClientCount(true)<quest_plimit||quest_round_time<120)
					{
						PrintToChat(i," \x05[任务系统]\x01人数少于20或回合时间小于2分钟，不结算![%d][%d]",GetClientCount(true),quest_round_time);
					}
				}
				UpdatePlayerQuestInfo(i);
			}
		}
	}
}
void QuestSet()
{
	QUEST quest;
	QuestList = CreateArray(sizeof(QUEST));
	quest.id = 0;
	quest.type = ZM_INFECT;
	quest.num = 4;
	quest.award = 150;
	quest.period = 1;
	quest.name = "[僵尸]感染人类";
	QuestList.PushArray(quest);

	quest.id =1;
	quest.type = HM_KILLZM;
	quest.num = 2;
	quest.award = 150;
	quest.period = 1;
	quest.name = "[人类]击杀僵尸";
	QuestList.PushArray(quest);

	quest.id=2;
	quest.type = ZM_DMGTAKE;
	quest.num = 80000;
	quest.award= 150;
	quest.period = 1;
	quest.name = "[僵尸]承受伤害";
	QuestList.PushArray(quest);

	quest.id = 3;
	quest.type = HM_DMGMAKE;
	quest.num = 40000;
	quest.award = 150;
	quest.period = 1;
	quest.name = "[人类]造成伤害";
	QuestList.PushArray(quest);

	quest.id = 4;
	quest.type = HM_PASS;
	quest.num = 2;
	quest.award = 150;
	quest.period = 1;
	quest.name = "[人类]通关回合";
	QuestList.PushArray(quest);

	quest.id = 5;
	quest.type = HM_NADE;
	quest.num = 10;
	quest.award = 150;
	quest.period = 1;
	quest.name = "[人类]投掷手雷";
	QuestList.PushArray(quest);
/*
	quest.id = 6;
	quest.type = ZM_INFECT;
	quest.num = 30;
	quest.award = 300;
	quest.period = 7;
	QuestList.PushArray(quest);

	quest.id = 7;
	quest.type = HM_KILLZM;
	quest.num = 10;
	quest.award = 300;
	quest.period = 7;
	QuestList.PushArray(quest);

	quest.id = 8;
	quest.type = ZM_DMGTAKE;
	quest.num = 500000;
	quest.award = 300;
	quest.period = 7;
	QuestList.PushArray(quest);

	quest.id = 9;
	quest.type = HM_DMGMAKE;
	quest.num = 250000;
	quest.award = 300;
	quest.period = 7;
	QuestList.PushArray(quest);
	*/
}
void QuestZombieInfectHuman(int attacker)
{
	if(GetClientCount(true)<quest_plimit)	return;
	if(playerquest_list[attacker].taskdata[0]<=1000)
	{
		playerquest_list[attacker].taskdata[0]++;
	}
}
void QuestHumanKillZombie(int attacker)
{
	if(GetClientCount(true)<quest_plimit)	return;
	if(playerquest_list[attacker].taskdata[1]<=1000)
	{
		playerquest_list[attacker].taskdata[1]++;
	}
}
void QuestHumanDmgCount(int attacker,int victim,int dmg)
{
	if(GetClientCount(true)<quest_plimit)	return;
	if(playerquest_list[victim].taskdata[2]<1000000)
	{
		playerquest_list[victim].taskdata[2]+=dmg;
	}
	if(playerquest_list[attacker].taskdata[3]<1000000)
	{
		playerquest_list[attacker].taskdata[3]+=dmg;
	}
}

void QuestHumanNadeCount(int client)
{
	if(GetClientCount(true)<quest_plimit)	return;
	if(playerquest_list[client].taskdata[5]<=1000)
	{
		playerquest_list[client].taskdata[5]++;
	}
}

void FestivalMenuBuild(int client)
{
	Menu menu = CreateMenu(FestivalMenuHandler);
	menu.SetTitle("春节活动~恭贺新年");
	menu.AddItem("","活动仓库");
	menu.AddItem("","奖励兑换");
	menu.AddItem("","活动商店");
	menu.AddItem("","活动指南");
	menu.Display(client,MENU_TIME_FOREVER);
}

int FestivalMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		if(param==0)
		{
			FestivalInvMenuBuild(client);
		}
		if(param==1)
		{
			FestivalAwardMenuBuild(client);
		}
		if(param==2)
		{
			FestivalShopMenuBuild(client);
		}
		if(param==3)
		{
			FestivalInstructMenuBuild(client);
		}
		return 0;
	}
}

void FestivalInvMenuBuild(int client)
{
	char buffer[256];
	Menu menu = CreateMenu(FestivalInvMenuHandler);
	menu.SetTitle("活动仓库-括号内为今日掉落数");
	Format(buffer,sizeof(buffer),"[2]:%d张(%d)\n[0]:%d张(%d)\n[3]:%d张(%d)",playerquest_list[client].letter[0],playerquest_list[client].letter_daily[0],playerquest_list[client].letter[1],playerquest_list[client].letter_daily[1],playerquest_list[client].letter[2],playerquest_list[client].letter_daily[2]);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"[新]:%d张(%d)\n[春]:%d张(%d)\n[快]:%d张(%d)\n[乐]:%d张(%d)",playerquest_list[client].letter[3],playerquest_list[client].letter_daily[3],playerquest_list[client].letter[4],playerquest_list[client].letter_daily[4],playerquest_list[client].letter[5],playerquest_list[client].letter_daily[5],playerquest_list[client].letter[6],playerquest_list[client].letter_daily[6]);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"贺岁红包剩余数量:%d",playerquest_list[client].crate_total);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int FestivalInvMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		return 0;
	}
	else if (param == MenuCancel_ExitBack) FestivalMenuBuild(client);
}

void FestivalAwardMenuBuild(int client)
{
	Menu menu = CreateMenu(FestivalAwardMenuHandler);
	menu.SetTitle("奖励兑换");
	char buffer[256];
	Format(buffer,sizeof(buffer),"兑换一个新春箱子\n消耗一组[2023]");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"兑换[乐]\n消耗六组[2023]");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"兑换MVP音乐[好运来][180天]\n消耗一组[2023]");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"兑换人物模型-Padoru[永久]\n消耗一组[新春快乐]");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"打开新春箱子-剩余%d个",playerquest_list[client].crate);
	menu.AddItem("",buffer,playerquest_list[client].crate>=1?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"兑换[春]字-消耗3个庆典碎片\n剩余%d个",playerquest_list[client].frag);
	menu.AddItem("",buffer,playerquest_list[client].frag>=3?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int FestivalAwardMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		int item_id;
		int current_time = GetTime();
		if(param==0)
		{
			if(playerquest_list[client].letter[0]>=2&&playerquest_list[client].letter[1]>=1&&playerquest_list[client].letter[2]>=1&&playerquest_list[client].crate<=99)
			{
				playerquest_list[client].letter[0]-=2;
				playerquest_list[client].letter[1]-=1;
				playerquest_list[client].letter[2]-=1;
				playerquest_list[client].crate+=1;
				PrintToChat(client," \x05[活动系统]\x01你获得了新春箱子*1");
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你没有集齐对应的字或者你的箱子太多了!");
			}
		}
		if(param==1)
		{
			if(playerquest_list[client].letter[0]>=12&&playerquest_list[client].letter[1]>=6&&playerquest_list[client].letter[2]>=6)
			{
				playerquest_list[client].letter[0]-=12;
				playerquest_list[client].letter[1]-=6
				playerquest_list[client].letter[2]-=6;
				playerquest_list[client].letter[6]+=1;
				PrintToChat(client," \x05[活动系统]\x01你获得了[乐]*1");
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你没有集齐对应的字!");
			}
		}
		if(param==2)
		{
			if(!g_pStore)
			{
				PrintToChat(client," \x05[活动系统]\x01商店插件未载入!");
				return 0;
			}
			if(playerquest_list[client].letter[0]>=2&&playerquest_list[client].letter[1]>=1&&playerquest_list[client].letter[2]>=1)
			{
				item_id = Store_GetItemId("uid_mvp_sound_haoyunlai");
				if(Store_HasClientItem(client,item_id))
				{
					PrintToChat(client," \x05[活动系统]\x01你已经拥有该物品!");
				}
				else
				{
					playerquest_list[client].letter[0]-=2;
					playerquest_list[client].letter[1]-=1;
					playerquest_list[client].letter[2]-=1;
					Store_GiveItem(client,item_id,0,current_time+180*86400,0);
					PrintToChat(client," \x05[活动系统]\x01恭喜你获得道具\x07MVP音乐*好运来* 180天");
				}
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你没有集齐对应的字!");
			}
		}
		if(param==3)
		{
			if(!g_pStore)
			{
				PrintToChat(client," \x05[活动系统]\x01商店插件未载入!");
				return 0;
			}
			if(playerquest_list[client].letter[3]>=2&&playerquest_list[client].letter[4]>=1&&playerquest_list[client].letter[5]>=1&&playerquest_list[client].letter[6]>=1)
			{
				item_id = Store_GetItemId("uid_model_Padoru");
				if(Store_HasClientItem(client,item_id))
				{
					PrintToChat(client," \x05[活动系统]\x01你已经拥有该物品!");
				}
				else
				{
					playerquest_list[client].letter[3]-=1;
					playerquest_list[client].letter[4]-=1;
					playerquest_list[client].letter[5]-=1;
					playerquest_list[client].letter[6]-=1;
					char player_name[64];
					GetClientName(client,player_name,sizeof(player_name))
					Store_GiveItem(client,item_id,0,0,0);
					PrintToChatAll(" \x05[活动系统]\x01恭喜玩家%s获得道具\x07人物模型-Padoru[永久]",player_name);
				}
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你没有集齐对应的字!");
			}
		}
		if(param==4)
		{
			if(!g_pStore)
			{
				PrintToChat(client," \x05[活动系统]\x01商店插件未载入!");
				return 0;
			}
			if(playerquest_list[client].crate>=1)
			{
				OpenCrate(client);
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你还没有多余的新春箱子!");
			}
		}
		if(param==5)
		{
			if(playerquest_list[client].frag>=3)
			{
				playerquest_list[client].frag-=3;
				playerquest_list[client].letter[4]+=1;
				PrintToChat(client," \x05[活动系统]\x01使用3个庆典碎片兑换了一个\x07[春]\x01字");
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你还没有足够的庆典碎片");
			}
		}
		UpdatePlayerQuestInfo(client);
		return 0;
	}
	else if (param == MenuCancel_ExitBack) FestivalMenuBuild(client);
}

void FestivalShopMenuBuild(int client)
{
	if(!g_pStore)
	{
		PrintToChat(client," \x05[活动系统]\x01商店插件未载入!");
		return;
	}
	Menu menu = CreateMenu(FestivalShopMenuHandler);
	menu.SetTitle("活动商店");
	char buffer[256];
	int credits = Store_GetClientCredits(client);
	Format(buffer,sizeof(buffer),"购买一个新春箱子\n消耗2000积分");
	menu.AddItem("",buffer,credits>=2000?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"打开新春箱子-剩余%d个",playerquest_list[client].crate);
	menu.AddItem("",buffer,playerquest_list[client].crate>=1?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"兑换*M249-草神*[180天]\n消耗50个贺岁红包");
	menu.AddItem("",buffer,playerquest_list[client].crate_total>=50?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"兑换*[快]字*\n消耗100个贺岁红包");
	menu.AddItem("",buffer,playerquest_list[client].crate_total>=100?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int FestivalShopMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		if(param==0)
		{
			int credits = Store_GetClientCredits(client);
			if(credits>=2000&&playerquest_list[client].crate<=99)
			{
				Store_SetClientCredits(client,credits-2000,"活动");
				playerquest_list[client].crate++;
				PrintToChat(client," \x05[活动系统]\x01购买了一个新春箱子")
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你没有足够的积分或者你箱子太多了");
			}
		}
		if(param==1)
		{
			if(!g_pStore)
			{
				PrintToChat(client," \x05[活动系统]\x01商店插件未载入!");
				return 0;
			}
			if(playerquest_list[client].crate>=1)
			{
				OpenCrate(client);
			}
			else
			{
				PrintToChat(client," \x05[活动系统]\x01你还没有多余的新春箱子!");
			}
		}
		if(param==2)
		{
			if(!g_pStore)
			{
				PrintToChat(client," \x05[活动系统]\x01商店插件未载入!");
				return 0;
			}
			int item_id = Store_GetItemId("uid_wepskin_naxitan");
			if(Store_HasClientItem(client,item_id))
			{
				PrintToChat(client," \x05[活动系统]\x01您已拥有该道具，请勿重复兑换!");
				return 0;
			}
			else
			{
				playerquest_list[client].crate_total -= 50;
				Store_GiveItem(client,item_id,0,GetTime()+180*86400,0);
				PrintToChat(client," \x05[活动系统]\x01消耗50个\x07*贺岁红包*\x01兑换了\x07*M249-草神* 180天");
			}
		}
		if(param==3)
		{
			if(playerquest_list[client].letter[5]>=1)
			{
				PrintToChat(client," \x05[活动系统]\x01您已拥有该道具，请勿重复兑换!");
				return 0;
			}
			else
			{
				playerquest_list[client].letter[5]+=1;
				playerquest_list[client].crate_total -= 100;
				PrintToChat(client," \x05[活动系统]\x01消耗100个\x07*贺岁红包*\x01兑换了\x07[快]字");
			}
		}
		UpdatePlayerQuestInfo(client);
		return 0;
	}
	else if (param == MenuCancel_ExitBack) FestivalMenuBuild(client);
}

void FestivalInstructMenuBuild(int client)
{
	Menu menu = CreateMenu(FestivalInstructMenuHandler);
	menu.SetTitle("活动指南");
	menu.AddItem("","----获得字---",ITEMDRAW_DISABLED);
	menu.AddItem("","[2]:无法获得",ITEMDRAW_DISABLED);
	menu.AddItem("","[0]:无法获得",ITEMDRAW_DISABLED);
	menu.AddItem("","[3]:无法获得",ITEMDRAW_DISABLED);
	menu.AddItem("","[新]:无法获得",ITEMDRAW_DISABLED);
	menu.AddItem("","[春]:无法获得:",ITEMDRAW_DISABLED);
	menu.AddItem("","[快]:在节日箱子内抽取(概率2%)",ITEMDRAW_DISABLED);
	menu.AddItem("","[乐]:6组[2023]兑换",ITEMDRAW_DISABLED);
	menu.AddItem("","----新春箱子------",ITEMDRAW_DISABLED);
	menu.AddItem("","2%获得[快]字",ITEMDRAW_DISABLED);
	menu.AddItem("","5%获得草神M249180天",ITEMDRAW_DISABLED);
	menu.AddItem("","3%获得托马斯小火车坐骑30天",ITEMDRAW_DISABLED);
	menu.AddItem("","5%获得MVP音乐*恭喜发财*180天",ITEMDRAW_DISABLED);
	menu.AddItem("","必定掉落一个贺岁红包，其余普通奖励见论坛",ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int FestivalInstructMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
		return 0;
	}
	else if (param == MenuCancel_ExitBack) FestivalMenuBuild(client);
}