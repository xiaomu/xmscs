/*****************************************************************************
 *  Copyright          :  All Rights Reserved.
 *
 *  Date               :  2012-10-09 13:52:07
 *  Author/Corporation :  Dengzhaoqun
 *  Email              :  dengzhaoqun@163.com
 *****************************************************************************/
#include <stdio.h>
#include "xm_ruf.h"

int main(int argc, char *argv[])
{
#if 0	
	XM_RUF_T ruf;
	int i, j;

	xm_ruf_create(&ruf, "tmp", 20);
	for(i=0; i<10; i++)
	{
		xm_ruf_rollup(&ruf);
		for(j=0; j<9; j++)
		{
			fprintf(ruf.fp, "%d", i);
		}
		fprintf(ruf.fp, "\n");
		fflush(ruf.fp);
		sleep(1);
	}
	xm_ruf_fclose(&ruf);
#endif

	XM_RUF_T ruf;
	char buf[20];
	
	xm_ruf_fopen(&ruf, "tmp", "r");
	fscanf(ruf.fp, "%s", buf);
	printf("%s\n", buf);
	
	xm_ruf_rewind(&ruf);
	fscanf(ruf.fp, "%s", buf);
	printf("%s\n", buf);
	xm_ruf_fclose(&ruf);
	
	return 0;
}
