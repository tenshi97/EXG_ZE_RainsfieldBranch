void PftBanOnUserLoadCheck(int client)
{
	char query[512];
	int uid = g_Users[client].uid;
	Format(query, sizeof(query), "SELECT * FROM exgusers_pftbanlog WHERE UID = %d AND EFFECT = 1", uid);
	if(!IsClientInGame(client)) return;
	if(client <= 0 || client > 64) return;
	DbTQuery(PftBanLoadCallback, query, client);
}

void PftBanLoadCallback(Handle owner, Handle hndl, char[] error, any data)
{
	int client = data;
	int pban_expire;
	int bid;
	int pban;
	int current_time = GetTime();
	if(owner == INVALID_HANDLE || hndl == INVALID_HANDLE)
	{
		SetFailState("数据库错误: %s", error);
		return;
	}
	if (!SQL_GetRowCount(hndl)) {
		pban = 0;
		pban_expire = 0;
	}
	else
	{
		while (SQL_FetchRow(hndl)) {
			bid = DbFetchInt(hndl, "BID");
			pban = 1;
			pban_expire = DbFetchInt(hndl, "EXPIRE");
			if(pban_expire <= current_time)
			{
				pban = 0;
				pban_expire = 0;
				DeletePftBanLog(bid);
			}
		}
	}
	SetPftBanState(client, pban, pban_expire);
	delete hndl;
}
void SetPftBanState(int client,int pban,int pban_expire)
{
	if(client <=0 || client > 64) return;
	if(!IsClientInGame(client)) return;
	if(IsFakeClient(client)) return;

	g_Users[client].pftban = pban;
	if(pban == 1)
	{
		g_Users[client].pftban_expiretime = pban_expire;
	}
	else
	{
		g_Users[client].pftban = 0;
		g_Users[client].pftban_expiretime = 0;
	}
}
void AddPftBanLog(int client,int duration,char[] reason="",int adminclient)
{
	char query[512];
	int expire = GetTime() + duration*60;
	if(g_Users[client].loaded == 0)
	{
		PrintToChat(client," \x05[收益封禁] \x01目标信息未载入");
		return;
	}
	if(g_Users[client].pftban)
	{
		PrintToChat(client," \x05[收益封禁] \x01玩家正处于封禁收益中");
		return;
	}
	int uid = g_Users[client].uid;
	int adminuid = g_Users[client].uid;
	char pName[64];
	char adminName[64];
	GetClientName(client, pName, sizeof(pName));
	GetClientName(adminclient, adminName, sizeof(adminName));
	if(duration == 0)
	{
		expire = TIME_PERMANENT+1;
		PrintToChatAll(" \x05[收益封禁] \x01玩家 \x07%s \x01被管理员 \x07%s \x01永久封禁收益",pName,adminName);

	}
	else
	{
		PrintToChatAll(" \x05[收益封禁] \x01玩家\x07%s\x01被管理员\x07%s\x01封禁收益\x07%d分钟",pName,adminName,duration);
	}
	SetPftBanState(client, 1, expire);
	Format(query, sizeof(query), "INSERT INTO exgusers_pftbanlog (UID,EXPIRE,REASON,ADMIN_UID,EFFECT) VALUES(%d,%d,'%s',%d,1)", uid, expire, reason, adminuid);
	DbTQuery(DbQueryErrorCallback, query);
}

void RemovePftBanLog(int client,int adminclient)
{
	if(g_Users[client].loaded == 0)
	{
		PrintToChat(client," \x05[收益封禁] \x01目标信息未载入");
		return;
	}
	if(!g_Users[client].pftban)
	{
		PrintToChat(client," \x05[收益封禁] \x01玩家没有被封禁收益");
		return;
	}
	int uid = g_Users[client].uid;
	char pName[64];
	char adminName[64];
	GetClientName(client, pName, sizeof(pName));
	GetClientName(adminclient, adminName, sizeof(adminName));
	PrintToChatAll(" \x05[收益封禁] \x01玩家\x07%s\x01被管理员\x07%s\x01解除收益封禁", pName, adminName);
	g_Users[client].pftban = 0;
	g_Users[client].pftban_expiretime = 0;
	DeletePftBanLogByUID(uid);
}

void DeletePftBanLog(int bid)
{
	char query[512];
	Format(query, sizeof(query), "UPDATE exgusers_pftbanlog SET EFFECT = 0 WHERE BID = %d",bid);
	DbTQuery(DbQueryErrorCallback, query);
}
void DeletePftBanLogByUID(int uid)
{
	char query[512];
	Format(query, sizeof(query), "UPDATE exgusers_pftbanlog SET EFFECT = 0 WHERE UID = %d",uid);
	DbTQuery(DbQueryErrorCallback, query);
}
void PftBanLogTimerCheck()
{
	char query[512];
	Format(query, sizeof(query), "SELECT * FROM exgusers_pftbanlog WHERE EFFECT = 1");
	DbTQuery(PftBanLogTimerCheckCallback, query);
}
void PftBanLogTimerCheckCallback(Handle owner, Handle hndl, char[] error, any data)
{
	int current_time = GetTime();
	int bid,pban_expire,uid;
	char pName[64];
	if(owner == INVALID_HANDLE || hndl == INVALID_HANDLE)
	{
		SetFailState("数据库错误: %s", error);
		return;
	}
	if(!SQL_GetRowCount(hndl))
	{
		return;
	}
	while (SQL_FetchRow(hndl))
	{
		bid = DbFetchInt(hndl, "BID");
		pban_expire = DbFetchInt(hndl, "EXPIRE");
		uid = DbFetchInt(hndl, "UID");
		if(pban_expire <= current_time)
		{
			for(int i = 1; i <= 64; i++)
			{
				if(g_Users[i].uid == uid)
				{
					SetPftBanState(i, 0, 0);
					GetClientName(i, pName, sizeof(pName));
					PrintToChatAll(" \x05[收益封禁] \x01玩家\x07%s\x01的订图收益封禁已过期", pName);
				}
				DeletePftBanLog(bid);
			}
		}
	}
	delete hndl;
}

public int Native_EXGUSERS_GetUserPbanState(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	char buffer[256];
	if(g_Users[client].loaded == 0 || g_Users[client].pftban == 0)
	{
		return 0;
	}
	return 1;
}