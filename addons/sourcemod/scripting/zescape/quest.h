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
	menu.AddItem("","每日任务");
	menu.AddItem("","月度挑战");
	menu.AddItem("","赛季任务");
	menu.Display(client, MENU_TIME_FOREVER);
}