#define BLYNK_TEMPLATE_ID "TEMPLATE ID"
#define BLYNK_DEVICE_NAME "DEVICE NAME"
#define BLYNK_AUTH_TOKEN "BLYNK AUTH TOKEN"
#define BLYNK_PRINT Serial

#include <FirebaseESP32.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TridentTD_LineNotify.h>

#include "DHT.h"
#define DHTPIN 13    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22
#define LINE_TOKEN "LINE TOKEN"
#define LIGHTPIN 35
#define SOILPIN 34
#define WateringPin 27
#define RelayPin 5

DHT dht(DHTPIN, DHTTYPE);

const char ssid[] = "PeeNong";
const char pass[] = "11111111";
const char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;

void timerEvent();
void lineEvent();

byte t;
byte h;
byte hic;
int val;
int val1;
int newval;
int newval1;
bool isFirstConnect = true;

BLYNK_CONNECTED() {
 if(isFirstConnect) {
   Blynk.syncAll();
   Blynk.syncVirtual(V5);
   isFirstConnect = false;
 }
}

BLYNK_WRITE(V5) {
 if (param.asInt() == 1) {
   if (digitalRead(WateringPin) != LOW) {
      digitalWrite(WateringPin, LOW);
      digitalWrite(RelayPin, LOW);
   }
 }
 else {
  digitalWrite(WateringPin, HIGH);
  digitalWrite(RelayPin, HIGH);
 }
}

#define API_KEY "AIzaSyBhI5oYvb9WVDxGrWFRuKzHqhjz8D7UknI"
#define DATABASE_URL "https://smartfarm-23892-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "aruchakhem@gmail.com"
#define USER_PASSWORD "adz20080"

FirebaseData fbdo;
FirebaseAuth authe;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(10000L, CheckConnection);
  timer.setInterval(1000L, timerEvent);
  timer.setInterval(3600000L, lineEvent);


  xTaskCreate(&Task1, "Task1",  2000, NULL, 8, NULL);
  xTaskCreate(&Task2, "Task2", 3000, NULL, 10, NULL);
  
  Serial.println(LINE.getVersion());

  dht.begin();
  LINE.setToken(LINE_TOKEN);
  pinMode(WateringPin, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, HIGH);

  config.api_key = API_KEY;

  authe.user.email = USER_EMAIL;
  authe.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.signer.tokens.legacy_token = "cYCzcJ7yNLE5kSzGFdtejihKzkpTpgeaby5w1i4k";
  
  Firebase.begin(&config, &authe);
}

void loop() {
  Blynk.run();
  timer.run();
}

void lineEvent(){
  t = dht.readTemperature();
  h = dht.readHumidity();
  hic = dht.computeHeatIndex(t, h, false);
  LINE.notify("อุณหภูมิ : " + (String)t + "°C");
  LINE.notify("ความชื้น : " + (String)h + "%");
  LINE.notify("ดัชนีความร้อน : " + (String)hic + "°C");
}


void timerEvent(){
  t = dht.readTemperature();
  h = dht.readHumidity();
  hic = dht.computeHeatIndex(t, h, false);
  val = analogRead(SOILPIN);
  newval = map(val, 0, 4095, 100, 0);
  val1 = analogRead(LIGHTPIN);
  newval1 = map(val1, 0, 4095, 100, 0);
  
  Blynk.virtualWrite(V0,t);
  Blynk.virtualWrite(V1,h);
  Blynk.virtualWrite(V2,hic);
  Blynk.virtualWrite(V3,newval);
  Blynk.virtualWrite(V4,newval1);
  Firebase.setIntAsync(fbdo, "/SmartFarm/Temperature", t);
  Firebase.setIntAsync(fbdo, "/SmartFarm/Humidity", h);
  Firebase.setIntAsync(fbdo, "/SmartFarm/HeatIndex", hic);
  Firebase.setIntAsync(fbdo, "/SmartFarm/Soil", newval);
  Firebase.setIntAsync(fbdo, "/SmartFarm/Light", newval1);
}

void CheckConnection(){
  if(!Blynk.connected()){
    Serial.println("Not connected to Blynk server.");
    Serial.println("Connecting to Blynk server...");
    Blynk.connect();
  }
}


void Task1(void *p) {
  while(1) {
    if(newval < 40 and newval != 0) {
     val = analogRead(SOILPIN);
     newval = map(val, 0, 4095, 100, 0);
     digitalWrite(WateringPin, 0);
     digitalWrite(RelayPin, LOW);
     Serial.println("Plant watering...");
     LINE.notify("Plant watering...");
     vTaskDelay(15000 / portTICK_PERIOD_MS);
     
     digitalWrite(RelayPin, HIGH);
     digitalWrite(WateringPin, 1);
     LINE.notify("Stop watering");
     Serial.println("Stop watering.");
    }
    
    vTaskDelay(36000 / portTICK_PERIOD_MS);
  }
}

void Task2(void *p) {
  while(1) {
    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi Disconnected.");
      WiFi.begin((char*)ssid, (char*)pass);
    }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
 }
}
