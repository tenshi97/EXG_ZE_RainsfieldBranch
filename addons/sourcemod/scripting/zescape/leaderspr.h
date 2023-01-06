int leader_spr_entity;
bool leader_spr_created;

void LeaderSpriteOnPluginStart()
{
    leader_spr_created = false;
    PrecacheGeneric("particles/exg_commander_p3.pcf",true); //Precache Any pcf(particle file)
}
void LeaderSpriteOnMapStart()
{
    PrecacheGeneric("particles/exg_commander_p3.pcf",true); //Precache Any pcf(particle file)
}
void LeaderSpriteOnRoundStart()
{
    leader_spr_created = false;
}
Action LeaderSpriteTest(int client,int args)
{
    if(!IsClientInGame(client)) return Plugin_Handled;
    if(IsFakeClient(client))    return Plugin_Handled;
    if(client<=0||client>=65)   return Plugin_Handled;
    if(leader_spr_created)
    {
        UTIL.RemoveEntity(leader_spr_entity,0.0);
        leader_spr_created = false;
        return Plugin_Handled;
    }
    float leader_pos[3]
    GetClientAbsOrigin(client,leader_pos);
    leader_pos[2]+=128;
    leader_spr_created = true;
    leader_spr_entity = UTIL.CreateParticle(client,leader_pos,_,_,"leader_icon",0.0);
    return Plugin_Handled;
}

void CreateLeaderIcon(int leader)
{
    if(leader_spr_created)
    {
        leader_spr_created = false;
        if(!IsValidEntity(leader_spr_entity))   return;
        AcceptEntityInput(leader_spr_entity,"DestroyImmediately");
        UTIL.RemoveEntity(leader_spr_entity,0.0);
    }
    float leader_pos[3];
    GetClientAbsOrigin(leader,leader_pos);
    leader_pos[2]+=100;
    leader_spr_entity = UTIL.CreateParticle(leader,leader_pos,_,_,"leader_icon",0.0);
    leader_spr_created = true;
}
void HideLeaderIcon()
{
    if(!leader_spr_created||!IsValidEntity(leader_spr_entity))
    {
        leader_spr_created = false;
        return;
    }
    AcceptEntityInput(leader_spr_entity,"DestroyImmediately");
    UTIL.RemoveEntity(leader_spr_entity,0.0);
    leader_spr_created = false;
}

public void Leader_OnLeaderSet(int leader)
{
    CreateLeaderIcon(leader);
}
public void Leader_OnLeaderRemoved(int leader)
{
    HideLeaderIcon();
}
public int Native_ToggleSprite(Handle plugin,int numParams)
{
    int leader = GetNativeCell(1);
    if(leader_spr_created)
    {
        HideLeaderIcon();
    }
    else
    {
        CreateLeaderIcon(leader);
    }
}