bool EXGUSERS_serverconnected = false;
ArrayList ServerList;
static int server_ip;
static int server_port;
int server_chat_receive = 0;
void ServerOnPluginStart()
{
	server_ip = FindConVar("hostip").IntValue;
	server_port = FindConVar("hostport").IntValue;
	RegAdminCmd("sm_svreload",ReloadServerListCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_sa",ServerAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_serveradmin",ServerAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_jump",JumpCommand,ADMFLAG_GENERIC);
	ServerList = CreateArray(sizeof(SERVER_LOG));
}
void ServerOnDbConnected()
{
	delete ServerList;
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
Action ServerAdminCommand(int client, int args)
{
	if(!IsClientInGame(client))	return Plugin_Handled;
	if(client<=0||client>=65)	return Plugin_Handled;
	ServerAdminMenu(client);
}
void ServerAdminMenu(int client)
{
	Menu menu = CreateMenu(ServerAdminMenuHandler);
	char buffer[256];
	Format(buffer,sizeof(buffer),"服务器设置:%s",g_current_server.name);
	menu.SetTitle(buffer);
	Format(buffer,sizeof(buffer),"端口号:%d",g_current_server.port);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"[ZE]疲劳设置:%s",g_current_server.mode==0?(g_current_server.ze_fatigue?"开启":"关闭"):"不适用");
	menu.AddItem("",buffer,g_current_server.mode==0?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"跳转信息:[缩写]%s,[IP]%s:%d",g_current_server.shortname,g_current_server.ip,g_current_server.port);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"接受全服聊天:%s",server_chat_receive==0?"接受":"关闭");
	menu.AddItem("",buffer);
	menu.Display(client,MENU_TIME_FOREVER);
}
bool Is_Server_Chat_Received()
{
	if(server_chat_receive==0)	return true;
	return false;
}
int ServerAdminMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End)
	{
		menu.Close();
	}
	else if (action == MenuAction_Select)
	{
		if(param == 1)
		{
			if(g_current_server.ze_fatigue==0)	g_current_server.ze_fatigue =1;
			else g_current_server.ze_fatigue = 0;
			ServerModify(g_current_server);
			ServerAdminMenu(client);
		}
		if(param == 3)
		{
			if(server_chat_receive==0)	server_chat_receive=1;
			else	server_chat_receive = 0;
			ServerAdminMenu(client);
		}
		return 0;
	}
}
void ReloadServerList()
{
	char query[256];
	if(!isDbConnected())
	{
		EXGUSERS_serverconnected = false;
		return;
	}
	delete ServerList;
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
		DbFetchString(hndl,"SVIP",server_temp.ip,sizeof(server_temp.ip));
		DbFetchString(hndl,"SVSHORTNAME",server_temp.shortname,sizeof(server_temp.shortname));
		server_temp.mode = DbFetchInt(hndl,"SVMODE");
		server_temp.ze_fatigue = DbFetchInt(hndl,"SVZEFATI");
		ServerList.PushArray(server_temp,sizeof(server_temp));
	}
	LoadCurrentServer();
}
void LoadCurrentServer()
{
	int hostip_int = FindConVar("hostip").IntValue;
	int hostport = FindConVar("hostport").IntValue;
	char hostip[64];
	IPNumToIPV4(hostip_int,hostip,sizeof(hostip));
	SERVER_LOG server_temp;
	for(int i=0;i<ServerList.Length;i++)
	{
		GetArrayArray(ServerList,i,server_temp,sizeof(server_temp));
		if(server_temp.port==hostport&&strcmp(hostip,server_temp.ip,false)==0)
		{

			g_current_server = server_temp;
			break;
		}
	}
}
void GetServerBySID(int sid,SERVER_LOG server)
{
	SERVER_LOG server_temp;
	for(int i=0;i<ServerList.Length;i++)
	{
		GetArrayArray(ServerList,i,server_temp,sizeof(server_temp));
		if(server_temp.sid==sid)
		{
			server = server_temp;
			return;
		}
	}
	server.sid = 0;
	server.port = 0;
	server.name = "未知服务器";
	server.mode = 0;
	server.ze_fatigue = 0;
	return;
}

public int Native_EXGUSERS_GetServerByPort(Handle plugin, int numParams)
{
	int ip_int = GetNativeCell(1);
	int port = GetNativeCell(2);
	char ip[64];
	IPNumToIPV4(ip_int,ip,sizeof(ip));
	bool port_found=false;
	SERVER_LOG server_ret;
	for(int i=0;i<ServerList.Length;i++)
	{
		GetArrayArray(ServerList,i,server_ret,sizeof(server_ret));
		if(server_ret.port==port&&strcmp(ip,server_ret.ip,false)==0)
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
		server_ret.shortname = "unknown"
		server_ret.mode = 0;
		server_ret.ze_fatigue = 0;
	}
	SetNativeArray(3,view_as<int>(server_ret),sizeof(SERVER_LOG));

}

void ServerModify(SERVER_LOG server)
{
	SERVER_LOG server_temp;
	char query[512];
	for(int i=0;i<ServerList.Length;i++)
	{
		GetArrayArray(ServerList,i,server_temp,sizeof(server_temp));
		if(server_temp.sid==server.sid)
		{
			SetArrayArray(ServerList,i,server,sizeof(server));
		}
		Format(query,sizeof(query),"UPDATE exgusers_servers SET SVZEFATI = %d WHERE SID = %d",server.ze_fatigue,server.sid);
		DbTQuery(ServerQueryCallback,query);
	}
}
void GetServerByPort(int ip,int port,SERVER_LOG server)
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
	server.ze_fatigue = 0;
	return;
}

Action JumpCommand(int client,int args)
{
	char arg1[256];
	char arg2[256];
	int target_count;
	int target_list[65];
	char target_name[256];
	bool tn_is_ml;
	int jump_self;
	jump_self = 0;
	if(args<=1)
	{
		PrintToChat(client," \x05[EXG-服务器跳转]用法:\x01!jump 玩家 1F/2F/3F/4F/5F……");
		return Plugin_Handled;
	}
	else
	{
		GetCmdArg(1,arg1,sizeof(arg1));
		GetCmdArg(2,arg2,sizeof(arg2));
		target_count = ProcessTargetString(arg1,client,target_list,64,COMMAND_FILTER_CONNECTED,target_name,sizeof(target_name),tn_is_ml);
		if(target_count>0)
		{
			for(int i=0;i<target_count;i++)
			{
				if(target_list[i]==client)
				{
					jump_self = client;
				}
				else
				{
					JumpServer(target_list[i],arg2);
				}
				if(jump_self!=0)
				{
					JumpServer(jump_self,arg2);
				}

			}
		}
	}
	return Plugin_Handled;
}

void JumpServer(int client,char[] dest)
{
	// char buffer[256];
	if(!IsClientInGame(client))	return;
	if(IsFakeClient(client))	return;
	// GetClientName(client,buffer,sizeof(buffer));
	char ip_buffer[128];
	SERVER_LOG server_ret;
	for(int i=0;i<ServerList.Length;i++)
	{
		GetArrayArray(ServerList,i,server_ret,sizeof(server_ret));
		if(strcmp(dest,server_ret.shortname,false)==0)
		{
			Format(ip_buffer,sizeof(ip_buffer),"%s:%d",server_ret.ip,server_ret.port);
			RedirectClient(client,ip_buffer);
		}
	}
	/*
	if (strcmp(dest, "1f", false) == 0)
	{
		RedirectClient(client,"zegod.cn:27015");
	}
	else if(strcmp(dest, "2f", false) == 0)
	{
		RedirectClient(client,"zegod.cn:27025");
	}
	else if(strcmp(dest, "3f", false) == 0)
	{
		RedirectClient(client,"zegod.cn:27035");
	}
	else if(strcmp(dest, "4f", false) == 0)
	{
		RedirectClient(client,"zegod.cn:27045");
	}
	else if(strcmp(dest,"xs1f",false) == 0 )
	{
		RedirectClient(client,"xs.zegod.cn:25065");
	}
	else if(strcmp(dest,"xs2f",false) == 0 )
	{
		RedirectClient(client,"xs.zegod.cn:25075");
	}*/
}