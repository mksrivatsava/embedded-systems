#include <SD.h>
#include "ThingSpeak.h"

#include <ESP8266HTTPClient.h>


#include  <ArduinoJson.h>

#include "ESP8266WiFi.h"

// your network SSID (name) & network password
char myssid[] = "Your wifi/hotspot name";
char mypass[] = "Your password";
unsigned long Channel_ID = 1259819; // Your Channel ID
const char * myWriteAPIKey = "03IRD338HUL7YY7R"; //Your write API key
// unwiredlabs Hostname & Geolocation Endpoint url
const char* Host = "www.unwiredlabs.com";
String endpoint = "/v2/process.php";

// UnwiredLabs API_Token. Signup here to get a free token https://unwiredlabs.com/trial
String token = "d99cccda52ec0b";

String jsonString = "{\n";

// Variables to store unwiredlabs response
float latitude = 0.0;
float longitude = 0.0;
float accuracy = 0.0;
const int Field_Number_1 = 1;
const int Field_Number_2 = 2;

WiFiClient  client;

void setup(){
    Serial.begin(115200);
    ThingSpeak.begin(client);


    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.println("Setup done");

    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(myssid);
    WiFi.begin(myssid, mypass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(".");
}

void loop() {
    char bssid[6];
    DynamicJsonBuffer jsonBuffer;

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");

    if (n == 0 ) {
        Serial.println("No networks available");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
    }

      jsonString = "{\n";
      jsonString += "\"token\" : \"";
      jsonString += token;
      jsonString += "\",\n";
      jsonString += "\"id\" : \"charan kaushal\",\n";
      jsonString += "\"wifi\": [\n";
      for (int j = 0; j < n; ++j) {
          jsonString += "{\n";
          jsonString += "\"bssid\" : \"";
          jsonString += (WiFi.BSSIDstr(j));
          jsonString += "\",\n";
          jsonString += "\"signal\": ";
          jsonString += WiFi.RSSI(j);
          jsonString += "\n";
          if (j < n - 1) {
              jsonString += "},\n";
          } else {
              jsonString += "}\n";
          }
      }
      jsonString += ("]\n");
      jsonString += ("}\n");
      Serial.println(jsonString);

      WiFiClientSecure client;
      client.setInsecure();

      //Connect to the client and make the api call
      Serial.println("Requesting URL: https://" + (String)Host + endpoint);
      if (client.connect(Host, 443)) {
          Serial.println("Connected");
          client.println("POST " + endpoint + " HTTP/1.1");
          client.println("Host: " + (String)Host);
          client.println("Connection: close");
          client.println("Content-Type: application/json");
          client.println("User-Agent: Arduino/1.0");
          client.print("Content-Length: ");
          client.println(jsonString.length());
          client.println();
          client.print(jsonString);
          delay(500);
      }

      //Read and parse all the lines of the reply from server
      while (client.available()) {
          String line = client.readStringUntil('\r');
          JsonObject& root = jsonBuffer.parseObject(line);
          if (root.success()) {
              latitude    = root["lat"];
              longitude   = root["lon"];
              accuracy    = root["accuracy"];

              Serial.println();
              Serial.print("Latitude = ");
              Serial.println(latitude, 6);
              Serial.print("Longitude = ");
              Serial.println(longitude, 6);
     
          }
      }

      Serial.println("closing connection");
      Serial.println();
      client.stop();
    upload();
      delay(5000);

}
      void upload()
{
  ThingSpeak.writeField(Channel_ID, Field_Number_1, latitude, myWriteAPIKey);
  delay(1000);
  ThingSpeak.writeField(Channel_ID, Field_Number_2, longitude, myWriteAPIKey);
  delay(1000);

  

}
