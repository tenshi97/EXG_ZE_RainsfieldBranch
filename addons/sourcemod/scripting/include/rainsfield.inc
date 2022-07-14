#if defined _rainsfield_included_
    #endinput
#endif
#define _rainsfield_included_

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
    int zmclass;
    float zmhpscale;
    int mr;
    float mrx;
    float mry;
    float mrz;
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
 * @return	True if Current Map Banned JK Functiom
 */
native bool RY_MapProperty_BanJK();

/**
 * Return Current Map's Property of Bhop Limit
 * @return	True if Current Map Set no limit to bhop
 */

native bool RY_MapProperty_NoBhopLimit();

native int RY_Mission_GetClientMissionLevel(int client);

native int RY_Mission_GiveClientExp(int client,int exp);

native int RY_Map_GetMapInfo(char[] map_name,any output[sizeof(Map_Info)]);
native void RY_Map_GetCurrentMapInfo(any output[sizeof(Map_Info)]);

forward void RY_Map_OnMapConfigLoaded();

forward RY_Daily_TimerCheck_Update(int start_time,int new_time,int days_delta);

forward void EMC_Forward_RTV_StartMapVote();
forward void EMC_Forward_NextmapSelected(char[] nextmap_name);
forward void EMC_Forward_RTV_Extended();
forward void EMC_Forward_RTV_Nochange();

forward void EMC_Forward_MapConfigLoaded();