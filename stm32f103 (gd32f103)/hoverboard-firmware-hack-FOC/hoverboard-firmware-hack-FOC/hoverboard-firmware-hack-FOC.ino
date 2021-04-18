/*
 * Программа для демонстрации отправки команд управления гироскутером с прошивкой
 * hoverboard-firmware-hack-FOC от EmanuelFeru
 * https://github.com/EmanuelFeru/hoverboard-firmware-hack-FOC
 * 
 * Описание: один раз в TIME_SEND милисекунд формирует пакет управления и посылает
 * его через программный UART. Пин 2 - Rx, пин 3 - Tx. Rx можно не подключать.
 */
#include <SoftwareSerial.h>
//------------------------------------------------------------------------------
#define SERIAL_BAUD         115200      // Скорость порта
#define START_FRAME         0xABCD      // Трогать не надо
#define TIME_SEND           80          // Период отправки команды

#define TxBufSize 8
//------------------------------------------------------------------------------
uint8_t TxBuf [TxBufSize];
SoftwareSerial HoverSerial(2, 3);        // RX, TX
//------------------------------------------------------------------------------
void setup() 
{
  HoverSerial.begin (SERIAL_BAUD);
}
//------------------------------------------------------------------------------
void Send(int16_t uSteer, int16_t uSpeed)
{
  uint16_t sum = START_FRAME ^ uSteer ^ uSpeed; 
  TxBuf [0] = (START_FRAME >> 0) & 0xFF;
  TxBuf [1] = (START_FRAME >> 8) & 0xFF;
  TxBuf [2] = (uSteer      >> 0) & 0xFF;
  TxBuf [3] = (uSteer      >> 8) & 0xFF;
  TxBuf [4] = (uSpeed      >> 0) & 0xFF;
  TxBuf [5] = (uSpeed      >> 8) & 0xFF;
  TxBuf [6] = (sum         >> 0) & 0xFF;
  TxBuf [7] = (sum         >> 8) & 0xFF;

  // Вывод в порт
  HoverSerial.write(TxBuf, TxBufSize); 
}
//------------------------------------------------------------------------------
unsigned long iTimeSend = 0;
//------------------------------------------------------------------------------
void loop(void)
{ 
  unsigned long timeNow = millis();

  // Send commands
  if (iTimeSend > timeNow) 
  return;
  iTimeSend = timeNow + TIME_SEND;
  Send(0, 200);
}
//------------------------------------------------------------------------------
