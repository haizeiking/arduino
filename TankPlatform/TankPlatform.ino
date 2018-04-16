#define M1_A 7
#define M1_B 8
#define M1_S 11

#define M2_A 5
#define M2_B 6
#define M2_S 10

#define MOTOR_ENABLE_PIN 4

//cmd motors: m;cmd;val | d - direction, r - rotate, b - break

#include <SoftwareSerial.h>

// Bluetooth
SoftwareSerial Bluetooth(2, 3); // RX, TX
uint16_t speed = 0;

char cmd; // the data received

void setup() {
    Serial.begin(9600);
     Bluetooth.begin(9600);

    //setup motor output pins and enable
    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
    digitalWrite(MOTOR_ENABLE_PIN,HIGH);

    pinMode(M1_A, OUTPUT);
    pinMode(M1_B, OUTPUT);
    pinMode(M1_S, OUTPUT);

    pinMode(M2_A, OUTPUT);
    pinMode(M2_B, OUTPUT);
    pinMode(M2_S, OUTPUT);

    Serial.println("Testing motors");
    
    //TestMovements();

    Serial.println("TankPlatform Ready");
}

void loop() {
  BluetoothComm();
}

void BluetoothComm() {
   if (Bluetooth.available()){ //wait for data received
    cmd=Bluetooth.read();
    Serial.println(cmd);
    //uncomment block bellow to use Serial interface instead of bluetooth. For dev and debugging
   /*if (Serial.available()) {
     cmd = Serial.read();*/
    switch(cmd) {
      case 'S':
        Break();
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
        //Ligths(cmd);
      break;
      case 'V':
      case 'v':
        //Horn(cmd);
      break;
      case 'X':
      case 'x':
        //Extra(cmd);
      break;
      default:
        Speed(cmd);
    }
    Serial.println(cmd);
  }
delay(100);  
}

void TestMovements() {
  MotorForward(150);
  delay(2000);
  MotorBackward(100);
  delay(2000);
  MotorRotateLeft(120);
  delay(2000);
  MotorRotateRight(100);
  delay(2000);
  Break();
  delay(2000);
}

void SerialCmd() {
    if (Serial.available()) {
        String cmd = Serial.readString();
        Serial.println(cmd);
        Forward(cmd.toInt());
    }  
}

// Motor Commands
void MotorBackward(int speed) {
    digitalWrite(M1_A, HIGH);
    digitalWrite(M1_B, LOW);
    analogWrite(M1_S, speed);

    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, LOW);
    analogWrite(M2_S, speed);
}

void MotorForward(int speed) {
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, HIGH);
    analogWrite(M1_S, speed);

    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, HIGH);
    analogWrite(M2_S, speed);
}

void MotorRotateLeft(int speed) {
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, HIGH);
    analogWrite(M1_S, speed);

    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, LOW);
    analogWrite(M2_S, speed);
}

void MotorRotateRight(int speed) {
    digitalWrite(M1_A, HIGH);
    digitalWrite(M1_B, LOW);
    analogWrite(M1_S, speed);

    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, HIGH);
    analogWrite(M2_S, speed);
}

void Break() {
    digitalWrite(M1_A, HIGH);
    digitalWrite(M1_B, HIGH);

    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, HIGH);
}

// Motor Commands End


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
      //CarBackwardLeft();
    }
    break;
    case 'J': {
      //CarBackwardRight();
    }
    break;
  }
}

void Right() {
  //Serial.print("Right ");
  MotorRotateRight(speed);
}

void Left() {
  //Serial.print("Left ");
  MotorRotateLeft(speed);
}

void Ligths(char cmd) {
    // W/w - front ligths
  // U/u - back ligths
  Serial.println("ligths");

  
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
  
  analogWrite(M1_S, speed);
  analogWrite(M2_S, speed);
  
}

void CarForward() {
  MotorForward(speed);
}

void CarForwardRight() {
  CarForward();
}

void CarForwardLeft() {
  CarForward();
}

void CarBackward() {
  MotorBackward(speed);
}

void CarBackwardRight() {
  CarBackward();
}

void CarBackwardLeft() {
  CarBackward();
}




