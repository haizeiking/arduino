#include "IRLibAll.h"
#include <string.h>
#include <stdlib.h>
#include "Timer.h"

//serial msg: node;cmd;payload

#define JSON_BUFFER_SIZE 70
#define TOKEN_COUNT 3


char tokens[TOKEN_COUNT][JSON_BUFFER_SIZE/TOKEN_COUNT];
long code;
int inputPin = 6;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;       
String node, cmd, payload;

//Create a sender
IRsend mySender; 
Timer t;

void Tokenize(String msg, char* delimiter, char tokens[TOKEN_COUNT][JSON_BUFFER_SIZE/TOKEN_COUNT], short* tokenCount);
void HandleIR(char payload[JSON_BUFFER_SIZE/TOKEN_COUNT]);

 

void setup() {
  Serial1.begin(57600);
  delay(2000);
  Serial1.println(F("Ready"));

  t.every(300, HandlePIR);
}
 
void loop() {
   ReadSerial();
   t.update();
   //HandlePir();
}

void ParseMessage(String msg) {
  short tokenCount;
  Tokenize(msg,";",tokens, &tokenCount);

  /*for (short i=0; i<tokenCount;i++) {
   Serial1.println(tokens[i]);
  }
*/
  node = String(tokens[0]);
  cmd = String(tokens[1]);
  payload = String(tokens[2]);

  if (node.equals("ir")) {
   HandleIR(payload);
  } else if (node.equals("display")) {
    HandleDisplay(cmd, payload);
  }

}


void HandleIR(String payload) {
  code = GetCodeFromHex(payload);
 
  mySender.send(7, code, 32);
}

void HandleDisplay(String cmd, String payload) {
  Serial1.println("handle display");
  Serial1.println(cmd);
  Serial1.println(payload);
}

void HandlePIR() {
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
  
    if (pirState == LOW) {
      // we have just turned on
      Serial1.println("pir;1");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned off
      Serial1.println("pir;0");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
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

void ReadSerial(){
  while (Serial1.available()){
    String msg = Serial1.readString();

    ParseMessage(msg);
  }
}



void Tokenize(String msg, char* delimiter, char tokens[TOKEN_COUNT][JSON_BUFFER_SIZE/TOKEN_COUNT], short* tokenCount)
{
  char *s = new char[msg.length()+1];
  msg.toCharArray(s, msg.length()+1);

  *tokenCount = 0;
   
  char* token = strtok(s, delimiter);
    
  while (token) {
        strcpy(tokens[*tokenCount],token);
        token = strtok(NULL, delimiter);

        (*tokenCount)++;
    }
  delete[] s;
}