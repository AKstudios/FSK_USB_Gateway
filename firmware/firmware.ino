// FSK USB Gateway firmware
// Updated on 11/2/2018

#include <RFM69.h>         //http://github.com/lowpowerlab/rfm69
//#include <SPIFlash.h>      //http://github.com/lowpowerlab/spiflash
#include <SPI.h>           

#define ENCRYPTKEY    "Tt-Mh=SQ#dn#JY3_" //has to be same 16 characters/bytes on all nodes, not more not less!
#define NODEID        1
#define NETWORKID     1
#define FREQUENCY     RF69_915MHZ //(others: RF69_433MHZ, RF69_868MHZ)
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#define LED           9
#define SERIAL_BAUD   115200
//*****************************************************************************************************************************

RFM69 radio;
//SPIFlash flash(FLASH_CS, 0xEF30); //EF40 for 16mbit windbond chip
char data[100];
char _rssi[5];

void setup()
{
  pinMode(10, OUTPUT);
  Serial.begin(SERIAL_BAUD);

  radio.initialize(FREQUENCY,NODEID,NETWORKID);
#ifdef IS_RFM69HW
  radio.setHighPower(); //uncomment only for RFM69HW!
#endif
  radio.encrypt(ENCRYPTKEY);
  //flash.initialize();
}

void loop()
{
  if (radio.receiveDone())
  {
    int rssi = radio.RSSI;

    if (radio.DATALEN > 0)
    {
      for (byte i = 0; i < radio.DATALEN; i++)
        data[i] = (char)radio.DATA[i];
    }
    
    if (radio.ACKRequested())
    {
      //byte theNodeID = radio.SENDERID;
      radio.sendACK();

      dtostrf(rssi, 3, 0, _rssi);
      strcat(data, ",r:");
      strcat(data, _rssi);
      
      Serial.println(data);
      delay(1);
      
      Blink(LED,5);
      
      memset(data, 0, sizeof data);
      memset(_rssi, 0, sizeof _rssi);
    }
  }
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}
