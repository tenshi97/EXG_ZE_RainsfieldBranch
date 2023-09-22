Handle g_users_timer;
void TimerOnPluginStart()
{
	g_users_timer = CreateTimer(60.0,UserTimerHndl,_,TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}
void TimerOnMapStart()
{
	g_users_timer = CreateTimer(60.0,UserTimerHndl,_,TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}
Action UserTimerHndl(Handle timer)
{
	NomBanLogTimerCheck();
	PftBanLogTimerCheck();
	return Plugin_Continue;
}

