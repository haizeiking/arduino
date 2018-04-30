#include "BluetoothControl.h"
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "MotorControl.h"
#include "ObstacleAvoidanceStateMachine.h"


// Bluetooth
SoftwareSerial Bluetooth(2, 3); // RX, TX

char cmd; // the data received

void Disconnect() {
  Serial.print("disconnect");
  Break();
}

void BT_Setup() {
    Bluetooth.begin(9600);
}

void Auto(char cmd) {
  if (cmd == 'X') {
    Serial.println("Auto enabled");
    ObstacleAvoidance(true);

  } else if (cmd == 'x') {
    Serial.println("Auto disabled");
    ObstacleAvoidance(false);
    Stop();
  }
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
        Auto(cmd);
      break;
      default:
        MotorSpeed(cmd);
    }
    Serial.println(cmd);
  }
delay(100);  
}