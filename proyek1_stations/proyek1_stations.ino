#include "DHT.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <HTTPClient.h>

#define DHTPIN 18
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define BUZZER 5    // used for ESP32
#define FIRE 32    // used for ESP32
#define GAS 33
// int Fire_digital = 2;   // used for ESP32

// Mengatur SSID dan password untuk AP (Access Point)
const char* ssid = "PLN sigli";
const char* password = "chelseaucl";

AsyncWebServer server(80);
int humidity = 0;
int temperature = 0;
int fireSensor;
int gasSensor;

String temp = "0";
/*
String tempToString = "0"; // temperature
String humidityToString = "0"; // temperature
String fireToString = "0";
String gasToString = "0";
*/
String buzz = "0";
unsigned long previousMillis = 0;

// Contoh fungsi untuk bme
String readTemp(){
    return temp;
}

String readHumid(){
    return String(humidity);
}

String readFire(){
    return String(fireSensor);
}

String readGas(){
    return String(gasSensor);
}

String readAlarm(){
    return buzz;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(FIRE, INPUT);
  pinMode(GAS, INPUT);  

  // kosongkan parameter password apabila ingin membuka AP
  // Serial.print("Setting Access Point...");
  // Set Mode to Station
  WiFi.mode(WIFI_STA);
  /*
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP Address: ");
  Serial.println(IP);
  */

  WiFi.begin(ssid, password);
    Serial.println("Connecting...");
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // Fungsionalitas dapat ditambahkan disini
  // Contoh:
    
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });

  server.on("/humid", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumid().c_str());
  });
  
  server.on("/fire", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readFire().c_str());
  });

  server.on("/gas", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readGas().c_str());
  });

  server.on("/buzzer", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readAlarm().c_str());
  });
  
  dht.begin();
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 10000){
    previousMillis = currentMillis;
    humidity = dht.readHumidity();
    temperature = dht.readTemperature() + 273;
    temp = String(temperature);
    Serial.print("Temp String: ");
    Serial.println(temp);
    fireSensor = analogRead(FIRE);
    gasSensor = analogRead(GAS);
    buzz = "0";
    // int firesensorDigital = digitalRead(Fire_digital);

    if(isnan(humidity)||isnan(temperature)){
      Serial.println(F("Failed reading the DHT sensor"));
    }

    Serial.print(F("Fire Sensor: "));
    Serial.print(fireSensor);
    Serial.print("\t");
    Serial.print(F("Gas Sensor: "));
    Serial.print(gasSensor);
    Serial.print("\n");
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(F("% Temperature: "));
    Serial.print(temperature);
    Serial.println(F("K"));
    if (fireSensor < 1000 && gasSensor > 1000 && temperature > 298) {
      buzz = "1";
      Serial.println("Fire");
      digitalWrite (BUZZER, HIGH) ; //send tone
      delay(9000);
      digitalWrite (BUZZER, LOW) ;  //no tone
    }
    else {
      Serial.println("No Fire");
    }
    Serial.print("\n");
    delay(100);
  }
  
}