#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(10, 9); // RX, TX
int LED = 13; // the on-board LED
char cmd; // the data received
 
void setup() {
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  Bluetooth.println("Send 1 to turn on the LED. Send 0 to turn Off");
  pinMode(LED,OUTPUT);
 
}
 
void loop() {
  //delay(50);
  if (Bluetooth.available()){ //wait for data received
    cmd=Bluetooth.read();
    switch(cmd) {
      case 'S':
        Stop();
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
    }
    Serial.println(cmd);
  }
delay(100);
}

void Stop() {
  Serial.println("stop");
}

void Forward(char cmd) {
  // G - FL
  // I - FR
  Serial.print("Forward ");
  Serial.println(cmd);
}

void Back(char cmd) {
  // H - BL
  // J - BR
  Serial.print("Back ");
  Serial.println(cmd);
}

void Right() {
  Serial.print("Right ");
}

void Left() {
  Serial.print("Left ");
}

void Ligths(cmd) {
  // W/w - front ligths
  // U/u - back ligths
  Serial.println("ligths");
  Serial.println(cmd);
}

void Horn(cmd) {
  // V/v
  Serial.println("horn");
}

void Extra(cmd) {
  // X/x
  Serial.println("extra");
}

void Speed(cmd) {
  // 1-9,q
  Serial.print("Speed ");
  Serial.println(cmd);
}
