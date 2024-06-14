#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Konstanta WiFi
const char *WIFI_SSID = "30i"; // Ganti dengan SSID WiFi Anda
const char *WIFI_PASS = "0000aaaa"; // Ganti dengan password WiFi Anda

// Pin sensor
#define DHTPIN 34     
#define DHTTYPE DHT11   
int sensor_input = 34; 

// Objek global
DHT dht(DHTPIN, DHTTYPE);

float get_temperature_data() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Error reading temperature!"));
    return 0.0; 
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.println(F("°C"));
    return t;
  }
}

float get_humidity_data() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println(F("Error reading humidity!"));
    return 0.0; 
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.println(F("%"));
    return h;
  }
}

int get_air_quality_data() {
  int analog_data = analogRead(sensor_input);
  float voltage = analog_data * (5.0 / 4095.0);
  int air_quality = int(voltage * 100); 
  Serial.print("Air Quality: ");
  Serial.println(air_quality);
  delay(300); 
  return air_quality;
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Koneksi WiFi
  Serial.println("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperature = get_temperature_data();
  float humidity = get_humidity_data();
  int air_quality = get_air_quality_data();

  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.116.49:5000/sensor/data"); // Ganti dengan alamat IP komputer Anda
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"air_quality\":" + String(air_quality) + "}";
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(2000); 
}
