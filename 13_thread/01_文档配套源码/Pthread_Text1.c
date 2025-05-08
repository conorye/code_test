#include <pthread.h>
#include <stdio.h>

int main()
{
	pthread_t tid = pthread_self(); //这时侯是主线程
	printf("tid = %lu\n",(unsigned long)tid);
	return 0;
}
