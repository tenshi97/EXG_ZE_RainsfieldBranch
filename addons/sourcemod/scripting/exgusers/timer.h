Handle g_users_timer;
void TimerOnPluginStart()
{
    g_users_timer = CreateTimer(60.0,UserTimerHndl,_,TIMER_REPEAT);
}
Action UserTimerHndl(Handle timer)
{
    NomBanLogTimerCheck();
	return Plugin_Continue;
}

