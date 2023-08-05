/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLLojn8Kor"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "9Aq492aWzDVchaZtdTJl0AwokjQfzKRC"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PLN sigli";
char pass[] = "chelseaucl";

//  Contoh URL path menggunakan domain name atau IP
const char* serverTemp = "http://192.168.129.165/temp";
const char* serverFire = "http://192.168.129.165/fire";
const char* serverGas = "http://192.168.129.165/gas";
const char* serverBuzz = "http://192.168.129.165/buzzer";
const char* serverHumid = "http://192.168.129.165/humid";

String temp;
String fire;
String gas;
String buzz;
String humid;

unsigned long previousMillis = 0;
const long interval = 2000;

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
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

  Blynk.begin(auth, ssid, pass);
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

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
          previousMillis = currentMillis;
        //if(WiFi.status() == WL_CONNECTED){
            // Tambahkan fungsionalitas disini
            // Contoh
            // temperature = httpGETRequest(serverNameTemp);
            temp = httpGETRequest(serverTemp);
            humid = httpGETRequest(serverHumid);
            gas = httpGETRequest(serverGas);
            fire = httpGETRequest(serverFire);
            buzz = httpGETRequest(serverBuzz);
            Serial.print("temp: ");
            Serial.println(temp);
            Serial.print("humid: ");
            Serial.println(humid);
            Serial.print("fire: ");
            Serial.println(fire);
            Serial.print("gas: ");
            Serial.println(gas);
            Serial.print("buzz: ");
            Serial.println(buzz);
            Blynk.virtualWrite(V4, temp);
            Blynk.virtualWrite(V5, humid);
            Blynk.virtualWrite(V6, fire);
            Blynk.virtualWrite(V7, gas);                        
            if(buzz.equals("1")){
              Blynk.logEvent("fire_alert");
              Serial.println("Fire Detected");
              Blynk.virtualWrite(V8, buzz);
              delay(5000);
              buzz = "0";
              Blynk.virtualWrite(V8, buzz);
            }
            else if (buzz.equals("0")){
              Serial.println("No Flame");
            }
        //}
        //else {
        //  Serial.println("Wifi Disconnect");
        //}
        delay(1000);
    }
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

String httpGETRequest(const char* serverName){
    WiFiClient client;
    HTTPClient http;
    
    // URL path menggunakan nama domain atau IP Address
    http.begin(client, serverName);
    
    // Mengirim request HTTP POST
    int httpResponseCode = http.GET();
    
    String payload = "--";
    
    if(httpResponseCode > 0){
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else{
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    
    // Membebaskan resource
    http.end();
    
    return payload;
}
