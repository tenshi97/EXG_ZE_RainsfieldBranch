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
	int fest[4];
	int fest_reward;
}
ArrayList QuestList;
PlayerQuestInfo playerquest_list[65];
void QuestOnPluginStart()
{
	RegConsoleCmd("sm_rw",QuestCommand);
	RegConsoleCmd("sm_quest",QuestCommand);
	RegAdminCmd("sm_raffleuser",PrintRaffleUser,ADMFLAG_GENERIC);
	ReloadAllPlayerQuestInfo();
	QuestSet();
}
Action PrintRaffleUser(int client,int args)
{
	char query[256];
	Format(query,sizeof(query),"SELECT * FROM zequest WHERE FEST1 = 1 AND FEST2 = 1 AND FEST3 = -1 AND FEST4 = -1");
	DbTQuery(RaffleUserQueryCallback,query);
}
void RaffleUserQueryCallback(Handle owner, Handle hndl, char[] error, any data)
{
	char raffle_path[256];
	BuildPath(Path_SM,raffle_path,sizeof(raffle_path),"configs/raffle.txt");
	File rafflefile = OpenFile(raffle_path,"w");
	if(!hndl)
	{
		return;
	}
	while(SQL_FetchRow(hndl))
	{
		int uid = DbFetchInt(hndl,"UID");
		PrintToChatAll("%d",uid);
		rafflefile.WriteLine("%d",uid);
	}
	delete rafflefile;
}

void QuestOnMapStart()
{
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
	menu.AddItem("","每日任务",ITEMDRAW_DISABLED);
	menu.AddItem("","每周任务",ITEMDRAW_DISABLED);
	menu.AddItem("","赛季任务");
	menu.AddItem("","季节活动~元旦");
	menu.Display(client, MENU_TIME_FOREVER);
	return Plugin_Handled;
}
int QuestMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End)
	{
		menu.Close();
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
		if(param == 3)
		{
			FestivalMenuBuild(client);
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
				PrintToChat(client," \x05[任务系统]\x01完成了每日任务[%s]，获得%d积分",quest.name,quest.award);
				playerquest_list[client].taskcomplete[questid]=1;
				Store_SetClientCredits(client,credits+quest.award);
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
		playerquest_list[client].fest[0]=DbFetchInt(hndl,"FEST1");
		playerquest_list[client].fest[1]=DbFetchInt(hndl,"FEST2");
		playerquest_list[client].fest[2]=DbFetchInt(hndl,"FEST3");
		playerquest_list[client].fest[3]=DbFetchInt(hndl,"FEST4");
		playerquest_list[client].fest_reward=DbFetchInt(hndl,"FESTRE");

		Format(query,sizeof(query),"UPDATE zequest SET TIMESTAMP = %d WHERE UID = %d",current_time,playerquest_list[client].uid);
		if(playerquest_list[client].timestamp<g_daily_timestamp)
		{
			ClearPlayerDailyQuestInfo(client);
		}
	}
	playerquest_list[client].timestamp = current_time;
	PrintToServer(query);
	DbTQuery(DbQueryErrorCallback,query);
	playerquest_list[client].loaded = 1;
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
	Format(query,sizeof(query),"UPDATE zequest SET DINFECT = 0, DKILL = 0, DDMGTAKE = 0, DDMGMAKE = 0, DPASS = 0, DNADE = 0, DT1C = 0, DT2C = 0, DT3C = 0, DT4C = 0, DT5C = 0, DT6C = 0");
	DbTQuery(DbClearPlayerQuestInfoCallback,query);
}

void DbClearPlayerQuestInfoCallback(Handle owner, Handle hndl, char[] error, any data)
{
	ReloadAllPlayerQuestInfo();
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
	int current_time = GetTime();
	char query[1024];
	for(int i=0;i<=5;i++)
	{
		taskdata[i]=playerquest_list[client].taskdata[i];
		taskcomplete[i]=playerquest_list[client].taskcomplete[i];
	}
	Format(query,sizeof(query),"UPDATE zequest SET TIMESTAMP = %d, DINFECT = %d, DKILL = %d, DDMGTAKE = %d, DDMGMAKE = %d, DPASS = %d, DNADE = %d, DT1C = %d, DT2C = %d, DT3C = %d, DT4C = %d, DT5C = %d, DT6C = %d, FEST1 = %d, FEST2 = %d, FEST3 = %d, FEST4 = %d, FESTRE = %d WHERE UID = %d",current_time,taskdata[0],taskdata[1],taskdata[2],taskdata[3],taskdata[4],taskdata[5],taskcomplete[0],taskcomplete[1],taskcomplete[2],taskcomplete[3],taskcomplete[4],taskcomplete[5],playerquest_list[client].fest[0],playerquest_list[client].fest[1],playerquest_list[client].fest[2],playerquest_list[client].fest[3],playerquest_list[client].fest_reward,playerquest_list[client].uid);
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
	playerquest_list[client].loaded = 1;
	UpdatePlayerQuestInfo(client);
}
void QuestOnRoundEnd(int winner)
{
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				if(winner==3&&IsPlayerAlive(i)&&ZR_IsClientHuman(i)&&GetClientCount(true)>=20)
				{
					if(playerquest_list[i].taskdata[4]<=10000)
					{
						playerquest_list[i].taskdata[4]++;
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
	quest.award = 50;
	quest.period = 1;
	quest.name = "[僵尸]感染人类";
	QuestList.PushArray(quest);

	quest.id =1;
	quest.type = HM_KILLZM;
	quest.num = 2;
	quest.award = 50;
	quest.period = 1;
	quest.name = "[人类]击杀僵尸";
	QuestList.PushArray(quest);

	quest.id=2;
	quest.type = ZM_DMGTAKE;
	quest.num = 80000;
	quest.award= 50;
	quest.period = 1;
	quest.name = "[僵尸]承受伤害";
	QuestList.PushArray(quest);

	quest.id = 3;
	quest.type = HM_DMGMAKE;
	quest.num = 40000;
	quest.award = 50;
	quest.period = 1;
	quest.name = "[人类]造成伤害";
	QuestList.PushArray(quest);

	quest.id = 4;
	quest.type = HM_PASS;
	quest.num = 2;
	quest.award = 50;
	quest.period = 1;
	quest.name = "[人类]通关回合";
	QuestList.PushArray(quest);

	quest.id = 5;
	quest.type = HM_NADE;
	quest.num = 10;
	quest.award = 50;
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
	if(GetClientCount(true)<20)	return;
	if(playerquest_list[attacker].taskdata[0]<=1000)
	{
		playerquest_list[attacker].taskdata[0]++;
	}
}
void QuestHumanKillZombie(int attacker)
{
	if(GetClientCount(true)<20)	return;
	if(playerquest_list[attacker].taskdata[1]<=1000)
	{
		playerquest_list[attacker].taskdata[1]++;
	}
}
void QuestHumanDmgCount(int attacker,int victim,int dmg)
{
	if(GetClientCount(true)<20)	return;
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
	if(GetClientCount(true)<0)	return;
	if(playerquest_list[client].taskdata[5]<=1000)
	{
		playerquest_list[client].taskdata[5]++;
	}
}

void FestivalMenuBuild(int client)
{
	Menu menu = CreateMenu(FestivalMenuHandler);
	menu.SetTitle("元旦活动~2023也请支持EXG");
	menu.AddItem("","节日打卡",ITEMDRAW_DISABLED);
	menu.AddItem("","节日任务");
	menu.AddItem("","庆典活动");
	bool quest_complete = false;
	if(playerquest_list[client].fest[0]==1&&playerquest_list[client].fest[1]==1&&playerquest_list[client].fest[2]==-1&&playerquest_list[client].fest[3]==-1)
	{
		quest_complete = true;
	}
	menu.AddItem("","领取任务奖励-10级经验",quest_complete?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
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
		if(param==1)
		{
			FestivalTaskMenuBuild(client);
		}
		if(param==2)
		{
			FestivalEventMenuBuild(client);
		}
		if(param==3)
		{
			if(playerquest_list[client].fest_reward == 1)
			{
				PrintToChat(client," \x05[活动系统]\x01你已经领取过奖励了，不要想着薅羊毛!");
			}
			else
			{
				GrantExp(client,10000);
				playerquest_list[client].fest_reward = 1;
				PrintToChat(client," \x05[活动系统]\x01领取奖励成功");
				UpdatePlayerQuestInfo(client);
			}
		}
		return 0;
	}
}

void FestivalTaskMenuBuild(int client)
{
	Menu menu = CreateMenu(FestivalTaskMenuHandler);
	menu.SetTitle("元旦活动:节日任务[任务3和4需要服务器内20人以上才可完成]");
	char buffer[256];
	Format(buffer,sizeof(buffer),"%s",playerquest_list[client].fest[0]?"[元]":"完成节日打卡");
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"%s",playerquest_list[client].fest[1]?"[旦]":"在2023年1月1日0点~24点间输入任意含有2023的语句\n获得M4A4-星际重炮7日");
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"%s",playerquest_list[client].fest[2]<0?"[快]":"在12月31日~1月3日期间累计对僵尸造成202300伤害\n获得2023积分");
	if(playerquest_list[client].fest[2]>=0)
	{
		Format(buffer,sizeof(buffer),"%s\n[当前:%d]",buffer,playerquest_list[client].fest[2]);
	}
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"%s",playerquest_list[client].fest[3]<0?"[乐]":"在12月31日~1月3日期间通任意地图5次\n获得M249-赤色彗星7日");
	if(playerquest_list[client].fest[3]>=0)
	{
		Format(buffer,sizeof(buffer),"%s\n[当前:%d]",buffer,playerquest_list[client].fest[3]);
	}
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int FestivalTaskMenuHandler(Menu menu, MenuAction action, int client, int param)
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

void FestivalEventMenuBuild(int client)
{
	Menu menu = CreateMenu(FestivalEventMenuHandler);
	menu.SetTitle("元旦活动:庆典活动");
	char buffer[256];
	Format(buffer,sizeof(buffer),"在节日任务内集齐[元][旦][快][乐]四字，自动参与抽奖");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"开奖时间:2023年1月4日0时\n开奖方式:游戏内公开抽奖[待定,可能会有其他形式]");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"奖品:参与者皆赠送10000点赛季活动经验\n 三等奖:随机高级皮肤 15天+2023积分 5人\n二等奖: 40000赛季经验 3人\n一等奖: 赛季等级满级直升 1人\n神秘奖: 1人(与前述奖品平行抽取)");
	menu.AddItem("",buffer);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int FestivalEventMenuHandler(Menu menu, MenuAction action, int client, int param)
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
