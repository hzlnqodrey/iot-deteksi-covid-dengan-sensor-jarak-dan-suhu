/*************************************************************
  This example shows how to synchronize Button widget
    and physical button state.

    App project setup:
      Button widget attached to V2 (Switch mode)
 
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLUHzZCPAy"
#define BLYNK_DEVICE_NAME           "Kontrol Lampu LED"
#define BLYNK_AUTH_TOKEN            "L98NhSLVpiLBUuvvbuBtHit2YbSIA1la"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define USE_WEMOS_D1_MINI

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>

// Wifi Config
char SSID[] = "Bolivar Outdoor";        // harus tersambung wifi dulu bukan tethering
char PASS[] = "PEANUTBUTTER";

char AUTH[] = BLYNK_AUTH_TOKEN;


#define GRN_LED D1
#define YLW_LED D0
#define RED_LED D5

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V1) // this command is listening when something is written to V1
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  if (pinValue == 1){
   // do something when button is pressed;
   digitalWrite(GRN_LED, HIGH);
  } else if (pinValue == 0) {
   // do something when button is released;
   digitalWrite(GRN_LED, LOW);
  }
  
  Serial.print("V1 button value is: "); // printing value to serial monitor
  Serial.println(pinValue);
}

BLYNK_WRITE(V0) // this command is listening when something is written to V1
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  if (pinValue == 1){
   // do something when button is pressed;
   digitalWrite(YLW_LED, HIGH);
   digitalWrite(RED_LED, HIGH);
  } else if (pinValue == 0) {
   // do something when button is released;
   digitalWrite(YLW_LED, LOW);
   digitalWrite(RED_LED, LOW);
  }
  
  Serial.print("V3 button value is: "); // printing value to serial monitor
  Serial.println(pinValue);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V0);
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  pinMode(GRN_LED, OUTPUT);
  pinMode(YLW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  Serial.println("mulai");
  initWifi();
  Blynk.begin(AUTH, SSID, PASS);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  // digitalWrite(GRN_LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(500);
  // digitalWrite(GRN_LED, LOW);  // turn the LED on (HIGH is the voltage level)
  // delay(500);
  
  // digitalWrite(YLW_LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(500); 
  // digitalWrite(YLW_LED, LOW);  // turn the LED on (HIGH is the voltage level)
  // delay(500);

  // digitalWrite(RED_LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(500);
  // digitalWrite(RED_LED, LOW);  // turn the LED on (HIGH is the voltage level)
  // delay(500);
}

void initWifi()
{
    Serial.println();
    Serial.println();

    WiFi.begin(SSID, PASS);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

}
