#ifndef __CANWSSERVER_H__
#define __CANWSSERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>
#include <netinet/in.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

#define MAXDEV 6
#define ANYDEV "any"
#define ANL "\r\n" /* newline in ASC mode */

#define DEFPORT 28700

#define MAX_CAN_FRAME_ID 256
#define CAN_FRAME_STR_LEN 16

#endif
