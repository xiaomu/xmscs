/*
 * author: dengzhaoqun
 * date  : 2011/07/22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oper_config.h"

static int check_key(char *line, char *key);
static char *get_value(char *line);


// fp 需要以读的方式得到
char *
read_config(FILE *fp, char *key)
{
	char line[MAX_LINE_LEN] = {'\0'};
	char *value = NULL;
	int ret;

	while(fgets( line, sizeof(line), fp) != NULL)
	{
		ret = check_key(line, key);
		if(ret == -1)
		{
			continue;
		}
		else
		{
			value = get_value(line);
			if(value == NULL)
			{
				del_config(fp, key);
				return NULL;
			}
			else
			{
				return value;
			}
		}
	}/* while */

	return NULL;
}


static int
check_key(char *line, char *key)
{
	char *k_start, *k_end;		// 指示 key 在 line 中的起始和结束位置
	int line_len;

	line_len = strlen(line);
	if(line_len < 3)
	{
		return(-1);
	}
	else
	{
		k_start = &line[0];
		while(((*k_start == ' ') || (*k_start == '\t'))
				&& ( k_start <= &line[line_len - 1]))
		{
			k_start ++;
		}

		if(*k_start == '#')
		{
			return(-1);
		}

		k_end = strchr(line, '=');
		if(k_end == NULL)
		{
			return(-1);
		}
		k_end --;
		while(((*k_end == ' ') || (*k_end == '\t'))
				&& (k_end >= k_start))
		{
			k_end --;
		}
		if((*k_end == ' ') || (*k_end == '\t'))
		{
			return(-1);
		}

		if(strncmp(key, k_start, k_end-k_start + 1) != 0)
		{
			return(-1);
		}
	}

	return(0);
}/* check_key() */


static char*
get_value(char *line)
{
	char *v_start, *v_end;		// 指示 value 在 line 中的起始和结束位置
	char *value = NULL;
	int line_len;
	int val_len;

	line_len = strlen(line);

	v_start = strchr(line, '=');	// 已经在 check_key 中检验过'＝'的存在
	v_start ++;
	if(((*v_start == ' ') || (*v_start == '\t'))
			&& (v_start <= &line[line_len - 1]))
	{
		v_start ++;
	}

	v_end = &line[line_len - 1];
	if(((*v_end == ' ') || (*v_end == '\t')
			|| (*v_end == '\n')
			|| (*v_end == '\r'))
			&& (v_end >= v_start))
	{
		v_end --;
	}
	if((*v_end == ' ') || (*v_end == '\t')
			|| (*v_end == '\n')
			|| (*v_end == '\r'))
	{
		return NULL;
	}

	val_len = v_end - v_start + 1;
	value = (char *)malloc((val_len + 1) * sizeof(char));
	if(value == NULL)
	{
		printf("malloc failed.\n");
		return NULL;
	}
	strncpy(value, v_start, val_len);
	value[val_len] = '\0';
	return value;
}/* get_value() */


// fp 需要以添加的方式得到
int
add_config(FILE *fp, char *key, char *value)
{
	char *line = NULL;
	int key_len;
	int val_len;

	key_len = strlen(key);
	val_len = strlen(value);
	
	if( (fp == NULL) || (key == NULL) || (value == NULL))
	{
		return(-1);
	}

	line = (char *)malloc((key_len + val_len + 5) * sizeof(char));
	if(line == NULL)
	{
		printf("malloc failed.\n");
		return(-1);
	}
	else
	{
		strncpy(line, key, key_len);
		line[key_len] = ' ';
		line[key_len + 1] = '=';
		line[key_len + 2] = ' ';
		line[key_len + 3] = '\0';
		strncat(line, value, val_len);
		line[key_len + val_len + 3] = '\n';
		line[key_len + val_len + 4] = '\0';

		if(fputs(line, fp) == EOF)
		{
			return(-1);
		}
	}

	free(line);

	return(0);
}/* add_config() */
	

#if 0
// fp 需要以读写的方式打开
// 由于删除需要打开流，先读取， 关闭流，重新打开流，写入， 所以单纯传递文件流无法实现
int
del_config(FILE *fp, char *key)
{
	char line[MAX_LINE_LEN] = {'\0'};
	char *file;
	long f_len;
	int ret;
	int i;

	fseek(fp, 0, SEEK_END);
	f_len = ftell(fp);

	file = (char *)malloc( (f_len + 2) * sizeof(char));
	if(file == NULL)
	{
		printf("malloc failed.\n");
		return (-1);
	}
	memset(file, 0, (f_len +2));

	fseek(fp, 0, SEEK_SET);
	while(fgets( line, sizeof(line), fp) != NULL)
	{
		ret = check_key(line, key);
		if(ret == -1)
		{
			strncat(file, line, strlen(line));
		}
	}

	fseek(fp, 0, SEEK_SET);
	for(i = 0; i< f_len; i++)
	{
		fputc('\0', fp);
	}
	
	fseek(fp, 0, SEEK_SET);
	fputs(file, fp);

	return(0);
}/* del_config() */

// fp 需要以读写的方式打开
// 修改需要先删除，故同样无法实现
int
alter_config(FILE *fp, char *key, char *new_value)
{
	// add_config 是在配置文件后添加一条记录
	// dle_config 删除碰到的第一条 鍵值 为key的记录
	if((add_config(fp, key, new_value) == 0) 
		       && (del_config(fp, key) == 0))
	{
		return(0);
	}
	
	return(-1);
}/* alter_config() */
#endif
