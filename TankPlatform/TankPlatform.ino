

//cmd motors: m;cmd;val | d - direction, r - rotate, b - break

#include "MotorControl.h"
#include "BluetoothControl.h"
#include "ObstacleAvoidanceStateMachine.h"


void setup() {
    Serial.begin(9600);
    BT_Setup();

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
    delay(1000);
    Serial.println("TankPlatform Ready");
}

void loop() {
  BluetoothComm();
  //ObstacleAvoidance(true);
  run_state_machine();
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









