/*
 * @Author: yc && qq747339545@163.com
 * @Date: 2025-03-01 17:17:00
 * @LastEditTime: 2025-03-14 15:52:46
 * @FilePath: /code_test/12_socket/tcp/client.c
 * @Description: 
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

/* socket
 * connect
 * send/recv
 */

#define SERVER_PORT 8888

int main(int argc, char **argv)
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	
	int iRet;
	unsigned char ucSendBuf[1000];
	int iSendLen;

	if (argc != 2)
	{
		printf("Usage:\n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	}
	// create socket
	iSocketClient = socket(AF_INET, SOCK_STREAM, 0);

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	//tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
 	if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))
 	{
		printf("invalid server_ip\n");
		return -1;
	}
	//填充 于将 `tSocketServerAddr` 结构体中的 `sin_zero` 字段的所有字节设置为 0
	//in_zero` 是 `sockaddr_in` 结构体中的一个字符数组，通常用于填充，使 `sockaddr_in` 结构体的大小与 `sockaddr` 结构体一致。
	// 尽管在大多数情况下这个字段不会被使用，但将其初始化为 0 是一个良好的编程习惯
	memset(tSocketServerAddr.sin_zero, 0, 8);


	iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));	
	if (-1 == iRet)
	{
		printf("connect error!\n");
		return -1;
	}

	while (1)
	{	
		//fgets 函数从标准输入（stdin）（也就是我们可以在终端里写）读取最多 999 个字符，并将其存储在 ucSendBuf 缓冲区中。
		if (fgets(ucSendBuf, 999, stdin))
		{
			iSendLen = send(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0);
			if (iSendLen <= 0)
			{
				close(iSocketClient);
				return -1;
			}
		}
	}
	
	return 0;
}

