int g_client_nametag_inventory_group1[65];
int g_client_nametag_equip[65];
int dataprotect_nametag[65];
enum struct Nametag_Group
{
    int nt_id;
    int level;
    int buyable;
    int price;
    int title_serie[10];
    int group;
    int uid;
    char name[64];
}
char Nametag_String[][]=
{
    "[LVlvl]开电解冰", //0
    "[LVlvl]尸群狂风", //1
    "[LVlvl]断口加速",//2
    " \x05[LVlvl]黑洞大飞", //3
    " \x05[LVlvl]反身盖土", //4
    " \x07[LVlvl]跳刀终极",  //5 
    "[LVlvl]先锋",   //6
    "[LVlvl]卫士",    //7
    "[LVlvl]中军",    //8
    "[LVlvl]统帅",    //9
    "[LVlvl]传奇",    //10
    " \x05[LVlvl]万古流芳", //11
    " \x06[LVlvl]超凡入圣", //12
    " \x07[LVlvl]冠绝一世", //13
    "[LVlvl]坚韧黑铁",   //14
    "[LVlvl]英勇黄铜",   //15
    "[LVlvl]不屈白银",   //16
    "[LVlvl]荣耀黄金",   //17
    "[LVlvl]华贵铂金",  //18
    "[LVlvl]璀璨钻石",  //19
    " \x05[LVlvl]超凡大师",   //20
    " \x06[LVlvl]傲世宗师",//21
    " \x07[LVlvl]最强王者",//22
    " \x08神壕",             //23
    " \x10天人合一",         //24
    " \x06服主",        //25
    " \x08狗管理",      //26
    " \x0A自\x0B由\x0C之\x0D雨"     //27
}
Nametag_Group g_Nametags[10];
const g_Nametags_Num = 8;
int title_nums_g1 =1;
void NameTagOnPluginStart()
{
    RegConsoleCmd("sm_nametag",NameTagMenu);
    RegConsoleCmd("sm_tx",NameTagMenu);
    Nametag_TestInitialize();
}
void Nametag_TestInitialize()
{
    g_Nametags[0].nt_id = 0;
    g_Nametags[0].level = 5;
    g_Nametags[0].price = 0;
    g_Nametags[0].buyable = 0;
    g_Nametags[0].group = 0;
    g_Nametags[0].title_serie[0] = 0;
    g_Nametags[0].title_serie[1] = 1;
    g_Nametags[0].title_serie[2] = 2;
    g_Nametags[0].title_serie[3] = 3;
    g_Nametags[0].title_serie[4] = 4;
    g_Nametags[0].title_serie[5] = 5;
    g_Nametags[0].uid = 0;
    g_Nametags[0].name = "EXG 默认";
    g_Nametags[1].nt_id = 1;
    g_Nametags[1].level = 7;
    g_Nametags[1].price = 5000;
    g_Nametags[1].buyable = 1;
    g_Nametags[1].title_serie[0] = 6;
    g_Nametags[1].title_serie[1] = 7;
    g_Nametags[1].title_serie[2] = 8;
    g_Nametags[1].title_serie[3] = 9;
    g_Nametags[1].title_serie[4] = 10;   
    g_Nametags[1].title_serie[5] = 11;   
    g_Nametags[1].title_serie[6] = 12;   
    g_Nametags[1].title_serie[7] = 13;   
    g_Nametags[1].uid = 0;
    g_Nametags[1].name = "DOTA2 段位"
    g_Nametags[2].nt_id = 1;
    g_Nametags[2].level = 8;
    g_Nametags[2].price = 5000;
    g_Nametags[2].buyable = 1;
    g_Nametags[2].title_serie[0] = 14;
    g_Nametags[2].title_serie[1] = 15;
    g_Nametags[2].title_serie[2] = 16;
    g_Nametags[2].title_serie[3] = 17;
    g_Nametags[2].title_serie[4] = 18;   
    g_Nametags[2].title_serie[5] = 19;   
    g_Nametags[2].title_serie[6] = 20;   
    g_Nametags[2].title_serie[7] = 21;   
    g_Nametags[2].title_serie[8] = 22;   
    g_Nametags[2].uid = 0;
    g_Nametags[2].name = "LOL 段位"
    g_Nametags[3].nt_id = 3;
    g_Nametags[3].level = 0;
    g_Nametags[3].price = 114514;
    g_Nametags[3].buyable = 1;
    g_Nametags[3].title_serie[0] = 23;
    g_Nametags[3].uid = 0;
    g_Nametags[3].name = "神壕";
    g_Nametags[4].nt_id = 4;
    g_Nametags[4].level = 0;
    g_Nametags[4].price = 0;
    g_Nametags[4].buyable = 0;
    g_Nametags[4].group = 44474787;
    g_Nametags[4].title_serie[0] = 24;
    g_Nametags[4].uid = 0;
    g_Nametags[4].name = "EXG PRO";
    g_Nametags[5].nt_id = 5;
    g_Nametags[5].level = 0;
    g_Nametags[5].price = 0;
    g_Nametags[5].buyable = 0;
    g_Nametags[5].uid = -1;
    g_Nametags[5].title_serie[0] = 25;
    g_Nametags[5].name = "服主"
    g_Nametags[6].nt_id = 56;
    g_Nametags[6].level = 0;
    g_Nametags[6].price = 0;
    g_Nametags[6].buyable = 0;
    g_Nametags[6].uid = -2;
    g_Nametags[6].title_serie[0] = 26;
    g_Nametags[6].name = "管理员"
    g_Nametags[7].nt_id = 5;
    g_Nametags[7].level = 0;
    g_Nametags[7].price = 0;
    g_Nametags[7].buyable = 0;
    g_Nametags[7].uid = 3;
    g_Nametags[7].title_serie[0] = 27;
    g_Nametags[7].name = "Rainsfield"
}
void NameTag_OnClientConnected(int client)
{
    g_client_nametag_inventory_group1[client] = 1;
    g_client_nametag_equip[client] = 0;
    dataprotect_nametag[client] = 0;
    int uid = g_Users[client].uid;
    char query[512];
    Format(query,sizeof(query),"SELECT * FROM exgusers WHERE UID = %d",uid);
    Handle result = SQL_Query(Db_Connection,query);
    if(SQL_GetRowCount(result)==1)
	{
		SQL_FetchRow(result);
		g_client_nametag_inventory_group1[client] = SQL_FetchInt(result,DbField(result,"TAG1"));
        g_client_nametag_equip[client] = SQL_FetchInt(result,DbField(result,"TAGE"));
	}
    else
    {
        g_client_nametag_inventory_group1[client] = 1;
        g_client_nametag_equip[client] = 0;
    }
    Nametag_CheckGroup(client);
    if(g_client_nametag_inventory_group1[client]&(1<<g_client_nametag_equip[client])==0)    //Check if client doesn't have the equipped nametag
    {
        g_client_nametag_equip[client] = 0;
    }
    UpdatePlayerNameTagInventory(client);
}
void Nametag_CheckGroup(int client)
{
    for(int i=0;i<g_Nametags_Num;i++)
    {
        if(g_Nametags[i].group!=0)
        {
            if(IsPlayerInUserGroup(client,g_Nametags[i].group))
            {
                g_client_nametag_inventory_group1[client]|=(1<<(i));
            }
            else
            {
                if(g_client_nametag_inventory_group1[client]&(1<<(i))!=0)
                {
                    g_client_nametag_inventory_group1[client]^=(1<<(i));  
                }     
            }
        }
        if(g_Nametags[i].uid!=0)
        {
            if(g_Nametags[i].uid==-1)
            {
                if(g_Users[client].uid==1||g_Users[client].uid==2)
                {
                    g_client_nametag_inventory_group1[client]|=(1<<(i));                   
                }
            }
            if(g_Nametags[i].uid==-2)
            {
                if(GetAdminFlag(GetUserAdmin(client),Admin_Generic))
                {
                    g_client_nametag_inventory_group1[client]|=(1<<(i));                    
                }
            }
            if(g_Nametags[i].uid>0)
            {
                if(g_Users[client].uid==g_Nametags[i].uid)
                {
                    g_client_nametag_inventory_group1[client]|=(1<<(i));                    
                }
            }
        }
    }
}
Action NameTagMenu(int client,int args)
{
    if(!IsClientInGame(client))
    {
        return Plugin_Handled;
    }
    if(client<=0||client>64)
    {0
        return Plugin_Handled;
    }
    NameTagMenu_Build(client);
    return Plugin_Handled;
}
void NameTagMenu_Build(int client)
{
    Menu menu = CreateMenu(NameTagMainMenuHandler);
    menu.SetTitle("聊天头衔");
    char buffer[256];
    for(int i=0;i<g_Nametags_Num;i++)
    {
        Format(buffer,sizeof(buffer),"%s%s",g_Nametags[i].name,g_client_nametag_equip[client]==i?"[佩戴中]":g_client_nametag_inventory_group1[client]&(1<<i)?"[已拥有]":"");
        menu.AddItem("",buffer);
    }
    menu.Display(client,MENU_TIME_FOREVER);

}
int NameTagMainMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
        NameTag_ItemMenuBuild(client,param);
	}
	return 0;
}
void NameTag_ItemMenuBuild(int client,int id)
{
    char buffer[256];
    char id_str[64];
    IntToString(id,id_str,sizeof(id_str));
    Menu menu = CreateMenu(NameTagItemMenuHandler);
    Format(buffer,sizeof(buffer),"%s",g_Nametags[id].name);
    menu.SetTitle("称号系统");
    menu.AddItem(id_str,buffer,ITEMDRAW_DISABLED);
    Format(buffer,sizeof(buffer),"价格:%d积分[%s]",g_Nametags[id].price,g_Nametags[id].buyable?"可购买":"不可购买");
    menu.AddItem(id_str,buffer,ITEMDRAW_DISABLED);
    menu.ExitBackButton = true;
    bool owned = false;
    bool equipped = false;
    if(g_client_nametag_inventory_group1[client]&(1<<id)!=0)
    {
        owned = true;
    }
    if(g_client_nametag_equip[client]==id)
    {
        equipped = true;
    }
    if(owned)
    {
        Format(buffer,sizeof(buffer),"[已拥有]");
    }
    else
    {
        if(g_Nametags[id].buyable)
        {
            Format(buffer,sizeof(buffer),"购买[%d积分]",g_Nametags[id].price);
        }   
        else
        {
            Format(buffer,sizeof(buffer),"不可购买");
        } 
    }
    menu.AddItem(id_str,buffer,(g_Nametags[id].buyable==0||owned)?ITEMDRAW_DISABLED:ITEMDRAW_DEFAULT);
    if(owned)
    {
        if(equipped)
        {
            Format(buffer,sizeof(buffer),"卸下");
        }
        else
        {
            Format(buffer,sizeof(buffer),"佩戴");
        }
    }
    else
    {
        Format(buffer,sizeof(buffer),"尚未拥有");
    }
    menu.AddItem(id_str,buffer,owned?ITEMDRAW_DEFAULT:ITEMDRAW_DISABLED);
    menu.Display(client,MENU_TIME_FOREVER);
}

int NameTagItemMenuHandler(Menu menu, MenuAction action, int client, int param)
{
	if (action == MenuAction_End||client<=0||client>=65)
	{
		delete menu;
		return 0;
	}
	else if(action == MenuAction_Select)
	{
        char id_str[64];
        int id;
        menu.GetItem(0,id_str,sizeof(id_str));
        id = StringToInt(id_str);
        if(param == 2)
        {
            if(GetTime()<dataprotect_nametag[client])
            {
                PrintToChat(client," \x05[商店系统]\x01积分处于数据保护中，剩余\x07%d\x01秒",dataprotect_nametag[client]-GetTime());
            }
            if(Store_GetClientCredits(client)<g_Nametags[id].price)
            {
                PrintToChat(client," \x05[商店系统]\x01积分不足!")
                return 0;
            }
            Store_SetClientCredits(client,Store_GetClientCredits(client)-g_Nametags[id].price,"购买头衔");
            g_client_nametag_inventory_group1[client]|=(1<<id);
            NameTag_ItemMenuBuild(client,id);
        }
        if(param == 3)
        {
            if(g_client_nametag_equip[client]==id)
            {
                g_client_nametag_equip[client]=0;            
            }
            else
            {
                g_client_nametag_equip[client]=id;
            }
            NameTag_ItemMenuBuild(client,id);
        }
        UpdatePlayerNameTagInventory(client);
	}
	else if (param == MenuCancel_ExitBack)
	{
        NameTagMenu_Build(client);
	}
	return 0;
}

void UpdatePlayerNameTagInventory(int client)
{
    char buffer[512];
    Format(buffer,sizeof(buffer),"UPDATE exgusers SET TAGE = %d, TAG1 = %d WHERE UID = %d",g_client_nametag_equip[client],g_client_nametag_inventory_group1[client],g_Users[client].uid);
    DbTQuery(DbQueryErrorCallback,buffer);

}
void NameTag_GetUserNameTag(int client,char[] NameTagBuffer)
{
    if(g_Users[client].loaded==0)
    {
        return;
    }
    int tag = g_client_nametag_equip[client];
    int serie_num,lvl;
    lvl = Hero_GetLevel(client);
    if(g_Nametags[tag].level>=1)
    {
        serie_num = Min(g_Nametags[tag].level,(lvl+1)/1000);
    }
    else
    {
        serie_num = 0;
    }
    char nametag[64];
    char level_str[64];
    IntToString(lvl,level_str,sizeof(level_str));
    strcopy(nametag,sizeof(nametag),Nametag_String[g_Nametags[tag].title_serie[serie_num]]);
    ReplaceString(nametag,sizeof(nametag),"lvl",level_str,true);
    Format(NameTagBuffer,64,"%s",nametag);
    return;    
}
public int Native_GetUserNameTag(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);
    char nametag_native[64];
    NameTag_GetUserNameTag(client,nametag_native);
    SetNativeString(2,nametag_native,64);
    return 0;
}