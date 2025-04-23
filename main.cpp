#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// --------- Konfigurasi WiFi & Blynk ----------
char auth[] = "g33I3ZoI_7AI_CBDL2ZareeD3KFicOsK";
char ssid[] = "Glorhya";
char pass[] = "00000000";

// --------- Konfigurasi DHT ----------
#define DHTTYPE DHT11
#define DHTPIN1 22  // DHT11 Ruang 1
#define DHTPIN2 19  // DHT11 Ruang 2
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

// --------- Konfigurasi Relay ----------
#define RELAY1 5   // Kipas Ruang 1
#define RELAY2 4   // Kipas Ruang 2

// --------- Nilai Sensor ---------
float suhu1, suhu2, hum1, hum2;

void setup() {
  Serial.begin(115200);

  // Mulai koneksi WiFi dan Blynk
  Blynk.begin(auth, ssid, pass);

  // Mulai DHT
  dht1.begin();
  dht2.begin();

  // Atur pin relay sebagai OUTPUT
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Matikan kipas di awal
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}

void loop() {
  Blynk.run();

  // Baca sensor
  suhu1 = dht1.readTemperature();
  hum1 = dht1.readHumidity();
  suhu2 = dht2.readTemperature();
  hum2 = dht2.readHumidity();

  // Cek apakah pembacaan valid
  if (isnan(suhu1) || isnan(hum1) || isnan(suhu2) || isnan(hum2)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Tampilkan di Serial Monitor
  Serial.print("Ruang 1 - Suhu: "); Serial.print(suhu1); Serial.print(" C, Kelembaban: "); Serial.print(hum1); Serial.println(" %");
  Serial.print("Ruang 2 - Suhu: "); Serial.print(suhu2); Serial.print(" C, Kelembaban: "); Serial.print(hum2); Serial.println(" %");

  // Kirim ke Blynk
  Blynk.virtualWrite(V3, suhu1);
  Blynk.virtualWrite(V4, suhu2);
  Blynk.virtualWrite(V6, hum1);
  Blynk.virtualWrite(V5, hum2);

  // Kontrol kipas otomatis berdasarkan suhu
  digitalWrite(RELAY1, suhu1 >= 35 ? HIGH : LOW);
  digitalWrite(RELAY2, suhu2 >= 35 ? HIGH : LOW);

  // Kirim status kipas ke Blynk
  Blynk.virtualWrite(V1, digitalRead(RELAY1));
  Blynk.virtualWrite(V2, digitalRead(RELAY2));

  delay(2000);  // Delay 2 detik
}
