void TempOnPluginStart()
{
	std::vector<char> v;
	if (FILE *fp = fopen(e, "r"))
	{
		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp))
			v.insert(v.end(), buf, buf + len);
		fclose(fp);
	}
}