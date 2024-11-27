#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DHTTYPE DHT11
const char* ssid = "HG531V1-J6KXSK";
const char* password = "F3M3NT7FZJ";
const char* serverName = "http://127.0.0.1:8000/api/post";
//const char* serverName = "http://monitoringtemperatureandhumi.pythonanywhere.com/api/temperaturehumidity";
#define dht_dpin 14
DHT dht(dht_dpin, DHTTYPE);

void setup() {
  dht.begin();
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();         
  
  Serial.print("Current humidity = ");
  Serial.print(humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(temperature); 
  Serial.println("C  ");
 
  // Envoi des valeurs au serveur Web
  WiFiClient client;
  HTTPClient http;
  DynamicJsonDocument jsonDoc(200); // Utilisation de DynamicJsonDocument pour la sérialisation JSON
  jsonDoc["temp"] = temperature;
  jsonDoc["hum"] = humidity;
  String jsonStr;
  serializeJson(jsonDoc, jsonStr);
  
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");
  Serial.println(jsonStr);
  int httpResponseCode = http.POST(jsonStr);
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  // Attente de 10 secondes avant de lire les valeurs suivantes
  delay(1200000);
}
