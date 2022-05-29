#if defined _rainsfield_included_
    #endinput
#endif
#define _rainsfield_included_


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

forward RY_Daily_TimerCheck_Update(int start_time,int new_time,int days_delta);

forward EMC_Forward_RTV_StartMapVote();
forward EMC_Forward_NextmapSelected(char[] nextmap_name);
forward EMC_Forward_RTV_Extended();
forward EMC_Forward_RTV_Nochange();