public void OnClientSayCommand_Post(int client, const char[] command, const char[] sArgs)
{
	if(!IsClientInGame(client)||!client||IsChatTrigger())	return;
	if(sArgs[0]='\\')
	{
		char message[512];
		strcopy(message,sizeof(message),sArgs);
		ReplaceString(message, sizeof(message), "\"", "");
		PrintToChatAll(message);
	}
}