char g_wp_path[256];
KeyValues kv_wp;
ArrayList g_WP_List;
ArrayList g_WP_Default_List;
enum struct Weapon_AdmEditing
{
	int hook_say;
	int weapon_num;
}
Weapon_AdmEditing g_client_weaponedit[65];
void WeaponEditOnPluginStart()
{
	g_WP_List = CreateArray(sizeof(WeaponPurchase_Log));
	RegAdminCmd("sm_wa",WeaponAdminCommand,ADMFLAG_GENERIC);
	for(int i=1;i<=64;i++)
	{
		g_client_weaponedit[i].hook_say = 0;
	}
}
void WeaponEditOnClientDisconnect(int client)
{
	g_client_weaponedit[client].hook_say = 0;
}
void WeaponEditOnMapStart()
{
	for(int i=1;i<=64;i++)
	{
		g_client_weaponedit[i].hook_say = 0;
	}
	delete kv_wp;
	delete g_WP_List;
	g_WP_List = CreateArray(sizeof(WeaponPurchase_Log));
	char map_name[64];
	GetCurrentMap(map_name,sizeof(map_name));
	BuildPath(Path_SM,g_wp_path,sizeof(g_wp_path),"configs/gun_menu/%s.txt",map_name);
	PrintToConsoleAll("[Rainsfield Debugger]%s",g_wp_path);
	LoadCurrentMapWeaponSetting();
}
void LoadCurrentMapWeaponSetting()
{
	if(!FileExists(g_wp_path))
	{
		char wp_default_path[256];
		BuildPath(Path_SM,wp_default_path,sizeof(wp_default_path),"configs/gun_menu/default.txt");
		KeyValues kv_wp_temp = new KeyValues("weapons");
		kv_wp_temp.ImportFromFile(wp_default_path);
		kv_wp_temp.ExportToFile(g_wp_path);
		delete kv_wp_temp;
	}
	kv_wp = new KeyValues("weapons");
	WeaponPurchase_Log wp_temp;
	if(!kv_wp.ImportFromFile(g_wp_path))
	{
		delete kv_wp;
		return;
	}
	if(!kv_wp.GotoFirstSubKey())
	{
		delete kv_wp;
		return;
	}
	do
	{
		kv_wp.GetSectionName(wp_temp.name,sizeof(wp_temp.name));
		wp_temp.price = kv_wp.GetNum("price",3000);
		wp_temp.restrict = kv_wp.GetNum("restrict",1);
		wp_temp.maxpurchase = kv_wp.GetNum("maxpurchase",10);
		g_WP_List.PushArray(wp_temp,sizeof(wp_temp));
	}while(kv_wp.GotoNextKey());
	delete kv_wp;

}
Action WeaponAdminCommand(int client,int args)
{
	if(client<=0||client>64)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	WeaponMenu(client);
	return Plugin_Handled;
}
Action WeaponAdminPlusCommand(int client,int args)
{
	if(client<=0||client>64)	return Plugin_Handled;
	if(!IsClientInGame(client))	return Plugin_Handled;
	WeaponPlusMenu(client);
	return Plugin_Handled;
}
void WeaponMenu(int client)
{
	WeaponPurchase_Log weapon_temp;
	Menu menu = CreateMenu(WeaponMenuHandler);
	menu.SetTitle("武器管理");
	for(int i=0;i < g_WP_List.Length;i++)
	{
		GetArrayArray(g_WP_List,i,weapon_temp,sizeof(weapon_temp));
		menu.AddItem("",weapon_temp.name);
	}
	menu.Display(client,MENU_TIME_FOREVER);
}
void WeaponPlusMenu(int client)
{

}
int WeaponMenuHandler(Menu menu, MenuAction action, int client, int param) 
{
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{
		WeaponSet(client,param);
		return 0;
	}
}

void WeaponSet(int client,int weapon_num)
{
	Menu menu = CreateMenu(WeaponSetMenuHandler)
	WeaponPurchase_Log weapon_temp;
	GetArrayArray(g_WP_List,weapon_num,weapon_temp,sizeof(weapon_temp));
	char weapon_num_char[64];
	char buffer[256];
	IntToString(weapon_num,weapon_num_char,sizeof(weapon_num_char));
	Format(buffer,sizeof(buffer),"修改武器配置:%s",weapon_temp.name);
	menu.SetTitle(buffer);
	Format(buffer,sizeof(buffer),"价格:%d",weapon_temp.price);
	menu.AddItem(weapon_num_char,buffer);
	Format(buffer,sizeof(buffer),"限制购买:%s",weapon_temp.restrict?"是":"否");
	menu.AddItem(weapon_num_char,buffer);
	Format(buffer,sizeof(buffer),"最大购买次数:%d(0为不限制)",weapon_temp.maxpurchase);
	menu.AddItem(weapon_num_char,buffer);
	menu.AddItem("","重载");
	menu.ExitBackButton = true;
	menu.Display(client,MENU_TIME_FOREVER);
}

int WeaponSetMenuHandler(Menu menu, MenuAction action, int client, int param) 
{
	if (action == MenuAction_End) menu.Close();
	else if (action == MenuAction_Select)
	{
		char item[4];
		menu.GetItem(0, item, sizeof(item));
		int weapon_num = StringToInt(item);
		WeaponPurchase_Log weapon_temp;
		GetArrayArray(g_WP_List,weapon_num,weapon_temp,sizeof(weapon_temp));
		switch(param)
		{
			case 0:
			{
				g_client_weaponedit[client].hook_say = 1;
				g_client_weaponedit[client].weapon_num = weapon_num;
				PrintToChat(client," \x05[武器管理]\x01输入数字修改武器价格,输入cancel取消");
			}
			case 1:
			{
				weapon_temp.restrict = !(weapon_temp.restrict);
				g_WP_List.SetArray(weapon_num,weapon_temp,sizeof(weapon_temp));
				ModifyWeaponKv(weapon_num);
				WeaponSet(client,weapon_num);
			}
			case 2:
			{
				g_client_weaponedit[client].hook_say = 2;
				g_client_weaponedit[client].weapon_num = weapon_num;
				PrintToChat(client," \x05[武器管理]\x01输入数字修改武器购买次数,输入cancel取消");
			}
			case 3:
			{
				delete g_WP_List;
				g_WP_List = CreateArray(sizeof(WeaponPurchase_Log));
				LoadCurrentMapWeaponSetting();
				ServerCommand("sm_reloadweapon");
			}
		}
		return 0;
	}
	else if (param == MenuCancel_ExitBack)
	{
		WeaponMenu(client);
		return 0;
	}
}

void ModifyWeaponKv(int weapon_num)
{
	WeaponPurchase_Log weapon_temp;
	KeyValues kv_wp_temp = new KeyValues("weapons");
	if(!kv_wp_temp.ImportFromFile(g_wp_path))
	{
		delete kv_wp_temp;
		return;
	}
	GetArrayArray(g_WP_List,weapon_num,weapon_temp,sizeof(weapon_temp));
	kv_wp_temp.Rewind();
	if(!kv_wp_temp.GotoFirstSubKey())
	{
		delete kv_wp_temp;
		return;
	}
	char secname[64];
	kv_wp_temp.GetSectionName(secname,sizeof(secname));
	while(strcmp(secname,weapon_temp.name)!=0)
	{
		if(!kv_wp_temp.GotoNextKey())
		{
			delete kv_wp_temp;
			return;
		}
		kv_wp_temp.GetSectionName(secname,sizeof(secname));
	}
	kv_wp_temp.SetNum("price",weapon_temp.price);
	kv_wp_temp.SetNum("restrict",weapon_temp.restrict);
	kv_wp_temp.SetNum("maxpurchase",weapon_temp.maxpurchase);
	kv_wp_temp.Rewind();
	kv_wp_temp.ExportToFile(g_wp_path);
	delete kv_wp_temp;
}

bool WeaponIsClientModifying(int client,const char[] sArgs)
{
	if(g_client_weaponedit[client].hook_say != 0)
	{
		int itemnum = g_client_weaponedit[client].weapon_num;
		WeaponPurchase_Log weapon_temp;
		GetArrayArray(g_WP_List,itemnum,weapon_temp,sizeof(weapon_temp));
		if(g_client_weaponedit[client].hook_say == 1)
		{
			weapon_temp.price = StringToInt(sArgs);

		}
		else
		{
			weapon_temp.maxpurchase = StringToInt(sArgs);
		}
		PrintToChat(client," \x05[地图管理]\x01修改成功");
		g_WP_List.SetArray(itemnum,weapon_temp,sizeof(weapon_temp));
		g_client_weaponedit[client].hook_say = 0;
		ModifyWeaponKv(itemnum);
		WeaponSet(client,itemnum);
		return true;
	}
	else
	{
		return false;
	}
}