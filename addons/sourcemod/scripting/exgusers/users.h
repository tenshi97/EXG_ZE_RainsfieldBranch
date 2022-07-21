enum struct user_log
{
	int uid;
	char steamauth[64];
	char name[64];
	int mute;
	int eban;
	int nomban;
	int zhban;
	int cban;
	int mute_expiretime;
	int eban_expiretime;
	int nomban_expiretime;
	int zhban_expiretime;
	int cban_expiretime;
	int credits;
	int credits_gain;
	int stitle;
	int title_group;
	int cpuid;
	int loaded;//temp
	int last_login;
	int regtime;
}
user_log g_Users[65];
char player_title_name[6][64]={"狗管理","服主","MAPPER","MODDER","大爷","糕手"};

void UsersAdmOnPluginStart()
{
//	RegAdminCmd("sm_uadmin",UserAdminMenu,ADMFLAG_GENERIC);
//	RegAdminCmd("sm_ua",UserAdminMenu,ADMFLAG_GENERIC);
	RegConsoleCmd("sm_user",UserInfoMenuCommand);
	RegConsoleCmd("sm_i",UserInfoMenuCommand);

}
void UsersOnClientInServer(int client)
{
	LoadUserInfo(client);
}
void UserInfoOnRoundStart(int client)
{
	for(int i=1;i<=64;i++)
	{
		if(g_Users[i].loaded==0)
		{
			LoadUserInfo(i);
		}
	}
}
void LoadUserInfo(int client)
{
	char auth_id[64];
	char client_name[64];
	GetClientName(client,client_name,sizeof(client_name));
	if(!isDbConnected())
	{
		PrintToServer("玩家%s[%d]信息载入失败(数据库未连接),等待下回合重载",client_name,client);
		g_Users[client].loaded=0;
		return;
	}
	if(!IsClientInGame(client))
	{
		PrintToServer("玩家%s[%d]信息载入失败(数据库未连接),等待下回合重载",client_name,client);
		g_Users[client].loaded=0;
		return;
	}
	char query[512];
	if(!GetClientAuthId(client,AuthId_Steam2,auth_id,sizeof(auth_id),true))
	{
		PrintToServer("玩家%s[%d]信息载入失败(数据库未连接),等待下回合重载",client_name,client);
		g_Users[client].loaded=0;
		return;
	}
	Format(query,sizeof(query),"SELECT * FROM exgusers WHERE AUTHID = '%s'",auth_id);
	PrintToServer(query);
	DataPack dp = new DataPack();
	dp.WriteCell(client);
	dp.WriteString(auth_id);
	dp.WriteString(client_name);
	DbTQuery(LoadUserInfoCallBack,query,dp);
}

int LoadUserInfoCallBack(Handle owner, Handle hndl, char[] error, DataPack dp)
{
	dp.Reset();
	int client = dp.ReadCell();
	int new_client=0;
	char auth_id[64];
	char client_name[64];
	char query[512];
	dp.ReadString(auth_id,sizeof(auth_id));
	dp.ReadString(client_name,sizeof(client_name));
	delete dp;
	int current_time = GetTime();
	PrintToConsoleAll("[调试]载入反馈 %d %s %s",client,auth_id,client_name);
	if(!SQL_FetchRow(hndl))
	{
		PrintToConsoleAll("[调试]未检测到玩家%s[%d][%s]用户数据，注册新玩家",client_name,client,auth_id);
		Format(query,sizeof(query),"INSERT INTO exgusers (NAME,AUTHID,LAST_LOGIN,REGTIME) VALUES('%s','%s',%d,%d)",client_name,auth_id,current_time,current_time);
		PrintToServer(query);
		new_client=client;
		g_Users[client].last_login = current_time;
		g_Users[client].regtime = current_time;
	}
	else
	{
		new_client=0;
		g_Users[client].uid = DbFetchInt(hndl,"UID");
		g_Users[client].regtime = DbFetchInt(hndl,"REGTIME");
		g_Users[client].last_login = current_time;
		strcopy(g_Users[client].name,sizeof(client_name),client_name);
		Format(query,sizeof(query),"UPDATE exgusers SET NAME = '%s', LAST_LOGIN = %d WHERE AUTHID = '%s'",client_name,current_time,auth_id);
		PrintToServer(query);
		g_Users[client].loaded=1;
	}
	DbTQuery(LoadUpdateUserInfoCallback,query,new_client);

}
void LoadUpdateUserInfoCallback(Handle owner, Handle hndl, char[] error, any data)
{
	int client = data;
	if(client>0&&client<=64)
	{
		char auth_id[64];
		char query[512];
		GetClientAuthId(client,AuthId_Steam2,auth_id,sizeof(auth_id),true);
		Format(query,sizeof(query),"SELECT * FROM exgusers WHERE AUTHID = '%s'",auth_id);
		PrintToConsoleAll(query);
		DbTQuery(ReloadNewUserInfoCallback,query,client);
	}
}
void ReloadNewUserInfoCallback(Handle owner, Handle hndl, char[] error, any data)
{
	int client=data;
	char client_name[64];
	GetClientName(client,client_name,sizeof(client_name));
	if(SQL_FetchRow(hndl))
	{
		g_Users[client].uid = DbFetchInt(hndl,"UID");
		g_Users[client].loaded=1;
	}
	else
	{
		g_Users[client].loaded = 0;
	}
}
/*void UserAdminMenu(int client)
{
	Menu menu = CreateMenu(UserAdminMenuHandler);
}*/
Action UserInfoMenuCommand(int client,int args)
{
	UserInfoMenu(client);
	return Plugin_Handled;
}
void UserInfoMenu(int client)
{
	Menu menu = CreateMenu(UserInfoMenuHandler);
	int legacy_id = GetSteamAccountID(client,true);
	char buffer[512];
	menu.SetTitle("用户信息\nUID:%d",g_Users[client].uid);
	Format(buffer,sizeof(buffer),"旧UID:%d",legacy_id);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	menu.Display(client,MENU_TIME_FOREVER);
}
int UserInfoMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		menu.Close();
	}
}