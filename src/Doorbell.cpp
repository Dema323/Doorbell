#include <ArduinoOTA.h>
#include <Arduino.h>
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b982a9763e0f41468dc6f433ed87454d";

char ssid[] = "WiFi-2.4-D4F0";
char pass[] = "A26YXkP73nuz";

WidgetBridge bridge1(V1);
WidgetBridge bridge2(V2);

BLYNK_CONNECTED() {
bridge1.setAuthToken("7a4a7059df0c4336b5af5bfac5cd0bab"); // Roomhub
bridge2.setAuthToken("7d24ea1e22264d179b9e4223d6936665");
}

// We make these values volatile, as they are used in interrupt context
volatile bool pinChanged = false;
volatile int  pinValue   = 0;

// Most boards won't send data to WiFi out of interrupt handler.
// We just store the value and process it in the main loop.
void checkPin()
{
  // Invert state, since button is "Active LOW"
  pinValue = !digitalRead(D1);

  // Mark pin value changed
  pinChanged = true;
}

void setup()
{
  WiFi.mode(WIFI_STA);
  Blynk.begin(auth, ssid, pass);
  while (Blynk.connect() == false) {}
  //ArduinoOTA.setHostname("xxxxxxx"); // OPTIONAL
  ArduinoOTA.begin();
  pinMode(D1, INPUT_PULLUP);
  pinMode(D4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(D1), checkPin, CHANGE);
}

void Doorbell()
{
  if (pinChanged) {

    // Process the value
    if (pinValue) {
      bridge1.virtualWrite(V3, HIGH);
      bridge2.virtualWrite(V11, HIGH);
      digitalWrite(D4, HIGH);
      delay(1000);
      digitalWrite(D4, LOW);
    }else{
    bridge1.virtualWrite(V3, LOW);
    bridge2.virtualWrite(V11, LOW);
    }

    // Clear the mark, as we have processed the value
    pinChanged = false;
  }
}
void loop()
{
  Blynk.run();
  Doorbell();
  ArduinoOTA.handle();
}
