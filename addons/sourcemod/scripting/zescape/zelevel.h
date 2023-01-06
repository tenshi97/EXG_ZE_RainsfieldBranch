char g_mlevel_path[128];
char map_name[64];
bool g_mlevel_exist;
ArrayList g_mlevel_list;
int extra_check[64];
LEVEL_LOG current_level,default_level;

GlobalForward g_fwOnLevelSet;

char LEVEL_DIFFICULTY[][] =
{
    "简单/默认",    //mako Normal
    "普通",   //Minas Stage2~3    mako Hard/EX
    "困难",   //Minas Stage4~EX1  mako EX 2
    "高难",   //Minas EX2~EX4 westersand81 god
    "极难",   //EX寒渊  westersand81 insane god 死核D npstv1_2+boss 狮子王ACT3里世界
    "绝境",   //死核s7 Insane D EX狮子王ACT3里世界
    "梦魇",   //死核s7 Knightmare D EX寒渊
    "末日"    //留空
};

void LevelOnPluginStart()
{
	g_fwOnLevelSet = new GlobalForward("EXGZE_OnLevelSet", ET_Ignore, Param_Any);
	default_level.id = -1;
	default_level.name = "默认";
	default_level.diff = 0;
	default_level.extra = -1;
	default_level.precon = 0;
	current_level = default_level;
	for(int i=0;i<64;i++)
	{
		extra_check[i]=0;
	}
	g_mlevel_exist = false;
	g_mlevel_list = CreateArray(sizeof(LEVEL_LOG));
	GetCurrentMap(map_name,sizeof(map_name));
	BuildPath(Path_SM,g_mlevel_path,sizeof(g_mlevel_path),"configs/map_level/%s.txt",map_name);
	LoadMapLevelSetting();
}
void LevelOnMapStart()
{
	for(int i=0;i<64;i++)
	{
		extra_check[i]=0;
	}
	current_level = default_level;
	LevelOnSetLevel(default_level);
	g_mlevel_exist = false;
	delete g_mlevel_list;
	g_mlevel_list = CreateArray(sizeof(LEVEL_LOG));
	GetCurrentMap(map_name,sizeof(map_name));
	BuildPath(Path_SM,g_mlevel_path,sizeof(g_mlevel_path),"configs/map_level/%s.txt",map_name);
	LoadMapLevelSetting();
}
void LoadMapLevelSetting()
{
	if(FileExists(g_mlevel_path))
	{
		g_mlevel_exist = true;
	}
	LevelReload();
}
void PrintLevel(LEVEL_LOG level)
{
	PrintToChatAll( "\x05[关卡系统]\x01读入关卡[%s],实体名[%s],触发名[%s]",level.name,level.entity_name,level.output_name);
}
void LevelReload()
{
	if(g_mlevel_exist)
	{
		LEVEL_LOG level_temp;
		PrintToChatAll(" \x05[关卡系统]\x01读取到当前地图关卡");
		KeyValues kv_mplvl = new KeyValues("levels");
		kv_mplvl.ImportFromFile(g_mlevel_path);
		kv_mplvl.Rewind();
		if(!kv_mplvl.GotoFirstSubKey())
		{
			g_mlevel_exist = false;
			delete kv_mplvl;
			return;
		}
		do
		{
			level_temp.id = kv_mplvl.GetNum("id",0);
			kv_mplvl.GetString("name",level_temp.name,64,"Default");
			ReplaceString(level_temp.name,sizeof(level_temp.name),"*","\"");
			level_temp.entity_id = kv_mplvl.GetNum("hammerid",0);
			kv_mplvl.GetString("entname",level_temp.entity_name,64,"");
			kv_mplvl.GetString("entclass",level_temp.entity_class,64,"");
			kv_mplvl.GetString("outputname",level_temp.output_name,256,"");
			kv_mplvl.GetString("outputarg",level_temp.output_arg,256,"");
			level_temp.diff = kv_mplvl.GetNum("difficulty",0);
			level_temp.extra = kv_mplvl.GetNum("extra",-1);
			level_temp.precon = kv_mplvl.GetNum("precon",0);
			level_temp.bounty = kv_mplvl.GetNum("bounty",0);
			g_mlevel_list.PushArray(level_temp,sizeof(level_temp));
			PrintLevel(level_temp);
		}while(kv_mplvl.GotoNextKey())
		delete kv_mplvl;
	}
	else
	{
		PrintToChatAll(" \x05[关卡系统]\x01未读取到当前地图关卡");
	}
}
void LevelOnRoundEnd()
{
	for(int i=0;i<64;i++)
	{
		extra_check[i]=0;
	}
	current_level = default_level;
}
void LevelOnEntityCreated(int entity, const char[] classname)
{
	if(strcmp(classname,"logic_case")==0||strcmp(classname,"trigger_once")==0||strcmp(classname,"trigger_multiple")==0||strcmp(classname,"logic_relay")==0||strcmp(classname,"trigger_teleport")==0||strcmp(classname,"point_template")==0)
	{
    	SDKHook(entity, SDKHook_SpawnPost, LevelOnEntitySpawned);
    }
}
void LevelOnEntitySpawned(int entity)
{
	char ent_name[64];
	GetEntPropString(entity,Prop_Data,"m_iName",ent_name,sizeof(ent_name));
	int ent_id = GetEntProp(entity, Prop_Data, "m_iHammerID");
	LEVEL_LOG level_temp;
	//PrintToConsoleAll(" [Basic Debug]%s",ent_name);
	for(int i=0;i<g_mlevel_list.Length;i++)
	{
		GetArrayArray(g_mlevel_list,i,level_temp,sizeof(level_temp));
		if(strcmp(ent_name,level_temp.entity_name)==0||(level_temp.entity_id!=0&&level_temp.entity_id == ent_id))
		{
			PrintToConsoleAll(" [Debug][%s][%s]",ent_name,level_temp.output_name);
			HookSingleEntityOutput(entity,level_temp.output_name,LevelOnOutput,true);
		}
	}
	SDKUnhook(entity, SDKHook_SpawnPost, LevelOnEntitySpawned);
}
void LevelOnOutput(const char[] output, int caller, int activator, float delay)
{
	char ent_name[64];
	GetEntPropString(caller,Prop_Data,"m_iName",ent_name,sizeof(ent_name));
	int ent_id = GetEntProp(caller, Prop_Data, "m_iHammerID");
	PrintToConsoleAll(" [Output Debug][%s][%d][%s]",ent_name,ent_id,output);
	int level_buffer[64];
	int level_num = LevelOutputGetIndex(caller,output,level_buffer);
	if(level_num==-1)	return;
	for(int i=0;i<=level_num;i++)
	{
		int level_id=level_buffer[i];
		LEVEL_LOG level_temp;
		GetArrayArray(g_mlevel_list,level_id,level_temp,sizeof(level_temp));
		if(level_temp.precon==1)
		{
			extra_check[level_temp.id]++;
			PrintToConsoleAll(" [DEBUG TEST] %d %d",level_temp.id,extra_check[level_temp.id]);
			if(extra_check[level_temp.id]>=2)
			{
				LevelOnSetLevel(level_temp);
			}
			continue;
		}
		if(level_temp.extra!=-1)
		{
			LEVEL_LOG level_extra_buffer;
			extra_check[level_temp.extra]++;
			PrintToConsoleAll(" [DEBUG TEST] %d %d",level_temp.extra,extra_check[level_temp.extra]);
			if(extra_check[level_temp.extra]>=2)
			{
				PrintToConsoleAll(" [DEBUG TEST] %d",level_temp.extra);
				for(int j=0;j<g_mlevel_list.Length;j++)
				{
					GetArrayArray(g_mlevel_list,j,level_extra_buffer,sizeof(level_extra_buffer));
					if(level_extra_buffer.id==level_temp.extra)
					{
						LevelOnSetLevel(level_extra_buffer);
						return;
					}
				}
			}
		}
		if(current_level.id!=level_temp.extra||current_level.id==-1)
		{
			LevelOnSetLevel(level_temp);
		}
	}
}
int LevelOutputGetIndex(int entity,const char[] output,int[] result)
{
	int ret=-1;
	char ent_name[64];
	GetEntPropString(entity,Prop_Data,"m_iName",ent_name,sizeof(ent_name));
	int ent_id = GetEntProp(entity, Prop_Data, "m_iHammerID");
	LEVEL_LOG level_temp;
	for(int i=0;i<g_mlevel_list.Length;i++)
	{
		GetArrayArray(g_mlevel_list,i,level_temp,sizeof(level_temp));
		if((ent_id==level_temp.entity_id&&ent_id!=0)||strcmp(ent_name,level_temp.entity_name)==0)
		{
			if(strcmp(output,level_temp.output_name)==0)
			{
				ret++;
				result[ret]=level_temp.id;
			}
		}
	}
	return ret;
}

void LevelOnSetLevel(LEVEL_LOG level_set)
{
	current_level = level_set;
	InstructorHud_Icon level_icon;
	PrintToChatAll(" \x05[关卡系统]\x01当前关卡:\x07 %s",current_level.name);
	PrintToChatAll(" \x05[关卡系统]\x01当前关卡:\x07 %s",current_level.name);
	PrintToChatAll(" \x05[关卡系统]\x01当前关卡:\x07 %s",current_level.name);
	int r=255;
	int g=255;
	int b=255;
	if(current_level.diff<3)
	{
		level_icon = Icon_tip;
	}
	else if(current_level.diff<5)
	{
		g=0;
		b=0;
		level_icon = Icon_alert_red;
	}
	else
	{
		r=0;
		g=0;
		level_icon = Icon_skull;
	}
	for(int i=1;i<=64;i++)
	{
		if(!IsClientInGame(i))	continue;
		if(IsFakeClient(i))	continue;
		CreateInstructorHint(i,8.0,level_icon,0,r,g,b,"当前关卡:%s  难度:%s",current_level.name,current_level.diff>=0?LEVEL_DIFFICULTY[current_level.diff]:"无");
	}
	Call_StartForward(g_fwOnLevelSet);
	Call_PushArray(level_set,sizeof(LEVEL_LOG));
	Call_Finish();
}

public int Native_GetLevelDiffName(Handle plugin, int numParams)
{
	int difficulty = GetNativeCell(1);
	SetNativeString(2,LEVEL_DIFFICULTY[difficulty],64);
	return 0;
}

public int Native_GetCurrentLevel(Handle plugin, int numParams)
{
	SetNativeArray(1,current_level,sizeof(LEVEL_LOG));
	return 0;
}
