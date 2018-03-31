#define MOTOR_A 7
#define MOTOR_B 8
#define MOTOR_SPEED 11

void setup() {
    Serial.begin(9600);

    pinMode(4, OUTPUT);
    digitalWrite(4,HIGH);

    pinMode(MOTOR_A, OUTPUT);
    pinMode(MOTOR_B, OUTPUT);
    pinMode(MOTOR_SPEED, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        String cmd = Serial.readString();
        Serial.println(cmd);
        Forward(cmd.toInt());
    }
}

void Forward(int speed) {
    digitalWrite(MOTOR_A, HIGH);
    digitalWrite(MOTOR_B, LOW);
    analogWrite(MOTOR_SPEED, speed);
}

