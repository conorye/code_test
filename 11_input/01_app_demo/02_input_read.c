/*
 * @Author: yc && qq747339545@163.com
 * @Date: 2025-02-07 10:57:13
 * @LastEditTime: 2025-02-07 16:52:13
 * @FilePath: /code_test/11_input/01_app_demo/02_input_read.c
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

/*./02_input_read  /dev/input/event0 [noblock]
app以noblock的方式读取数据*/
int main(int argc, char **argv)
{
    int fd, err, len, i, bit;
    struct input_id id;
    struct input_event event;

    unsigned int evbit[2];
    unsigned char byte;

    char *ev_names[] = {
        "EV_SYN ",
        "EV_KEY ",
        "EV_REL ",
        "EV_ABS ",
        "EV_MSC ",
        "EV_SW	",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "NULL ",
        "EV_LED ",
        "EV_SND ",
        "NULL ",
        "EV_REP ",
        "EV_FF	",
        "EV_PWR ",
    };

    if (argc < 2)
    {
        printf("usage %s <dev> [noblock]\n", argv[0]);
        return -1;
    }

    if (argc == 3 && !strcmp(argv[2], "noblock"))
    {
        fd = open(argv[1], O_RDWR | O_NONBLOCK);
    }
    else
    {
        fd = open(argv[1], O_RDWR);
    }

    if (fd < 0)
    {
        printf("open %s err\n", argv[0]);
        return -1;
    }

    /*获取设备的id*/
    err = ioctl(fd, EVIOCGID, &id);
    if (err == 0)
    {

        printf("bustype  = 0x%x\n", id.bustype);
        printf("vendor   = 0x%x\n", id.vendor);
        printf("product  = 0x%x\n", id.product);
        printf("version  = 0x%x\n", id.version);
    }

    /*获取设备的evbit*/
    len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
    if (len > 0 && len <= sizeof(evbit))
    {
        printf("support ev type");
        for (i = 0; i < len; i++)
        {
            byte = ((unsigned char *)evbit)[i];
            for (bit = 0; bit < 8; bit++)
            {
                if (byte & (1 << bit))
                {
                    printf(" %s", ev_names[i * 8 + bit]);
                }
            }
        }
        printf("\n");
    }

    while (1)
    {
        len = read(fd, &event, sizeof(event));
        printf("len:%d\n",len);
        printf("sizeof(event):%lu\n",sizeof(event));

        if (len == sizeof(event))
        {
            printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
        }
        else
        {
            printf("read err %d", len);
        }
    }
}
