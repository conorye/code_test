#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

int main(int argc, char **argv)
{
    int fd, fd2 = -1; // 初始化 fd2
    int err, len, res, i;
    unsigned char byte;
    int bit;
    struct input_id id;
    unsigned int evbit[2];
    struct input_event event;
    struct pollfd fds[2];
    nfds_t nfds = 2;

    if (argc < 3)
    {
        printf("Usage: %s <dev1> <dev2>\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        perror("open fd1");
        return -1;
    }

    fd2 = open(argv[2], O_RDWR);
    if (fd2 < 0)
    {
        perror("open fd2");
        return -1;
    }

    err = ioctl(fd, EVIOCGID, &id);
    if (err == 0)
    {
        printf("bustype = 0x%x\n", id.bustype);
        printf("vendor  = 0x%x\n", id.vendor);
        printf("product = 0x%x\n", id.product);
        printf("version = 0x%x\n", id.version);
    }

    len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
    if (len > sizeof(evbit)) len = sizeof(evbit);

    while (1)
    {
        fds[0].fd = fd;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        fds[1].fd = fd2;
        fds[1].events = POLLIN;
        fds[1].revents = 0;

        res = poll(fds, nfds, 5000);

        if (res > 0)
        {
            if (fds[0].revents & POLLIN)
            {
                ssize_t bytes_read;
                while ((bytes_read = read(fd, &event, sizeof(event))) > 0)
                {
                    printf("fd event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
                }
                if (bytes_read == -1) perror("read fd");
            }

            if (fds[1].revents & POLLIN)
            {
                ssize_t bytes_read;
                while ((bytes_read = read(fd2, &event, sizeof(event))) > 0)
                {
                    printf("fd2 event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
                }
                if (bytes_read == -1) perror("read fd2");
            }
        }
        else if (res == 0)
        {
            printf("time out\n");
        }
        else
        {
            perror("poll error");
        }
    }

    return 0;
}
