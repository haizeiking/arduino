//
//  SerialApi.cpp
//  EP
//
//  Created by <author> on 11/04/2017.
//
//

#include "SerialApi.hpp"

#include <string.h>
#include <stdlib.h>


#if defined(Linux) || defined(Due)
char* itoa(int value, char* str, int radix) {
    static char dig[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    int n = 0, neg = 0;
    unsigned int v;
    char* p, *q;
    char c;
    if (radix == 10 && value < 0) {
        value = -value;
        neg = 1;
    }
    v = value;
    do {
        str[n++] = dig[v%radix];
        v /= radix;
    } while (v);
    if (neg)
        str[n++] = '-';
    str[n] = '\0';
    for (p = str, q = p + (n-1); p < q; ++p, --q)
        c = *p, *p = *q, *q = c;
    return str;
}
#endif

void Tokenize(const char message[BUFFER_SIZE], char* delimiter, char groups[6][PAYLOAD_SIZE], short* groupCount)
{
    *groupCount = 0;

    char s[BUFFER_SIZE];
    strcpy(s, message);
    char* token = strtok(s, delimiter);
    while (token) {
        strcpy(groups[*groupCount],token);

        token = strtok(NULL, delimiter);

        (*groupCount)++;
    }
}


SerialApi::SerialApi()  {}
SerialApi::~SerialApi() {}

void SerialApi::ParseMessage(char buffer[BUFFER_SIZE], void (*handler)(struct SerialMessage m)){
  char tokens[6][PAYLOAD_SIZE];
  short groupCount;

  char sanitized[BUFFER_SIZE];
  unsigned short i=0, separatorCount=0;
  char c;
  while (i < BUFFER_SIZE){
    c = buffer[i];
    if (c == '\n' || c=='\r' || c=='\0'){
      break;
    }

    if (c == ';')
    {
      separatorCount++;
    }
    sanitized[i]=c;
    i++;
  }

  if (separatorCount != 5){
    return;
  }
  sanitized[i]='\0';

  Tokenize(sanitized, ";", tokens, &groupCount);

  if (groupCount != 6)
  {
    return;
  }

  struct SerialMessage serialMessage;
  serialMessage.NodeId = atoi(tokens[0]);
  serialMessage.ChildSensorId = atoi(tokens[1]);
  serialMessage.Command = atoi(tokens[2]);
  serialMessage.Ack = atoi(tokens[3]) == 0 ? 0 : 1;
  serialMessage.Type = atoi(tokens[4]);
  strcpy(serialMessage.Payload,tokens[5]);

  handler(serialMessage);
}
void SerialApi::Serialize(struct SerialMessage m, char* buffer){

  // enough to hold all numbers up to 64-bits - 21
  char numstr[10];

  char result[BUFFER_SIZE];

  strcpy(result,itoa(m.NodeId, numstr, 10));
  strcat(result,";");
  strcat(result,itoa(m.ChildSensorId, numstr, 10));
  strcat(result,";");
  strcat(result,itoa(m.Command, numstr, 10));
  strcat(result,";");
  strcat(result,itoa(m.Ack, numstr, 10));
  strcat(result,";");
  strcat(result,itoa(m.Type, numstr, 10));
  strcat(result,";");
  strcat(result,m.Payload);

  strcat(result,"\0");

  strcpy(buffer, result);
}

struct SerialMessage SerialApi::CreateMessage(short nodeId
,short childSensorId
,short commandId
,short ack
,short type
,char* payload)
{
  struct SerialMessage serialMessage;
  serialMessage.NodeId = nodeId;
  serialMessage.ChildSensorId = childSensorId;
  serialMessage.Command = commandId;
  serialMessage.Ack = ack == 0 ? 0 : 1;
  serialMessage.Type = type;
  serialMessage.Payload = payload;

  return serialMessage;
}

void MessageHandler::RegisterHandler(void (*handler)(struct SerialMessage m), short nodeId, short childId){
  struct NodeFilter filter;
  filter.NodeId = nodeId;
  filter.ChildSensorId = childId;
  filter.Handler = handler;
  this->handlers[this->count++]=filter;
}

void MessageHandler::HandleMessage(SerialMessage msg){
  for (short i=0;i < this->count; i++){
    if (handlers[i].NodeId == msg.NodeId && handlers[i].ChildSensorId == msg.ChildSensorId){
      handlers[i].Handler(msg);
      break;
    }
  }
}