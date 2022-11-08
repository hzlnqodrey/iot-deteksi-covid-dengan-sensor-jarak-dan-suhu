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

// Declare Static Variable
#define ECHO_PIN 3
#define TRIGGER_PIN 4
#define buzzer 5
#define RED_LED 7
#define YLW_LED 8

// Declare Helper Variable
long waktu;
int jarak;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    
    // LED
    pinMode(RED_LED, OUTPUT);
    pinMode(YLW_LED, OUTPUT);
    // Buzzer
    pinMode(buzzer, OUTPUT);
    // Ultrasonik
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    // Temperature
    
    Serial.begin(9600);
    Serial.println("mulai");
    initWifi();
}

void loop() {

    // 1. Ultrasonic Configuration
    // [*] Matikan dahulu trigger pin
    digitalWrite(TRIGGER_PIN, LOW); 
    delayMicroseconds(2);
    // [*] Aktivasi trigger pin
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(15);
    digitalWrite(TRIGGER_PIN, LOW);
    // [*] Menghitung Waktu dari gelombang ultrasonik
    // echo pin sebagai signal receiver dari trigger pin yang 
    // bertugas sebagai signal transmitter
    waktu = pulseIn(ECHO_PIN, HIGH);
    // [*] Menghitung Jarak ( kec * duration )
    jarak = waktu * (0.0345 / 2 );
    // [*] Info pembantu di serial monitor
    Serial.print("Jarak : ");
    Serial.print(jarak);
    Serial.println(" cm");
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