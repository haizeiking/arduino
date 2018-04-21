#define M1_A 7
#define M1_B 8
#define M1_S 11

#define M2_A 5
#define M2_B 6
#define M2_S 10

#define MOTOR_ENABLE_PIN 4

#include "Arduino.h"


void MotorBackward(int speed);
void MotorForward(int speed);
void MotorRotateLeft(int speed);
void MotorRotateRight(int speed);
void Break();

void Stop();
void Forward(char cmd);
void Back(char cmd);
void Right();
void Left();
void Ligths(char cmd);
void Horn(char cmd);
void MotorSpeed(char cmd);
void CarForward();
void Extra(char cmd);
void CarForwardRight();
void CarForwardLeft();
void CarBackward();
void CarBackwardRight();
void CarBackwardLeft();