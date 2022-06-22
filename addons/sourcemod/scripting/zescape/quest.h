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
	char name[64];
	char authid[64];
	int taskdata[16];
	int taskcomplete[16];
	int loaded;
	int timestamp;
}
ArrayList QuestList;
PlayerQuestInfo playerquest_list[65];
void QuestOnPluginStart()
{
	RegConsoleCmd("sm_rw",QuestCommand);
	RegConsoleCmd("sm_quest",QuestCommand);
	ReloadAllPlayerQuestInfo();
	QuestSet();
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
	menu.AddItem("","每日任务");
	menu.AddItem("","每周任务",ITEMDRAW_DISABLED);
	menu.AddItem("","赛季任务",ITEMDRAW_DISABLED);
	menu.Display(client, MENU_TIME_FOREVER);
	return Plugin_Handled;
}
int QuestMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End)
	{
		menu.Close();
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
	}
}
void DailyQuestMenu(int client)
{
	Menu menu = CreateMenu(DailyQuestMenuHandler);
	menu.SetTitle("每日任务\n%s", (g_pStore) ? "完成后请点击任务领取积分奖励" : "未检测到商店插件, 禁用完成功能";
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
	if(client<=0||client>=65)	return;
	if (action == MenuAction_End)
	{
		menu.Close();
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
}

void LoadPlayerQuestInfo(int client)
{
	char client_authid[64];
	char client_name[64];
	char query[512];
	if(client<=0||client>=65)	return;
	if(!IsClientInGame(client)||!isDbConnected())
	{
		playerquest_list[client].loaded = 0;
		PrintToConsoleAll(" [任务系统]玩家%d的数据载入出错，等待重载",client);
		return;
	}
	GetClientName(client,client_name,sizeof(client_name));
	GetClientAuthId(client,AuthId_Steam3,client_authid,sizeof(client_authid),true);
	Format(query,sizeof(query),"SELECT * FROM zequest WHERE AUTHID = '%s'",client_authid);
	DbTQuery(LoadPlayerQuestInfoCallBack,query,client);
}
void LoadPlayerQuestInfoCallBack(Handle owner, Handle hndl, char[] error, any data)
{	
	char query[512];
	int client = data;
	char client_authid[64];
	char client_name[64];
	if(client<=0||client>=65)	return;
	GetClientName(client,client_name,sizeof(client_name));
	GetClientAuthId(client,AuthId_Steam3,client_authid,sizeof(client_authid),true);
	int current_time = GetTime();
	playerquest_list[client].authid = client_authid;
	if(!SQL_FetchRow(hndl))
	{
		PrintToConsoleAll(" [任务系统]未检测到玩家%s[client:%d]数据，注册新玩家，当前活动时间戳:%d",client_name,client,g_daily_timestamp);
		Format(query,sizeof(query),"INSERT INTO zequest (AUTHID,NAME,TIMESTAMP) VALUES('%s','%s',%d)",client_authid,client_name,current_time);
	}
	else
	{
		playerquest_list[client].timestamp = DbFetchInt(hndl,"TIMESTAMP");
		PrintToConsoleAll(" [任务系统]载入玩家%s[client:%d]数据,当前活动时间戳:%d，玩家上次登录时间戳:%d",client_name,client,g_daily_timestamp,playerquest_list[client].timestamp);
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
		Format(query,sizeof(query),"UPDATE zequest SET NAME = '%s', TIMESTAMP = %d WHERE AUTHID = '%s'",client_name,current_time,client_authid);	
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
	char client_name[64];
	char client_authid[64];
	char query[1024];
	GetClientName(client,client_name,sizeof(client_name));
	GetClientAuthId(client,AuthId_Steam3,client_authid,sizeof(client_authid),true);	
	for(int i=0;i<=5;i++)
	{
		taskdata[i]=playerquest_list[client].taskdata[i];
		taskcomplete[i]=playerquest_list[client].taskcomplete[i];
	}
	Format(query,sizeof(query),"UPDATE zequest SET NAME = '%s', TIMESTAMP = %d, DINFECT = %d, DKILL = %d, DDMGTAKE = %d, DDMGMAKE = %d, DPASS = %d, DNADE = %d, DT1C = %d, DT2C = %d, DT3C = %d, DT4C = %d, DT5C = %d, DT6C = %d WHERE AUTHID = '%s'",client_name,current_time,taskdata[0],taskdata[1],taskdata[2],taskdata[3],taskdata[4],taskdata[5],taskcomplete[0],taskcomplete[1],taskcomplete[2],taskcomplete[3],taskcomplete[4],taskcomplete[5],client_authid);
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
				if(winner==3&&IsPlayerAlive(i)&&ZR_IsClientHuman(i)&&GetClientCount(true)>=0)
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
	if(GetClientCount(true)<0)	return;
	if(playerquest_list[attacker].taskdata[0]<=1000)
	{
		playerquest_list[attacker].taskdata[0]++;
	}
}
void QuestHumanKillZombie(int attacker)
{
	if(GetClientCount(true)<0)	return;
	if(playerquest_list[attacker].taskdata[1]<=1000)
	{
		playerquest_list[attacker].taskdata[1]++;
	}
}
void QuestHumanDmgCount(int attacker,int victim,int dmg)
{
	if(GetClientCount(true)<0)	return;
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