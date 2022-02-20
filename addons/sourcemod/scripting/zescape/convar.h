StringMap ConVars;

ConVar ConVarFind(const char[] name) {
	ConVar con;
	if (!ConVars.GetValue(name, con)) {
		con = FindConVar(name);
		if (con) ConVars.SetValue(name, con);
	}
	return con;
}