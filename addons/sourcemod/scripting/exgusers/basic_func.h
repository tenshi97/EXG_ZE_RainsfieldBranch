const int g_const_midnight_timestamp=1654185600;
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
float fMax(float a,float b)
{
	return a>=b?a:b;
}
float fMin(float a,float b)
{
	return a<=b?a:b;
}
int IsClientVIP(int client)
{
	if(client<=0||client>=65)	return -1;
	if(!IsClientInGame(client))	return -1;
	if(IsFakeClient(client))	return -1;
	if (GetUserFlagBits(client) && ADMFLAG_RESERVATION)
	{
		return 1;
	}
	return 0;
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