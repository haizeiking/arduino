/*
 Sharp GP2Y0A21YK0F infrared proximity sensor (#28995)
 Displays raw digital value of approximate distance every 
 quarter second (250 milliseconds).
 
 Distance measurement range is 10cm to 80cm (3.9" to 31.5")
 
 Returns basic 10-bit analog-to-digital value where 0=0 volts, and 1023=5 volts,
 and displays it in the Arduino Serial Monitor window. After uploading the
 sketch, open the Serial Monitor and be sure to set communication speed
 to 9600 baud.
 
 Typical values are from 45 (maximum distance) to 425-475, indicating 
 minimum distance. Values outside these ranges may occur when the object 
 is too close to the sensor (under 10cm)
 
 Keep in mind output is not linear to distance. See the Sharp GP2Y0A21YK0F 
 datasheet for more information.
 
 This example code is for the Arduino Uno and direct compatible boards, using the
 Arduino 1.0 or later IDE software. It has not been tested, nor designed for, other 
 Arduino boards, including the Arduino Due.
 
Connections:
Sensor       Arduino
GND          GND
Vcc          5V
VO(utput)    Analog pin 0 (A0)

Note: The following standard color-coded is use with the adapter cable included
from Parallax with this sensor:

Color       Connection on sensor
Black       GND
Red         Vcc
White       VO

*/

const int irSense = A0;            // Connect sensor to analog pin A0
int distance = 0;

void setup() {
  Serial.begin(9600);              // Use Serial Monitor window
}

void loop() {
  distance = analogRead(irSense);  // Read the sensor
  Serial.println(distance, DEC);   // Display value in Serial Monitor window
  delay(250);                      // Wait 1/4 second for the next read
}
