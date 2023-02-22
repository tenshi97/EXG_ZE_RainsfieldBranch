Handle Db_Connection;
Handle DbM_Connection;
void DbOnPluginStart()
{
	if (!SQL_CheckConfig("exg_csgo_ry"))
	{
		PrintToServer("Database Error:Config Not Exsit");
		return;
	}
	SQL_TConnect(Db_Connect_Callback, "exg_csgo_ry");
	if(!SQL_CheckConfig("exg_csgo_zemaps"))
	{
		PrintToServer("Datanase Error:Map Database Config Not Exist");
		return;
	}
	SQL_TConnect(DbM_Connect_Callback, "exg_csgo_zemaps");
}
void Db_Connect_Callback(Handle owner, Handle hndl, const char[] error, any data) {
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE) PrintToServer("Database Error: %s", error);
	SQL_SetCharset(hndl, "utf8");
	Db_Connection = hndl;
	DbOnDbConnected_MapStartPost();
	CheckValidMission();
	ReloadAllPlayerMissionInfo();
}
void DbM_Connect_Callback(Handle owner, Handle hndl, const char[] error, any data) {
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE) PrintToServer("Database Error: %s", error);
	SQL_SetCharset(hndl, "utf8");
	DbM_Connection = hndl;
	DbOnDbConnected_MapStartPost();
}

bool isDbConnected()
{
	return	(Db_Connection!=INVALID_HANDLE);
}

void DbTQuery(SQLTCallback callback, const char[] query, any data = 0, DBPriority prio = DBPrio_Normal) {
	SQL_TQuery(Db_Connection, callback, query, data, prio);
}
void DbMQuery(SQLTCallback callback, const char[] query, any data = 0, DBPriority prio = DBPrio_Normal) {
	SQL_TQuery(DbM_Connection, callback, query, data, prio)
}
void DbOnDbConnected_MapStartPost()
{
	MapAdmOnDbConnected_MapStartPost();
}

void Db_Close()
{
	if (Db_Connection != INVALID_HANDLE) Db_Connection.Close();
	if(DbM_Connection != INVALID_HANDLE) DbM_Connection.Close();
}

int DbField(Handle hndl, const char[] field) {
	int value;
	if (!SQL_FieldNameToNum(hndl,field, value)) SetFailState("数据库错误：未知列 %s", field);
	return value;
}

int DbFetchInt(Handle hndl, const char[] field) {
	return SQL_FetchInt(hndl,DbField(hndl, field));
}

bool DbFetchBool(Handle hndl, const char[] field) {
	return SQL_FetchInt(hndl,DbField(hndl, field)) != 0;
}

float DbFetchFloat(Handle hndl, const char[] field) {
	return SQL_FetchFloat(hndl,DbField(hndl, field));
}

int DbFetchString(Handle hndl, const char[] field, char[] data, int size) {
	return SQL_FetchString(hndl,DbField(hndl, field), data, size);
}

void DbQueryErrorCallback(Handle owner, Handle hndl, char[] error, any data) {
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE) PrintToServer("数据库错误: %s", error);
}

void DbQueryFatalCallback(Handle owner, Handle hndl, char[] error, any data) {
	if (owner == INVALID_HANDLE || hndl == INVALID_HANDLE) SetFailState("数据库错误: %s", error);
}
