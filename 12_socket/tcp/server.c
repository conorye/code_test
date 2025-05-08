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
	int iSocketServer; // �������˵�socket
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr; // �������˵�socket��ַ ��ip��port
	struct sockaddr_in tSocketClientAddr;
	int iRet; 
	int iAddrLen;

	int iRecvLen;
	unsigned char ucRecvBuf[1000];

	int iClientNum = -1;

	signal(SIGCHLD,SIG_IGN); // ���⽩ʬ����
	
	iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == iSocketServer)
	{+-
		printf("socket error!\n");
		return -1;
	}

	tSocketServerAddr.sin_family      = AF_INET;
	// `htons` �����������ֽ���host byte order��ת��Ϊ�����ֽ���network byte order������Ϊ��ͬ�ļ������ϵ�ṹ����ʹ�ò�ͬ���ֽ���
	//�����紫��ͨ��ʹ�ô���ֽ���big-endian����`SERVER_PORT` ��һ�����������ʾ�����������Ķ˿ںš�
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	
	//���� IP ��ַΪ `192.168.5.11`����ʾ���������󶨵�ָ��������ӿڡ�
	// ����ζ�ŷ��������Խ��շ��͵��� IP ��ַ����������
	// tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	tSocketServerAddr.sin_addr.s_addr = inet_addr("192.168.5.11");
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet) 
	{
		perror("bind failed");  // �������errno���� 
		fprintf(stderr, "[DEBUG] Attempting to bind to %s:%d\n", 
			   inet_ntoa(tSocketServerAddr.sin_addr),  
			   ntohs(tSocketServerAddr.sin_port)); 
		return -1;
	}
	// `listen` �������������˵� socket ����Ϊ����ģʽ���ȴ��ͻ��˵���������
	iRet = listen(iSocketServer, BACKLOG);
	if (-1 == iRet)
	{	
		// ����ֻ�Ǽ�������ʧ��
		printf("listen error!\n");
		return -1;
	}

	while (1)
	{
		socklen_t iAddrLen = sizeof(struct sockaddr_in);
		iSocketClient = accept(iSocketServer, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
		if (-1 != iSocketClient)
		{	//���ӿͻ��˼�������ӡ�ͻ�����Ϣ��ip��ַ��
			iClientNum++;
			printf("Get connect from client %d : %s\n",  iClientNum, inet_ntoa(tSocketClientAddr.sin_addr));
			//�����ӽ��̴���ͻ���ͨ��
			if (!fork())
			{
				/* �ӽ��̵�Դ�� */
				while (1)
				{
					/* ���տͻ��˷��������ݲ���ʾ���� */
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


