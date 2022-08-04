void AdminLogOnPluginStart()
{
	RegAdminCmd("sm_alog",AdminLogCommand,ADMFLAG_GENERIC);
}
public int Native_EXGUSERS_AddAdminLog(Handle plugin, int numParams)
{
	ADMIN_LOG admlog_add;
	GetNativeArray(1,admlog_add,sizeof(ADMIN_LOG));
	if(!isDbConnected())	return 0;
	char query[1024];
	CheckSQLInjectString(admlog_add.name,sizeof(admlog_add.name));
	CheckSQLInjectString(admlog_add.valuestr,sizeof(admlog_add.valuestr));
	Format(query,sizeof(query),"INSERT INTO exgusers_admlog (UID,NAME,TIMESTAMP,TYPE,VALUE,VALUESTR,TARGET,TARGETSTR) VALUES(%d,'%s',%d,%d,%d,'%s',%d,'%s')",admlog_add.uid,admlog_add.name,admlog_add.timestamp,admlog_add.type,admlog_add.value,admlog_add.valuestr,admlog_add.target,admlog_add.targetstr);
	DbTQuery(DbQueryErrorCallback,query);
	return 0;
}
Action AdminLogCommand(int client,int args)
{
	return Plugin_Handled;
}