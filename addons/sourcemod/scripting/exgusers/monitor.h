void MonitorOnPluginStart()
{
	RegAdminCmd("sm_prtuser",PrintUserCommand,ADMFLAG_GENERIC);
}

Action PrintUserCommand(int client,int args)
{
	PrintUser(client);
}
void PrintUser(int client)
{
	int current_timestamp = GetTime();
	char path[PLATFORM_MAX_PATH];
	BuildPath(Path_SM,path,sizeof(path),"logs/onlineusers_%d.txt",current_timestamp);
	File useroutput = OpenFile(path,"w",false);
	char buffer[2048];
	char name[64];
	char auth_id[64];
	int client_team;
	char team_name[64];
	int server_port = FindConVar("hostport").IntValue;
	GetClientName(client,name,sizeof(name));
	GetClientAuthId(client,AuthId_Steam2,auth_id,sizeof(auth_id),true)
	Format(buffer,sizeof(buffer),"Record Time:%d\r\nRecorder:%s[%s]\r\nPort:%d\r\n",current_timestamp,name,auth_id,server_port);
	WriteFileString(useroutput,buffer,false);
	for(int i=1;i<=64;i++)
	{
		if(IsClientInGame(i))
		{
			if(!IsFakeClient(i))
			{
				client_team=GetClientTeam(i);
				if(client_team<=1)
				{
					team_name="观察";
				}
				else
				{
					if(client_team==2)
					{
						team_name="T";
					}
					else
					{
						if(client_team==3)
						{
							team_name="CT";
						}
					}
				}
				GetClientName(i,name,sizeof(name));
				GetClientAuthId(i,AuthId_Steam2,auth_id,sizeof(auth_id),true)
				Format(buffer,sizeof(buffer),"[%d-%s]%s[%s]\r\n",i,team_name,name,auth_id);
				WriteFileString(useroutput,buffer,false);
			}
		}
	}
	useroutput.Close();
}

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