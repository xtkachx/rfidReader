#ifndef CLREADER_API_H
#define CLREADER_API_H
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<stdlib.h>

#include <signal.h>
#include <sys/ioctl.h>

typedef unsigned char  u8 ;
typedef unsigned short u16 ;
typedef unsigned long  u32 ;

typedef unsigned char  U8 ;
typedef unsigned short U16 ;
typedef unsigned long  U32 ;

typedef unsigned char  UINT8 ;
typedef unsigned short UINT16 ;
typedef unsigned long  UINT32 ;

typedef enum 
{
	B_9600      =   0,
	B_19200     =   1, 
	B_115200    =   2, 
}eBAUD;

typedef enum 
{
	ANT_1      =   1 << 0,
	ANT_2      =   1 << 1,
	ANT_3      =   1 << 2,
	ANT_4      =   1 << 3,
	ANT_5      =   1 << 4,
	ANT_6      =   1 << 5,
	ANT_7      =   1 << 6,
	ANT_8      =   1 << 7,
}eANT;

typedef enum 
{
	SINGLE    = 0,
	INVENTORY = 1,
}eREAD_TYPE;


typedef enum
{
	REGION_GB1 = 0,
	REGION_GB2 = 1,
	REGION_GB3 = 2,
	REGION_FCC = 3,
	REGION_ETSI = 4,
	REGION_JP = 5,
	REGION_TW = 6,
	REGION_ID = 7,
	REGION_RUS = 8,
}eRF_REGION;

typedef enum  //frequency switch mode
{
	SEQUEN_SWITCH = 0,
	AUTO_SWITCH = 0,
}eFREQ_JMP;

typedef struct tag_read_s
{
	u16 read_pos;
	u16 read_len;
}TAG_READ_PARAM;

typedef struct tid_read_s
{
	u8  mode;
	u8  read_len;
}TID_READ_PARAM;

typedef struct tag_read_mach_s
{
	u8  mach_bank;
	u16 mach_pos;
	u8 mach_len;
	u8 mach_payload[256];
}TAG_READ_MACH_PARAM;

typedef struct CL_TAG
{
	int TagType;	
	u8 ReaderName[50];	
	u8 ReaderName_len;		
	u8 EPC[1024];	
	u16 EPC_len;	
	u8 TID[1024];	
	u16 TID_len;	
	u8 UserData[1024];
	u16 UserData_len;		
	u8 PC[10];	
	u8 PC_len;		
	u8 ANT_NUM;	
	u8 RSSI;	
	u8 Result;	
	u8 TagetData[1024];	
	u16 TagetData_len;		
	u8 sub_ant_num;
}S_TAG;

typedef struct ability_s
{
	u8 min_power;
	u8 max_power;
	u8 ant_nums;
	u8 freq_list[60];
	u8 protocol_list[5];
}RFID_ABILITY_S;

//-------------------------callback function, return tag struct------------------------------------

typedef int (*CL_CALLBACK)(S_TAG*);

//connecttion
extern int CreateSerialConn(u8 *com_name, eBAUD baud, CL_CALLBACK clcall);

extern int CreateSerialRs485Conn(u8 *com_name, eBAUD baud, u8 addr, CL_CALLBACK clcall);

extern int CreateTCPConn(u8 *IP, u16 port, CL_CALLBACK clcall);


//-------------------------------rfid operation---------------------------------------------------
/*
antNum, ANT_1 | ANT2 ...
readType, 0--SINGLE, 1--INVENTORY
*/
int GetEpc(u8 antNum, eREAD_TYPE readType,
	   TAG_READ_MACH_PARAM *mach_param,
	    TID_READ_PARAM *read_tid, TAG_READ_PARAM *read_user, TAG_READ_PARAM *read_reserve,\
            u8 *pwd);

int GetReaderProperty(RFID_ABILITY_S *ability);

int GetReaderBaseBandSoftVersion(u8 *base_version, u8 max_len, u8 *real_len);

int GetANTPowerParam(u8 *power_buff, u8 max_len, u8 *real_len);

int SetANTPowerParam(u8 *power_buff, u8 len);

int SetReaderRF(eRF_REGION region);

int GetReaderRF(eRF_REGION *region);

int SetReaderWorkFrequency(eFREQ_JMP mode, u8 *freq_list, u8 len);

int GetReaderWorkFrequency(eFREQ_JMP *mode, u8 *buff, u8 max_len, u8 *real_len);

int SetReaderANT(u8 config);

int GetReaderANT(u8 *config);

int SetTagUpdateParam(u16 filter_time, u8 rssi);

int GetTagUpdateParam(u16 *filter_time, u8 *rssi);

int SetEPCBaseBandParam(u8 base_speed, u8 Q_value, u8 session, u8 inventory_flag);

int GetEPCBaseBandParam(u8 *base_speed, u8 *Q_value, u8 *session, u8 *inventory_flag);

int Stop(void);


/////////////////system param operation ///////////////////////

int SetReaderNetworkParam(u8 *buff);

int GetReaderNetworkParam(u8 *buff);

int SetReaderUTC(u32 utc_sec, u32 utc_usec);

int GetReaderUTC(u32 *utc_sec, u32 *utc_usec);

int SetReaderSerialPortParam(eBAUD baudrate);

int GetReaderSerialPortParam(eBAUD *baudrate);

int SetReaderMacParam(u8 *buff);

int GetReaderMacParam(u8 *buff);

int SetReader485(u8 addr, eBAUD baudrate);

int GetReader485(u8 *addr, eBAUD *baudrate);

int GetReaderInformation(u8 *info, u8 max_len, u8 *real_len);

int GetReaderGpiState(u8 *buff, u8 max_len, u8 *real_len);

int SetReaderGpoState(u8 *buff, u8 gpo_len);

int SetReaderWG(u8 on_off, u8 format, u8 payload);

int GetReaderWG(u8 *on_off, u8 *format, u8 *payload);

int readFile(int);
int rFileWStruct(void);
void readPorts (void);
int useDataRfid(void);
unsigned char aToHex(unsigned char bufChars);
int writeToFileConnect(u8, u8);
int writeFile(int numberPosProduct, int state);
void rewriteBuyFile(void);
void rewriteProductFile(void);
void setPorts(void);
void printInfo(int state);
void readerStop(void);

/*
SetDHCP

GetDHCP

SetReaderServerOrClient

GetReaderServerOrClient
*/
#endif
