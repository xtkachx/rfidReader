#include "files.h"
int nProduct = 0;
int rfidEnable = 0;
int checkDoor = 0;

int maxCounterTag = 150;
char strMarker[50] = "EmptyFileBuyProduct";
int flagActiveRfid = 0;
int stateInfo = 0;
int retRfid = 0;

fridge_t listProduct[50];
unsigned char aToHex(unsigned char bufChars)
{
  if (bufChars == '0') return 0;
  if (bufChars == '1') return 1;
  if (bufChars == '2') return 2;
  if (bufChars == '3') return 3;
  if (bufChars == '4') return 4;
  if (bufChars == '5') return 5;
  if (bufChars == '6') return 6;
  if (bufChars == '7') return 7;
  if (bufChars == '8') return 8;
  if (bufChars == '9') return 9;
  if ((bufChars == 'a') || (bufChars == 'A')) return 10;
  if ((bufChars == 'b') || (bufChars == 'B')) return 11;
  if ((bufChars == 'c') || (bufChars == 'C')) return 12;
  if ((bufChars == 'd') || (bufChars == 'D')) return 13;
  if ((bufChars == 'e') || (bufChars == 'E')) return 14;
  if ((bufChars == 'f') || (bufChars == 'F')) return 15;
  return 0;
}
int readFile(int position)
{
  FILE *fileRead;
  int buf = 0;
  if ((fileRead = fopen("../FolderData/Files/FileConnect.txt", "rt")) == NULL){
      printf ("Ошибка чтения файла конфигурации\n");
      return -1;
    }
  for (int i = 0; i <= position; i++){
      fscanf (fileRead, "%*s %d", &buf); //чтение флага включения ридера
    }
  fclose(fileRead);
  return buf;
}
int writeToFileConnect(u8 pos, u8 state)
{
  FILE *file;
  char bufStr[51];
  char bufStrFake[51];
  file = fopen("../FolderData/Files/FileConnect.txt", "r+t");
  if (file == NULL){
      return -1;
    }
  int i = 0;
  //читаем весь файл и записываем в буфер каждую строчку
  while (i < nProduct) {
      if (NULL == fgets (bufStrFake,sizeof(bufStrFake),file)){
          printf ("ERROR READ fgets\n");
          break;
        }
      //если строчка с нужным индексом, записываем ее в нормальный буфер
      if (i == pos){
          for (int k = 0; k < (int)sizeof(bufStr); k++){
              bufStr[k] = bufStrFake[k];
            }
          break;
        }
      i++;
    }
  rewind(file);
  i = 0;
  //доходим до нужной строчки и перезаписываем строку
  for (i=0; i < pos; i++){
      fgets (bufStrFake,sizeof(bufStrFake),file);
    }
  int symbol = 0;
  while (bufStr[symbol] != ' '){
      symbol++;
    }
  symbol++;
  if (state == 1){
      bufStr[symbol] = '1';
    } else {
      bufStr[symbol] = '0';
    }
  fprintf(file, "%s", bufStr);
  fclose(file);
  rFileWStruct();
  return 0;
}
void rewriteBuyFile()
{
  FILE * fProduct;
  FILE * fBuyProduct;
  char bufStr[50];
  fProduct = fopen("../FolderData/Files/FileProduct.txt", "r+t");
  fBuyProduct = fopen("../FolderData/Files/FileBuyProduct.txt", "w+");
  if (fProduct == NULL){
      printf ("Ошибка при записи в файл продуктов\n");
      return;
    }
  if (fBuyProduct == NULL){
      printf ("Ошибка при записи в файл купленнных\n");
      return;
    }
  int i = 0;
  int flagBuyProduct = 0;
  while (i < nProduct) {
      if (NULL == fgets (bufStr,sizeof(bufStr),fProduct)){
          break;
        }
      if (bufStr[0] == '0'){
          flagBuyProduct = 1;
          fprintf(fBuyProduct, "%s", bufStr);
        }
      i++;
    }
  if (flagBuyProduct == 0){
      fprintf(fBuyProduct, "%s", strMarker);
    }
  fclose(fProduct);
  fclose(fBuyProduct);
}
int writeFile(int numberPosProduct, int state)
{
  FILE * fProduct;
  // Переменная, в которую поочередно будут помещаться считываемые строки
  char bufStr[51];
  char bufStrFake[51];
  fProduct = fopen("../FolderData/Files/FileProduct.txt", "r+t");
  if (fProduct == NULL){
      printf ("Ошибка при записи в файл\n");
      return -1;
    } else {
      //      printf ("считаны строки:\n");
    }
  int i = 0;
  //читаем весь файл и записываем в буфер каждую строчкуasdas
  while (i < nProduct) {
      if (NULL == fgets (bufStrFake,sizeof(bufStrFake),fProduct)){
          break;
        }
      //если строчка с нужным индексом, записываем ее в нормальный буфер
      if (i == numberPosProduct){
          for (int k = 0; k < (int)(sizeof(bufStr)); k++){
              bufStr[k] = bufStrFake[k];
            }
          break;
        }
      i++;
    }
  rewind(fProduct);
  i = 0;
  //доходим до нужной строчки и перезаписываем строку
  for (i=0; i < numberPosProduct; i++){
      fgets (bufStrFake,sizeof(bufStrFake),fProduct);
    }
  if (state == 1){
      bufStr[0] = '1';
    } else {
      bufStr[0] = '0';
    }
  fprintf(fProduct, "%s", bufStr);
  fclose(fProduct);
  rFileWStruct();
  rewriteBuyFile();
  return 0;
}
void rewriteProductFile()
{
  FILE *file;
  //printf ("Открытие файла: ");
  file = fopen("../FolderData/Files/FileProduct.txt", "w+");
  if (file == NULL){
      printf ("Ошибка перезаписи файла продуктов\n");
      return;
    }
  for (int i = 0; i < nProduct; i++){
      if (listProduct[i].state == 1){
          fprintf(file, "%d ", listProduct[i].state);
          for (int k = 0; k < (int)sizeof(listProduct->TIDnumber); k++){
              fprintf(file, "%2.2x", listProduct[i].TIDnumber[k]);
            }
          fprintf(file, " %-17s\n",  listProduct[i].marker);
        }
    }
  fclose(file);
}
int rFileWStruct(void)
{
  FILE *file;
  unsigned char bufMas[25] = {0};
  int i = 0;
  //printf ("Открытие файла: ");
  file = fopen("../FolderData/Files/FileProduct.txt", "rt");
  if (file == NULL){
      printf ("Ошибка чтения при записи в структуру\n");
      return -1;
    }
  while (fscanf (file, "%d %s %s", &(listProduct[i].state), bufMas, listProduct[i].marker) != EOF) {
      for (unsigned long k = 0; k < sizeof(listProduct->TIDnumber); k++){
          u8 a;
          u8 b;
          a = aToHex(bufMas[k*2])*16;
          b = aToHex(bufMas[(k*2)+1]);
          listProduct[i].TIDnumber[k] = a + b;
        }
      i++;
    }
  nProduct = i;
  fclose(file);
  return 0;
}
