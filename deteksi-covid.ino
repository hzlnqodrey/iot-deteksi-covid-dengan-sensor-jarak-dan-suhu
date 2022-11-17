// Import Library
#include "FirebaseESP8266.h"  //  (https://github.com/mobizt/Firebase-ESP8266)
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h>
#include <time.h>
#include <DHT.h>        // adafruit unified sensor
#include <NTPClient.h>

// Firebase Config
#define FIREBASE_HOST "https://tedi-backend-web-default-rtdb.asia-southeast1.firebasedatabase.app/" // Change to your Firebase RTDB project ID e.g. Your_Project_ID.firebaseio.com
#define FIREBASE_AUTH "IrNMHPxzEy9sMGXZAYSmVidwHG1ycdYSS1MlJDBB"
// Define Firebase Data Objects
FirebaseData firebaseData;
String path = "/Node";
String nodeID = "WemosD1Mini";

// Wifi Config
char SSID[] = "Aderald";        // harus tersambung wifi dulu bukan tethering
char PASS[] = "AderaldAbghari";

// Declare Static Variable
#define ECHO_PIN D5
#define TRIGGER_PIN D6
#define buzzer D2
#define RED_LED D0
#define YLW_LED D1
#define DHTPIN D7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Declare Helper Variable
long waktu;
int jarak;

const long utcOffsetInSeconds = 25200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);

void setup()
{
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
    dht.begin();

    Serial.println("mulai");

    initWifi();
    timeClient.begin();
}

void loop()
{

    WiFiClient client;
    timeClient.update();
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
    jarak = waktu * (0.0345 / 2);
    // [*] Info pembantu di serial monitor
    Serial.print("Jarak : ");
    Serial.print(jarak);
    Serial.print(" cm  ");

    // 2. Temperature Configuration
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");

    // 3. Statement pengendalian pendeteksi covidnya
    if (jarak <= 100)
    {
        String waktu = String(timeClient.getFormattedTime());
        if (Firebase.setString(firebaseData, "/data_terakhir", waktu))
        {
            // Serial.println("Data terkirim!");
        }

        if (Firebase.setString(firebaseData, "/data_jarak", jarak)) {
            // Serial.println("Data  Jarak terkirim!");
        }

        // 4. Alarm yang memberi tahu kalau ada orang yang mendekat ke sensor
        // - LED Kuning Menyala
        // - Buzzer berbunyi dari frekuensi kecil ke besar
        digitalWrite(YLW_LED, LOW);
        delay(100);
        digitalWrite(YLW_LED, HIGH);
        delay(100);
        tone(buzzer, 100, 1000); // Send 100 Hz sound signal
        delay(1000);
        tone(buzzer, 250, 1000); // Send 250 Hz sound signal
        delay(1000);
        tone(buzzer, 500, 1000); // Send 500 Hz sound signal
        delay(1000);
        tone(buzzer, 700, 1000); // Send 700 Hz sound signal
        delay(1000);
        tone(buzzer, 800, 1000); // Send 800 Hz sound signal
        delay(1000);
        tone(buzzer, 1000, 1000); // Send 1 KHz sound signal
        delay(1000);

        // 5. Jika Suhu Orang di depan sensor kita lebih dari 35C, maka berikan notif pada alarm
        // - LED Merah
        // - BUZZER berbunyi cepat
        // - kirim format waktu dan data temperature ke firebase real time database

        // karena sensor suhu yang dipakai itu untuk membaca "AMBIENCE TEMPERATUR" / Temperatur ruangan, maka untuk mencapai
        // suhu tubuh manusia itu tidak memungkinkan | jadi kita set logic tempnya cuman 20 derajat Celcius
        while (t >= 20.0)
        {
            if (Firebase.setFloat(firebaseData, "/temperature_in", t))
            {
                // Serial.println("Data suhu terkirim!");
            }

            digitalWrite(RED_LED, LOW);
            delay(100);
            digitalWrite(RED_LED, HIGH);

            delay(100);
            digitalWrite(YLW_LED, LOW);

            tone(buzzer, 1000);
            delay(50);
            noTone(buzzer);
            delay(50);
        }
    }
    else
    {
        digitalWrite(YLW_LED, LOW);
        delay(100);
        noTone(buzzer);
        delay(200);
    }
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

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    // Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    // tiny, small, medium, large and unlimited.
    // Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(firebaseData, "tiny");

    if (!Firebase.beginStream(firebaseData, path + "/" + nodeID))
    {
        Serial.println("Could not begin stream");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println();
    }
}