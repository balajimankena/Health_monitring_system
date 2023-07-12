#include <SPI.h>

#include <Wire.h>

#include <Adafruit_SSD1306.h>

#include <Adafruit_GFX.h>

#include <ESP8266WiFi.h>

#include <ThingSpeak.h>

Adafruit_SSD1306 display(128,64,&Wire);

WiFiClient client;

const int sensorPin = A0;                               // A0 is the input pin for the heart rate sensor

int sensorValue;                                        // Variable to store the value coming from the sensor

int count = 0;

unsigned long starttime = 0;

int heartrate = 0;

boolean counted = false;

long myChannelNumber = 2099478;

const char myWriteAPIKey[] = "9U8F67EB6SYT8M6Q";

void setup (void)

  { display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    pinMode (D8, OUTPUT);                              // D8 LED as Status Indicator

    Serial.begin (115200);                             // Start Serial Communication @ 115200

    display.clearDisplay();

    WiFi.begin("Semicon Media 2.4", "cdfiP29to665");

      while(WiFi.status() != WL_CONNECTED)

      {delay(200);

        Serial.print("..");}

      Serial.println();

      Serial.println("NodeMCU is connected!");

      Serial.println(WiFi.localIP());

      ThingSpeak.begin(client);

    }

void loop ()

  { starttime = millis();

    while (millis()<starttime+20000)                                            // Reading pulse sensor for 20 seconds

    { sensorValue = analogRead(sensorPin);

        //Serial.println (sensorValue);

          delay(50);

      if ((sensorValue >= 590 && sensorValue <=680) && counted == false)       // Threshold value is 590 (~ 2.7V)

      {  count++;

        digitalWrite (D8,HIGH);

          delay (10);       

        digitalWrite (D8, LOW);

        counted = true;

        }

      else if (sensorValue < 590)

       {  counted = false;

          digitalWrite (D8, LOW);

          }

      }

    Serial.print ("Pulse ");

    Serial.println (count);

    heartrate = (count)*3;                              // Multiply the count by 3 to get beats per minute

    Serial.println ();

    Serial.print ("BPM = ");

    Serial.println (heartrate);                         // Display BPM in the Serial Monitor

    Serial.println ();

    count = 0;

    display.clearDisplay();

    display.setTextColor(WHITE);

    display.setCursor(0,0);

    display.setTextSize(2);

    display.println("Heart Rate");

    display.setCursor(0,28);

    display.print("BPM: ");

    display.print(heartrate);

    display.display();

    ThingSpeak.writeField(myChannelNumber, 1, heartrate, myWriteAPIKey);

}