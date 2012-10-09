#ifndef _XM_RUF_H_

// ruf -- roll up file

#define XM_RUF_META_LEN 128
#define XM_RUF_BUF_LEN 1024

typedef struct
{
	FILE *fp;
	int e_pos; // extra region  position
	int f_pos; // file content position
	int extra_size;
}XM_RUF_T;

int xm_ruf_create(XM_RUF_T *ruf, const char *path, const int extra_size);
XM_RUF_T *xm_ruf_fopen(const char *path, const char *mode);
void xm_ruf_fclose(XM_RUF_T *ruf);
void xm_ruf_rewind(XM_RUF_T *ruf);
void xm_ruf_rollup(XM_RUF_T *ruf);

#endif
