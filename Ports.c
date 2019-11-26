#include "Ports.h"
int oldState = 0;
unsigned char masGPO[4] = {1,0, 2,0};
int oldOut1 = 0;
int oldOut2 = 0;
u16 port_num = 9090;
u8 real_ip[4] = {192, 168, 0, 116};
TID_READ_PARAM tid_param = {0, 6};
void readPorts(void)  //чтение тегов и установка состояния портов
{
  u8 real_len = 0;
  u8 masGPI[8] = {0};
  GetReaderGpiState(masGPI, sizeof(masGPI), &real_len);
  //запоминаем старое состояние порта, и если оно изменилось, перезаписываем файл
  if (masGPI[1] != oldState){
      if (masGPI[1] == 1){
          writeToFileConnect(3, 0);
        } else {
          writeToFileConnect(3, 1);
        }
      oldState = masGPI[1];
    }
}
void setPorts()
{
  if ((oldOut1 != ((u8)readFile(OUT_1)) || (u8)readFile(OUT_2))){
      masGPO[1] = (u8)readFile(OUT_1);
      masGPO[3] = (u8)readFile(OUT_2);
      oldOut1 = (int)masGPO[1];
      oldOut2 = (int)masGPO[3];
      SetReaderGpoState(masGPO, sizeof(masGPO));
    }
}
