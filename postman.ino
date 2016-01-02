/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Postman v1.0
#include <SoftwareSerial.h>
SoftwareSerial gsm(7, 8);

int sensitivity = 6;
int sensor = 0;
int state = 1;
int ledPin = 2;
int sensorPin = A0;

// Phone nuber including the country code
char phone[] = "+336XXXXXXXX";
// Text message to send
char message[] = "You've got mails !";

void setup() 
{
  gsm.begin(19200);
  Serial.begin(19200);
  Serial.print("Postman v1.0\n");

  power();
   
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  delay(500);
  Serial.print("Ready\n\n");
}

void loop() 
{
  // Magic. Do not touch.
  if (sensor < sensitivity && !state)
  {
    Serial.print("Got mails !\n");

    // Send a sms
    sendSms(phone, message);
    state = !state;
  }
  else if (sensor > sensitivity && state)
  {
    state = !state;
  }

  // Power on led and read sensor
  digitalWrite(ledPin, HIGH); 
  sensor = analogRead(sensorPin);

  // Secure delay
  delay(500);
}

void power()
{ 
  // Power on gsm module
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);

  // Delay for network registration
  Serial.print("Scanning GSM network..\n");
  delay(10000);
}

void sendSms(char number[], char sms[])
{
  // If you think it doesn't work, think again
  
  // Enable text mode
  gsm.print("AT+CMGF=1\r");
  delay(100);

  // Set phone number
  gsm.print("AT + CMGS = \"");
  gsm.print(number);
  gsm.print("\"\r");// 
  delay(100);

  // Set text message
  gsm.println(sms);
  delay(100);

  // Special char to end the message
  gsm.println((char)26);
  delay(100);
  
  gsm.println();
}

