public Action OnClientSayCommand(int client, const char[] command, const char[] sArgs)
{
	if(client<=0||client>64)	return Plugin_Continue;
	if(!IsClientInGame(client)||IsChatTrigger())	return Plugin_Continue;
	if(sArgs[0]=='\\')
	{
		char message[512];
		strcopy(message,sizeof(message),sArgs);
		ReplaceString(message, sizeof(message), "\\", "");
		SuperChat(client,message);
		return Plugin_Handled;
	}
	if(SayJudgeOnHB(client,command,sArgs))
	{
		return Plugin_Handled;
	}
	return Plugin_Continue;
}