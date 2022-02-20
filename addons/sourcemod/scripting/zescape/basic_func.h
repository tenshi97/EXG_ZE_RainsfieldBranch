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