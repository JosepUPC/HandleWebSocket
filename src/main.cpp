/* aquest es un codi introductori a l'us d'emagatzematge de dades 
en la memoria flash de la esp32-s3 amb taskques */

#include <Arduino.h>
#include <FreeRTOS.h>
#include <SPIFFS.h>

TaskHandle_t * handleRead;
TaskHandle_t * handleWrite;
volatile uint8_t value = 1;
volatile uint8_t buff;

//init les funcions de les tasques a determinar
void writeData(void * parameters);
void readData(void * parameters);

void setup(){
  Serial.begin(115200);

  //init el codi amb llibreries SPIFFS
  if(!SPIFFS.begin(true))
    Serial.println("SPIFFS can't begin");
  else
    Serial.println("SPIFFS succesfully begin");

  //creem les taskques
  xTaskCreatePinnedToCore(
    writeData,
    "Escriptura",
    1024*4,
    NULL,
    1,
    handleWrite,
    0
  );
  xTaskCreatePinnedToCore(
    readData,
    "Lectura",
    1024*4,
    NULL,
    2,
    handleRead,
    1
  );
}

void loop(){
}
//def la tasca d'escriptura amb un periode determinat
void writeData(void * parameters)
{
  for(;;)
  {
    File file = SPIFFS.open("/val.bin",FILE_WRITE);
    file.printf("%d.\n",value);
    file.close();
    value = value*(-1);
    vTaskDelay(22);
  }
}

//def la tasca d'escriptura amb un periode determinat
void readData(void * parameters)
{
  for(;;)
  {
    if(SPIFFS.exists("/val.bin")){
      File file = SPIFFS.open("/val.bin",FILE_READ);
      if(!file)
        Serial.println("couldn't read the file");
      else{
        buff=file.read();
        Serial.printf("%d.\n",buff);
      file.close();
      }
    }
    vTaskDelay(3000);
  }
}