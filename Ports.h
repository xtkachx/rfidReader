#ifndef PORTS_H
#define PORTS_H
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "CLReader_api.h"
#define RFID_ENABLE 0
#define OUT_1 1
#define OUT_2 2
#define CHECK_DOOR 3
extern int oldState;
extern int oldOut1;
extern int oldOut2;
extern unsigned char masGPO[4];
extern u16 port_num;
extern u8 real_ip[4];
extern TID_READ_PARAM tid_param;
void readPorts(void);  //чтение тегов и установка состояния портов
void setPorts(void);
#endif // PORTS_H
