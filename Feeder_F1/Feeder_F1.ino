#define motorPin 8
long int currTimeHour = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motorPin,OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
unsigned long runMillis= millis();
unsigned long allSeconds=millis()/1000;
int hours= allSeconds/3600;
int secsRemaining=allSeconds%3600;
int runMinutes=secsRemaining/60;
int runSeconds=secsRemaining%60;

char buf[21];
sprintf(buf,"Runtime%02d:%02d:%02d", hours,runMinutes,runSeconds);

hours == 1;
hours ==0;
Serial.println(buf);
delay(5000);




currTimeHour = hours;


if(hours%4 == 0) 
  {
    if(runMinutes <= 1)
        {
         digitalWrite(motorPin, HIGH);
         }
     else
     {
        digitalWrite(motorPin, LOW);
     }
  }
else
    {
     digitalWrite(motorPin, LOW);
    }
  
}


  
