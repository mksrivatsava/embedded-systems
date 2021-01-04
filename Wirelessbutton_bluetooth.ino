#include <rm4.h>//requried librarey for YKO4 reciver
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(5,6);

static const int EnablePin = 13;

static const int kD0 = 9;
static const int kD1 = 10;
static const int kD2 = 11;
static const int kD3 = 12;

RM4 remote(kD0, kD1, kD2, kD3);

void setup() {
  
  BTSerial.begin(38400);

  // Turn on the receiver.
  pinMode(EnablePin, OUTPUT);
  digitalWrite(EnablePin, HIGH);
}

void loop() {
  
  const int button_code = remote.buttonCode();
   if (button_code == 8) {   //here 8 is default value in remote.buttonCode when nothing is pressed.                 
    delay (2000);                                
  } else {                                        
     if (BTSerial.available())
    Serial.write(BTSerial.read());                  
}
}
