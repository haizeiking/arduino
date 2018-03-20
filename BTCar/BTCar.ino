//HC-05 bluetooth module used. Pair using password 1234

#include <SoftwareSerial.h>
#include <Servo.h>

#define MOTOR_A 7
#define MOTOR_B 8
#define MOTOR_SPEED 6
#define SERVO 5
#define FRONT_LIGHTS 13
#define REAR_LIGHTS 12

// servo limits 30 - 90 - 150

SoftwareSerial Bluetooth(10, 9); // RX, TX
char cmd; // the data received

Servo myservo;  // create servo object to control a servo
 
uint16_t speed = 0;

void setup() {
  delay(2000);
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");

  pinMode(FRONT_LIGHTS, OUTPUT);
  pinMode(REAR_LIGHTS, OUTPUT);

  myservo.attach(SERVO);

  digitalWrite(FRONT_LIGHTS, LOW);
  digitalWrite(REAR_LIGHTS, LOW);
}
 
void loop() {
  if (Bluetooth.available()){ //wait for data received
    cmd=Bluetooth.read();
   // Serial.println(cmd);
    //uncomment block bellow to use Serial interface instead of bluetooth. For dev and debugging
   /*if (Serial.available()) {
     cmd = Serial.read();*/
    switch(cmd) {
      case 'S':
        Stop();
      break;
      case 'D':
        Disconnect();
      break;
      case 'F':
      case 'I':
      case 'G':
        Forward(cmd);
      break;
      case 'B':
      case 'J':
      case 'H':
        Back(cmd);
      break;
      case 'R':
        Right();
      break;
      case 'L':
        Left();
      break;
      case 'W':
      case 'w':
      case 'U':
      case 'u':
        Ligths(cmd);
      break;
      case 'V':
      case 'v':
        Horn(cmd);
      break;
      case 'X':
      case 'x':
        Extra(cmd);
      break;
      default:
        Speed(cmd);
    }
    Serial.println(cmd);
  }
delay(100);
}

void Disconnect() {
  Serial.print("disconnect");
  Break();
}

void Stop() {
  Serial.println("stop");
  Break();
}

void Forward(char cmd) {
  // G - FL
  // I - FR
  //Serial.print("Forward ");
  switch (cmd) {
    case 'F': {
      CarForward();
    }
    break;
    case 'G': {
      CarForwardLeft();
    }
    break;
    case 'I': {
      CarForwardRight();
    }
    break;
  }
}

void Back(char cmd) {
  // H - BL
  // J - BR
  //Serial.print("Back ");
   switch (cmd) {
    case 'B': {
      CarBackward();
    }
    break;
    case 'H': {
      CarBackwardLeft();
    }
    break;
    case 'J': {
      CarBackwardRight();
    }
    break;
  }
}

void Right() {
  //Serial.print("Right ");
  ServoPosition(150);
}

void Left() {
  //Serial.print("Left ");
  ServoPosition(30);
}

void Ligths(char cmd) {
    // W/w - front ligths
  // U/u - back ligths
  Serial.println("ligths");
  Serial.println(cmd);
  switch(cmd) {
    case 'W':
      digitalWrite(FRONT_LIGHTS,HIGH);
    break;
    case 'w':
      digitalWrite(FRONT_LIGHTS,LOW);
    break;
    case 'U':
      digitalWrite(REAR_LIGHTS,HIGH);
    break;
    case 'u':
      digitalWrite(REAR_LIGHTS,LOW);
    break;
  }
}

void Horn(char cmd) {
  // V/v
  Serial.println("horn");
}

void Extra(char cmd) {
  // X/x
  Serial.println("extra");
}

void Speed(char cmd) {
  // 1-9,q
  Serial.print("Speed ");
  
  if (cmd == 'q') {
    speed = 255;
  }
  else {
    //Chars '0' - '9' have an integer equivalence of 48 - 57, accordingly.
    if((cmd >= 48) && (cmd <= 57)) { 
      //Subtracting 48 changes the range from 48-57 to 0-9.
      //Multiplying by 25 changes the range from 0-9 to 0-225.
      speed = (cmd - 48)*25;       
    }
  }
  analogWrite(MOTOR_SPEED, speed);
}

void CarForward() {
  ServoPosition(90);
  digitalWrite(MOTOR_A, HIGH);
  digitalWrite(MOTOR_B, LOW);
}

void CarForwardRight() {
  CarForward();
  Right();
}

void CarForwardLeft() {
  CarForward();
  Left();
}

void CarBackward() {
  ServoPosition(90);
  digitalWrite(MOTOR_A, LOW);
  digitalWrite(MOTOR_B, HIGH);
}

void CarBackwardRight() {
  CarBackward();
  Right();
}

void CarBackwardLeft() {
  CarBackward();
  Left();
}


void Break() {
  digitalWrite(MOTOR_A, LOW);
  digitalWrite(MOTOR_B, LOW);

  // turn on rear lights for a short period
  digitalWrite(REAR_LIGHTS, HIGH);
  delay(200);
  digitalWrite(REAR_LIGHTS, LOW);
  
  
}

void ServoPosition(int angle) {
    myservo.write(angle);              
    delay(15);  
}
