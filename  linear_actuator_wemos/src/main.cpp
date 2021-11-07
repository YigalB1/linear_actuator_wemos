#include <Arduino.h>
#include <servo.h>

#define SERVO_OPEN   0
#define SERVO_CLOSE  180 
#define SERVO_INC    10
#define SERVO_DELAY  200

uint8_t Servo1_pin = 0;
uint8_t Servo2_pin = 4;

bool led_toggle = false;
Servo MyServo1;
Servo MyServo2;

int servo_jump = int((SERVO_CLOSE-SERVO_OPEN)/SERVO_INC);

void move_servo(Servo _servo,int _start,int _end,int _inc);

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  MyServo1.attach(Servo1_pin);
    
  Serial.begin(9600);
  Serial.println("starting  setup()");
  Serial.println(ESP.getChipId());

  digitalWrite(LED_BUILTIN, LOW);
  MyServo1.write(SERVO_CLOSE);
  delay(300);
  MyServo1.write(SERVO_OPEN);
  delay(300);
   MyServo1.write(SERVO_CLOSE);
  delay(300);
  digitalWrite(LED_BUILTIN, HIGH);
} // of setup

void loop(){

  move_servo(MyServo1,SERVO_OPEN,SERVO_CLOSE,SERVO_INC);
  delay(1000);
  move_servo(MyServo1,SERVO_CLOSE,SERVO_OPEN,SERVO_INC);
  delay(1000);

} // of loop()

void move_servo(Servo _servo,int _start,int _end,int _inc) {
  
  if (_end>_start) {
    Serial.print("going UP ");
    for (int i=_start;i<_end;i+=_inc) {
      Serial.print(i);
      Serial.print("  ");
      _servo.write(i);
      delay(SERVO_DELAY);
    } // of for loop
  } // of if()
  else {
    Serial.print("going DOWN ");
      for (int i=_end;i>_start;i-=_inc) {
        Serial.print(i);
        Serial.print("  ");
        _servo.write(i);
      delay(SERVO_DELAY);
    } // of for loop
  } // of else()
  Serial.println("     next ");
  

} // of move_servo