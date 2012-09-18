#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define CMD_LEN 1024
#define PATH_LEN 1024
#define LINE_LEN 2048

int check_file(char *path, int argc, char *argv[]);

int main(int argc, char *argv[])
{
	char *path;
	char *tmp_c_file = "tmp_c_file";
	char cmd[CMD_LEN];
	char line[PATH_LEN];
	FILE *tmp_fp;
	int len;
	
	if(argc < 2)
	{
		printf("%s dir fun fun2 ...\n%s c_file fun fun2 ...\n", argv[0], argv[0]);
		return -1;
	}
	path = argv[1];	
	if(!(strcmp(&path[strlen(path)-2], ".c")))
	{
		check_file(path, argc, argv);
	}
	else
	{
		chdir(path);
		sprintf(cmd, "ls -1 *.c > %s", tmp_c_file);
		cmd[CMD_LEN -1] = '\0';
		system(cmd);
		if((tmp_fp = fopen(tmp_c_file, "r")) == NULL)
		{
			printf("fopen %s failed\n", tmp_c_file);
			return -1;
		}
		while(fgets(line, PATH_LEN, tmp_fp) != NULL)
		{
			len = strlen(line);
			if(line[len - 1] == '\n')
			{
				line[len-1] = '\0';
			}
			check_file(line, argc, argv);
		}
		fclose(tmp_fp);
	}
	return 0;
}

int check_file(char *path, int argc, char *argv[])
{
	char **c_funs = NULL;
	char line[LINE_LEN];
	FILE *fp;
	char **ptr, *p_start, c_pre;
	int i_start, len, sig_count, como_count, flag_in_squ, line_num, j;
	char sig = '%', esc='\\';
	
	c_funs = (char **)malloc((4+argc-2+1)*sizeof(char *));
	if(c_funs == NULL)
	{
		printf("malloc failed\n");
		return -1;
	}
	j = 0;
	c_funs[j++] = "printf";
	c_funs[j++] = "fprintf";
	c_funs[j++] = "sprintf";
	c_funs[j++] = "snprintf";
	for(; j<4+argc-2; j++)
	{
		c_funs[j] = argv[j-4+2];
	}
	c_funs[j] = NULL;
	
	
	fp = fopen(path, "r");
	if(fp == NULL)
	{
		printf("fopen %s failed\n", path);
		return -1;
	}
	
	line_num = 0;
	while(fgets(line, LINE_LEN, fp) != NULL)
	{
		len = strlen(line);
		if(line[len-1] == '\n')
		{
			line[len-1] = '\0';
			line_num ++;
		}
		
		len = len - 1;
		i_start = 0;
		while(isspace(line[i_start]) && (i_start < len-1))
		{
			i_start++;
		}
		if(i_start >= len - 1)
		{
			continue;
		}
		ptr = c_funs;
		while(*ptr != NULL)
		{
			if(!(strncmp(*ptr, &line[i_start], strlen(*ptr))))
			{
				sig_count = 0;
				p_start = strchr(&line[i_start], '"');
				if(p_start == NULL)
				{
					*ptr++;
					continue;
				}
				p_start ++;
				c_pre = ' ';
				while((p_start < &line[len-1]) && ((*p_start != '"') || ((*p_start == '"') && (c_pre == esc))))
				{
					if(*p_start == sig)
					{
						sig_count ++;
					}
					c_pre = *p_start;
					p_start ++;
				}
				p_start ++;
				
				como_count = 0;
				flag_in_squ = 0;
				c_pre = ' ';
				while(p_start < &line[len-1])
				{
					if(flag_in_squ)
					{
						if((*p_start == '"') && (c_pre != esc))
						{
							flag_in_squ = 0;
						}
					}
					else if(*p_start == '"')
					{
						flag_in_squ = 1;
					}
					else if(*p_start == ',')
					{
						como_count ++;
					}
					p_start++;
				}
				
				if(sig_count != como_count)
				{
					printf("file: %s -- line: %d  do not match\n", path, line_num);
				}
			}
			*ptr ++;
		}
	}
	
	fclose(fp);
	
	return 0;
	
}



