void StorePlusOnPluginStart()
{
    RegAdminCmd("sm_gift",Command_Gift,ADMFLAG_GENERIC);
    RegConsoleCmd("sm_zz",Command_Pay);
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
Action Command_Pay(int client,int args)
{
    if(args!=1)
    {
            PrintToChat(client," \x05[积分转账]\x01参数错误!")
            return Plugin_Handled;
    }
    char arg1[64];
    GetCmdArg(1,arg1,sizeof(arg1));
    int pay_credits;
    int credits_sender;
    int credits_receiver;
    char cp_auth[65];
    pay_credits = StringToInt(arg1);
    if(pay_credits<=0)
    {
        PrintToChat(client," \x05[积分转账]\x01参数错误!")
        return Plugin_Handled;
    }
    credits_sender = Store_GetClientCredits(client);
    GetPartnerID(client,cp_auth,sizeof(cp_auth));
    if(pay_credits>credits_sender)
    {
        PrintToChat(client," \x05[积分转账]\x01你没有那么多积分，别想啦!");
        return Plugin_Handled;
    }
    for(int i=1;i<=64;i++)
    {
        if(!IsClientInGame(i))
        {
            continue;
        }
        if(IsFakeClient(i))
        {
            continue;
        }
        if(i==client)
        {
            continue;
        }
        char auth_id[65];
        GetClientAuthId(i, AuthId_Steam2, auth_id, sizeof(auth_id), true)
        if(strcmp(auth_id,cp_auth)==0)
        {
            char sender_name[64];
            char receiver_name[64];
            GetClientName(client,sender_name,sizeof(sender_name));
            GetClientName(i,receiver_name,sizeof(receiver_name));
            credits_receiver = Store_GetClientCredits(i);
            Store_SetClientCredits(client,credits_sender-pay_credits,"CP转账[给出]");
            Store_SetClientCredits(i,credits_receiver+pay_credits,"CP转账[接收]");
            PrintToChatAll(" \x05[积分转账]\x01 \x05%s \x01向TA的CP \x05%s \x01转账了\x07%d \x01积分，这其中恐怕存在什么交易!",sender_name,receiver_name,pay_credits);
            return Plugin_Handled;
        }
    }
    PrintToChat(client," \x05[积分转账]\x01找不到您的CP!可能您是一条单身狗，或您的CP不在线!")
    return Plugin_Handled;
}