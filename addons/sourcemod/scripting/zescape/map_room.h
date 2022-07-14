void MapRoomOnPluginStart()
{
	RegAdminCmd("sm_mr",MapRoomCommand,ADMFLAG_GENERIC);
}
Action MapRoomCommand(int client,int args)
{
	MapRoomMenu(client);
	return Plugin_Handled;
}
void MapRoomMenu(int client)
{
	Menu menu = CreateMenu(MapRoomMenuHandler);
	menu.SetTitle("地图传送");
	menu.AddItem("","传送至调关房",Pmap.mr?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
	menu.AddItem("","保存当前位置");
	menu.Display(client,MENU_TIME_FOREVER);
}
int MapRoomMenuHandler(Menu menu, MenuAction action, int client, int param) 
{
	if (action == MenuAction_End)
	{
		menu.Close();
	}
	else if (action == MenuAction_Select)
	{
		if(param==0)
		{
			if(Pmap.mr==1)
			{
				float mr_pos[3];
				mr_pos[0]=Pmap.mrx;
				mr_pos[1]=Pmap.mry;
				mr_pos[2]=Pmap.mrz;
				TeleportEntity(client, mr_pos, NULL_VECTOR,NULL_VECTOR);
			}
		}
		else if(param==1)
		{
			float client_pos[3];
			GetClientAbsOrigin(client, client_pos);
			Pmap.mrx=client_pos[0];
			Pmap.mry=client_pos[1];
			Pmap.mrz=client_pos[2];
			Pmap.mr=1;
			MapCfgUpdate(Pmap);
			Pmap_Reload();

		}            
	}
}
