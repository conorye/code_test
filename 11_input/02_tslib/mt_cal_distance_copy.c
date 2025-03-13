
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

int distance(struct ts_sample_mt *point1, struct ts_sample_mt *point2)
{
	int x1 = point1->x;
	int y1 = point1->y;
	int x2 = point2->x;
	int y2 = point2->y;
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

int main(int argc, char **argv)
{	
	printf("1");
	struct tsdev *ts;
	int ret, i, touch_cnt;
	struct ts_sample_mt **samp_mt;
	// 定义上一次的数据
	struct ts_sample_mt **pre_samp_mt;

	struct input_absinfo slot;
	int max_slots;
	int point_pressed[100];

	ts = ts_setup(NULL, 0);
	if (!ts)
	{
		perror("ts_setup_err!");
		return -1;
	}

	// 读取一个触点有多少信息,因为可能触摸屏不一样,要从驱动程序中读取
	if (ioctl(ts_fd(ts), EVIOCGABS(ABS_MT_SLOT), &slot) < 0)
	{
		perror("ioctl EVIOGABS");
		ts_close(ts);
		return errno;
	}
	max_slots = slot.maximum + 1 - slot.minimum;


	samp_mt = malloc(sizeof(struct ts_sample_mt *));
	if (!samp_mt)
	{
		ts_close(ts);
		return -ENOMEM;
	}
	samp_mt[0] = calloc(max_slots, sizeof(struct ts_sample_mt));
	if (!samp_mt[0])
	{
		free(samp_mt);
		ts_close(ts);
		return -ENOMEM;
	}

	pre_samp_mt = malloc(sizeof(struct ts_sample_mt *));
	if (!pre_samp_mt)
	{
		ts_close(ts);
		return -ENOMEM;
	}
	pre_samp_mt[0] = calloc(max_slots, sizeof(struct ts_sample_mt));
	if (!pre_samp_mt[0])
	{
		free(pre_samp_mt);
		ts_close(ts);
		return -ENOMEM;
	}
	
	for (i = 0; i < max_slots; i++)
		pre_samp_mt[0][i].valid = 0;

	while (1)
	{
		// int ts_read_mt(struct tsdev *ts, struct ts_sample_mt **samp, int max_slots,
		// int nr)
		ret = ts_read_mt(ts, samp_mt, max_slots, 1);
		if (ret < 0)
		{
			perror("ts_read_mt_err\n");
			ts_close(ts);
			return -1;
		}
		// 将原数据与新数据进行对比

		for (i = 0; i < max_slots; i++)
		{
			if (samp_mt[0][i].valid)
			{
				memcpy(&pre_samp_mt[0][i], &samp_mt[0][i], sizeof(struct ts_sample_mt));
			}
		}

		touch_cnt = 0;
		for (i = 0; i < max_slots; i++)
		{

			if ((pre_samp_mt[0][i].valid) && ((pre_samp_mt[0][i].tracking_id) != -1))
			{
				// i对应第0套的第i个结构体 所有的结构体都需要被扫描一次
				// touch_cnt对应真正被按下的点的个数
				// point_pressed对应被按下的点的序号
				point_pressed[touch_cnt++] = i;
			}
		}

		if (touch_cnt == 2)
		{
			// distance期望得到的是一个指针，必须传入地址，pre_samp_mt[0][point_pressed[0]]是一个
			printf("\r\ndistance: %08d", distance(&pre_samp_mt[0][point_pressed[0]], &pre_samp_mt[0][point_pressed[1]]));
		}
	}

	return 0;
}
