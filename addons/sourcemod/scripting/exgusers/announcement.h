Handle g_Timer_SuperChat_Check;
void AnnouncementOnPluginStart()
{
	RegConsoleCmd("sm_sch",SuperChatCommand);
	g_Timer_SuperChat_Check = CreateTimer(10.0,g_Timer_SuperChat_Check_Hndl,_, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}
void AnnouncementOnMapStart()
{
	g_Timer_SuperChat_Check = INVALID_HANDLE;
	KillTimerSafe(g_Timer_SuperChat_Check);
	g_Timer_SuperChat_Check = CreateTimer(10.0,g_Timer_SuperChat_Check_Hndl,_, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);

}
Action g_Timer_SuperChat_Check_Hndl(Handle timer)
{
	char query[512];
	int current_time = GetTime();
	current_time-=60;
	if(!isDbConnected())	return Plugin_Handled;
	Format(query,sizeof(query),"SELECT * FROM announcement WHERE TIMESTAMP >= %d",current_time);
	DbTQuery(SuperChatCheckQueryCallBack,query);
	return Plugin_Handled;
}
int SuperChatCheckQueryCallBack(Handle owner, Handle hndl, char[] error, any data)
{
	if(!hndl)
	{
		return;
	}
	if (!SQL_GetRowCount(hndl)) {
		return;
	}
	int broadcastag;
	int server_port = FindConVar("hostport").IntValue;
	SERVER_LOG server_log,sender_server;
 	GetServerByPort(server_port,server_log);
 	char buffer[256];
 	int server_tag = 1<<(server_log.sid);
	while(SQL_FetchRow(hndl))
	{
		broadcastag = DbFetchInt(hndl,"BROADCASTAG");
		if(broadcastag&server_tag==0)
		{
			broadcastag = broadcastag|server_tag;
			char message[512];
			char sender_name[64];
			char query[512];
			int sender_server_port;
			int mid;
			mid = DbFetchInt(hndl,"MID");
			DbFetchString(hndl,"NAME",sender_name,sizeof(sender_name));
			DbFetchString(hndl,"MESSAGE",message,sizeof(message));
			sender_server_port = DbFetchInt(hndl,"SERVERPORT");
			GetServerByPort(sender_server_port,sender_server);
			Format(buffer,sizeof(buffer)," \x05[全服聊天][%s]\x07%s:\x04%s",sender_server.name,sender_name,message);
			PrintToChatAll(buffer);
			Format(query,sizeof(query),"UPDATE announcement SET BROADCASTAG = %d WHERE MID = %d",broadcastag,mid);
			DbTQuery(DbQueryErrorCallback,query);
		}
	}
}
Action SuperChatCommand(int client,int args)
{
	char message[512];
	if (args == 1)
	{
		GetCmdArgString(message,sizeof(message));
		ReplaceString(message, sizeof(message), "\"", "");
		SuperChat(client,message);
	}
	else
	{
		PrintToChat(client," \x05[公告系统]\x01全服聊天格式不正确，用法:/sch \"你想说的话\",或用\\你想说的话");
	}
}
void SuperChat(int client,const char[] message)
{
	char client_name[64];
	int current_time = GetTime();
	char query[512];
	int server_port = FindConVar("hostport").IntValue;
	if(!g_pStore)
	{
		GetClientName(client,client_name,sizeof(client_name));
		CheckSQLInjectString(client_name,sizeof(client_name));
		Format(query,sizeof(query),"INSERT INTO announcement (NAME,SERVERPORT,MESSAGE,TIMESTAMP) VALUES('%s',%d,'%s',%d)",client_name,server_port,message,current_time);
		DbTQuery(DbQueryErrorCallback,query);
		return;
	}
	if(!IsClientVIP(client))
	{
		int credits = Store_GetClientCredits(client);
		if(credits<10)
		{
			PrintToChat(client," \x05[公告系统]\x01积分不足,无法使用全服聊天(不会有人没有10积分吧，不会吧)");
			return;
		}
		Store_SetClientCredits(client,credits-10);

	}
	GetClientName(client,client_name,sizeof(client_name));
	CheckSQLInjectString(client_name,sizeof(client_name));
	Format(query,sizeof(query),"INSERT INTO announcement (NAME,SERVERPORT,MESSAGE,TIMESTAMP) VALUES('%s',%d,'%s',%d)",client_name,server_port,message,current_time);
	DbTQuery(DbQueryErrorCallback,query);
}