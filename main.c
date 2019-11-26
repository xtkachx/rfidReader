#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "CLReader_api.h"
#include <time.h>
#include "files.h"
#include "Ports.h"
#include "main.h"
#define RFID_ENABLE 0
#define OUT_1 1
#define OUT_2 2
#define CHECK_DOOR 3

int masCounterTag[50] = {0};

int callback_test(S_TAG* tag_data)
{
  int posFindTag = -1;
  //  printf("call back test ...\n");
  if(NULL != tag_data) {
      if(0 < tag_data->TID_len) {
          for (int i=0; i<nProduct; i++){
              //перебираем все байты, если байт равен, переходим к следующему
              for (int nByte = 0; nByte < tag_data->TID_len; nByte++){
                  if (listProduct[i].TIDnumber[nByte] == tag_data->TID[nByte]){
                      if (nByte == (tag_data->TID_len - 1)){
                          posFindTag = i;
                        }
                    } else {
                      break;
                    }
                }
            }
          for (int m = 0; m < nProduct; m++){
              if (m == posFindTag){
                  masCounterTag[m] = 0;
                  //                  printf( "%d, ",  m);
                  if (listProduct[m].state == 0){
                      listProduct[m].state = 1;                     //изменим значение в структуре
                      writeFile(m, 1);
                    }
                } else {
                  masCounterTag[m]++;                               //иначе прибавим к его счетчику "ненахода" +1
                }
              if (listProduct[m].state == 1){
                  if (masCounterTag[m] >= maxCounterTag) {          //если достигли критического значения                    !!!!!!!изменить обращение к файлу? обращаться, только если это нужно делать
                      listProduct[m].state = 0;                     //изменим значение в структуре
                      writeFile(m, 0);                              //перепишем файл
                    }
                }
            }
        }
    }
  rFileWStruct();
  return 0;
}


void printInfo(int state)
{
  if (state != stateInfo){
      if (state == 1){
          printf("Считыватель активен(!), дверь закрыта!\n");
          stateInfo = state;
        }
      if (state == 2){
          printf("Считыватель активен(!), дверь открыта(!)!\n");
          stateInfo = state;
        }
      if (state == 3){
          printf("Дверь закрыли, ожидаем изменения RFID_ENABLE! Совершить Транзакцию! \n");
          stateInfo = state;
        }
      if (state == 5){
          printf("Недоступное состояние!\n");
          stateInfo = state;
        }
      if (state == 6){
          printf("Считыватель не активен, дверь закрыта!\n");
          stateInfo = state;
        }

    }
}
void readerStop()
{
  Stop();
  flagActiveRfid = 0;
}
int main(void)
{
  printf ("Старт! \n");
  int ret = 0;
  int flagTransaction = 0;
  u16 port_num = 9090;
  u8 real_ip[4] = {192, 168, 0, 116};
  rFileWStruct();
  rewriteBuyFile();
  ret = CreateTCPConn(real_ip, port_num, callback_test);
  readerStop();
  sleep(1);
  TID_READ_PARAM tid_param;
  tid_param.mode =
  tid_param.mode = 0;
  tid_param.read_len = 6;
  while(1){
      usleep(10000);
      if (!(readFile(RFID_ENABLE)) &&
          !(readFile(CHECK_DOOR))){
          //пока не активировали считыватель и дверь закрыта | RFID_ENABLE - 0 CHECK_DOOR - 0
          printInfo(6);
          readPorts();
          readerStop();
          setPorts();
        }
      if ((readFile(RFID_ENABLE)) && // | RFID_ENABLE - 1 | CHECK_DOOR - 0
          !(readFile(CHECK_DOOR)) &&
          (flagTransaction == 0)){
          // активировали считыватель дргуой прогой, ждем открытия двери
          printInfo(1);
          if (!flagActiveRfid){
              flagActiveRfid = 1;
              retRfid = GetEpc(ANT_1 | ANT_2 | ANT_3 | ANT_4 | ANT_5, INVENTORY, NULL, &tid_param, NULL, NULL, NULL);
              printf("Reader ON\n");
            }
          readPorts();
          setPorts();
        }
      if ((readFile(RFID_ENABLE)) && // | RFID_ENABLE - 1 CHECK_DOOR - 1
          (readFile(CHECK_DOOR))){
          //пока дверь открыта читаем порты и устанавляиваем состояние портов| RFID_ENABLE - 0 CHECK_DOOR - 0
          printInfo(2);
          flagTransaction = 1;
          readPorts();
          setPorts();
        }
      if ((readFile(RFID_ENABLE)) && // | RFID_ENABLE - 1 CHECK_DOOR - 0
          !(readFile(CHECK_DOOR)) &&
          (flagTransaction == 1)){
          printInfo(3);
          while (readFile(RFID_ENABLE)){
              //закрыли дверь, нужно провести транзакцию, и ждать RFID_ENABLE = 0;
              usleep(10000);
              flagTransaction = 0;
              readPorts();
            }
          rewriteProductFile();
          rewriteBuyFile();
          rFileWStruct();
          readPorts();
        }
      if (!(readFile(RFID_ENABLE)) && // | RFID_ENABLE - 0 CHECK_DOOR - 1
          (readFile(CHECK_DOOR))){
          printInfo(5);
          readerStop();
          readPorts();
        }
    }
}
