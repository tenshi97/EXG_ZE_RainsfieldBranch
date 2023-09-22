#pragma semicolon 1
#pragma newdecls required
#pragma dynamic 131072 

//////////////////////////////
//    PLUGIN DEFINITION     //
//////////////////////////////
#define PLUGIN_NAME         "EXGBHOP_CHECK"
#define PLUGIN_AUTHOR       "Rainsfield&WackoD&EXGNullable"
#define PLUGIN_DESCRIPTION  "For Server MACRO CHECK USE"
#define PLUGIN_VERSION      "1.0"
#define PLUGIN_URL          "https://zegod.cn"

public Plugin myinfo =
{
	name 		= PLUGIN_NAME,
	author 		= PLUGIN_AUTHOR,
	description = PLUGIN_DESCRIPTION,
	version 	= PLUGIN_VERSION,
	url 		= PLUGIN_URL
};

//////////////////////////////
//          INCLUDES        //
//////////////////////////////
#include <sourcemod>
#include <sdktools>
#include <rainsfield>

//////////////////////////////
//          DEFINE          //
//////////////////////////////
float afPlayerAverageSpeed[65];
float afPlayerAverageJumps[65];
int aiPlayerJumps[65];
int aiPlayerLastJumps[65][30];
int aiPlayerLastPos[65];
float afPlayerAveragePerfJumps[65];
bool bHoldingJump[65];
bool bLastOnGround[65];
bool SuspectTag[65];

void ArrayReset()
{
	for(int i=1;i<=64;i++)
	{
		afPlayerAverageSpeed[i]=250.0;
		afPlayerAverageJumps[i]=5.0;
		aiPlayerJumps[i]=0;
		for(int j=0;j<=29;j++)
		{
			aiPlayerLastJumps[i][j]=0;
		}
		aiPlayerLastPos[i]=0;
		afPlayerAveragePerfJumps[i]=0.3333;
		bHoldingJump[i]=false;
		bLastOnGround[i]=false;
		SuspectTag[i]=false;
	}
}

public void OnClientConnected(int client)
{
	afPlayerAverageSpeed[client]=250.0;
	afPlayerAverageJumps[client]=5.0;
	aiPlayerJumps[client]=0;
	for(int i=0;i<=29;i++)
	{
		aiPlayerLastJumps[client][i]=0;
	}
	aiPlayerLastPos[client]=0;
	afPlayerAveragePerfJumps[client]=0.3333;
	bHoldingJump[client]=false;
	bLastOnGround[client]=false;
	SuspectTag[client]=false;
}

public void OnClientDisconnected(int client)
{
	afPlayerAverageSpeed[client]=250.0;
	afPlayerAverageJumps[client]=5.0;
	aiPlayerJumps[client]=0;
	for(int i=0;i<=29;i++)
	{
		aiPlayerLastJumps[client][i]=0;
	}
	aiPlayerLastPos[client]=0;
	afPlayerAveragePerfJumps[client]=0.3333;
	bHoldingJump[client]=false;
	bLastOnGround[client]=false;
	SuspectTag[client]=false;
}

public void OnPluginStart()
{
	HookEvent("player_jump",OnPlayerJump,EventHookMode_Post);
	ArrayReset();

	RegAdminCmd("sm_bhc",	Command_BhopCheck,				ADMFLAG_GENERIC);
	RegAdminCmd("sm_bhcl",	Command_BhopCheck_SuspectList,	ADMFLAG_GENERIC);
}

public void OnMapStart()
{
	ArrayReset();
}

public void OnPlayerJump(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event,"userid"));
	float player_velocity[3];
	GetEntPropVector(client,Prop_Data,"m_vecVelocity",player_velocity);
	player_velocity[2]=0.0;			//Let the Velocity Z Component be zero
	float player_speed = GetVectorLength(player_velocity);
	afPlayerAverageJumps[client] = (afPlayerAverageJumps[client]*9.0 + float(aiPlayerJumps[client]))/10.0
	afPlayerAverageSpeed[client]=(player_speed+9.0*afPlayerAverageSpeed[client])/(10.0);
	aiPlayerLastJumps[client][aiPlayerLastPos[client]] = aiPlayerJumps[client];
	aiPlayerLastPos[client]++;
	if(aiPlayerLastPos[client]>=30)
	{
		aiPlayerLastPos[client]=0;
	}
	aiPlayerJumps[client]=0;
	PatternIdentification(client);


}

public Action OnPlayerRunCmd(int client, int &buttons, int &impulse, float vel[3], float angles[3], int &weapon)
{
	if(!IsClientInGame(client))	return Plugin_Continue;
	if(IsFakeClient(client))	return Plugin_Continue;
	if(IsPlayerAlive(client))
	{
		if(buttons & IN_JUMP)	//敲击+jump
		{
			if(!bHoldingJump[client])	//按住+jump 上锁
			{
				bHoldingJump[client] = true;
				aiPlayerJumps[client]++;
				if(bLastOnGround[client] && (GetEntityFlags(client) & FL_ONGROUND))
				{
					afPlayerAveragePerfJumps[client] = (afPlayerAveragePerfJumps[client] *9.0 + 0)/10.0;
				}
				else if(!bLastOnGround[client] && (GetEntityFlags(client)& FL_ONGROUND))
				{
					afPlayerAveragePerfJumps[client] = (afPlayerAveragePerfJumps[client] *9.0 + 1)/10.0;
				}
			}

		}
		else if(bHoldingJump[client])	//-jump 解锁
		{
			bHoldingJump[client] = false;
		}
		bLastOnGround[client] = GetEntityFlags(client) & FL_ONGROUND;
	}
	return Plugin_Continue;
}

Action Command_BhopCheck_SuspectList(int client,int args)
{
	char name[64];
	for(int i = 1; i <= 64; i++)
	{
		if(SuspectTag[i])
		{
			GetClientName(i,name,sizeof(name));
			PrintToConsole(client,"[嫌疑] %s",name);
		}
	}
	return Plugin_Handled;
}

Action Command_BhopCheck(int client,int args)
{
	if(args<1||args>=2)
	{
		PrintToChat(client," \x05[连跳检测] \x01正确用法:!bhc <#userid> / <@all @ct @t>");
		return Plugin_Handled;
	}
	else
	{
		char stime[256];
		FormatTime(stime,sizeof(stime),NULL_STRING,GetTime());
		char buffer[256];
		char map_name[64];
		GetCurrentMap(map_name,sizeof(map_name));
		GetCmdArg(1,buffer,sizeof(buffer));
		PrintToConsole(client," -------连跳检测 地图:%s 时间:%s-------------",map_name,stime);
		if(strcmp(buffer,"@all",false)==0)
		{
			for(int i=1;i<=64;i++)
			{
				if(IsClientInGame(i))
				{
					if(!IsFakeClient(i))
					{

						ShowBhopStats(client,i);
					}
				}
			}
		}
		else
		{
			if(strcmp(buffer,"@ct",false)==0)
			{
				for(int i=1;i<=64;i++)
				{
					if(IsClientInGame(i))
					{
						if(!IsFakeClient(i))
						{
							if(GetClientTeam(i)==3)
							{
								ShowBhopStats(client,i);
							}
						}
					}
				}
			}
			else
			{
				if(strcmp(buffer,"@t",false)==0)
				{
					for(int i=1;i<=64;i++)
					{
						if(IsClientInGame(i))
						{
							if(!IsFakeClient(i))
							{
								if(GetClientTeam(i)==2)
								{
									ShowBhopStats(client,i);
								}
							}
						}
					}
				}
				else
				{
					if(strcmp(buffer,"@me",false)==0)
					{
						ShowBhopStats(client,client);
					}
					else
					{
						for(int i=1;i<=64;i++)
						{
							if(IsClientInGame(i))
							{
								if(!IsFakeClient(i))
								{
									char client_name[64];
									GetClientName(i,client_name,sizeof(client_name));
									if(StrContains(client_name,buffer,false)!=-1)
									{
										ShowBhopStats(client,i);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return Plugin_Handled;
}

void ShowBhopStats(int client,int target)
{
	char buffer[512];
	int UID = EXGUSERS_GetUserUID(target);
	char auth_id[64];
	GetClientAuthId(target, AuthId_Steam2, auth_id, sizeof(auth_id), true);
	char name[64];
	GetClientName(target,name,sizeof(name));
	Format(buffer, sizeof(buffer), "玩家序号:%d UID:%d STEAMID:%s 昵称:%s\n平均跳跃次数:%f 平均速度:%f 同步率:%.4f 最近30次跳跃次数:%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		target,UID,auth_id,name,
		afPlayerAverageJumps[target],
		afPlayerAverageSpeed[target],
		afPlayerAveragePerfJumps[target],
		aiPlayerLastJumps[target][0],
		aiPlayerLastJumps[target][1],
		aiPlayerLastJumps[target][2],
		aiPlayerLastJumps[target][3],
		aiPlayerLastJumps[target][4],
		aiPlayerLastJumps[target][5],
		aiPlayerLastJumps[target][6],
		aiPlayerLastJumps[target][7],
		aiPlayerLastJumps[target][8],
		aiPlayerLastJumps[target][9],
		aiPlayerLastJumps[target][10],
		aiPlayerLastJumps[target][11],
		aiPlayerLastJumps[target][12],
		aiPlayerLastJumps[target][13],
		aiPlayerLastJumps[target][14],
		aiPlayerLastJumps[target][15],
		aiPlayerLastJumps[target][16],
		aiPlayerLastJumps[target][17],
		aiPlayerLastJumps[target][18],
		aiPlayerLastJumps[target][19],
		aiPlayerLastJumps[target][20],
		aiPlayerLastJumps[target][21],
		aiPlayerLastJumps[target][22],
		aiPlayerLastJumps[target][23],
		aiPlayerLastJumps[target][24],
		aiPlayerLastJumps[target][25],
		aiPlayerLastJumps[target][26],
		aiPlayerLastJumps[target][27],
		aiPlayerLastJumps[target][28],
		aiPlayerLastJumps[target][29]
	);
	PrintToConsole(client,buffer);
}

void PatternIdentification(int client)
{
	if(afPlayerAverageSpeed[client]<=295)
	{
		return;
	}
	//Pattern 1:Perfect Landing-Jumping
	//With Perf>0.95 and Speed>295
	//macro or cheat
	if(afPlayerAveragePerfJumps[client]>=0.95)
	{
		SuspectReport(client,1);
		return;
	}
	//Pattern 2:One Jump
	//With every jump no extra +jump press(last 1 1 1 1 1 ...)
	//this could be bhop cheat
	if(afPlayerAverageJumps[client]<1.1)
	{
		SuspectReport(client,2);
	}
	//Pattern 3:Jump Master
	//With evert jump too much +jump press(>=15)
	//Maybe no-friction mouse wheel user(caution!)
	if(afPlayerAverageJumps[client]>=18)
	{
		SuspectReport(client,3);
	}

}

void SuspectReport(int client,int pattern)
{
	if(SuspectTag[client])
	{
		return;
	}
	SuspectTag[client]=true;
	for(int i = 1; i <= 64; i++)
	{
		if(!IsClientInGame(i)||IsFakeClient(i))
		{
			continue;
		}
		if(GetUserFlagBits(i) & ADMFLAG_GENERIC)
		{
			int uid = EXGUSERS_GetUserUID(client);
			char auth_id[64];
			char name[64];
			GetClientName(client, name, sizeof(name));
			GetClientAuthId(client, AuthId_Steam2, auth_id, sizeof(auth_id), true);
			PrintToChat(i," \x05[连跳检测] \x01玩家\x07%s\x01有疑似\x07连跳作弊/宏\x01嫌疑,若您有空请检测其数据并观察其视角进行录像 [玩家UID:%d,STEAMID:%s]", name, uid, auth_id);
			PrintToChat(i," \x05[连跳检测] \x06注意,由于地图自带的推力(包括神器和风的吸推),下坡,低重力区域会导致数据异常,请遵循疑罪从无原则");
		}
	}
}