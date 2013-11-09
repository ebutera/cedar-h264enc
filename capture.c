#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>             
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <time.h>

#include "type.h"
#include "capture.h"

typedef struct buffer 
{
	void * start;
	size_t length;
}buffer;

extern int cdxalloc_open(void);
extern int cdxalloc_close(void);
extern void* cdxalloc_alloc(int size);
extern void* cdxalloc_allocregs();
extern void cdxalloc_free(void *address);
extern unsigned int cdxalloc_vir2phy(void *address);
extern void cdxalloc_createmapping(void *virt,void *phys,int size);

static void		**cedarbufs	= NULL;

static int64_t start_timestamp;

#define CLEAR(x) memset (&(x), 0, sizeof (x))

int InitCapture()
{
	int i;
	struct timeval tv;

	if (cdxalloc_open()) {
		printf("cdxalloc open error!\n");
		return -1;
	}

	cedarbufs = calloc(CAPTURE_BUFFERS, sizeof(void *));

	for (i=0; i<CAPTURE_BUFFERS; i++) {
		char fname[100];
		FILE *fp;
		size_t bytesread = 0;

		cedarbufs[i] = cdxalloc_alloc(CAPTURE_BUFFER_SIZE);
		if (!cedarbufs[i]) {
			printf("cdxalloc_alloc error!\n");
			return -1;
		}
		printf("DBG: cdx buffer: %p\n", cedarbufs[i]);

		snprintf(fname, 99, "160x120-0%d.nv12", i);
		fp = fopen(fname, "r");
		if (!fp) {
			printf("Cannot open file %s\n", fname);
			return -1;
		}
		while (!feof(fp)) {
			bytesread += fread(((unsigned char *)cedarbufs[i])+bytesread, CAPTURE_BUFFER_SIZE, 1, fp);
		}
		fclose(fp);
		printf("read %d bytes\n", bytesread);
		bytesread = 0;
	}

	if (gettimeofday(&tv, NULL)) {
		printf("Cannot get time\n");
		return -1;
	}

	start_timestamp = (int64_t)((int64_t)tv.tv_usec + (((int64_t)tv.tv_sec) * 1000000));

	return 0;
}

void DeInitCapture()
{
	int i;

	printf("DeInitCapture");

	for (i=0; i<CAPTURE_BUFFERS; i++) {
		cdxalloc_free(cedarbufs[i]);
	}
}

int GetPreviewFrame(V4L2BUF_t *pBuf)
{
	int ret = -1; 
	struct timeval tv;
	static int buf_index = 0;

	if (gettimeofday(&tv, NULL)) {
		printf("Cannot get timestamp\n");
		return -1;
	}
	printf("buf_index: %d\n", buf_index);

	pBuf->addrPhyY	= cdxalloc_vir2phy(cedarbufs[buf_index]);
	pBuf->index 	= buf_index;
	pBuf->timeStamp = (int64_t)((int64_t)tv.tv_usec + (((int64_t)tv.tv_sec) * 1000000)) - start_timestamp;
	buf_index++;

	if (buf_index >= CAPTURE_BUFFERS)
		buf_index = 0;

	return 0;
}
