//#include <Arduino.h>
#include <servo.h>

uint8_t const SERVO_INC   = 10;
uint8_t const SERVO_MIN_angle   = 0;
uint8_t const SERVO_MAX_angle   = 180;
uint8_t const SERVO_DELAY = 200;
int const SERVO_MIN_us   = 600;
int const SERVO_MAX_us   = 2800;


uint8_t const SERVO1_PIN   = 4; // was0;

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;  // value read from the pot

bool run_endless = false;
Servo MyServo1;

int servo_jump;
int servo_angle;

void my_wait(int ms_period);

void setup(){
  Serial.begin(9600);
  Serial.println("starting  setup()");
  Serial.println(ESP.getChipId());

  pinMode(LED_BUILTIN, OUTPUT);
  MyServo1.attach(SERVO1_PIN,SERVO_MIN_us,SERVO_MAX_us);

  Serial.print("  ON  ");
  digitalWrite(LED_BUILTIN, LOW);
  MyServo1.write(SERVO_MAX_angle);
  delay(1000);
  //yig_wait(1000);

  Serial.print("  OFF  ");
  digitalWrite(LED_BUILTIN, HIGH);
  MyServo1.write(SERVO_MIN_angle);
  delay(1000);
  //yig_wait(1000);   

  servo_jump = int((SERVO_MAX_angle-SERVO_MIN_angle)/SERVO_INC);
  Serial.print("servo_jump: ");
  Serial.println(servo_jump);

} // of setup


int tmp_1;
int tmp_2;
bool led_tmp = false;

void loop(){
  
    
    
    /*
    for (int i=800;i<=2500;i+=100) {
      MyServo1.writeMicroseconds(i);
      Serial.print("i: ");
      Serial.print(i);
      tmp_2 = MyServo1.readMicroseconds();
      Serial.print("   uS: ");
      Serial.println(tmp_2);
      delay(400);
    }
    delay(1000);
    if (led_tmp) {
      digitalWrite(LED_BUILTIN, LOW); // turn led on
      led_tmp = false;
    } // of if()
    else {
      digitalWrite(LED_BUILTIN, HIGH); // turn led off
      led_tmp = true;
    }
    
    return;
    */


    sensorValue = analogRead(analogInPin);
    // tmp_1 = map(sensorValue, 0, 1023, 1000, 2000);
    
    
    //Serial.print(" ");
    Serial.print(sensorValue);
    Serial.print(" ");

    // translate the potentiometer value to the microseconds span
    // of the servo (better than angle)
    servo_angle = map(sensorValue, 0, 1023, SERVO_MIN_us, SERVO_MAX_us);
    Serial.print(servo_angle);

    MyServo1.write(servo_angle);
    tmp_1 = MyServo1.read();
    tmp_2 = MyServo1.readMicroseconds();
    Serial.print(" ");
    Serial.print(tmp_1);
    Serial.print(" ");
    Serial.println(tmp_2);


    delay(200);
    //yig_wait(5000);
  

} // of loop()



void my_wait(int ms_period) {
  unsigned long time_now = millis();
  while(millis() < time_now + ms_period){        
  } // of while()
} // of my_wait()

