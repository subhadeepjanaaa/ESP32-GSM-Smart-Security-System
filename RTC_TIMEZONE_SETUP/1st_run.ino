#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include "RTClib.h"

// ================= WIFI =================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ================= RTC =================
RTC_DS3231 rtc;

// India Timezone
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // UTC +5:30
const int daylightOffset_sec = 0;

void setup() {

  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!rtc.begin()) {
    Serial.println("RTC NOT FOUND");
    while (1);
  }

  // ================= WIFI CONNECT =================

  Serial.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  // ================= GET INTERNET TIME =================

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // ================= UPDATE RTC =================

  rtc.adjust(DateTime(
    timeinfo.tm_year + 1900,
    timeinfo.tm_mon + 1,
    timeinfo.tm_mday,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  ));

  Serial.println("RTC UPDATED FROM INTERNET TIME!");
}

void loop() {

  DateTime now = rtc.now();

  Serial.print("Date: ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());

  Serial.print(" Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  delay(1000);
}
