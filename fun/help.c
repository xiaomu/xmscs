int help()
{
	char *menu[] = {
		"dnppi filename\n",
		NULL
	};

	char **ptr;

	ptr = menu;
	while(*ptr != NULL)
	{
		printf("%s", *ptr);
		*ptr ++;
	}

	return 0;
}