/*
 Name:		RFIDLock.ino
 Created:	11/5/2017 3:59:33 PM
 Author:	Mads
*/
#include <SPI.h>
#include <MFRC522.h>

#define KeySize 10
#define MaxKnownKeys 3

#define ACCESS_DELAY 3000

#define NEWKEYBUTTON_PIN 5

#define GREEN_PIN 6
#define RED_PIN 7

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

bool allowNewKey = false;
byte knownKeysAmount = 0;
byte KnownKeys[MaxKnownKeys][KeySize];

void setup() {
	pinMode(NEWKEYBUTTON_PIN, INPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(RED_PIN, OUTPUT);
	Serial.begin(9600);	// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();	// Init MFRC522 card
	Serial.println("Scan PICC to see UID and type...");
}

void loop() {

	if (digitalRead(NEWKEYBUTTON_PIN))
	{
		if (knownKeysAmount >= MaxKnownKeys)
		{
			digitalWrite(RED_PIN, HIGH);
			delay(250);
			digitalWrite(RED_PIN, LOW);
			delay(250);
			digitalWrite(RED_PIN, HIGH);
			delay(250);
			digitalWrite(RED_PIN, LOW);
			delay(250);
			digitalWrite(RED_PIN, HIGH);
			delay(250);
			digitalWrite(RED_PIN, LOW);
			allowNewKey = false;
			return;

		}
		allowNewKey = true;
		delay(ACCESS_DELAY);
	}

	// Look for new cards
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	
	if (allowNewKey)
	{
		for (int i = 0; i < KeySize; i++)
		{
			KnownKeys[knownKeysAmount][i] = mfrc522.uid.uidByte[i];
		}
		allowNewKey = false;
		knownKeysAmount++;
		digitalWrite(GREEN_PIN, HIGH);
		digitalWrite(RED_PIN, HIGH);
		delay(ACCESS_DELAY);
		digitalWrite(RED_PIN, LOW);
		digitalWrite(GREEN_PIN, LOW);
		return;
	}
	
	else
	{
		bool test = false;
		for (byte x = 0; x < knownKeysAmount; x++)
		{
			for (byte i = 0; i < KeySize; i++)
			{
				if (KnownKeys[x][i] != mfrc522.uid.uidByte[i])
				{
					goto label;
					
				}
			}
			test = true;
			break;
		label:;
		}

		if (test)
		{
			Serial.println("Access Granted");
			digitalWrite(GREEN_PIN, HIGH);
			delay(ACCESS_DELAY);
			digitalWrite(GREEN_PIN, LOW);
		}
		else
		{
			Serial.println("Access Denied");
			digitalWrite(RED_PIN, HIGH);
			delay(ACCESS_DELAY);
			digitalWrite(RED_PIN, LOW);
		}


	}
	
}