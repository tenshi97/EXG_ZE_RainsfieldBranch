const int cdeltat = 1668614400;
public int Native_EXGUSERS_AddAdminLog(Handle plugin, int numParams)
{
	ADMIN_LOG admlog_add;
	GetNativeArray(1,admlog_add,sizeof(ADMIN_LOG));
	if(!isDbConnected())	return 0;
	char query[1024];
	int deltad,tlower,tupper;
	deltad = (GetTime()-cdeltat)/86400;
	tlower = cdeltat+deltad*86400;
	tupper = tlower+86400;
	CheckSQLInjectString(admlog_add.name,sizeof(admlog_add.name));
	CheckSQLInjectString(admlog_add.valuestr,sizeof(admlog_add.valuestr));
	Format(query,sizeof(query),"SELECT * FROM exgusers_mapadmlog WHERE UID = %d AND TARGETSTR = '%s' AND TYPE = %d AND TIMESTAMP >= %d AND TIMESTAMP <= %d",admlog_add.uid,admlog_add.targetstr,admlog_add.type,tlower,tupper);
	//Format(query,sizeof(query),"UPDATE exgusers_mapadmlog SET UID = %d, NAME = '%s', TIMESTAMP = %d, TYPE = %d, VALUE = %d, VALUESTR = '%s', TARGET = %d, TARGETSTR = '%s' WHERE UID = %d AND TARGETSTR = '%s' AND TIMESTAMP >= %d AND TIMESTAMP <= %d",admlog_add.uid,admlog_add.name,admlog_add.timestamp,admlog_add.type,admlog_add.value,admlog_add.valuestr,admlog_add.target,admlog_add.targetstr,admlog_add.uid,admlog_add.targetstr,tlower,tupper);
	//Format(query,sizeof(query),"INSERT INTO exgusers_mapadmlog (UID,NAME,TIMESTAMP,TYPE,VALUE,VALUESTR,TARGET,TARGETSTR) VALUES(%d,'%s',%d,%d,%d,'%s',%d,'%s')",admlog_add.uid,admlog_add.name,admlog_add.timestamp,admlog_add.type,admlog_add.value,admlog_add.valuestr,admlog_add.target,admlog_add.targetstr);
	DataPack dp = CreateDataPack();
	dp.WriteCell(admlog_add.uid);
	dp.WriteString(admlog_add.name);
	dp.WriteCell(admlog_add.timestamp);
	dp.WriteCell(admlog_add.type);
	dp.WriteCell(admlog_add.value);
	dp.WriteString(admlog_add.valuestr);
	dp.WriteCell(admlog_add.target);
	dp.WriteString(admlog_add.targetstr);
	DbTQuery(DbQueryAdmLogCallback,query,dp);
	return 0;
}

int DbQueryAdmLogCallback(Handle owner, Handle hndl, char[] error, DataPack dp)
{
	dp.Reset();
	ADMIN_LOG admlog_add;
	admlog_add.uid = dp.ReadCell();
	dp.ReadString(admlog_add.name,sizeof(admlog_add.name));
	admlog_add.timestamp = dp.ReadCell();
	admlog_add.type = dp.ReadCell();
	admlog_add.value = dp.ReadCell();
	dp.ReadString(admlog_add.valuestr,sizeof(admlog_add.valuestr));
	admlog_add.target = dp.ReadCell();
	dp.ReadString(admlog_add.targetstr,sizeof(admlog_add.targetstr));
	delete dp;
	char query[1024];
	int deltad,tlower,tupper;
	deltad = (GetTime()-cdeltat)/86400;
	tlower = cdeltat+deltad*86400;
	tupper = tlower+86400;
	if(!SQL_HasResultSet(hndl)||!SQL_FetchRow(hndl))
	{
		Format(query,sizeof(query),"INSERT INTO exgusers_mapadmlog (UID,NAME,TIMESTAMP,TYPE,VALUE,VALUESTR,TARGET,TARGETSTR) VALUES(%d,'%s',%d,%d,%d,'%s',%d,'%s')",admlog_add.uid,admlog_add.name,admlog_add.timestamp,admlog_add.type,admlog_add.value,admlog_add.valuestr,admlog_add.target,admlog_add.targetstr);
		DbTQuery(DbQueryErrorCallback,query);
	}
	else
	{
		Format(query,sizeof(query),"UPDATE exgusers_mapadmlog SET UID = %d, NAME = '%s', TIMESTAMP = %d, TYPE = %d, VALUE = %d, VALUESTR = '%s', TARGET = %d, TARGETSTR = '%s' WHERE UID = %d AND TARGETSTR = '%s' AND type = %d AND TIMESTAMP >= %d AND TIMESTAMP <= %d",admlog_add.uid,admlog_add.name,admlog_add.timestamp,admlog_add.type,admlog_add.value,admlog_add.valuestr,admlog_add.target,admlog_add.targetstr,admlog_add.uid,admlog_add.targetstr,admlog_add.type,tlower,tupper);
		DbTQuery(DbQueryErrorCallback,query);
	}
	return 0;
}