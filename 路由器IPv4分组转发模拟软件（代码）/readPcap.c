#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<errno.h>
#include<netinet/in.h>

#define BUFSIZE 10240
#define STRSIZE 1024

typedef int bpf_int32;
typedef unsigned int bpf_u_int32;
typedef unsigned short  u_short;
typedef unsigned int u_int32;
typedef unsigned short u_int16;
typedef unsigned char u_int8;

//pcap文件头结构体
struct pcap_file_header
{
	bpf_u_int32 magic;
	u_short version_major;
	u_short version_minor;
	bpf_int32 thiszone;
	bpf_u_int32 sigfigs;
	bpf_u_int32 snaplen;
	bpf_u_int32 linktype;
};

//时间戳
struct time_val
{
	int tv_sec;
	int tv_usec;
};

//pcap数据包头结构体
struct pcap_pkthdr
{
	struct time_val ts;
	bpf_u_int32 caplen;
	bpf_u_int32 len;
};

//数据帧头
typedef struct FramHeader_t
{
	u_int8 DstMAC[6];
	u_int8 SrcMAC[6];
	u_short FrameType;
}FramHeader_t;

//IP数据报头
typedef struct IPHeader_t
{
	u_int8 Ver_HLen;
	u_int8 TOS;
	u_int16 TotalLen;
	u_int16 ID;
	u_int16 Flag_Segment;
	u_int8 TTL;
	u_int8 Protocol;
	u_int16 Checksum;
	u_int32 SrcIP;
	u_int32 DstIP;
}IPHeader_t;

typedef struct IP_Link
{
	IPHeader_t *IP_Data;
	struct IP_Link * next;
}IP_Link;

int read_pcap(char *pcapname)
{
	struct pcap_pkthdr *ptk_header;
	FramHeader_t *mac_header;
	IPHeader_t *ip_header;
	IP_Link *head, *data;

	FILE *fp;
	int pkt_offset, i = 0;
	int ip_len, ip_proto;

	char my_time[STRSIZE];
	char src_ip[STRSIZE], dst_ip[STRSIZE];
	int verLen,ttl;
   

	//初始化
	ptk_header = (struct pcap_pkthdr *)malloc(sizeof(struct pcap_pkthdr));
	mac_header = (FramHeader_t *)malloc(sizeof(FramHeader_t));
	ip_header = (IPHeader_t *)malloc(sizeof(IPHeader_t));
	head = (struct IP_Link *)malloc(sizeof(IP_Link));
	data = (struct IP_Link *)malloc(sizeof(IP_Link));

	if ((fp = fopen(pcapname, "r")) == NULL)
	{
		printf("error: can not open pcap file\n");
		exit(0);
	}

	//开始读数据包
	pkt_offset = 24; //pcap文件头结构 24个字节

	head->next = data;
	while(fseek(fp,pkt_offset,SEEK_SET) == 0)
	{
		i++;

		memset(ptk_header,0,sizeof(struct pcap_pkthdr));
		if(fread(ptk_header,16,1,fp) != 1)
		{
			printf("\nread end of pcap file\n");
			break;
		}
		pkt_offset += 16 + ptk_header->caplen;

		struct tm *timeinfo;
		time_t t = (time_t)(ptk_header->ts.tv_sec);
		timeinfo = localtime(&t);
		strftime(my_time,sizeof(my_time),"%Y-%m-%d %H:%M:%S",timeinfo);

		fseek(fp,14,SEEK_CUR);


		memset(ip_header,0,sizeof(IPHeader_t));

		if(fread(ip_header,sizeof(IPHeader_t),1,fp) != 1)
		{
			printf("%d: can not read ip_header\n\n",i);
			break;
		}

		inet_ntop(AF_INET,(void *)&(ip_header->SrcIP),src_ip,16);
		inet_ntop(AF_INET,(void *)&(ip_header->DstIP),dst_ip,16);

		stud_fwd_deal(ip_header,i,my_time);

		ip_proto = ip_header->Protocol;
		verLen = (int)(((ip_header->Ver_HLen)&0xf0)>>4);
		ttl = (int)ip_header->TTL;
		unsigned int address;
		inet_pton(AF_INET,src_ip,&address);

		ip_len = ip_header->TotalLen;

	}

	fclose(fp);
	free(ptk_header);
	free(ip_header);
	return 1;
}




