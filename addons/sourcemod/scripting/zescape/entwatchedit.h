bool g_Current_Map_EntWatch;
KeyValues kv;
bool g_KV_Load_Status;
bool g_KV_MapPacked;
bool g_KV_NormalKv;
bool g_KV_Override;
ArrayList g_Ent_List;
ArrayList g_PendingWeapons_List;
char g_ew_path[256];
char colors_name[24][64]={"{default}","{green}","{darkred}","{purple}","{lightgreen}","{lime}","{red}","{grey}","{olive}","{a}","{lightblue}","{blue}","{d}","{pink}","{darkorange}","{orange}","{white}","{yellow}","{magenta}","{silver}","{bluegrey}","{lightred}","{cyan}","{gray}"};
char colors_cnname[24][64]={"默认色","绿色","暗红","紫色","浅绿","酸橙绿","红色","灰色","橄榄绿","a","浅蓝色","蓝色","d","粉色","暗橙","橙色","白色","黄色","洋红色","银色","蓝灰色","浅红色","天青色","暗灰色"};
enum EntEditing_Type
{
	MODIFY_NAME,
	MODIFY_SHORTNAME,
	MODIFY_COOLDOWN,
	MODIFY_MAXUSES
};
enum struct Ent_AdmEditing
{
	int hook_say;
	EntEditing_Type type;
	int item_num;
}
Ent_AdmEditing g_client_entedit[65];
void ResetBool()
{
	g_Current_Map_EntWatch = false;
	g_KV_Load_Status = true;
	g_KV_MapPacked = false;
	g_KV_NormalKv = false;
	g_KV_Override = false;
	for(int i=1;i<=64;i++)
	{
		g_client_entedit[i].hook_say = 0;
	}
}
void EWEditOnClientDisconnect(int client)
{
	g_client_entedit[client].hook_say = 0;
}
void EWEditOnPluginStart()
{
	ResetBool();
	g_Ent_List = CreateArray(sizeof(Ent_Log));
	g_PendingWeapons_List = CreateArray(sizeof(ItemWeapon_Log));
	RegAdminCmd("sm_ea",EntAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_ehudadmin",EntAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_entadmin",EntAdminCommand,ADMFLAG_GENERIC);
	RegAdminCmd("sm_eap",EntAdminPendingWeaponCommand,ADMFLAG_GENERIC);
}
void EWEditOnMapStart()
{
	ResetBool();
	delete kv;
	delete g_Ent_List;
	delete g_PendingWeapons_List;
	g_Ent_List = CreateArray(sizeof(Ent_Log));
	g_PendingWeapons_List = CreateArray(sizeof(ItemWeapon_Log));
	kv = new KeyValues("entities");
	char map_name[64];
	char ew_path_override[256];
	GetCurrentMap(map_name,sizeof(map_name));
	Format(ew_path_override,sizeof(ew_path_override),"cfg/sourcemod/entwatch/maps/%s_override.cfg",map_name);
	if(FileExists(ew_path_override))
	{
		g_KV_Override = true;
		//PrintToServer(" [Rainsfield]Override EntWatch Detected");
		strcopy(g_ew_path,sizeof(g_ew_path),ew_path_override);
		BuildCurrentMapEntList();
		return;
	}
	Format(g_ew_path,sizeof(g_ew_path),"cfg/sourcemod/entwatch/maps/%s.cfg",map_name);
	BuildCurrentMapEntList();

}
void BuildCurrentMapEntList()
{
	if(!g_KV_Override&&FileExists(g_ew_path,false))
	{
		g_KV_NormalKv = true;
	}
	else
	{
		g_KV_NormalKv = false;
	}
	Ent_Log ent_temp;
	char buffer[256];
	kv.ImportFromFile(g_ew_path);
	if(!kv.GotoFirstSubKey())
	{
		delete kv;
		g_KV_Load_Status = false;
		g_KV_NormalKv = false;
		g_KV_MapPacked = false;
		return;
	}
	else
	{
		if(!g_KV_NormalKv&&!g_KV_Override)
		{
			g_KV_MapPacked = true;
		}
	}
	do
	{
		kv.GetString("name",ent_temp.name,256);
		kv.GetString("shortname",ent_temp.shortname,64);
		kv.GetString("color",buffer,64);
		ent_temp.color = 0;
		for(int i=0;i<=23;i++)
		{
			if(strcmp(buffer,colors_name[i])==0)
			{
				ent_temp.color=i;
			}
		}
		kv.GetString("buttonclass",ent_temp.class,64);
		if(kv.GetString("hasfiltername",buffer,sizeof(buffer)))
		{
			if(strcmp(buffer,"true",false)==0)
			{
				ent_temp.hasfilter=true;
				kv.GetString("filtername",ent_temp.filtername,64);

			}
			else
			{
				ent_temp.hasfilter=false;
			}
		}
		else
		{
			ent_temp.hasfilter=false;
		}
		kv.GetString("chat",buffer,64);
		if(strcmp(buffer,"true",false)==0)
		{
			ent_temp.chat=1;
		}
		else
		{
			ent_temp.chat=0;
		}
		kv.GetString("hud",buffer,64);
		if(strcmp(buffer,"true",false)==0)
		{
			ent_temp.hud=1;
		}
		else
		{
			ent_temp.hud=0;
		}
		ent_temp.hammerid = kv.GetNum("hammerid",0);
		ent_temp.mode = kv.GetNum("mode",0);		
		ent_temp.maxuses = kv.GetNum("maxuses",0);
		ent_temp.maxamount = kv.GetNum("maxamount",0);
		ent_temp.cooldown = kv.GetNum("cooldown",0);
		g_Ent_List.PushArray(ent_temp,sizeof(Ent_Log));
	}while(kv.GotoNextKey());
	if(g_KV_MapPacked)
	{
		PrintToServer(" Building Override EntWatch");
		PrintToChatAll(" \x05[地图管理]\x01检测到地图自带EntWatch,创建Override EntWatch");
		kv.Rewind();
		char map_name[64];
		GetCurrentMap(map_name,sizeof(map_name));
		Format(g_ew_path,sizeof(g_ew_path),"cfg/sourcemod/entwatch/maps/%s_override.cfg",map_name);
		kv.ExportToFile(g_ew_path);
		g_KV_MapPacked = false;
		g_KV_Override = true;
	}
	delete kv;
}
Action EntAdminCommand(int client,int args)
{	
	EntAdmMenuCreate(client);
	return Plugin_Handled;
}
void EntAdmMenuCreate(int client)
{
	if(g_Ent_List.Length == 0 )
	{
		PrintToChat(client," \x05[神器管理]\x01当前地图无神器!");
	}
	Menu menu = CreateMenu(EntAdminMenuHandler);
	menu.SetTitle("神器管理");
	char num[4];
	Ent_Log ent_temp;
	for(int i = 0 ; i < g_Ent_List.Length ; i++)
	{
		IntToString(i,num,sizeof(num));
		GetArrayArray(g_Ent_List,i,ent_temp,sizeof(Ent_Log));
		menu.AddItem(num,ent_temp.shortname);
	}
	menu.Display(client,MENU_TIME_FOREVER);	
}
int EntAdminMenuHandler(Menu menu, MenuAction action, int client, int param) 
{
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{
		EntSetMenu(client,param);
		return 0;
	}
}
void EntSetMenu(int client,int itemnum)
{
	Ent_Log ent_temp;
	Menu menu = CreateMenu(EntSetMenuHandler);
	menu.SetTitle("神器管理");
	char buffer[256];
	char item[4];
	IntToString(itemnum,item,sizeof(item));
	GetArrayArray(g_Ent_List,itemnum,ent_temp,sizeof(Ent_Log));
	Format(buffer,sizeof(buffer),"名称:%s\n显示在聊天栏",ent_temp.name);
	menu.AddItem(item,buffer);
	Format(buffer,sizeof(buffer),"短名称:%s\n显示在hud列表",ent_temp.shortname);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"颜色(RGB):%s",colors_cnname[ent_temp.color]);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"触发类型:%s",ent_temp.class);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"HUD显示:%s",ent_temp.hud?"开":"关");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"聊天栏显示:%s",ent_temp.chat?"开":"关");
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"Hammer编码:%d\n不可修改，若有bug联系后台管理",ent_temp.hammerid);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"使用者过滤器:%s\n不可修改，若有bug联系后台管理",ent_temp.hasfilter?"有":"无");
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"过滤器名:%s\n不可修改，若有bug联系后台管理",ent_temp.filtername);
	menu.AddItem("",buffer,ITEMDRAW_DISABLED);
	Format(buffer,sizeof(buffer),"模式:%d",ent_temp.mode);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"冷却:%d(秒)",ent_temp.cooldown);
	menu.AddItem("",buffer);
	Format(buffer,sizeof(buffer),"最大使用次数:%d",ent_temp.maxuses)	
	menu.AddItem("",buffer);
	menu.AddItem("","重载");
	menu.AddItem("","保存所有(适用于BUG情况)");
	menu.AddItem("","删除该神器hud");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);

}
int EntSetMenuHandler(Menu menu, MenuAction action, int client, int param) 
{
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{
		g_client_entedit[client].hook_say = 0;
		char item[4];
		int item_num;
		menu.GetItem(0, item, sizeof(item));
		item_num = StringToInt(item);
		g_client_entedit[client].item_num = item_num;
		Ent_Log ent_temp;
		GetArrayArray(g_Ent_List,item_num,ent_temp,sizeof(ent_temp));
		switch(param)
		{
			case 0:
			{
				g_client_entedit[client].hook_say = 1;
				PrintToChat(client," \x05[神器管理]\x01输入内容修改神器名,输入\x09 cancel\x01取消，输入\x09 null\x01置空");
				g_client_entedit[client].type = MODIFY_NAME;
			}
			case 1:
			{
				g_client_entedit[client].hook_say = 1;
				PrintToChat(client," \x05[神器管理]\x01输入内容修改神器短名,输入\x09 cancel\x01取消，输入\x09 null\x01置空");
				g_client_entedit[client].type = MODIFY_SHORTNAME;
			}
			case 2:
			{
				EntSetColor(client,item_num);
			}
			case 3:
			{
				if(strcmp(ent_temp.class,"func_button",false)==0)
				{
					ent_temp.class = "game_ui";
				}
				else
				{
					ent_temp.class = "func_button";
				}
				g_Ent_List.SetArray(item_num,ent_temp,sizeof(ent_temp));
				ModifyKv(item_num);
				EntSetMenu(client,item_num);
			}
			case 4:
			{
				ent_temp.hud = !(ent_temp.hud);
				g_Ent_List.SetArray(item_num,ent_temp,sizeof(ent_temp));
				ModifyKv(item_num);
				EntSetMenu(client,item_num);
			}
			case 5:
			{
				ent_temp.chat = !(ent_temp.chat);
				g_Ent_List.SetArray(item_num,ent_temp,sizeof(ent_temp));
				ModifyKv(item_num);
				EntSetMenu(client,item_num);
			}
			case 9:
			{
				EntSetMode(client,item_num);
			}
			case 10:
			{
				g_client_entedit[client].hook_say = 1;
				PrintToChat(client," \x05[神器管理]\x01输入数字修改神器CD,输入\x09 cancel\x01取消");
				g_client_entedit[client].type = MODIFY_COOLDOWN;				
			}
			case 11:
			{
				g_client_entedit[client].hook_say = 1;
				PrintToChat(client," \x05[神器管理]\x01输入数字修改神器次数,输入\x09 cancel\x01取消");
				g_client_entedit[client].type = MODIFY_MAXUSES;			
			}
			case 12:
			{
				kv = new KeyValues("entities");
				delete g_Ent_List;
				delete g_PendingWeapons_List;
				g_Ent_List = CreateArray(sizeof(Ent_Log));
				g_PendingWeapons_List = CreateArray(sizeof(ItemWeapon_Log));
				if(g_KV_Override)
				{
					if(!FileExists(g_ew_path))
					{
						g_KV_Override = false;
					}
				}
				BuildCurrentMapEntList();
				ServerCommand("sm_ew_reloadconfig");
			}
			case 13:
			{
				ModifyAll();
			}
			case 14:
			{
				DeleteKv(item_num);
			}
		}
		return 0;
	}
	else if (param == MenuCancel_ExitBack)
	{
		EntAdmMenuCreate(client);
		return 0;
	}
}

void EntSetColor(int client,int itemnum)
{
	char buffer[256];
	char item[4];
	IntToString(itemnum,item,sizeof(item));
	Format(buffer,sizeof(buffer),"设置颜色");
	Menu menu = CreateMenu(EntSetColorHandler);
	menu.SetTitle(buffer)
	menu.AddItem(item,"占位",ITEMDRAW_DISABLED);
	for(int i=0;i<=23;i++)
	{
		menu.AddItem(colors_name[i],colors_cnname[i]);
	}
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int EntSetColorHandler(Menu menu, MenuAction action, int client, int param) 
{
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{
		char item[4];
		int item_num;
		menu.GetItem(0, item, sizeof(item));
		item_num = StringToInt(item);
		Ent_Log ent_temp;
		GetArrayArray(g_Ent_List,item_num,ent_temp,sizeof(ent_temp));
		ent_temp.color = param-1;
		g_Ent_List.SetArray(item_num,ent_temp,sizeof(ent_temp));
		ModifyKv(item_num);
		EntSetColor(client,item_num);
		return 0;
	}
	else if (param == MenuCancel_ExitBack)
	{
		char item[4];
		int item_num;
		menu.GetItem(0, item, sizeof(item));
		item_num = StringToInt(item);
		EntSetMenu(client,item_num);
		return 0;
	}
}

void EntSetMode(int client,int itemnum)
{
	char buffer[256];
	char item[4];
	IntToString(itemnum,item,sizeof(item));
	Format(buffer,sizeof(buffer),"设置模式");
	Menu menu = CreateMenu(EntSetModeHandler);
	menu.SetTitle(buffer)
	menu.AddItem(item,"占位",ITEMDRAW_DISABLED);
	menu.AddItem("","模式0:基本模式\n只是显示在HUD");
	menu.AddItem("","模式1:防刷屏模式\n同1，防spam");
	menu.AddItem("","模式2:CD模式\n需要设置CD");
	menu.AddItem("","模式3:有限次数模式\n需要设置使用次数");
	menu.AddItem("","模式4:有限次数有CD\n需要设置CD和次数-Builder/终极");
	menu.AddItem("","模式5:使用多次进入CD\n需要设置CD和次数-mako电");
	menu.AddItem("","模式6/7暂不支持(在做了)",ITEMDRAW_DISABLED);
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}
int EntSetModeHandler(Menu menu, MenuAction action, int client, int param) 
{
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{
		char item[4];
		int item_num;
		menu.GetItem(0, item, sizeof(item));
		item_num = StringToInt(item);
		Ent_Log ent_temp;
		GetArrayArray(g_Ent_List,item_num,ent_temp,sizeof(ent_temp));
		ent_temp.mode = param-1;
		g_Ent_List.SetArray(item_num,ent_temp,sizeof(ent_temp));
		ModifyKv(item_num);
		EntSetMode(client,item_num);
		return 0;
	}
	else if (param == MenuCancel_ExitBack)
	{
		char item[4];
		int item_num;
		menu.GetItem(0, item, sizeof(item));
		item_num = StringToInt(item);
		EntSetMenu(client,item_num);
		return 0;
	}
}

void ModifyKv(int item_num)
{
	char buffer[256];
	Ent_Log ent_temp;
	KeyValues kv_temp = new KeyValues("entities");
	if(!kv_temp.ImportFromFile(g_ew_path))
	{
		delete kv_temp;
		return;
	}
	GetArrayArray(g_Ent_List,item_num,ent_temp,sizeof(ent_temp));
	kv_temp.Rewind();
	kv_temp.GotoFirstSubKey();
	while(kv_temp.GetNum("hammerid")!=ent_temp.hammerid)
	{
		if(!kv_temp.GotoNextKey())
		{
			delete kv_temp;
			return;
		}
	}
	kv_temp.SetString("name",ent_temp.name);
	kv_temp.SetString("shortname",ent_temp.shortname);
	kv_temp.SetString("buttonclass",ent_temp.class);
	kv_temp.SetString("color",colors_name[ent_temp.color]);
	Format(buffer,sizeof(buffer),"%s",ent_temp.hud?"true":"false");
	kv_temp.SetString("hud",buffer);
	Format(buffer,sizeof(buffer),"%s",ent_temp.chat?"true":"false");
	kv_temp.SetString("chat",buffer);
	kv_temp.SetNum("mode",ent_temp.mode);
	kv_temp.SetNum("cooldown",ent_temp.cooldown);
	kv_temp.SetNum("maxuses",ent_temp.maxuses);
	kv_temp.Rewind();
	kv_temp.ExportToFile(g_ew_path);
	delete kv_temp;
}
void ModifyAll()
{
	char buffer[256];
	Ent_Log ent_temp;
	KeyValues kv_temp = new KeyValues("entities");
	if(!kv_temp.ImportFromFile(g_ew_path))
	{
		delete kv_temp;
		return;
	}
	for(int i=0;i<g_Ent_List.Length;i++)
	{
		GetArrayArray(g_Ent_List,i,ent_temp,sizeof(ent_temp));
		kv_temp.Rewind();
		kv_temp.GotoFirstSubKey();
		while(kv_temp.GetNum("hammerid")!=ent_temp.hammerid)
		{
			if(!kv_temp.GotoNextKey())
			{
				continue;
			}
		}
		kv_temp.SetString("name",ent_temp.name);
		kv_temp.SetString("shortname",ent_temp.shortname);
		kv_temp.SetString("buttonclass",ent_temp.class);
		kv_temp.SetString("color",colors_name[ent_temp.color]);
		Format(buffer,sizeof(buffer),"%s",ent_temp.hud?"true":"false");
		kv_temp.SetString("hud",buffer);
		Format(buffer,sizeof(buffer),"%s",ent_temp.chat?"true":"false");
		kv_temp.SetString("chat",buffer);
		kv_temp.SetNum("mode",ent_temp.mode);
		kv_temp.SetNum("cooldown",ent_temp.cooldown);
		kv_temp.SetNum("maxuses",ent_temp.maxuses);
	}
	kv_temp.Rewind();
	kv_temp.ExportToFile(g_ew_path);
	delete kv_temp;	
}
void BuildNewCfg()
{

}

bool EntAdmIsClientModifying(int client,const char[] sArgs)
{
	if(g_client_entedit[client].hook_say == 1)
	{
		int itemnum = g_client_entedit[client].item_num;
		Ent_Log ent_temp;
		GetArrayArray(g_Ent_List,itemnum,ent_temp,sizeof(ent_temp));
		switch(g_client_entedit[client].type)
		{
			case MODIFY_NAME:
			{
				if(strcmp(sArgs,"cancel",false)!=0)
				{
					if(strcmp(sArgs,"null",false)==0)
					{	
						ent_temp.name="";
					}
					else
					{
						strcopy(ent_temp.name,sizeof(ent_temp.name),sArgs);
					}
					PrintToChat(client," \x05[神器管理]\x01修改成功");
				}
				else
				{
					PrintToChat(client," \x05[神器管理]\x01已取消修改");					
				}

			}
			case MODIFY_SHORTNAME:
			{
				if(strcmp(sArgs,"cancel",false)!=0)
				{
					if(strcmp(sArgs,"null",false)==0)
					{	
						ent_temp.shortname="";
					}
					else
					{
						strcopy(ent_temp.shortname,sizeof(ent_temp.shortname),sArgs);
					}
					PrintToChat(client," \x05[神器管理]\x01修改成功");
				}		
				else
				{
					PrintToChat(client," \x05[神器管理]\x01已取消修改");					
				}		
			}
			case MODIFY_COOLDOWN:
			{
				if(strcmp(sArgs,"cancel",false)!=0)
				{
					int cd = StringToInt(sArgs);
					ent_temp.cooldown = cd;
					PrintToChat(client," \x05[神器管理]\x01修改成功");
				}		
				else
				{
					PrintToChat(client," \x05[神器管理]\x01已取消修改");					
				}					
			}
			case MODIFY_MAXUSES:
			{
				if(strcmp(sArgs,"cancel",false)!=0)
				{
					int mu = StringToInt(sArgs);
					ent_temp.maxuses = mu;
					PrintToChat(client," \x05[神器管理]\x01修改成功");
				}		
				else
				{
					PrintToChat(client," \x05[神器管理]\x01已取消修改");					
				}					
			}
		}
		g_Ent_List.SetArray(itemnum,ent_temp,sizeof(ent_temp));
		g_client_entedit[client].hook_say = 0;
		ModifyKv(itemnum);
		EntSetMenu(client,itemnum);
		return true;
	}
	else
	{
		return false;
	}
}

void EWEditOnEntityCreated(int entity, const char[] classname)
{
    if(StrContains(classname, "weapon_") == 0)
    {
    	SDKHook(entity, SDKHook_SpawnPost, EWEditOnWeaponSpawned);
    }
}
void EWEditOnWeaponSpawned(int iWeapon)
{
	int iPrevOwner = HasEntProp(iWeapon, Prop_Send, "m_hPrevOwner") ? GetEntPropEnt(iWeapon, Prop_Send, "m_hPrevOwner") : -1;
	if(iPrevOwner > 0 || SetClosestPlayerOwner(iWeapon))
	{
		SDKUnhook(iWeapon, SDKHook_SpawnPost, EWEditOnWeaponSpawned);
		return;
	}
	ItemWeapon_Log weapon_log,weapon_temp;
	Ent_Log ent_temp;
	weapon_log.hammerid = GetEntProp(iWeapon, Prop_Data, "m_iHammerID")
	GetEntPropString(iWeapon,Prop_Data,"m_iName",weapon_log.name,sizeof(weapon_log.name));
	GetEdictClassname(iWeapon,weapon_log.classname,sizeof(weapon_log.classname));
	//PrintToConsoleAll(" [Rainsfield Debug] New Item Weapon[%d:%s %s] Spawned Detected",weapon_log.hammerid,weapon_log.name,weapon_log.classname);
	SplitString(weapon_log.name,"&",weapon_log.name,sizeof(weapon_log.name));
	for(int i=0;i<g_Ent_List.Length;i++)
	{
		GetArrayArray(g_Ent_List,i,ent_temp,sizeof(ent_temp))
		if(ent_temp.hammerid==weapon_log.hammerid)	return;
	}
	for(int i=0;i<g_PendingWeapons_List.Length;i++)
	{
		GetArrayArray(g_PendingWeapons_List,i,weapon_temp,sizeof(weapon_temp));
		if(weapon_temp.hammerid==weapon_log.hammerid)	return;
	}
	g_PendingWeapons_List.PushArray(weapon_log,sizeof(weapon_log));
	SDKUnhook(iWeapon, SDKHook_SpawnPost, EWEditOnWeaponSpawned);
}

int SetClosestPlayerOwner(int iEntity)
{
	float fPos[3];
	GetEntPropVector(iEntity, Prop_Send, "m_vecOrigin", fPos);
	
	float fClosestDistance = -1.0;
	int iClient;
	
	for(int i = 1; i <= MaxClients; i++)
	{
		if(!IsClientInGame(i))
			continue;
		
		if(!IsPlayerAlive(i))
			continue;
		
		float fTargetPos[3];
		GetClientAbsOrigin(i, fTargetPos);
		
		if(fTargetPos[0] == 0.0 && fTargetPos[1] == 0.0 && fTargetPos[2] == 0.0)
			continue;
		
		float fDistance = GetVectorDistance(fPos, fTargetPos);
		
		if (fDistance < fClosestDistance || fClosestDistance == -1.0)
		{
			fClosestDistance = fDistance;
			iClient = i;
		}
	}
	
	if(iClient > 0 && fClosestDistance <= 0.0)
	{
		SetEntProp(iEntity, Prop_Send, "m_iTeamNum", iClient);
		return true;
	}
	
	return false;
}

Action EntAdminPendingWeaponCommand(int client,int args)
{
	PendingWeaponMenu(client);
	return Plugin_Handled;
}
void PendingWeaponMenu(int client)
{
	Menu menu =CreateMenu(PendingWeaponMenuHandler);
	menu.SetTitle("添加武器至entwatch\n点击添加");
	char buffer[256];
	ItemWeapon_Log item_temp;
	for(int i=0;i<g_PendingWeapons_List.Length;i++)
	{
		GetArrayArray(g_PendingWeapons_List,i,item_temp,sizeof(item_temp));
		Format(buffer,sizeof(buffer),"武器名:%s\n类型:%s\n",item_temp.name,item_temp.classname);
		menu.AddItem("",buffer);
	}
	menu.Display(client,MENU_TIME_FOREVER);
}

int PendingWeaponMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{
		Ent_Log ent_temp;
		ItemWeapon_Log item_temp;
		GetArrayArray(g_PendingWeapons_List,param,item_temp,sizeof(item_temp));
		strcopy(ent_temp.name,sizeof(ent_temp.name),item_temp.name);
		strcopy(ent_temp.shortname,sizeof(ent_temp.name),item_temp.name);
		if(strcmp(item_temp.name,"")==0||item_temp.hammerid==0)
		{
			g_PendingWeapons_List.Erase(param);
			return 0;
		}
		if(strcmp(item_temp.classname,"weapon_knife",false)==0||strcmp(item_temp.classname,"weapon_hegrenade",false)==0)
		{
			ent_temp.class = "game_ui";
		}
		else
		{
			ent_temp.class = "func_button";
		}
		ent_temp.color = 1;
		ent_temp.hasfilter = 0;
		ent_temp.filtername = "";
		ent_temp.chat = 0;
		ent_temp.hud = 0;
		ent_temp.hammerid = item_temp.hammerid;
		ent_temp.mode = 1;
		ent_temp.cooldown = 0;
		ent_temp.maxamount = 0;
		ent_temp.maxuses = 0;
		g_Ent_List.PushArray(ent_temp,sizeof(ent_temp));
		char num[64];
		IntToString(g_Ent_List.Length-1,num,sizeof(num));
		InsertKV(num,ent_temp);
		g_PendingWeapons_List.Erase(param);
		PendingWeaponMenu(client);
		return 0;
	}
}

void InsertKV(const char[] new_subkey,Ent_Log ent_temp)
{
	KeyValues kv_new = new KeyValues("entites");
	kv_new.ImportFromFile(g_ew_path);
	kv_new.JumpToKey(new_subkey,true);
	kv_new.SetString("name",ent_temp.name);
	kv_new.SetString("shortname",ent_temp.shortname);
	kv_new.SetString("color",colors_name[ent_temp.color]);
	kv_new.SetString("buttonclass",ent_temp.class);
	kv_new.SetString("hasfiltername",ent_temp.hasfilter?"true":"false");
	kv_new.SetString("filtername",ent_temp.filtername);
	kv_new.SetString("chat",ent_temp.chat?"true":"false");
	kv_new.SetString("hud",ent_temp.hud?"true":"false");
	kv_new.SetNum("hammerid",ent_temp.hammerid);
	kv_new.SetNum("mode",ent_temp.mode);
	kv_new.SetNum("cooldown",ent_temp.cooldown);
	kv_new.SetNum("maxamount",ent_temp.maxamount);
	kv_new.SetNum("maxuses",ent_temp.maxuses);
	kv_new.Rewind();
	kv_new.ExportToFile(g_ew_path);
	delete kv_new;
}

void DeleteKv(int item_num)
{
	KeyValues kv_delete = new KeyValues("entities");
	kv_delete.ImportFromFile(g_ew_path);
	if(!kv_delete.GotoFirstSubKey())	
	{
		delete kv_delete;
		return;
	}
	Ent_Log ent_temp;
	GetArrayArray(g_Ent_List,item_num,ent_temp,sizeof(ent_temp));
	while(kv_delete.GetNum("hammerid")!=ent_temp.hammerid)
	{
		if(!kv_delete.GotoNextKey())
		{
			delete kv_delete;
			return;
		}
	}
	kv_delete.DeleteThis();
	kv_delete.Rewind();
	kv_delete.ExportToFile(g_ew_path);
	delete kv_delete;
}