bool IsValidHuman(int client)
{
	if(IsClientInGame(client))
	{
		if(!IsFakeClient(client))
		{
			if(IsPlayerAlive(client))
			{
				if(ZR_IsClientHuman(client))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool IsValidHumanorBot(int client)
{
	if(IsClientInGame(client))
	{
		if(IsPlayerAlive(client))
		{
			if(ZR_IsClientHuman(client))
			{
				return true;
			}
		}
	}
	return false;
}

bool IsInSameTeam(int client1,int client2)
{
	if(GetClientTeam(client1)==GetClientTeam(client2))	return true;
	return false;
}
int Max(int a,int b)
{
	return a>=b?a:b;
}
int Min(int a,int b)
{
	return a<=b?a:b;
}
Action SetClientTeam(int client,int team)
{
	if(!IsClientInGame(client))	return;
	if (team == 1) FakeClientCommand(client, "jointeam spectator");
	if (team == 2) FakeClientCommand(client, "jointeam red");
	if (team == 3) FakeClientCommand(client, "jointeam blue");
}
bool DiffTimeTransform1(char buffer[PLATFORM_MAX_PATH],int timestamp1,int timestamp2,int total)
{
	int day,hour,minute;
	int diff_time;
	diff_time = total - (timestamp1 - timestamp2);
	if(diff_time <= 0)
	{
		return false;
	}
	else
	{
		day = diff_time / 86400;
		hour = (diff_time - day*86400)/ 3600;
		minute = (diff_time - day*86400 - hour *3600)/60;
		Format(buffer,sizeof(buffer),"冷却时间剩余:%d天%d小时%d分钟",day,hour,minute);
		return true;
	}
}

void KillTimerSafe(Handle& timer) {
	if (!timer) return;
	KillTimer(timer);
	timer = INVALID_HANDLE;
}

void CloseHandleSafe(Handle& hndl) {
	if (!hndl) return;
	hndl.Close();
	hndl = null;
}
