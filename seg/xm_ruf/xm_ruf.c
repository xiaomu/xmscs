#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include "xm_ruf.h"

// ruf -- roll up file



int xm_ruf_create(XM_RUF_T *ruf, const char *path, const int extra_size)
{
	FILE *fp;
	
	fp = fopen(path, "w");
	if(fp != NULL)
	{
		ruf->fp = fp;
		ruf->e_pos = XM_RUF_META_LEN;
		ruf->f_pos = ruf->e_pos;
		ruf->extra_size = extra_size;
		
		fprintf(fp, "%d ", ruf->e_pos);
		fprintf(fp, "%d ", ruf->f_pos);
		fprintf(fp, "%d\n", extra_size);
		
		ftruncate(fileno(fp), ruf->e_pos);
		fseek(fp, ruf->e_pos, SEEK_SET);
		return 0;
	}
	
	return -1;
}

XM_RUF_T *xm_ruf_fopen(const char *path, const char *mode)
{
	XM_RUF_T *ruf;
	
	if(access(path, 0) != 0)
	{
		fprintf(stderr, "%s no exist\n", path);
		return NULL;
	}
	
	ruf = (XM_RUF_T *)malloc(sizeof(XM_RUF_T));
	if(ruf == NULL)
	{
		return NULL;
	}
	memset(ruf, 0, sizeof(XM_RUF_T));
	
	if((!strcmp(mode, "r")) || (!strcmp(mode, "r+")) || (!strcmp(mode, "a")) || (!strcmp(mode, "a+")))
	{
		ruf->fp = fopen(path, mode);
		if(ruf->fp != NULL)
		{
			if((fscanf(ruf->fp, "%d", &ruf->e_pos) != EOF) && (fscanf(ruf->fp, "%d", &ruf->f_pos) != EOF) && (fscanf(ruf->fp, "%d", &ruf->extra_size) != EOF))
			{
				if((!strcmp(mode, "r")) || (!strcmp(mode, "r+")))
				{
					fseek(ruf->fp, ruf->f_pos, SEEK_SET);
				}
				return ruf;
			}
			else
			{
				fprintf(stderr, "not roll up file\n");
				free(ruf);
				return NULL;
			}
		}
		else
		{
			perror("");
			free(ruf);
			return NULL;
		}
	}
	else
	{
		fprintf(stderr, "wrong mode %s\n", mode);
		free(ruf);
		return NULL;
	}
}

void xm_ruf_fclose(XM_RUF_T *ruf)
{
	rewind(ruf->fp);
	fprintf(ruf->fp, "%d ", ruf->e_pos);
	fprintf(ruf->fp, "%d ", ruf->f_pos);
	fprintf(ruf->fp, "%d\n", ruf->extra_size);
	fclose(ruf->fp);
}

void xm_ruf_rewind(XM_RUF_T *ruf)
{
	fseek(ruf->fp, ruf->f_pos, SEEK_SET);
}

void xm_ruf_rollup(XM_RUF_T *ruf)
{
	int cur_pos;
	
	cur_pos = ftell(ruf->fp);
	if(cur_pos - ruf->e_pos < ruf->extra_size)
	{
		ruf->f_pos = ftell(ruf->fp);
	}
	else
	{
		int size;
		FILE *t_fp;
		char buf[XM_RUF_BUF_LEN];
		int ret;
		
		fseek(ruf->fp, 0, SEEK_END);
		size = ftell(ruf->fp);
		fseek(ruf->fp, cur_pos, SEEK_SET);
		
		t_fp = tmpfile();
		while((ret = fread(buf, XM_RUF_BUF_LEN, 1, ruf->fp)) > 0)
		{
			fwrite(buf, ret, 1, t_fp);
		}
		
		rewind(t_fp);
		fseek(ruf->fp, ruf->e_pos, SEEK_SET);
		while((ret = fread(buf, XM_RUF_BUF_LEN, 1, t_fp)) > 0)
		{
			fwrite(buf, ret, 1, ruf->fp);
		}
		ftruncate(fileno(ruf->fp), size - (cur_pos - ruf->e_pos));
		fseek(ruf->fp, ruf->e_pos, SEEK_SET);
		ruf->f_pos = ruf->e_pos;
		fclose(t_fp);
	}
}
