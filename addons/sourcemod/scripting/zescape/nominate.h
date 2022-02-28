enum struct Nomlist_Log
{
	int id;
	char name[64];
	int nominator_steamid;
	char nominator_name[64];
}
const int Nominate_Max_Num = 7;
Nomlist_Log NMQueue[Nominate_Max_Num];
bool Nominate_ALLOW;
ArrayList Nom_Map_List;
void NominateOnPluginStart()
{
//	RegConsoleCmd("sm_nominate",ActionBuildNominateMenu);
//	RegConsoleCmd("sm_nom",ActionBuildNominateMenu);
//	RegConsoleCmd("sm_yd",ActionBuildNominateMenu);
	Nominate_ALLOW=true;
}
void NominateOnMapStart()
{

}

void NominateOnDbConnected()
{
	Nom_Map_List.Clear();
	char query[256];
	Format(query, sizeof(query), "SELECT * FROM ZEMAPS WHERE available = 1 AND download = 1", Map_Data.Id);
	DbQuery(query, NominateListLoadCallback);
}

void NominateListLoadCallback(Handle owner, Handle hndl, char[] error, any data) 
{
	Nom_Map_List = CreateArray(sizeof(Map_Log));
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE) SetFailState("数据库错误: %s", error);
	int id,name;
	if (!SQL_FieldNameToNum(hndl, "id", id)) SetFailState("数据库错误: 未知列: id");
	if (!SQL_FieldNameToNum(hndl, "name", name)) SetFailState("数据库错误:未知列：name");
	While(SQL_FetchRow(hndl))
	{
		Map_Log MapL;
		MapL.id = SQL_FetchInt(hndl,id);
		SQL_FetchString(hndl,name,MapL.name,sizeof(MapL.name));
	}
	Nom_Map_List.Push(MapL);
	return;
}