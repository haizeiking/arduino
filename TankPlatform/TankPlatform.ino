#define M1_A 7
#define M1_B 8
#define M1_S 11

#define M2_A 5
#define M2_B 6
#define M2_S 10

//cmd motors: m;cmd;val | d - direction, r - rotate, b - break

void setup() {
    Serial.begin(9600);

    pinMode(4, OUTPUT);
    digitalWrite(4,HIGH);

    pinMode(M1_A, OUTPUT);
    pinMode(M1_B, OUTPUT);
    pinMode(M1_S, OUTPUT);

    pinMode(M2_A, OUTPUT);
    pinMode(M2_B, OUTPUT);
    pinMode(M2_S, OUTPUT);

    Serial.println("TankPlatform Ready");
}

void loop() {
  TestMovements();
}

void TestMovements() {
  Forward(150);
  delay(2000);
  Backward(100);
  delay(2000);
  RotateLeft(120);
  delay(2000);
  RotateRight(100);
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

void Backward(int speed) {
    digitalWrite(M1_A, HIGH);
    digitalWrite(M1_B, LOW);
    analogWrite(M1_S, speed);

    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, LOW);
    analogWrite(M2_S, speed);
}

void Forward(int speed) {
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, HIGH);
    analogWrite(M1_S, speed);

    digitalWrite(M2_A, LOW);
    digitalWrite(M2_B, HIGH);
    analogWrite(M2_S, speed);
}

void RotateLeft(int speed) {
    digitalWrite(M1_A, LOW);
    digitalWrite(M1_B, HIGH);
    analogWrite(M1_S, speed);

    digitalWrite(M2_A, HIGH);
    digitalWrite(M2_B, LOW);
    analogWrite(M2_S, speed);
}

void RotateRight(int speed) {
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

