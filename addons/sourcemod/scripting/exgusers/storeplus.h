int g_activating_hb_num = 0;
int g_activating_hb_money = 0;
int g_activating_hb_player = -1;
char g_activating_hb_question[256]="";
char g_activating_hb_answer[256]="";
Handle g_Timer_HB = INVALID_HANDLE;
int g_HB_Time = 20;
int g_player_hb_get[65]={0};
int StorePlus_PlayerDataProtect[65];
void StorePlusOnPluginStart()
{
    RegAdminCmd("sm_gift",Command_Gift,ADMFLAG_GENERIC);
    RegAdminCmd("sm_hb",Command_HB,ADMFLAG_GENERIC);
    RegConsoleCmd("sm_cpzz",Command_CPPay);
    RegConsoleCmd("sm_zz",Command_Pay);
    RegConsoleCmd("sm_qd",Command_QD);
    ResetHB();
//    RegConsoleCmd("sm_cj",Command_CJ);
//  RegConsoleCmd("sm_qd",Command_SignIn);
}
void StorePlusOnMapStart()
{
    ResetHB();
    for(int i=1;i<=64;i++)
    {
        StorePlus_PlayerDataProtect[i]=0;
    }
}
void StorePlusOnClientConnected(int client)
{
    StorePlus_PlayerDataProtect[client]=0;
}
bool CheckDataProtect(int client)
{
    if(GetTime()<StorePlus_PlayerDataProtect[client])
    {
        PrintToChat(client," \x05[转账系统]\x01触发数据保护无法转账，剩余\x07 %d\x01秒",StorePlus_PlayerDataProtect[client]-GetTime());
        return false;
    }
    return true;
}
void ResetHB()
{
    g_Timer_HB = INVALID_HANDLE;
    g_HB_Time = 0;
    g_activating_hb_num = 0;
    g_activating_hb_money = 0;
    g_activating_hb_player = -1;
    g_activating_hb_answer = "";
    g_activating_hb_question = "";
    for(int i=1;i<=64;i++)
    {
        g_player_hb_get[i]=0;
    }
    DeActivatingHB(1);

}
Action Command_Gift(int client,int args)
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

Action Command_HB(int client,int args)
{
    if(args!=4)
    {
        return Plugin_Handled;
    }
    char arg1[64];
    char arg2[64];
    char arg3[256];
    char arg4[256];
    GetCmdArg(1,arg1,sizeof(arg1));
    GetCmdArg(2,arg2,sizeof(arg2));
    GetCmdArg(3,arg3,sizeof(arg3));
    GetCmdArg(4,arg4,sizeof(arg4));
    int hb_num = StringToInt(arg1);
    int hb_credits = StringToInt(arg2);
    if(hb_num<=0||hb_credits<=0)
    {
        return Plugin_Handled;
    }
    int credits = Store_GetClientCredits(client);
    if(GetTime()<=Store_GetClientDataProtect(client))
    {
        return Plugin_Handled;
    }
    if(credits<hb_num*hb_credits)
    {
        PrintToChat(client," \x05[商店系统]\x01你的积分不够发放!");
        return Plugin_Handled;
    }
    if(g_activating_hb_player>=0)
    {
        return Plugin_Handled;
    }
    strcopy(g_activating_hb_question,256,arg3);
    strcopy(g_activating_hb_answer,256,arg4);
    g_activating_hb_player = client;
    g_activating_hb_num = hb_num;
    g_activating_hb_money = hb_credits;
    Store_SetClientCredits(client,credits-hb_num*hb_credits,"红包赠出");
    ActivatingHB();
    return Plugin_Handled;
}

Action g_Timer_HB_HNDL(Handle timern)
{
	char buffer[256];
	if(g_HB_Time>=0&&g_activating_hb_player>=0&&g_activating_hb_num>0)
	{
		Format(buffer,sizeof(buffer),"[红包]回答问题:%s\n剩余:%d秒 %d份",g_activating_hb_question,g_HB_Time,g_activating_hb_num);
		PrintCenterTextAll(buffer);
		g_HB_Time--;
		return Plugin_Handled;
    }
    if(g_activating_hb_player>=0)
    {
        DeActivatingHB();
    }
	return Plugin_Stop;
}

void ActivatingHB()
{
    char hb_name[64];
    GetClientName(g_activating_hb_player,hb_name,sizeof(hb_name));
    PrintToChatAll(" \x05[红包系统]管理员\x07%s\x01发放了一个 \x09%d \x01份 \x09%d \x01积分的红包",hb_name,g_activating_hb_num,g_activating_hb_money);
    PrintToChatAll(" \x05[红包系统]管理员\x07%s\x01发放了一个 \x09%d \x01份 \x09%d \x01积分的红包",hb_name,g_activating_hb_num,g_activating_hb_money);
    PrintToChatAll(" \x05[红包系统]管理员\x07%s\x01发放了一个 \x09%d \x01份 \x09%d \x01积分的红包",hb_name,g_activating_hb_num,g_activating_hb_money);
    PrintToChatAll(" \x05[红包系统]\x01红包问题是\x07%s,\x01请在20秒内作答领取红包!",g_activating_hb_question);
    PrintToChatAll(" \x05[红包系统]\x01红包问题是\x07%s,\x01请在20秒内作答领取红包!",g_activating_hb_question);
    PrintToChatAll(" \x05[红包系统]\x01红包问题是\x07%s,\x01请在20秒内作答领取红包!",g_activating_hb_question);
    g_HB_Time = 20;
    delete g_Timer_HB;
    g_Timer_HB = CreateTimer(1.0,g_Timer_HB_HNDL,_,TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

bool SayJudgeOnHB(int client, const char[] command, const char[] sArgs)
{
    char ans_name[64];
    if(client>0&&IsClientInGame(client))
    {
        GetClientName(client,ans_name,sizeof(ans_name));
    }
    if(g_activating_hb_player<0||g_activating_hb_num<=0)
    {
        return false;
    }
    if(g_player_hb_get[client]!=0)
    {
        if(strcmp(g_activating_hb_answer,sArgs,false)==0)
        {
            PrintToChat(client, " \x05[红包系统]\x01您已经领取过，不要贪心哦!");
            return true;
        }
        return false;
    }
    if(GetTime()<=Store_GetClientDataProtect(client))
    {
        PrintToChat(client," \x05[红包系统]\x01数据保护中无法领取");
        return false;
    }
    int credits = Store_GetClientCredits(client);
    if(strcmp(g_activating_hb_answer,sArgs,false)==0)
    {
        PrintToChatAll(" \x05[红包系统]\x01玩家\x07%s\x01答题正确领取了红包!",ans_name);
        Store_SetClientCredits(client,credits+g_activating_hb_money,"红包获得");
        g_activating_hb_num-=1;
        g_player_hb_get[client]=1;
        if(g_activating_hb_num<=0)
        {
            DeActivatingHB();
        }
        return true;
    }
    return false;
}

void DeActivatingHB(int init=0)
{
    for(int i=1;i<=64;i++)
    {
        g_player_hb_get[i]=0;
    }
    g_activating_hb_money = 0;
    g_activating_hb_num = 0;
    delete g_Timer_HB;
    g_activating_hb_player = -1;
    if(init)    return;
    PrintToChatAll(" \x05[红包系统]\x01由于时间耗尽或红包已领完，红包环节结束");
    PrintToChatAll(" \x05[红包系统]\x01由于时间耗尽或红包已领完，红包环节结束");
    PrintToChatAll(" \x05[红包系统]\x01问题的答案是:\x07 %s",g_activating_hb_answer);
    PrintToChatAll(" \x05[红包系统]\x01问题的答案是:\x07 %s",g_activating_hb_answer);
}

Action Command_CPPay(int client,int args)
{
	if(GetTime()<=Store_GetClientDataProtect(client))
	{
		PrintToChat(client," \x05[赛季活动]\x01数据保护中，无法操作!");
        return Plugin_Handled;
	}
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
    if(!CheckDataProtect(client))
    {
        return Plugin_Handled;
    }
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
            char reason[256];
            GetClientName(client,sender_name,sizeof(sender_name));
            GetClientName(i,receiver_name,sizeof(receiver_name));
            credits_receiver = Store_GetClientCredits(i);
            int sender_uid = EXGUSERS_GetUserUID(client);
            int receiver_uid = EXGUSERS_GetUserUID(i);
            Format(reason,sizeof(reason),"CP转账[给出][%d-->%d]",sender_uid,receiver_uid);
            Store_SetClientCredits(client,credits_sender-pay_credits,reason);
            Format(reason,sizeof(reason),"CP转账[接收][%d-->%d]",sender_uid,receiver_uid);
            Store_SetClientCredits(i,credits_receiver+pay_credits,reason);
            PrintToChatAll(" \x05[积分转账]\x01 \x05%s \x01向TA的CP \x05%s \x01转账了\x07%d \x01积分，这其中恐怕存在什么交易!",sender_name,receiver_name,pay_credits);
            StorePlus_PlayerDataProtect[client] = GetTime()+60;
            return Plugin_Handled;
        }
    }
    PrintToChat(client," \x05[积分转账]\x01找不到您的CP!可能您是一条单身狗，或您的CP不在线!")
    return Plugin_Handled;
}

Action Command_Pay(int client,int args)
{
	if(GetTime()<=Store_GetClientDataProtect(client))
	{
		PrintToChat(client," \x05[积分转账]\x01数据保护中，无法操作!");
		return Plugin_Handled;
	}
    if(args!=2)
    {
        PrintToChat(client," \x05[积分转账]\x01参数错误，请输入!zz 用户名 转账数额")
        return Plugin_Handled;
    }
    if(!IsClientVIP(client))
    {
		PrintToChat(client," \x05[积分转账]\x01VIP才能使用普通转账!");
		return 0;
    }
    char arg1[64];
    char arg2[64];
    GetCmdArg(1,arg1,sizeof(arg1));
    GetCmdArg(2,arg2,sizeof(arg2));
    int pay_credits;
    int receiver;
    int credits_sender;
    int credits_receiver;
    int tax_rate = 0.1;
    pay_credits = StringToInt(arg2);
    if(pay_credits<50)
    {
        PrintToChat(client," \x05[积分转账]\x01参数错误，一次转账必须不少于50积分!")
        return Plugin_Handled;
    }
    credits_sender = Store_GetClientCredits(client);
    receiver = FindTarget(client,arg1,true,false);
    if(receiver <= 0||receiver==client)
    {
        PrintToChat(client," \x05[积分转账]\x01目标错误");
        return Plugin_Handled;
    }
    credits_receiver = Store_GetClientCredits(client);
    if(!CheckDataProtect(client))
    {
        return Plugin_Handled;
    }
    if(pay_credits>credits_sender)
    {
        PrintToChat(client," \x05[积分转账]\x01你没有那么多积分，别想啦!");
        return Plugin_Handled;
    }
    if(pay_credits+g_Users_sp[client].pay_daily>10000)
    {
        PrintToChat(client," \x05[积分转账]\x01你的每日转账上限不足以满足此次积分转账!");
        return Plugin_Handled;
    }
    char sender_name[64];
    char receiver_name[64];
    char reason[256];
    GetClientName(client,sender_name,sizeof(sender_name));
    GetClientName(receiver,receiver_name,sizeof(receiver_name));
    credits_receiver = Store_GetClientCredits(receiver);
    int sender_uid = EXGUSERS_GetUserUID(client);
    int receiver_uid = EXGUSERS_GetUserUID(receiver);
    Format(reason,sizeof(reason),"普通转账[给出][%d-->%d]",sender_uid,receiver_uid);
    Store_SetClientCredits(client,credits_sender-pay_credits,reason);
    Format(reason,sizeof(reason),"普通转账[接受][%d-->%d]",sender_uid,receiver_uid);
    Store_SetClientCredits(receiver,credits_receiver+RoundToFloor(pay_credits*0.9),reason);
    PrintToChatAll(" \x05[积分转账]\x01 \x05%s \x01向 \x05%s \x01转账了\x07%d \x01积分，这其中恐怕存在什么交易!",sender_name,receiver_name,RoundToFloor(pay_credits*0.9));
    g_Users_sp[client].pay_daily+=pay_credits;
    StorePlus_PlayerDataProtect[client] = GetTime()+60;
    UpdateUserInfo(client);
    return Plugin_Handled;
}

Action Command_QD(int client,int args)
{
	if(GetTime()<=Store_GetClientDataProtect(client))
	{
		PrintToChat(client," \x05[签到系统]\x01数据保护中，无法操作!");
		return 0;
	}
    if(g_Users_sp[client].loaded==0)
    {
		PrintToChat(client," \x05[签到系统]\x01用户数据未载入，无法操作!");
		return 0;
    }
    int credits = Store_GetClientCredits(client);
    int credits_bonus = 0;
    if(g_Users_sp[client].qd==0)
    {
        g_Users_sp[client].qd++;
        g_Users_sp[client].qd_start =   GetTime()-GetTime()%86400;
        g_Users_sp[client].qd_time =    g_Users_sp[client].qd_start;
        credits_bonus = 20;
        PrintToChat(client, "\x05[签到系统]\x01 完成签到获得20积分(若您为VIP则为双倍)!");
    }
    else
    {
        if(GetTime()-g_Users_sp[client].qd_start>=7*86400)
        {
            PrintToChat(client, "\x05[签到系统]\x01 距离上一轮签到已经过去七天了，进入新一轮签到轮次");
            g_Users_sp[client].qd = 1;
            g_Users_sp[client].qd_start = GetTime()-GetTime()%86400;
            g_Users_sp[client].qd_time = GetTime()-GetTime()%86400;
            credits_bonus = 20;
        }
        else
        {
            if(GetTime()-g_Users_sp[client].qd_time>=86400)
            {
                g_Users_sp[client].qd++;
                g_Users_sp[client].qd_time = GetTime()-GetTime()%86400;
                switch(g_Users_sp[client].qd)
                {
                    case 1: credits_bonus = 20;
                    case 2: credits_bonus = 40;
                    case 3: credits_bonus = 60;
                    case 4: credits_bonus = 80;
                    case 5: credits_bonus = 100;
                    case 6: credits_bonus = 150;
                    case 7: credits_bonus = 200;
                    default:
                    {
                        g_Users_sp[client].qd = 1;
                        credits_bonus = 20;
                    }
                }
            }
        }
        if(IsClientVIP(client))
        {
            if(credits_bonus>=100)  credits_bonus+=100;
            else    credits_bonus *=2;
        }
        if(credits_bonus>0)
        {
            PrintToChat(client, "\x05[签到系统]\x01 完成本轮连续%d天签到获得%d积分!",g_Users_sp[client].qd,credits_bonus);
            Store_SetClientCredits(client,credits+credits_bonus,"签到");
        }
        else
        {
            int tdelta = g_Users_sp[client].qd_time+86400-GetTime();
            PrintToChat(client," \x05[签到系统]\x01您已经签到过了，距离你下一次可以签到还有%d小时%d分",tdelta/3600,(tdelta%3600)/60)
        }
    }
    UpdateUserInfo(client);
}
