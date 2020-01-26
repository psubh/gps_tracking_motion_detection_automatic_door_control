#include <Servo.h>
#include <SoftwareSerial.h>
//#include <TinyGPS++.h>

TinyGPSPlus gps;
SoftwareSerial gps_Serial(4,3);//gps rx->arduino pin 3 and gps tx->arduino pin 4

Servo servo_door;
int angle = 0;

SoftwareSerial mySerial(2, 3);//SIM900 Tx & Rx is connected to Arduino #2 & #3
 
int door_relay = 9;
int pir_sensor =  8;
int door_relay_value = 0;
int pir_sensor_value = 0; 

void setup() {
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
   digitalWrite(12,LOW);
  digitalWrite(13,LOW);
  servo_door.attach(6);
  pinMode(door_relay, INPUT); 
  pinMode(pir_sensor, INPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  gps_Serial.begin(9600);
  Serial.println("wait......");
  delay(1000);

  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CSQ");
  updateSerial();
  mySerial.println("AT+CCID");
  updateSerial();
  mySerial.println("AT+CREG?");
  updateSerial();
}

void loop(){
  door_relay_value=digitalRead(door_relay);
  pir_sensor_value=digitalRead(pir_sensor);
  
  if(mySerial.available() && door_relay_value==HIGH )
  {
     for(angle = 90; angle >=0; angle -= 30)
  {                                  
    servo_door.write(angle);
    delay(15);                       
  } 
  }

  if( door_relay_value==LOW && pir_sensor_value==HIGH){ 
     for(angle = 0; angle <=90; angle += 30)
  {                                  
    servo_door.write(angle);
    delay(15);                       
  }  
  send_message();               
    }
//  while (gps_Serial.available() > 0){
//  gps.encode(gps_Serial.read());
//  if (gps.location.isUpdated())
//  {
//  Serial.print("Latitude= "); 
//  Serial.print(gps.location.lat(), 6);
//  Serial.print(" Longitude= "); 
//  Serial.println(gps.location.lng(), 6);
//  Serial.print("Speed in km/h = "); 
//  Serial.println(gps.speed.kmph()); 
//  }    
//  }
}
void send_message()
{
 mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+16239865740\"");
  updateSerial();
  mySerial.print("door is closed and chicken is safe inside");
  updateSerial();
  mySerial.write(26); 
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}
