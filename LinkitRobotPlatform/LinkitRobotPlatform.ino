//commands - size 20 char
//===Motor
//===1 - direction; 2 - stop
//======m;2;1;144 - device_type;device_id;command;payload -> motor;motor_id;forward;speed;
#include "Timer.h"     
#include <math.h>

// setup motor pins
#define MOTOR1_A 9
#define MOTOR1_B 8
#define MOTOR1_SPEED 3

#define MOTOR2_A 11
#define MOTOR2_B 10
#define MOTOR2_SPEED 5

// init buffer
#define BUFFER_SIZE 20

//ultrasonic timer setup
const unsigned long distance_check_interval = 200;    //one second
Timer t;                               //instantiate the timer object


char buffer[BUFFER_SIZE];
// defines pins numbers
const int trigPin = 13;
const int echoPin = 12;
// defines variables
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial1.begin(57600); // Starts the serial communication

  t.every(distance_check_interval, PrintDistance);

  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR1_SPEED, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(MOTOR2_SPEED, OUTPUT);
}

void loop() {
  t.update();
  ReadSerial(&ApplyCommand);
}

void PrintDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  long tmp = duration*0.034/2;

  //check if distance changed
  if (distance != tmp) {
    distance = tmp;
    SendString_Serial("d;1;1;"+String(distance));
  }

}

void CheckCommand() {
  ReadSerial(&ApplyCommand);
}

void ReadSerial(void (*callback)(String)) {
  if (Serial1.available()) {
    String cmd = ReadString_Serial();
    callback(cmd);
    
  }
}

void ApplyCommand(String cmd) {
  if (cmd[0]=='m') {
    Motor(cmd);  
  }
}

void Motor(String cmd) {
  //cmd.toCharArray(buffer, BUFFER_SIZE)
  char motor_id = cmd[2];
  char cmd_id = cmd[4];
  
  String payload = cmd.substring(6);
  MotorCommand(motor_id, cmd_id, payload);

}

void MotorCommand(char motor_id, char cmd_id, String payload) {
  int16_t value = payload.toInt();

  switch (cmd_id) {
    case '1':
      Direction(motor_id, value);
    break;
    case '2':
      Stop(motor_id);
    break;
  }
}

void Direction(char motor_id, int16_t value) {
  uint16_t pinA = 0, pinB = 0, pinPWM = 0;
  
  switch (motor_id) {
    case '1':
    {
      pinA = MOTOR1_A;
      pinB = MOTOR1_B;
      pinPWM = MOTOR1_SPEED;
    }
    break;
    case '2':
    {
      pinA = MOTOR2_A;
      pinB = MOTOR2_B;
      pinPWM = MOTOR2_SPEED;
    }
    break;
    default:
      {
        return; 
      }
      
    break;
  }
  /*Serial1.println(pinA);
  Serial1.println(" ");
  Serial1.println(pinB);
  Serial1.println(" ");
  Serial1.println(pinPWM);*/
  if (value >= 0 && value < 255) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    analogWrite(pinPWM, value);
  } else if(value < 0 && value > -255) {
    value = abs(value);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    analogWrite(pinPWM, value);
  }
   
}

void Stop(char motor_id) {
    uint16_t pinA = 0, pinB = 0, pinPWM = 0;
  
  switch (motor_id) {
    case '1':
    {
      pinA = MOTOR1_A;
      pinB = MOTOR1_B;
      pinPWM = MOTOR1_SPEED;
    }
    break;
    case '2':
    {
      pinA = MOTOR2_A;
      pinB = MOTOR2_B;
      pinPWM = MOTOR2_SPEED;
    }
    break;
    default:
      return;
    break;
  }

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
}

void SendString_Serial(String str) {
  Serial1.println(str);
}

String ReadString_Serial() {
  return Serial1.readString();
}

