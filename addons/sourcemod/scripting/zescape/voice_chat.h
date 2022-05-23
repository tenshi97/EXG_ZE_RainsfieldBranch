
void VoiceChatOnPluginStart()
{
	
	RegConsoleCmd("sm_vo",VoiceChatSettingCommand);
	RegConsoleCmd("autobuy",VoiceChatSettingCommand);
}

void VoiceChatOnClientConnected(int client)
{
	SetClientListeningFlags(client,VOICE_SPEAKALL);
	CreateTimer(30.0,MuteCheck,client,TIMER_FLAG_NO_MAPCHANGE);

}
Action MuteCheck(Handle timer,int client)
{
	if(SourceComms_GetClientMuteType(client))
	{
		PrintToChat(client," \x05[语音管理]\x01您当前处于被禁麦状态");
		SetClientListeningFlags(client,VOICE_MUTED);
	}
}
/*public void OnClientSpeakingEx(int client)
{

}*/

Action VoiceChatSettingCommand(int client,int args)
{
	if(SourceComms_GetClientMuteType(client))
	{
		PrintToChat(client," \x05[语音管理]\x01您当前处于被禁麦状态");	
		return Plugin_Handled;
	}
	if(GetClientListeningFlags(client) & VOICE_TEAM)
	{
	PrintToChat(client,"[语音频道] 现在你开麦说话所有人都可以听见.");
	SetClientListeningFlags(client,VOICE_SPEAKALL);
	return Plugin_Handled;
	}
	if(GetClientListeningFlags(client) & VOICE_SPEAKALL)
	{
	PrintToChat(client,"[语音频道] 现在你开麦说话只有队友可以听见.");
	SetClientListeningFlags(client,VOICE_TEAM);
	return Plugin_Handled;
	}
	PrintToChat(client,"[语音频道] 现在你开麦说话所有人都可以听见.");
	SetClientListeningFlags(client,VOICE_SPEAKALL);
	return Plugin_Handled;
}

