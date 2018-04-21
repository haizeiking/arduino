#include "MotorControl.h"

#include "DistanceSensor.h"
#include "Arduino.h"

void ObstacleAvoidance(bool enable) {
    Serial.println("Obstacle avoidance");
    int distance = GetSharpDistance();
    Serial.println(distance);

    delay(1000);
}