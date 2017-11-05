/*
 Name:		RFIDLock.ino
 Created:	11/5/2017 3:59:33 PM
 Author:	Mads
*/
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

bool firstTime = true;
byte test[10];

void setup() {
	Serial.begin(9600);	// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();	// Init MFRC522 card
	Serial.println("Scan PICC to see UID and type...");
}

void loop() {
	// Look for new cards
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	
	if (firstTime)
	{
		for (int i = 0; i < 10; i++)
		{
			test[i] = mfrc522.uid.uidByte[i];
		}
		firstTime = false;
		delay(5000);
		return;
	}
	
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (test[i] != mfrc522.uid.uidByte[i])
			{
				Serial.println("Access Denied");
				delay(5000);
				return;
			}
		}
		Serial.println("Access Granted");
		delay(5000);
	}


	//for (int i = 0; i < 10; i++)
	//{
	//	Serial.print(mfrc522.uid.uidByte[i], HEX);
	//}

	
}