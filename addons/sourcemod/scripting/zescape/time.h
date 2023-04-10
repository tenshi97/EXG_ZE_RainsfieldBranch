Handle g_DailyTimer_Check;
Handle g_Daily_Timercheck_Update;
int g_daily_timestamp;
int g_weekly_timestamp;
void TimeOnPluginStart()
{
	if(g_Daily_Timercheck_Update == null)
	{
		g_Daily_Timercheck_Update = CreateGlobalForward("RY_Daily_TimerCheck_Update",ET_Ignore,Param_Cell,Param_Cell,Param_Cell);
	}
	if(g_DailyTimer_Check == null)
	{
		g_DailyTimer_Check = CreateTimer(60.0,DailyTimerCheckerHndl,_,TIMER_REPEAT);
	}
}
Action DailyTimerCheckerHndl(Handle timer)
{
	char query[512];
	if(!isDbConnected())	return Plugin_Handled;
	Format(query,sizeof(query),"SELECT * FROM ZETIMER");
	DbTQuery(DailyTimerCheckCallBack,query);
	return Plugin_Handled;
}

void DailyTimerCheckCallBack(Handle owner, Handle hndl, char[] error, any data)
{
	MissionTimeCheck();
	int daily_timestamp;
	char record_name[64];
	bool daily_timestamp_found = false;
	while(SQL_FetchRow(hndl))
	{
		DbFetchString(hndl, "NAME", record_name,sizeof(record_name));
		if(strcmp(record_name,"Daily_Activity_Timer")==0)
		{
			daily_timestamp = DbFetchInt(hndl,"TIMESTAMP");
			g_daily_timestamp = daily_timestamp;
			daily_timestamp_found = true;
		}
	}
	if(daily_timestamp_found)
	{
		//PrintToChatAll(" \x05[调试：每日正午时刻检测]\x01上一次检测设定时间戳:%d",daily_timestamp);
		daily_timercheck(daily_timestamp);
	}
	delete hndl;
}
void daily_timercheck(int start_time)
{
	int current_time = GetTime();
	int days_delta;
	int new_timestamp;
	if(current_time - start_time >=86400)
	{
		days_delta = (current_time - start_time)/86400;
		new_timestamp = start_time + days_delta * 86400;
		g_daily_timestamp = new_timestamp;
		//PrintToChatAll(" \x05[调试：新活动日触发]\x01上一次检测设定时间戳:%d",start_time);
		//PrintToChatAll(" \x05[调试：新活动日触发]\x01经过日数:%d",days_delta);
		//PrintToChatAll(" \x05[调试：新活动日触发]\x01新活动日时间戳:%d",new_timestamp);
		Call_StartForward(g_Daily_Timercheck_Update);
		Call_PushCell(start_time);
		Call_PushCell(new_timestamp);
		Call_PushCell(days_delta);
		Call_Finish();
		TimerRecord_Update("Daily_Activity_Timer",new_timestamp);
		QuestOnDailyUpdate();
	}
}
void TimerRecord_Update(char[] record_name,int timestamp,int repeat=1,int period=0,int begin=0,int finish=0)
{
	char query[512];
	Format(query,sizeof(query),"UPDATE zetimer SET TIMESTAMP = %d, REPEATS = %d, PERIOD = %d, BEGINTIME = %d, ENDTIME = %d WHERE NAME = '%s' ",timestamp,repeat,period,begin,finish,record_name);
	DbTQuery(DbQueryErrorCallback,query);
}