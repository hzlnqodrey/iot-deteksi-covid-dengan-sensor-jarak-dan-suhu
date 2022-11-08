// Import Library
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <time.h>

// Firebase Config
#define FIREBASE_HOST "https://tedi-backend-web-default-rtdb.asia-southeast1.firebasedatabase.app/"  //Change to your Firebase RTDB project ID e.g. Your_Project_ID.firebaseio.com
#define FIREBASE_AUTH "IrNMHPxzEy9sMGXZAYSmVidwHG1ycdYSS1MlJDBB"
    // Define Firebase Data Objects
    FirebaseData firebaseData;
    String path = "/Node";
    String nodeID = "WemosD1Mini";

// Wifi Config
char SSID[] = "Hazlan Muhammad Qodri's iPhone";
char PASS[] = "nantiduluya";


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    initWifi();
}

void loop() {


}

void initWifi() {
    Serial.println();
    Serial.println();

    WiFi.begin(SSID, PASS);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    //tiny, small, medium, large and unlimited.
    //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(firebaseData, "tiny");

    if (!Firebase.beginStream(firebaseData, path + "/" + nodeID))
    {
        Serial.println("Could not begin stream");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println();
    }
}