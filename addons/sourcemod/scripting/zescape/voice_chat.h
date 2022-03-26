
void VoiceChatOnPluginStart()
{
	
	RegConsoleCmd("sm_vo",VoiceChatSettingCommand);
	RegConsoleCmd("autobuy",VoiceChatSettingCommand);
}

void VoiceChatOnClientConnected(int client)
{
	SetClientListeningFlags(client,VOICE_TEAM);

}
/*public void OnClientSpeakingEx(int client)
{

}*/
Action VoiceChatSettingCommand(int client,int args)
{
	if(GetClientListeningFlags(client) && VOICE_TEAM)
	{
	PrintToChat(client,"[语音频道]现在你开麦说话所有人都可以听见.");
	SetClientListeningFlags(client,VOICE_SPEAKALL);
	}
	if(GetClientListeningFlags(client) && VOICE_SPEAKALL)
	{
	PrintToChat(client,"[语音频道]现在你开麦说话只有队友可以听见.");
	SetClientListeningFlags(client,VOICE_TEAM);
	}
	
	return Plugin_Handled;
}
