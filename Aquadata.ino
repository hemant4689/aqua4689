#include <stdio.h>
#include <DHT.h>
#include <Wire.h> //i2c communication
#include <LiquidCrystal.h>
#include <DS3231.h> 
#include <SPI.h> //Library for SPI communication (Pre-Loaded into Arduino)
#include <SD.h> //Library for SD card (Pre-Loaded into Arduino)


#define dht1_PIN 9 //dht1 connected to pin 9
#define dht2_PIN 8 //dht2 connected to pin 8

#define dht1type DHT11 //dht11
#define dht2type DHT22 //dht22

DHT DHT1(dht1_PIN, dht1type);
DHT DHT2(dht2_PIN, dht2type);

LiquidCrystal lcd(3,2,34,36,38,40); // (RS, E, D4, D5, D6, D7)
const int chipSelect = 53; //SD card CS pin connected to pin 53 of Arduino
float Temp1, Temp2,soilm,t1,t2,h1,h2;

int tempPin1 = A0, tempPin2=A1;
int window = 6;
int fanout = 11;
int a,b; // a is window output and b is exhaustfan output


long time_curr = 0;

// Init the DS3231 using the hardware interface

DS3231  rtc(SDA, SCL);

void setup()
{
  // Setup Serial connection
  Serial.begin(9600);
  lcd.begin(16,2);
  DHT1.begin();
  DHT2.begin();
  Initialize_SDcard();
  Initialize_RTC();
  

  pinMode(tempPin1, INPUT);
  pinMode(tempPin2, INPUT);
  pinMode(window, OUTPUT);
  pinMode(fanout, OUTPUT);
  
}



void loop()

{
  time_curr = millis();

  if(time_curr > 60000)
  {
    Write_SDcard();
    time_curr = 0;
  }

   h1 = DHT1.readHumidity();
   t1 = DHT1.readTemperature();
   h2 = DHT2.readHumidity();
   t2 = DHT2.readTemperature();
  
       
   if(t2>35)
   {
      digitalWrite(window, HIGH);
      a= 0; 
   }
   else
   {
     digitalWrite(window,LOW);
     a= 1;
   }
   
   if(t2<30)
   {
    digitalWrite(fanout, HIGH);
    b=0;
    }
    else
    {
    digitalWrite(fanout, LOW);
    b=1; 
    }
   
  
  
  Temp1 = (0.5*(analogRead(tempPin1)));
  Temp2 = analogRead(tempPin2);  
  
  
  lcd.setCursor (0,0);
  lcd.print(t2);
  lcd.setCursor (6,0);
  lcd.print(t1);
  lcd.setCursor (11,0);
  lcd.print(Temp1,1);
  lcd.setCursor (0,1);
  lcd.print(h2);
  lcd.setCursor (6,1);
  lcd.print(h1);
  lcd.setCursor (11,1);
  lcd.print(Temp2,1);

  Serial.println(t2, 1);
  Serial.println(h2, 1);
  Serial.println(rtc.getDateStr());
  Serial.println(rtc.getTimeStr());
  Serial.println(Temp1, 1);
  delay (10000);

  }


void Write_SDcard()
{
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("GHdata1.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDateStr()); //Store date on SD card
    dataFile.print("       "); //Move to next column using a ","

    dataFile.print(rtc.getTimeStr()); //Store date on SD card
    dataFile.print("       "); //Move to next column using a ","
    
    dataFile.print(t1); //Store date on SD card
    dataFile.print("°       "); //Move to next column using a ","

    dataFile.print(t2); //Store date on SD card
    dataFile.print("°       "); //Move to next column using a ","
    
    dataFile.print(h1); //Store date on SD card
    dataFile.print("   "); //Move to next column using a ","

    dataFile.print(h2); //Store date on SD card
    dataFile.print("   "); //Move to next column using a ","

    dataFile.print(Temp1); //Store temp1 on SD card
    dataFile.print("°   ");

    dataFile.print(Temp2); //Store temp2 on SD card
    dataFile.print("°        ");

    dataFile.print(a); //Store temp2 on SD card
    dataFile.print("          ");

    dataFile.print(b); //Store temp2 on SD card
    dataFile.print("   ");
    
    dataFile.println(); //End of Row move to next row
    dataFile.close(); //Close the file
  }
  else
  Serial.println("OOPS!! SD card writing failed");
}

void Initialize_SDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("GHdata1.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("Date             Time           TempDHT11   TempDHT22   H11    H22   Temp1    Temp2     WaterPump   ExhauFan"); //Write the first row of the excel file
    dataFile.close();
    Serial.println("data being printed on SD card");
  }
}

void Initialize_RTC()
{
   // Initialize the rtc object
  rtc.begin();
}

void Read_DHT11()
{
int chk = DHT1.read(dht1_PIN);
}
void Read_DHT22()
{
 int chk = DHT2.read(dht2_PIN);
  }

/*void Read_DateTime()
{  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());
}*/

/*void Read_TempHum()
{
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
 // delay(1000);
}*/
