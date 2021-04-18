/*
 * Программа для демонстрации отправки команд управления гироскутером с прошивкой
 * Hoverboard-Firmware-Hack-Gen2
 * https://github.com/flo199213/Hoverboard-Firmware-Hack-Gen2
 * 
 * Описание: один раз в TIME_SEND милисекунд формирует пакет управления и посылает
 * его через программный UART. Пин 2 - Rx, пин 3 - Tx. Rx можно не подключать.
 */
#include <SoftwareSerial.h>
//------------------------------------------------------------------------------
#define SERIAL_BAUD         19200       // Скорость порта
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
uint16_t CalcCRC(uint8_t *ptr, int count) 
{
  uint16_t crc;
  uint8_t i;
  crc = 0;
  while (--count >= 0) {
    crc = crc ^ (uint16_t)*ptr++ << 8;
    i = 8;
    do {
      if (crc & 0x8000) {
        crc = crc << 1 ^ 0x1021;
      } else {
        crc = crc << 1;
      }
    } while (--i);
  }
  return (crc);
}
//------------------------------------------------------------------------------
void Send(int16_t uSteer, int16_t uSpeed)
{
  TxBuf [0] = '/';
  TxBuf [1] = (uSpeed      >> 8) & 0xFF;
  TxBuf [2] = (uSpeed      >> 0) & 0xFF;
  TxBuf [3] = (uSteer      >> 8) & 0xFF;
  TxBuf [4] = (uSteer      >> 0) & 0xFF;

  uint16_t crc = CalcCRC (TxBuf, 5);
  
  TxBuf [5] = (crc         >> 8) & 0xFF;
  TxBuf [6] = (crc         >> 0) & 0xFF;
  TxBuf [7] = '\n';

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
  Send(0, 300);
}
//------------------------------------------------------------------------------
