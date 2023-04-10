void StorePlusOnPluginStart()
{
    RegAdminCmd("sm_gift",Command_Gift,ADMFLAG_GENERIC);
}
Action Command_Gift(int client,int args)
{
    if(IsClientVIP(client))
    {
        if(args!=2)
        {
            return Plugin_Handled;
        }
        char arg1[256];
        char arg2[64];
        char target_name[256]
        int target_list[64];
        bool tn_is_ml;
        int target_count = 0;
        GetCmdArg(1,arg1,sizeof(arg1));
        GetCmdArg(2,arg2,sizeof(arg2));
        int credits_give = StringToInt(arg2);
        if(credits_give<=0)
        {
            return Plugin_Handled;
        }
        target_count = ProcessTargetString(arg1,0,target_list,64,COMMAND_FILTER_CONNECTED,target_name,sizeof(target_name),tn_is_ml);
        if(target_count<=0)
        {
            return Plugin_Handled;
        }
        int credits = Store_GetClientCredits(client);
        if(!tn_is_ml&&target_count>1)
        {
            PrintToChat(client," \x05[商店系统]\x01存在多个ID包含关键字的用户!");
            return Plugin_Handled;
        }
        if(credits<target_count*credits_give)
        {
            PrintToChat(client," \x05[商店系统]\x01你的积分不够发放!");           
            return Plugin_Handled;
        }
        int receiver_credits;
        char sender_name[64];
        GetClientName(client,sender_name,64);
        char receiver_name[64];
        for(int i=0;i<target_count;i++)
        {
            receiver_credits = Store_GetClientCredits(target_list[i]);
            GetClientName(target_list[i],receiver_name,64);
            Store_SetClientCredits(target_list[i],receiver_credits+credits_give,"赠送");
            PrintToChatAll(" \x05[商店系统]\x01玩家\x07%s\x01赠送给\x07%s %d\x01积分",sender_name,receiver_name,credits_give);
        }
        Store_SetClientCredits(client,credits-target_count*credits_give,"赠出");
        return Plugin_Handled;
    }
    return Plugin_Handled;
}