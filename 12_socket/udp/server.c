/*
 * @Author: yc && qq747339545@163.com
 * @Date: 2025-03-01 17:17:00
 * @LastEditTime: 2025-03-19 12:27:42
 * @FilePath: /code_test/12_socket/udp/server.c
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
#include <signal.h>


/* socket
 * bind
 * sendto/recvfrom
 */

#define SERVER_PORT 8888

int main(int argc, char **argv)
{
	int iSocketServer;
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	struct sockaddr_in tSocketClientAddr;
	int iRet;
	int iAddrLen;

	int iRecvLen;
	unsigned char ucRecvBuf[1000];

	int iClientNum = -1;
	// 表示创建一个数据报套接字（Datagram Socket）
	iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	// 将 `tSocketServerAddr` 结构体中的 `sin_zero` 字段的所有字节设置为 0
	// `sin_zero` 是 `sockaddr_in` 结构体中的一个字符数组，通常用于填充，使 `sockaddr_in` 结构体的大小与 `sockaddr` 结构体一致。
	// 尽管在大多数情况下这个字段不会被使用，但将其初始化为 0 是一个良好的编程习惯
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	// 将服务器端的套接字绑定到指定的 IP 地址和端口号
	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}


	while (1)
	{
	// 初始化 iAddrLen，用于存储客户端地址结构的大小。
	// 调用 recvfrom 函数，从客户端接收数据报，并将数据存储在缓冲区 ucRecvBuf 中，同时获取客户端的地址信息并存储在 tSocketClientAddr 中。
		//用于表示套接字地址的长度
		socklen_t iAddrLen = sizeof(struct sockaddr);
		// 从客户端接收数据
		// (struct sockaddr *)&tSocketClientAddr: 指向 struct sockaddr_in 类型的指针，用于存储发送方的地址信息
		iRecvLen = recvfrom(iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
		if (iRecvLen > 0)
		{
			ucRecvBuf[iRecvLen] = '\0';
			printf("Get Msg From %s : %s\n", inet_ntoa(tSocketClientAddr.sin_addr), ucRecvBuf);
		}
	}
	
	close(iSocketServer);
	return 0;
}


