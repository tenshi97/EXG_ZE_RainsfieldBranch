enum ZEITEMS
{
    NOM_HALFCOST = 0,
    EXP_BUFF = 1,
    MORE_PASS_CREDITS = 2,
    MORE_ONLINE_CREDITS = 3,
    AWARD_CREDITS = 4,
    AWARD_LEVELS = 5
};
char ZEITEMS_NAME[][] =
{
    "订图半价券",
    "赛季经验加成月卡",
    "通关奖励加成月卡",
    "在线积分加成月卡",
    "奖励积分",
    "奖励赛季等级"
};
enum struct ZEITEMS_STATUS
{
    int uid = 0;
    int items[6];
    int items_expire[6];
    int loaded = 0;
};
ZEITEMS_STATUS player_zeitems_inventory[65];
const int zeitems_count = 4;
void ItemsOnPluginStart()
{
    RegConsoleCmd("sm_item",Command_ZEItems);
}
Action Command_ZEItems(int client,int args)
{
    if(!IsClientInGame(client)) return Plugin_Handled;
    ZEItems_MenuBuild(client);
    return Plugin_Handled;
}
void ZEItems_LoadPlayerInventory(int client)
{
    char query[512];
    player_zeitems_inventory[client].loaded = 0;
    int uid = EXGUSERS_GetUserUID(client);
    if(uid!=-1||uid!=0)
    {
        if(!IsDbConnected())
        {
            player_zeitems_inventory[client].loaded = 0;
            return;
        }
        Format(query,sizeof(query),"SELECT * FROM EXGUSERS_ZE WHERE UID = %d");
        DbTQuery();
    }

}
void ZEItems_LoadPlayerInventory_Callback(Handle owner, Handle hndl, char[] error, any data)
{

}
void ZEItems_MenuBuild(int client)
{
    char buffer[256];
    menu.SetTitle("[僵尸逃跑]道具仓库");
    for(int i=0;i<zeitems_count;i++)
    {
        Foramt(buffer,sizeof(buffer),"%s:%d",ZEITEMS_NAME[i]);
        menu.AddItem("",buffer,ITEMDRAW_DISABLED);
    }
}