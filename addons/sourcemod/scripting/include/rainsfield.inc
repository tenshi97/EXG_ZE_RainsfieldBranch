#if defined _rainsfield_included_
    #endinput
#endif
#define _rainsfield_included_
enum struct ItemWeapon_Log
{
    char name[64];
    int hammerid;
    char classname[64];
}
enum struct WeaponPurchase_Log
{
    char name[64];
    int price;
    int restrict;
    int maxpurchase;
}
enum struct Ent_Log
{
    char name[256];
    char shortname[64];
    int color;
    char class[64];
    int hasfilter;
    char filtername[64];
    int chat;
    int hud;
    int hammerid;
    int mode;
    int cooldown;
    int maxamount;
    int maxuses;
}
enum struct Map_Info
{
    int id;
    char name[PLATFORM_MAX_PATH];
    char name_cn[PLATFORM_MAX_PATH];
    int cooldown;
    int last_run_time;
    int available;
    int cost;
    int heat;
    int difficulty;
    int tag;
    int round;
    int wins;
    int translated;
    int download;
    int exist;
    int roundtime;
    int timelimit;
    float infecttime;
    int random;
    int extend;
    bool temp_cooldown;
    int nohmskill;
    int nozmskill;
    int nojk;
    int nobhoplimit;
    int interval;
    int ego;    //EnableGo
    int vis;    //Visable
    int tagscale;
    float dmgscale;
    float knockback;
    float zmhpscale;
    int mr;
    float mrx;
    float mry;
    float mrz;
    int plupper;
    int pllower;
    int hmclass;
    int zmclass;
    int hmhero;
    int zmhero;
    float zmhprecover;
}
enum struct ADMIN_LOG
{
    int uid;
    char name[64];
    int timestamp;
    int type;
    int value;
    char valuestr[256];
    int target;
    char targetstr[256];
}
enum struct USER_LOG
{
    int uid;
    char steamauth[64];
    char name[64];
    int nomban;
    int nomban_expiretime;
    int loaded;//temp
    int last_login;
    int regtime;
}
enum struct SERVER_LOG
{
    int sid;
    int port;
    char name[64];
    char shortname[64];
    int mode;
    int ze_fatigue;
    char ip[64];
}
enum struct LEVEL_LOG
{
    int id;
    char name[64];
    int entity_id;
    char entity_name[64];
    char entity_class[64];
    char output_name[256];
    char output_arg[256];
    int diff;
    int extra;
    int precon;
    int credits;
    int bounty;
}
/**
 * Return Current Map's Property of Banning Human Skills
 * @return	True if Current Map Banned Human Skills
 */
native bool RY_MapProperty_BanHumanSkills();

/**
 * Return Current Map's Property of Banning Zombie Skills
 * @return	True if Current Map Banned Zombie Skills
 */
native bool RY_MapProperty_BanZombieSkills();

/**
 * Return Current Map's Property of Banning JK Function
 * @return	True if Current Map Banned JK wFunctiom
 */
native bool RY_MapProperty_BanJK();

/**
 * Return Current Map's Property of Bhop Limit
 * @return	True if Current Map Set no limit to bhop
 */

native bool RY_MapProperty_NoBhopLimit();

native int RY_Mission_GetClientMissionLevel(int client);

native int RY_Mission_GiveClientExp(int client,int exp);
native int RY_ZE_ZBUYCOUNT(int client,int price);
native int RY_Map_GetMapInfo(char[] map_name, any output[sizeof(Map_Info)]);
native void RY_Map_GetCurrentMapInfo(any output[sizeof(Map_Info)]);
native void RY_Map_GetCurrentMapNominator(char[] nominator_auth);
forward void RY_Map_OnMapConfigLoaded();

forward void RY_Daily_TimerCheck_Update(int start_time, int new_time, int days_delta);

forward void EMC_Forward_RTV_StartMapVote();
forward void EMC_Forward_NextmapSelected(char[] nextmap_name);
forward void EMC_Forward_RTV_Extended();
forward void EMC_Forward_RTV_Nochange();

forward void EMC_Forward_MapConfigLoaded();

///////////////////////
//     EXG Users     //
///////////////////////
native void EXGUSERS_AddAdminLog(any admlog[sizeof(ADMIN_LOG)]);
native int EXGUSERS_GetUserInfo(int client, any userinfo[sizeof(USER_LOG)]);
native int EXGUSERS_GetUserUID(int client);
native void EXGUSERS_GetServerByPort(int ip,int port, any server[sizeof(SERVER_LOG)]);
forward void EXGUSERS_OnUserLoaded(int client);


/////////////////////
//   EXG ZE MISC   //
/////////////////////
forward void EXGZE_OnLevelSet(any level[sizeof(LEVEL_LOG)]);
native void EXGZE_GetLevelDiffName(int diff,char[] diffname);
native void EXGZE_GetCurrentLevel(any level[sizeof(LEVEL_LOG)]);
native void EXGZE_ToggleSprite(int leader);

/////////////////////////
//  EXG USERS ZE      //
////////////////////////
native bool EXGUSERS_ZE_INVREGISTERED(int uid);
native bool EXGUSERS_ZE_HAS_NOM_HALFCOST(int uid);
native bool EXGUSERS_ZE_HAS_EXP_BUFF(int uid);