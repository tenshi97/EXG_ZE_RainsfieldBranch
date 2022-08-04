bool EXGUSERS_serverconnected = false;
ArrayList ServerList;
void ServerOnPluginStart()
{
	RegAdminCmd("sm_svreload",ReloadServerListCommand,ADMFLAG_GENERIC);
}
void ServerOnDbConnected()
{
	char query[256];
	Format(query,sizeof(query),"SELECT * FROM exgusers_servers");
	ServerList = CreateArray(sizeof(SERVER_LOG));
	DbTQuery(ServerQueryCallback,query);
	EXGUSERS_serverconnected = true;
}
Action ReloadServerListCommand(int client,int args)
{
	ReloadServerList();
}
void ReloadServerList()
{
	char query[256];
	if(!isDbConnected())
	{
		EXGUSERS_serverconnected = false;
		return;
	}
	CloseHandleSafe(ServerList);
	Format(query,sizeof(query),"SELECT * FROM exgusers_servers");
	ServerList = CreateArray(sizeof(SERVER_LOG));
	DbTQuery(ServerQueryCallback,query);
}
void ServerQueryCallback(Handle owner, Handle hndl, char[] error, any data)
{
	SERVER_LOG server_temp;
	while(SQL_FetchRow(hndl))
	{
		server_temp.sid = DbFetchInt(hndl,"SID");
		server_temp.port = DbFetchInt(hndl,"SVPORT");
		DbFetchString(hndl,"SVNAME",server_temp.name,sizeof(server_temp.name));
		server_temp.mode = DbFetchInt(hndl,"SVMODE");
		ServerList.PushArray(server_temp,sizeof(server_temp));
	}
}
public int Native_EXGUSERS_GetServerByPort(Handle plugin, int numParams)
{
	int port = GetNativeCell(1);
	bool port_found=false;
	SERVER_LOG server_ret;
	for(int i=0;i<ServerList.Length;i++)
	{
		GetArrayArray(ServerList,i,server_ret,sizeof(server_ret));
		if(server_ret.port==port)
		{
			port_found=true;
			break;
		}
	}
	if(!port_found)
	{
		server_ret.sid = 0;
		server_ret.port = 0;
		server_ret.name = "未知服务器";
		server_ret.mode = 0;
	}
	SetNativeArray(2,view_as<int>(server_ret),sizeof(SERVER_LOG));

}
void GetServerByPort(int port,SERVER_LOG server)
{
	SERVER_LOG server_ret;
	for(int i=0;i<ServerList.Length;i++)
	{
		GetArrayArray(ServerList,i,server_ret,sizeof(server_ret));
		if(server_ret.port==port)
		{
			server=server_ret;
			return;
		}
	}
	server.sid = 0;
	server.port = 0;
	server.name = "未知服务器";
	server.mode = 0;
	return;
}