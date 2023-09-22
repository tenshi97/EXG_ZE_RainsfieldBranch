void NomBanOnUserLoadCheck(int client)
{
	char query[512];
	int uid = g_Users[client].uid;
	Format(query, sizeof(query), "SELECT * FROM exgusers_nombanlog WHERE UID = %d AND EFFECT = 1", uid);
	if(!IsClientInGame(client)) return;
	if(client <=0 || client > 64) return;
	DbTQuery(NomBanLoadCallback, query, client);
}

void NomBanLoadCallback(Handle owner, Handle hndl, char[] error, any data)
{
	int client = data;
	int nban_expire;
	int bid;
	int nban;
	int current_time = GetTime();
	if(owner == INVALID_HANDLE || hndl == INVALID_HANDLE)
	{
		SetFailState("数据库错误: %s", error);
		return;
	}
	if (!SQL_GetRowCount(hndl)) {
		nban = 0;
		nban_expire = 0;
	}
	else
	{
		while (SQL_FetchRow(hndl)) {
			bid = DbFetchInt(hndl, "BID");
			nban = 1;
			nban_expire = DbFetchInt(hndl, "EXPIRE");
			if(nban_expire <= current_time)
			{
				nban = 0;
				nban_expire = 0;
				DeleteNomBanLog(bid);
			}
		}
	}
	SetNomBanState(client, nban, nban_expire);
	delete hndl;
}

void SetNomBanState(int client,int nban,int nban_expire)
{
	if(client <= 0 || client > 64) return;
	if(!IsClientInGame(client)) return;
	if(IsFakeClient(client)) return;

	g_Users[client].nomban = nban;
	if(nban == 1)
	{
		g_Users[client].nomban_expiretime = nban_expire;
	}
	else
	{
		g_Users[client].nomban = 0;
		g_Users[client].nomban_expiretime = 0;
	}
}

void AddNomBanLog(int client,int duration,char[] reason="",int adminclient)
{
	char query[512];
	int expire = GetTime()+duration*60;
	if(g_Users[client].loaded == 0)
	{
		PrintToChat(client," \x05[订图封禁] \x01目标信息未载入");
		return;
	}
	if(g_Users[client].nomban)
	{
		PrintToChat(client," \x05[订图封禁] \x01玩家正处于封禁订图权限中");
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
		PrintToChatAll(" \x05[订图封禁] \x01玩家\x07%s\x01被管理员\x07%s\x01永久封禁订图权限", pName, adminName);

	}
	else
	{
		PrintToChatAll(" \x05[订图封禁] \x01玩家\x07%s\x01被管理员\x07%s\x01封禁订图权限\x07%d分钟", pName, adminName, duration);
	}
	SetNomBanState(client, 1, expire);
	Format(query,sizeof(query), "INSERT INTO exgusers_nombanlog (UID,EXPIRE,REASON,ADMIN_UID,EFFECT) VALUES(%d,%d,'%s',%d,1)", uid, expire, reason, adminuid);
	DbTQuery(DbQueryErrorCallback, query);
}

void RemoveNomBanLog(int client,int adminclient)
{
	if(g_Users[client].loaded == 0)
	{
		PrintToChat(client," \x05[订图封禁] \x01目标信息未载入");
		return;
	}
	if(!g_Users[client].nomban)
	{
		PrintToChat(client," \x05[订图封禁] \x01玩家没有被封禁订图权限");
		return;
	}
	int uid = g_Users[client].uid;
	char pName[64];
	char adminName[64];
	GetClientName(client, pName, sizeof(pName));
	GetClientName(adminclient, adminName,s izeof(adminName));
	PrintToChatAll(" \x05[订图封禁] \x01玩家 \x07%s \x01被管理员 \x07%s \x01解除订图封禁", pName, adminName);
	g_Users[client].nomban = 0;
	g_Users[client].nomban_expiretime = 0;
	DeleteNomBanLogByUID(uid);
}

void DeleteNomBanLog(int bid)
{
	char query[512];
	Format(query, sizeof(query), "UPDATE exgusers_nombanlog SET EFFECT = 0 WHERE BID = %d", bid);
	DbTQuery(DbQueryErrorCallback, query);
}

void DeleteNomBanLogByUID(int uid)
{
	char query[512];
	Format(query, sizeof(query), "UPDATE exgusers_nombanlog SET EFFECT = 0 WHERE UID = %d", uid);
	DbTQuery(DbQueryErrorCallback, query);
}

void NomBanLogTimerCheck()
{
	char query[512];
	Format(query,sizeof(query), "SELECT * FROM exgusers_nombanlog WHERE EFFECT = 1");
	DbTQuery(NomBanLogTimerCheckCallback, query);
}

void NomBanLogTimerCheckCallback(Handle owner, Handle hndl, char[] error, any data)
{
	int current_time = GetTime();
	int bid, nban_expire, uid;
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
		nban_expire = DbFetchInt(hndl, "EXPIRE");
		uid = DbFetchInt(hndl, "UID");
		if(nban_expire <= current_time)
		{
			for(int i=1;i<=64;i++)
			{
				if(g_Users[i].uid == uid)
				{
					SetNomBanState(i, 0, 0);
					GetClientName(i, pName, sizeof(pName));
					PrintToChatAll(" \x05[订图封禁] \x01玩家 \x07%s \x01的订图封禁已过期", pName);
				}
				DeleteNomBanLog(bid);
			}
		}
	}
	delete hndl;
}