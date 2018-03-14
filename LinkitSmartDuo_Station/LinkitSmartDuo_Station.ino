#include "IRLibAll.h"
#include <string.h>
#include <stdlib.h>

//serial msg: node;cmd;payload

#define JSON_BUFFER_SIZE 70
#define TOKEN_COUNT 3


//Create a sender
IRsend mySender; 

void Tokenize(String msg, char* delimiter, char **tokens, short* tokenCount);
void HandleIR(char payload[JSON_BUFFER_SIZE/TOKEN_COUNT]);

 

void setup() {
  Serial1.begin(57600);
  delay(2000);
  Serial1.println(F("Ready"));
}
 
void loop() {
   ReadSerial();
   
}

void ParseMessage(String msg) {
 char tokens[TOKEN_COUNT][JSON_BUFFER_SIZE/TOKEN_COUNT];
 char** p = tokens;
 short tokenCount;
 Tokenize(msg,";",p, &tokenCount);

 for (short i=0; i<tokenCount;i++) {
   Serial1.println(tokens[i]);
 }

 if (strcmp(tokens[0], "ir")) {
   HandleIR(tokens[2]);
 }

}


void HandleIR(char payload[JSON_BUFFER_SIZE/TOKEN_COUNT]) {
   Serial1.println("IR message");
    char codes[3][10];
    short tokensCount;

    Tokenize(String(payload), ",", codes, &tokensCount);
   
    for (short i=0; i<tokensCount; i++) {
      long code = GetCodeFromHex(tokens[i]);
      mySender.send(7, code, 32);
      delay(200);
      Serial1.println("sent");
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
    delay(100);
 
    ParseMessage(msg);
  }
}


  
void Tokenize(String msg, char* delimiter, char** tokens, short* tokenCount)
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