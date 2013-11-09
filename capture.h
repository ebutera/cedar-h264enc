
#ifndef __CAPTURE__H__
#define __CAPTURE__H__

#include <asm/types.h> 
#include <linux/videodev2.h>
#include "type.h"

#define CAPTURE_BUFFERS		3
#define CAPTURE_WIDTH		160
#define CAPTURE_HEIGHT		120
#define CAPTURE_BUFFER_SIZE	((CAPTURE_WIDTH * CAPTURE_HEIGHT * 3) / 2)

int InitCapture();
void DeInitCapture();
int WaitCamerReady();
int GetPreviewFrame(V4L2BUF_t *pBuf);


#endif // __CAPTURE__H__
