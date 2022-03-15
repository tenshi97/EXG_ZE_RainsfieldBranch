void JumpOnPluginStart()
{
	RegAdminCmd("sm_jump",JumpCommand);
}

Action JumpCommand(int client,int args)
{
	if(!IsClientInGame(client))	return Plugin_Handled;
	
}