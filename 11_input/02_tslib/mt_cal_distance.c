/*
 * @Author: yc && qq747339545@163.com
 * @Date: 2025-02-21 17:23:26
 * @LastEditTime: 2025-02-23 17:23:52
 * @FilePath: /code_test/11_input/02_tslib/mt_cal_distance.c
 * @Description: 
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include "tslib.h"

int main(int argc,char **argv)
{
	struct tsdev *ts;
	ts = ts_setup(NULL, 0);
	if(!ts)
	{
		perror("ts_setup_err!");
		return -1;
	}
	while (1)
	{
		ret = ts_read_mt(ts,ts_sample_mt,max_slot,1);
	}
	


	return 0 ;
}
