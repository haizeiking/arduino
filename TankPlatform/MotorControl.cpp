#include "MotorControl.h"


int speed = 0;

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

void MotorSpeed(char cmd) {
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
