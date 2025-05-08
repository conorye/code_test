/*
 * @Author: yc && qq747339545@163.com
 * @Date: 2025-03-01 17:17:00
 * @LastEditTime: 2025-03-15 12:00:35
 * @FilePath: /code_test/12_socket/tcp/server.c
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
 * listen
 * accept
 * send/recv
 */

#define SERVER_PORT 8888
#define BACKLOG     10

int main(int argc, char **argv)
{
	int iSocketServer; // 服务器端的socket
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr; // 服务器端的socket地址 有ip和port
	struct sockaddr_in tSocketClientAddr;
	int iRet; 
	int iAddrLen;

	int iRecvLen;
	unsigned char ucRecvBuf[1000];

	int iClientNum = -1;

	signal(SIGCHLD,SIG_IGN); // 避免僵尸进程
	
	iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == iSocketServer)
	{+-
		printf("socket error!\n");
		return -1;
	}

	tSocketServerAddr.sin_family      = AF_INET;
	// `htons` 函数将主机字节序（host byte order）转换为网络字节序（network byte order），因为不同的计算机体系结构可能使用不同的字节序，
	//而网络传输通常使用大端字节序（big-endian）。`SERVER_PORT` 是一个宏或常量，表示服务器监听的端口号。
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	
	//设置 IP 地址为 `192.168.5.11`，表示服务器将绑定到指定的网络接口。
	// 这意味着服务器可以接收发送到该 IP 地址的连接请求。
	// tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	tSocketServerAddr.sin_addr.s_addr = inet_addr("192.168.5.11");
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet) 
	{
		perror("bind failed");  // 输出具体errno描述 
		fprintf(stderr, "[DEBUG] Attempting to bind to %s:%d\n", 
			   inet_ntoa(tSocketServerAddr.sin_addr),  
			   ntohs(tSocketServerAddr.sin_port)); 
		return -1;
	}
	// `listen` 函数将服务器端的 socket 设置为监听模式，等待客户端的连接请求。
	iRet = listen(iSocketServer, BACKLOG);
	if (-1 == iRet)
	{	
		// 这里只是监听启动失败
		printf("listen error!\n");
		return -1;
	}

	while (1)
	{
		socklen_t iAddrLen = sizeof(struct sockaddr_in);
		iSocketClient = accept(iSocketServer, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
		if (-1 != iSocketClient)
		{	//增加客户端计数并打印客户端信息（ip地址）
			iClientNum++;
			printf("Get connect from client %d : %s\n",  iClientNum, inet_ntoa(tSocketClientAddr.sin_addr));
			//创建子进程处理客户端通信
			if (!fork())
			{
				/* 子进程的源码 */
				while (1)
				{
					/* 接收客户端发来的数据并显示出来 */
					iRecvLen = recv(iSocketClient, ucRecvBuf, 999, 0);
					if (iRecvLen <= 0)
					{
						close(iSocketClient);
						return -1;
					}
					else
					{
						ucRecvBuf[iRecvLen] = '\0';
						printf("Get Msg From Client %d: %s\n", iClientNum, ucRecvBuf);
					}
				}				
			}
		}
	}
	
	close(iSocketServer);
	return 0;
}


