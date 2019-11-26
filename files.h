#ifndef FILES_H
#define FILES_H
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "CLReader_api.h"
extern int maxCounterTag;
extern int rfidEnable;
extern int checkDoor;
extern int oldState;
extern int oldOut1;
extern int oldOut2;
extern char strMarker[50];
extern int flagActiveRfid;
extern int stateInfo;
extern int retRfid;
extern unsigned char masGPO[4];
extern int nProduct;
typedef struct fridge {
  int state;
  u8 TIDnumber[12];
  char marker[20];
} fridge_t;
extern fridge_t listProduct[50];

unsigned char aToHex(unsigned char bufChars);
int readFile(int position);
int writeToFileConnect(u8 pos, u8 state);
void rewriteBuyFile(void);
int writeFile(int numberPosProduct, int state);
void rewriteProductFile(void);
int rFileWStruct(void);
void printInfo(int state);
#endif // FILES_H
