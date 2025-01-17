#include <SPI.h>
#include<MFRC522.h>
#include <Ethernet.h>

#define SS_PIN 10 
#define RST_PIN 9
#define No_Of_Card 3
#define Access_granted 7




MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key; 
byte id[No_Of_Card][4]={
  {250,133,711,79},             //RFID NO-1 25,013,371,179 card1 not allowed
  {250,184,29,64},             //RFID NO-2 250,184,29,64 key holder allowed
  {84,200,135,137}              //RFID NO-3 84,200,135,137 golden arrow allowed
};
byte id_temp[3][3];
byte i;
int j=0;

void setup() {
  Serial.begin(9600);
  pinMode(Access_granted, OUTPUT);

  SPI.begin();
  rfid.PCD_Init();
  for(byte i=0;i<6;i++)
  {
    key.keyByte[i]=0xFF;
  }
}
//------------------------------------------------------------------------------


/* Infinite Loop */
void loop()
{int m=0;
  if(!rfid.PICC_IsNewCardPresent())
  return;
  if(!rfid.PICC_ReadCardSerial())
  return;
  for(i=0;i<4;i++)
  {
   id_temp[0][i]=rfid.uid.uidByte[i]; 
             delay(50);
  }
  
   for(i=0;i<No_Of_Card;i++)
  {
          if(id[i][0]==id_temp[0][0])
          {
            if(id[i][1]==id_temp[0][1])
            {
              if(id[i][2]==id_temp[0][2])
              {
                if(id[i][3]==id_temp[0][3])
                {
                  Serial.println("Card detected:");
                  for(int s=0;s<4;s++)
                  {
                    Serial.print(rfid.uid.uidByte[s]);
                  }
                   Serial.print(" ");
                  Sending_To_db();
                  j=0;
                            
                            rfid.PICC_HaltA(); rfid.PCD_StopCrypto1();   return; 
                }
              }
            }
          }
   else
   {j++;
    if(j==No_Of_Card)
    {
      Serial.println("Card detected:");
      for(int s=0;s<4;s++)
        {
          Serial.print(rfid.uid.uidByte[s]);
        }
      Serial.print(" ");
      Sending_To_db();
      j=0;
    }
   }
  }
  
     // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
 }

 void Sending_To_db()  
 {
    if(j!=No_Of_Card)
    {
      Serial.print('1');
      Serial.print(" ");
      digitalWrite(Access_granted, HIGH);
      delay(1000);
      digitalWrite(Access_granted, LOW);
      delay(1000);
//      Serial.print(" ");
    }
    else
    {
      Serial.print('0');
      Serial.print(" ");
      Serial.print("Access denied");
      Serial.print(" ");
    }
 }
