#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <netinet/in.h>


#define BUFSIZE 10240
#define STRSIZE 1024
#define FILE_NAME "PC2_Ethernet0_to_R3_FastEthernet00.pcap"


typedef int bpf_int32;
typedef unsigned int bpf_u_int32;
typedef unsigned short  u_short;
typedef unsigned int u_int32;
typedef unsigned short u_int16;
typedef unsigned char u_int8;

 
//时间戳
struct time_val
{
    int tv_sec;         /* seconds 含义同 time_t 对象的值 */
    int tv_usec;        /* and microseconds */
};
 

//IP数据报头
typedef struct IPHeader_t
{ //IP数据报头
    u_int8 Ver_HLen;       //版本+报头长度
    u_int8 TOS;            //服务类型
    u_int16 TotalLen;       //总长度
    u_int16 ID; 	    //标识
    u_int16 Flag_Segment;   //标志+片偏移
    u_int8 TTL;            //生存周期
    u_int8 Protocol;       //协议类型
    u_int16 Checksum;       //头部校验和
    u_int32 SrcIP; 	   //源IP地址
    u_int32 DstIP; 	   //目的IP地址
} IPHeader_t;

typedef struct IP_Link
{
    IPHeader_t *IP_Data;
    struct IP_Link * next;
} IP_Link;


//TTL为0
#define FORWARD_TTLERROR 0

//Direct delivery直接交付
#define FORWARD_DIRECT_DELIVERY 1

//Indirect delivery间接交付
#define FORWARD_INDIRECT_DELIVERY 2

//
#define FORWARD_NOROUTE 3

typedef struct route_msg
{
    	char dest[STRSIZE];//目的网络
	char mask[STRSIZE];//掩码
    	unsigned int masklen; //掩码长度
    	char nexthop[STRSIZE];//下一跳
} route_msg;

static route_msg route_table[100];
static int table_size = 0;


void route_add(route_msg *proute);

int stud_fwd_deal(IPHeader_t *pBufferm,int number, char *my_time);

int fwd_DiscardPkt(IPHeader_t *pBuffer,int type,int number,char *my_time,char *next_Hop);



void route_add(route_msg *proute)
{
    	
	strcpy(route_table[table_size].dest, proute -> dest);

	strcpy(route_table[table_size].mask, proute -> mask);
	
	route_table[table_size].masklen = proute -> masklen;

	strcpy(route_table[table_size].nexthop, proute -> nexthop);

	table_size++;

}


int fwd_DiscardPkt(IPHeader_t *pBuffer,int type,int number,char *my_time,char *next_Hop)
{
	FILE *fp = NULL;
	char *filename0 = NULL, *filename1 = NULL, *filename2 = NULL;
	char *filename3 = NULL, *filenameD = NULL;

	char src_ip[STRSIZE], dst_ip[STRSIZE];
	int verLen,ttl;
	
	//时间戳
	struct tm *newtime;
	char outfile[128];
	time_t t1;
	time(&t1);
	newtime = localtime(&t1);
	strftime(outfile, 128, "%Y-%m-%d %H:%M:%S.txt", newtime);

	char file0[20] = "TTL_error_";
	char file1[20] = "Direct_";
	char file2[20] = "Indirect_";
	char file3[20] = "Noroute_";
	char fileD[20] = "Default_";
	
	filename0 = (char *)malloc(strlen(file0) + strlen(outfile));
	sprintf(filename0, "%s%s", file0, outfile);

	filename1 = (char *)malloc(strlen(file1) + strlen(outfile));
	sprintf(filename1, "%s%s", file1, outfile);
	
	filename2 = (char *)malloc(strlen(file2) + strlen(outfile));
	sprintf(filename2, "%s%s", file2, outfile);
	

	filename3 = (char *)malloc(strlen(file3) + strlen(outfile));
	sprintf(filename3, "%s%s", file3, outfile);
	
	filenameD = (char *)malloc(strlen(fileD) + strlen(outfile));
	sprintf(filenameD, "%s%s", fileD, outfile);

	if( type == 0){
		fp = fopen(filename0, "a+");
		printf("%s\n",filename0);
		verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

		ttl = (int)pBuffer->TTL;

		inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);

		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

		fprintf(fp,"number:%d\t\t源地址：%s\t\t目的地址：%s\t\tTTL:%d\n",number,src_ip,dst_ip,ttl);
		
	} else if(type == 1) {
		fp = fopen(filename1, "a+");
			printf("%s\n",filename1);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\t下一跳：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,next_Hop,ttl);	
	}else if(type == 2) {
		fp = fopen(filename2, "a+");
			printf("%s\n",filename2);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\t下一跳：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,next_Hop,ttl);
	} else if(type == 3){
		fp = fopen(filename3,"a+");
			printf("%s\n",filename3);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,ttl);
	}else {
		fp = fopen(filenameD, "a+");
			printf("%s\n",filenameD);
			verLen = (int)(((pBuffer->Ver_HLen) & 0xf0) >> 4);

			ttl = (int)pBuffer->TTL;

			inet_ntop(AF_INET, (void *)&(pBuffer->SrcIP), src_ip, 16);
 
        		inet_ntop(AF_INET, (void *)&(pBuffer->DstIP), dst_ip, 16);

			fprintf(fp,"number:%d\t\tTIME:%s\t\t源地址：%s\t\t目的地址：%s\t\tTTL:%d\n",number,my_time,src_ip,dst_ip,ttl);

	}

	fclose(fp);				
}


//查找路由表,确定下一步处理流程(丢弃/处理/转发)
int stud_fwd_deal(IPHeader_t *pBuffer,int number,char *my_time)
{
    	//获取目的地址，ttl,校验和
	unsigned int dest_address = pBuffer -> DstIP;
    	unsigned int ttl = (int)pBuffer -> TTL;

	
	unsigned int IPAddress,maskAddress;

	
    	//检验TTL，如果为0则丢弃：
    	if(ttl <= 0)
    	{
		printf("TTL error 数据包丢弃！\n");
        	fwd_DiscardPkt(pBuffer,FORWARD_TTLERROR, number, my_time, NULL);
        	return 1;
    	}

	int i;
	//如果目的地址是本机，接收
	//查找路由表，下一跳为直接交付
	for(i = 0;i < table_size; i++)
	{

		//对传入的IP数据包的目的地址与路由表的掩码做与操作得出目的网络，在路由表里遍历
		printf("路由表第%d个： dest:%s,nexthop:%s\n",i,route_table[i].dest, route_table[i].nexthop);

		inet_pton(AF_INET, route_table[i].dest, &IPAddress);
		inet_pton(AF_INET, route_table[i].mask, &maskAddress);
		
		char string[1024] = "直接交付";
		if((strcmp(route_table[i].nexthop,string) == 0) && IPAddress == (dest_address & maskAddress))
		{
			printf("直接交付\n");
			fwd_DiscardPkt(pBuffer,FORWARD_DIRECT_DELIVERY, number, my_time, route_table[i].nexthop);
			return 0;
		}
	}
	unsigned int max_masklen = 0;
    	char nexthop[STRSIZE];

//    如果目的地址不是本机
//    按照最长匹配查找路由表获取下一跳

    	for(i = 0;i < table_size;i++)
   	{
       		unsigned int masklen = route_table[i].masklen;

		inet_pton(AF_INET, route_table[i].dest, &IPAddress);
		inet_pton(AF_INET, route_table[i].mask, &maskAddress);
	
		printf("IPAddress:%d,maskAddress:%d,dest_address & maskAddress:%d\n",IPAddress, maskAddress, (dest_address & maskAddress));
	
        	if (route_table[i].masklen > max_masklen && IPAddress == (dest_address & maskAddress))
        	{
            		max_masklen = masklen; 
	   		strcpy(nexthop, route_table[i].nexthop);
			printf("route_table[table_size].nexthop:%s,nexthop:%s\n",route_table[i].nexthop,nexthop);
        	} 
    	}

    //查找失败
    if(max_masklen == 0)
    {
        printf("路由表内无路由\n");
        fwd_DiscardPkt(pBuffer, FORWARD_NOROUTE, number, my_time, NULL);
        return 1;
    }

	printf("间接发送，下一跳地址：%s\n",nexthop);
    	fwd_DiscardPkt(pBuffer,FORWARD_INDIRECT_DELIVERY, number, my_time, nexthop);
    	return 0;

	
}
