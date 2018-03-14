#include "IRLibAll.h"
 
//Create a receiver object to listen on pin 2
IRrecvPCI myReceiver(2);
 
//Create a sender
//IRsend mySender; 

//Create a decoder object 
IRdecode myDecoder;   


uint32_t code;
 
void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}
 
void loop() {
  //Send();
  Read();
}

void Read() {
   //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(false);  //Now print results. Use false for less detail
    myReceiver.enableIRIn();      //Restart receiver
  }
}

void Send (void) {
  long code=0; delay(100);
  if (Serial.available()>0) {
    String s = Serial.readString();
    
    code = GetCodeFromHex(s);
    Serial.println(s);
    //mySender.send(7, code, 32);
  };
}

long GetCodeFromHex(String s) {
  long Value=0;char C;
   char buf[9];
    s.toCharArray(buf, 9);

    for (uint8_t i = 0; i < 9; i++) {
      C= tolower(buf[i]);

      if ((C>='0')&&(C<='9'))
        C=C-'0';
      else
        if ((C>='a') && (C<='f'))
          C=C-'a'+10;
        else
          return Value;
      Value=  C+(Value<<4);
    }

    return Value;
}
