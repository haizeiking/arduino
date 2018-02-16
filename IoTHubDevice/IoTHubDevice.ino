#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>

#include <AzureIoTUtility.h>
#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include "Timer.h"

String ssid                         = "ssid";        // your network SSID (name)
String pass                         = "pass";  // your network password (use for WPA, or use as key for WEP)
static const char* connectionString = "device_connection_string";
                      

IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
IOTHUB_CLIENT_STATUS status;

WiFiClientSecure espClient;

Timer t;

void initWifi() {
    if (WiFi.status() != WL_CONNECTED) 
    {
        WiFi.stopSmartConfig();
        WiFi.enableAP(false);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        WiFi.begin(ssid.c_str(), pass.c_str());
    
        Serial.print("Waiting for Wifi connection.");
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
        }
    
        Serial.println("Connected to wifi");

        initTime();
        initIoTHub();
    }
}

void initTime() {
    time_t epochTime;

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true) {
        epochTime = time(NULL);

        if (epochTime == 0) {
            Serial.println("Failed fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        } else {
            Serial.print("Fetched NTP epoch time is: ");
            Serial.println(epochTime);
            break;
        }
    }
}

static void sendMessage(const char* message)
{
    static unsigned int messageTrackingId;
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromString(message);

    if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendMessageCallback, (void*)(uintptr_t)messageTrackingId) != IOTHUB_CLIENT_OK)
    {
        Serial.println(" ERROR: Failed to hand over the message to IoTHubClient");
    }
    else
    {
      (void)printf(" Message Id: %u Sent.\r\n", messageTrackingId);
    }

    IoTHubMessage_Destroy(messageHandle);
    messageTrackingId++;
}

void sendMessageCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    unsigned int messageTrackingId = (unsigned int)(uintptr_t)userContextCallback;

    (void)printf(" Message Id: %u Received.\r\n", messageTrackingId);
}

static IOTHUBMESSAGE_DISPOSITION_RESULT IoTHubMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    IOTHUBMESSAGE_DISPOSITION_RESULT result = IOTHUBMESSAGE_ACCEPTED;
    
    const char* messageId = "UNKNOWN";      // in case there is not a messageId associated with the message -- not required
    messageId = IoTHubMessage_GetMessageId(message);

    const unsigned char* buffer;
    size_t size;
    if (IoTHubMessage_GetByteArray(message, &buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        Serial.println(" Error: Unable to IoTHubMessage_GetByteArray");
        result = IOTHUBMESSAGE_ABANDONED;
    }
    else
    {
        char* tempBuffer = (char*)malloc(size + 1);
        if (tempBuffer == NULL)
        {
            Serial.println(" Error: failed to malloc");
            result = IOTHUBMESSAGE_ABANDONED;
        }
        else
        {
            result = IOTHUBMESSAGE_ACCEPTED;
            (void)memcpy(tempBuffer, buffer, size);
            
            String messageStringFull((char*)tempBuffer);
            String messageString = "UNKNOWN";
            messageString = messageStringFull.substring(0,size);

/*            if (messageString.startsWith("OTA")) {
                  String fullURL = messageString.substring(messageString.indexOf("://") - 4);;
                  // t_httpUpdate_return OTAStatus = OTA.update(fullURL.c_str());
                  // if we do OTA, then we never return the IOTHUBMESSAGE_ACCEPTED and we have issues
            }*/
            
            String messageProperties = "";
            MAP_HANDLE mapProperties = IoTHubMessage_Properties(message);
            if (mapProperties != NULL)
            {
            const char*const* keys;
            const char*const* values;
            size_t propertyCount = 0;
            if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
                {
                if (propertyCount > 0)
                    {
                    size_t index;
                    for (index = 0; index < propertyCount; index++)
                        {
                            messageProperties += keys[index];
                            messageProperties += "=";
                            messageProperties += values[index];
                            messageProperties += ",";
                        }
                    }
                }
            }

            Serial.print(" Message Id: ");
            Serial.print(messageId);
            Serial.print(" Received. Message: \"");
            Serial.print(messageString);
            Serial.print("\", Properties: \"");
            Serial.print(messageProperties);
            Serial.println("\"");
        }
        free(tempBuffer);
    }
    return result;
}

void initIoTHub() {
  iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
  if (iotHubClientHandle == NULL)
  {
      (void)printf("ERROR: Failed on IoTHubClient_LL_Create\r\n");
  } else {
    IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, IoTHubMessageCallback, NULL);
  }
}

void LEDOn() {
  digitalWrite(LED_BUILTIN, LOW);
}

void LEDOff() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  initWifi();
  pinMode(LED_BUILTIN, OUTPUT);
  LEDOff();
  t.every(60*10*1000, ping);
}

void loop() {
  t.update();
}

void ping() {
  initWifi();         // always checking the WiFi connection
  LEDOn();
  
  // we will process every message in the Hub
  while ((IoTHubClient_LL_GetSendStatus(iotHubClientHandle, &status) == IOTHUB_CLIENT_OK) && (status == IOTHUB_CLIENT_SEND_STATUS_BUSY))
  {
      IoTHubClient_LL_DoWork(iotHubClientHandle);
      ThreadAPI_Sleep(1000);
  }
  
  String  JSONMessage = "{\'name\':";
          JSONMessage += "\'ESP01Work\'";
          JSONMessage += "}";
  //Serial.println(JSONMessage.c_str());
  sendMessage(JSONMessage.c_str());  

  LEDOff();
}
