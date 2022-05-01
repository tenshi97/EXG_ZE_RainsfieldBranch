void QuestOnPluginStart()
{
	RegConsoleCmd("sm_rw",QuestCommand);
	RegConsoleCmd("sm_quest",QuestCommand);
}
Action QuestMenuCommand(int client,int args)
{
	if(client<=0)	return Plugin_Handled;
	if (!IsClientInGame(client)) return Plugin_Handled;
	char buffer[256];
	Menu menu = CreateMenu(QuestMenuHandler);
	menu.SetTitle("任务系统");
	menu.AddItem("","每日任务",ITEMDRAW_DISABLED);
	menu.AddItem("","月度挑战",ITEMDRAW_DISABLED);
	menu.AddItem("","赛季任务");
	menu.Display(client, MENU_TIME_FOREVER);
}
int QuestMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End)
	{
		menu.Close();
	}
	else if (action == MenuAction_Select)
	{            		
		if(param == 2)
		{
			MissionMenuBuild(client);
		}
	}
}