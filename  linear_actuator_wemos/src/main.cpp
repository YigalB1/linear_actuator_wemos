//#include <Arduino.h>
#include <Servo.h>

//#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
const int echoPin = 5; // GPIO 5 (D1) in schematics

//#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
const int trigPin = 0; // GPIO 0 (D3) in schematics

//#define red_led 12
const int red_led = 16; // GPIO 16 is D0 in schematics
//#define green_led 11
const int green_led = 14; // GPIO 14 is D5 in schematics

#define GATE_OPEN 0
#define GATE_CLOSED 1
#define CLOSE_DIST 10
#define MAX_DIST 100

#define SERVO_OPEN   0
#define SERVO_CLOSE 180 


long duration; 
int distance; 
int read_US_sensor();
Servo myservo;  
int pos = 0; 
//int state = GATE_CLOSED;
bool gate_is_opened = false;
bool change = true;

int p_dist = 101; // p: short for prev
int p_p_dist = 102;
int p_p_p_dist = 103;


void setup() {
  Serial.begin(9600);
  Serial.println("Starting SETUP");

  myservo.attach(12);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);

  // show signs of life
  for(int i=0;i<3;i++) {
    digitalWrite(LED_BUILTIN, LOW);  // D1 Mini: turns the LED *off*
    digitalWrite(green_led, HIGH);  // D1 Mini: turns the LED *off*
    digitalWrite(red_led, HIGH);  // D1 Mini: turns the LED *off*
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);     // D1 Mini: turns the LED *on*
    digitalWrite(green_led, LOW);  // D1 Mini: turns the LED *off*
    digitalWrite(red_led, LOW);  // D1 Mini: turns the LED *off*
    delay(300); 
  }  // of for()
  
  Serial.println(ESP.getChipId());
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
    
   

   myservo.write(SERVO_OPEN);
   delay(300);
   myservo.write(SERVO_CLOSE);
   delay(300);
   myservo.write(SERVO_OPEN);
   delay(300);
  // start with closed gate
  gate_is_opened = false;
  myservo.write(SERVO_CLOSE);
  digitalWrite(red_led,HIGH);
  digitalWrite(green_led,LOW);
  Serial.println("starting with closed gate");
  Serial.println("setup finished");
}

bool c1 = false; // close dist last
bool c2 = false; // close dist prev
bool c3 = false; // close dist prev prev
bool c4 = false; // close dist prev prev prev
bool f1 = false; // far dist last
bool f2 = false; // far dist prev
bool f3 = false; // far dist prev prev
bool f4 = false; // far dist prev prev prev
bool wait_period = false; // 3 readings of far to allow new change of gate state

// ---------------------------------------------------------------------------
void loop() {
  distance = read_US_sensor();
  Serial.print(distance);
  Serial.print("  ");
  if (distance==0)
    return; // 0 means zero value or larger than MAX_DIST

  c1 = distance < CLOSE_DIST;
  f1 = !c1;
  
  if (f2 && f3 && f4) {
    // last 3 readings were far, so gate can change again
    wait_period = true;
  } // of if()
  if (c1 && c2 && c3 && c4 && wait_period ) {
    gate_is_opened = !gate_is_opened;
    change = true;
    wait_period = false;
    Serial.println("  ");
  } // of if

  if (change) {
    if ( gate_is_opened ) {      
      myservo.write(SERVO_OPEN);
      digitalWrite(red_led,LOW);
      digitalWrite(green_led,HIGH);
      Serial.println("open gate");
    } // of IF
    else {
      myservo.write(SERVO_CLOSE);
      digitalWrite(red_led,HIGH);
      digitalWrite(green_led,LOW);
      Serial.println("close gate");
    } // of ELSE        
    change = false;
  } // of outer IF

  c4=c3;
  c3=c2;
  c2=c1;
  f4=f3;
  f3=f2;
  f2=f1;
  delay(100);  // to slow down so no false too fast readings
} // of LOOP()

// ----------------------------------------------------------------
int read_US_sensor() {
  int dist = 100;
  
  //ool wait_for_state_change = false;
  //bool valid_closed = false;
  //bool valid_open = false;

  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  if (dist>MAX_DIST) dist = 100; // value 100 should be ignored
  if (dist==0) dist = 100; // value 0 should be ignored
  return dist;
} // of read_US_sensor()