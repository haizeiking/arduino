/*
 Sharp GP2Y0A21YK0F infrared proximity sensor (#28995)
 Collects an average of five readings from the sensor and displays the
 resulting value about every half second.
 
 See the SharpGP2Y0A21_Simple demonstration for additional usage notes,
 including connection diagram.
 
 This example code is for the Arduino Uno and direct compatible boards, using the
 Arduino 1.0 or later IDE software. It has not been tested, nor designed for, other 
 Arduino boards, including the Arduino Due.

*/

const int irSense = A0;          // Connect sensor to analog pin A0
int distance = 0;

void setup() {
  Serial.begin(9600);            // Use Serial Monitor window
}

void loop() {
  Serial.println(irRead(), DEC); // Call irRead function to read sensor
                                 // Print value in Serial Monitor
  delay(250);                    // Wait another 1/4 second for the next read
                                 // (Note: Because of delays built into the
                                 //   irRead function the display of values will
                                 //   be slower than in the SharpGP2Y0A21_Simple
                                 //   sketch
}

// Take multiple readings, and average them out to reduce false readings
int irRead() {
  int averaging = 0;             //  Holds value to average readings

  // Get a sampling of 5 readings from sensor
  for (int i=0; i<5; i++) {
    distance = analogRead(irSense);
    averaging = averaging + distance;
    delay(55);      // Wait 55 ms between each read
                    // According to datasheet time between each read
                    //  is -38ms +/- 10ms. Waiting 55 ms assures each
                    //  read is from a different sample
  }
  distance = averaging / 5;      // Average out readings
  return(distance);              // Return value
}
