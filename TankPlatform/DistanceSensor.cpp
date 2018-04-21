#include "DistanceSensor.h"

#include <SharpIR.h>

#define ir A0
#define model 1080
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)

SharpIR SharpIR(ir, model);

int GetSharpDistance() {
  int dis=SharpIR.distance();  // this returns the distance to the object you're measuring
  //int dis = analogRead(A1);

  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);
   
  if (dis < 30)
  {
    digitalWrite(13,HIGH);  
  }
  else {
    digitalWrite(13,LOW);  
  }

  return dis;
}

int GetSonarDistance() {
    return 0;
}