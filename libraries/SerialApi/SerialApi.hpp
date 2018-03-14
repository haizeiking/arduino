//
//  SerialApi.hpp
//  EP
//
//  Created by <author> on 11/04/2017.
//
//

#ifndef SerialApi_hpp
#define SerialApi_hpp

#define BUFFER_SIZE 50
#define PAYLOAD_SIZE 30

#define Arduino 1
//#define Due 1
//#define Linux 1
//#define Windows 1



//node-id ; child-sensor-id ; command ; ack ; type ; payload \n
struct SerialMessage {
    short NodeId;
    short ChildSensorId;
    short Command;
    short Ack;
    short Type;
    char* Payload;
};

class SerialApi {
private:
public:
    SerialApi();
    ~SerialApi();
    void ParseMessage(char buffer[BUFFER_SIZE], void (*handler)(struct SerialMessage m));
    void Serialize(struct SerialMessage m, char* buffer);
    struct SerialMessage CreateMessage(short nodeId
    ,short childSensorId
    ,short commandId
    ,short ack
    ,short type
    ,char* payload);
protected:

};

struct NodeFilter{
  short NodeId;
  short ChildSensorId;
void (*Handler)(struct SerialMessage m);
};

class MessageHandler {
private:
  struct NodeFilter handlers[100];
  short count;
public:
  MessageHandler(){};
  ~MessageHandler(){};
  void RegisterHandler(void (*handler)(struct SerialMessage m), short nodeId, short childId);
  void HandleMessage(SerialMessage msg);
};


#endif /* SerialApi_hpp */