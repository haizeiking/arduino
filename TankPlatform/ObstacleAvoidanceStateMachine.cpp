#include "MotorControl.h"

#include "DistanceSensor.h"
#include "Arduino.h"

short states[4] = {0,1,2,3}; // wait,back,search_path,forward
short inputs[1] = {60}; //distance
int obstacle_threshold = 9;
int safe_threshold = 12;

short current_state = 0;
bool is_running = false;

//void run_state_machine();

void ObstacleAvoidance(bool enable) {
    Serial.println("Obstacle avoidance");

    is_running = enable;
    
    //run_state_machine();
}

short next_state(short state, int inputs[1]) {
   // short next = 0;
    int distance = inputs[0];
    
    switch (state) {
        case 0:
        {
            if (distance < obstacle_threshold)
                return 1;
            else
                return 0;
        }
        break;
        case 1:
        {
            if (distance > safe_threshold)
                return 2;
            else
                return 1;
        }
        break;
        case 2:
        {
            if (distance > safe_threshold)
                return 3;
            else
                return 2;
        }
        break;
        case 3:
        {
            if (distance > safe_threshold)
                return 0;
            else
                return 1;
        }
        break;
        default:
            return 0;
        break;
    }
}

void wait() {
    //Serial.println("wait");
    //Stop();
    Forward('F');
}

void back() {
    //Serial.println("back a little");
    Back('B');
}

void forward() {
    //Serial.println("forward a little");
    Forward('F');
}

void search_clear_path() {
    //Serial.println("search path");
    Right();
}

void run_state(short state) {
    switch (state) {
        case 0:
            wait();
        break;
        case 1:
            back();
        break;
        case 2:
            search_clear_path();
        break;
        case 3:
            forward();
        break;
        default:
        break;
    }
}

void run_state_machine() {
    if (is_running) {
        
        int inputs[1];
        inputs[0] = GetSharpDistance();
        Serial.print("distance=");
        Serial.println(inputs[0]);
        current_state = next_state(current_state, inputs);
        run_state(current_state);
        Serial.println(current_state);
        //delay(1000);
    }
}