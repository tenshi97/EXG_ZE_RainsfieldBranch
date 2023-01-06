
void MonitorOnClientConnect(int client)
{
	char auth_id[64];
	int current_timestamp = GetTime();
	char query[512];
	GetClientAuthId(client,AuthId_Steam2,auth_id,sizeof(auth_id),true)
	Format(query,sizeof(query),"INSERT INTO exgusers_connectlog (UID,AUTHID,NAME,TIMESTAMP,TYPE) VALUES(%d,'%s','%s',%d,'%s')",g_Users[client].uid,auth_id,g_Users[client].name,current_timestamp,"Connect");
	DbTQuery(DbQueryErrorCallback,query);
}

void MonitorOnClientDisconnect(int client)
{
	int current_timestamp = GetTime();
	char query[512];
	Format(query,sizeof(query),"INSERT INTO exgusers_connectlog (UID,NAME,TIMESTAMP,TYPE) VALUES(%d,'%s',%d,'%s')",g_Users[client].uid,g_Users[client].name,current_timestamp,"Disconnect");
	DbTQuery(DbQueryErrorCallback,query);
}