#include <Wire.h>
#include "RTClib.h"

// ======================================================
//        SMART SECURITY + GSM + AUTO FAN SYSTEM
// ======================================================

// ===================== PINS ===========================

#define PIR_PIN        13
#define FLAME_PIN      26
#define VIB_PIN        27
#define REED_PIN       14
#define RELAY_PIN      32

// ===================== GSM ============================

#define RXD2 16
#define TXD2 17

// ===================== RTC ============================

RTC_DS3231 rtc;

// ======================================================
//                    USER DATABASE
// ======================================================

const int TOTAL_USERS = 10;

String userNames[TOTAL_USERS] = {

  "USER A",
  "USER B",
  "USER C",
  "USER D",
  "USER E",
  "USER F",
  "USER G",
  "USER H",
  "USER I",
  "USER J"
};

String userNumbers[TOTAL_USERS] = {

  "+919999999991",
  "+919999999992",
  "+919999999993",
  "+919999999994",
  "+919999999995",
  "+919999999996",
  "+919999999997",
  "+919999999998",
  "+919999999999",
  "+919999999990"
};

// ======================================================
//                    SYSTEM VARIABLES
// ======================================================

bool cooldownActive = false;

unsigned long cooldownStart = 0;

const unsigned long COOLDOWN_TIME =
15UL * 60UL * 1000UL;

// ======================================================
//                    PIR VARIABLES
// ======================================================

bool pirDetected = false;

unsigned long pirMotionStart = 0;

unsigned long pirLastMotionTime = 0;

const unsigned long PIR_TRIGGER_TIME = 2000;

const unsigned long PIR_RESET_TIME = 3000;

// ======================================================
//                    FAN VARIABLES
// ======================================================

bool autoFanRunning = false;

unsigned long fanStartTime = 0;

unsigned long lastFanCycle = 0;

const unsigned long FAN_ON_TIME =
10UL * 60UL * 1000UL;

const unsigned long FAN_INTERVAL =
60UL * 60UL * 1000UL;

// ======================================================
//                    ALERT LOCK
// ======================================================

bool alertRunning = false;

// ======================================================

void setup() {

  Serial.begin(115200);

  // ================= GSM =================

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  delay(3000);

  Serial2.println("AT");
  delay(1000);

  Serial2.println("AT+CMGF=1");
  delay(1000);

  Serial2.println("AT+DDET=1");
  delay(1000);

  // ================= PINS =================

  pinMode(PIR_PIN, INPUT);

  pinMode(FLAME_PIN, INPUT);

  pinMode(VIB_PIN, INPUT);

  pinMode(REED_PIN, INPUT_PULLUP);

  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);

  // ================= RTC =================

  Wire.begin(21, 22);

  if (!rtc.begin()) {

    Serial.println("❌ RTC NOT FOUND");

    while (1);
  }

  // ================= PIR CALIBRATION =================

  Serial.println("");
  Serial.println("⏳ PIR CALIBRATING...");
  Serial.println("⏳ WAIT 30 SECONDS...");
  Serial.println("");

  delay(30000);

  // ================= DASHBOARD =================

  Serial.println("");
  Serial.println("======================================================");
  Serial.println("🚀 SMART SECURITY SYSTEM INITIALIZED");
  Serial.println("======================================================");

  Serial.println("✅ RTC CONNECTED");
  Serial.println("✅ GSM CONNECTED");
  Serial.println("✅ PIR READY");
  Serial.println("✅ FLAME READY");
  Serial.println("✅ VIBRATION READY");
  Serial.println("✅ DOOR SENSOR READY");
  Serial.println("✅ FAN READY");

  Serial.println("======================================================");
}

// ======================================================

void loop() {

  DateTime now = rtc.now();

  int currentHour = now.hour();

  // ===================================================
  //                  SECURITY TIME
  // ===================================================

  bool securityActive = false;

  // ACTIVE 9 PM → 5 AM

  if (currentHour >= 21 || currentHour < 5) {

    securityActive = true;
  }

  // ===================================================
  //                  COOLDOWN
  // ===================================================

  if (cooldownActive) {

    if (millis() - cooldownStart >= COOLDOWN_TIME) {

      cooldownActive = false;

      Serial.println("");
      Serial.println("✅ COOLDOWN FINISHED");
      Serial.println("🔒 SYSTEM ACTIVE AGAIN");
    }
    else {

      return;
    }
  }

  // ===================================================
  //                  FAN SYSTEM
  // ===================================================

  manageFan();

  // ===================================================
  //                  SERIAL HEADER
  // ===================================================

  dashboard(now, securityActive);

  // ===================================================
  //                  DAY MODE
  // ===================================================

  if (!securityActive) {

    Serial.println("🌞 DAY MODE ACTIVE");
    Serial.println("🔕 SECURITY SYSTEM OFF");
    Serial.println("======================================================");

    delay(1000);

    return;
  }

  // ===================================================
  //                  RAW SENSOR VALUES
  // ===================================================

  int pirRaw   = digitalRead(PIR_PIN);

  int flameRaw = digitalRead(FLAME_PIN);

  int vibRaw   = digitalRead(VIB_PIN);

  int reedRaw  = digitalRead(REED_PIN);

  // ===================================================
  //                  PIR FILTER
  // ===================================================

  if (pirRaw == HIGH) {

    pirLastMotionTime = millis();

    if (pirMotionStart == 0) {

      pirMotionStart = millis();
    }

    if ((millis() - pirMotionStart)
        >= PIR_TRIGGER_TIME) {

      pirDetected = true;
    }

  } else {

    if ((millis() - pirLastMotionTime)
        >= PIR_RESET_TIME) {

      pirDetected = false;

      pirMotionStart = 0;
    }
  }

  // ===================================================
  //                  SENSOR STATUS
  // ===================================================

  bool flameDetected = (flameRaw == LOW);

  bool vibrationDetected = (vibRaw == LOW);

  bool doorDetected = (reedRaw == HIGH);

  // ===================================================
  //                  SERIAL MONITOR
  // ===================================================

  Serial.print("👤 PIR SENSOR        : ");

  if (pirDetected)
    Serial.println("🚨 HUMAN DETECTED");
  else
    Serial.println("✅ NORMAL");

  Serial.print("🔥 FLAME SENSOR      : ");

  if (flameDetected)
    Serial.println("🔥 FIRE DETECTED");
  else
    Serial.println("✅ SAFE");

  Serial.print("🔨 VIBRATION SENSOR  : ");

  if (vibrationDetected)
    Serial.println("⚠️ VIBRATION DETECTED");
  else
    Serial.println("✅ NORMAL");

  Serial.print("🚪 DOOR SENSOR       : ");

  if (doorDetected)
    Serial.println("🚪 DOOR OPEN");
  else
    Serial.println("🔒 DOOR CLOSED");

  Serial.print("💨 FAN STATUS        : ");

  if (digitalRead(RELAY_PIN))
    Serial.println("ON");
  else
    Serial.println("OFF");

  // ===================================================
  //                  RAW DEBUG
  // ===================================================

  Serial.println("------------------------------------------------------");

  Serial.print("RAW VALUES => ");

  Serial.print("PIR=");
  Serial.print(pirRaw);

  Serial.print(" | FLAME=");
  Serial.print(flameRaw);

  Serial.print(" | VIB=");
  Serial.print(vibRaw);

  Serial.print(" | REED=");
  Serial.println(reedRaw);

  // ===================================================
  //                  EMERGENCY FAN
  // ===================================================

  if (pirDetected ||
      flameDetected ||
      vibrationDetected ||
      doorDetected) {

    digitalWrite(RELAY_PIN, HIGH);

    Serial.println("💨 EMERGENCY FAN ACTIVATED");
  }

  // ===================================================
  //                  ALERT SYSTEM
  // ===================================================

  if (!alertRunning &&
      (pirDetected ||
       flameDetected ||
       vibrationDetected ||
       doorDetected)) {

    alertRunning = true;

    Serial.println("");
    Serial.println("🚨 SECURITY ALERT ACTIVATED");

    // ===============================================
    //              SMS ONLY ONE TIME
    // ===============================================

    for (int i = 0; i < TOTAL_USERS; i++) {

      String msg =
      "🚨 SECURITY ALERT 🚨\n"
      "Pump Room Threat Detected!\n"
      "Check Immediately.";

      sendSMS(userNumbers[i], msg);
    }

    // ===============================================
    //              CALLING SYSTEM
    // ===============================================

    startCallingSystem();

    // ===============================================
    //              COOLDOWN
    // ===============================================

    cooldownActive = true;

    cooldownStart = millis();

    alertRunning = false;

    Serial.println("");
    Serial.println("⏳ 15 MINUTE COOLDOWN STARTED");

    digitalWrite(RELAY_PIN, LOW);
  }

  Serial.println("======================================================");

  delay(1000);
}

// ======================================================
//                    FAN SYSTEM
// ======================================================

void manageFan() {

  unsigned long currentMillis = millis();

  // START AUTO FAN

  if (!autoFanRunning &&
      (currentMillis - lastFanCycle >= FAN_INTERVAL)) {

    autoFanRunning = true;

    fanStartTime = currentMillis;

    digitalWrite(RELAY_PIN, HIGH);

    Serial.println("💨 AUTO FAN STARTED");
  }

  // STOP AUTO FAN

  if (autoFanRunning &&
      (currentMillis - fanStartTime >= FAN_ON_TIME)) {

    autoFanRunning = false;

    lastFanCycle = currentMillis;

    digitalWrite(RELAY_PIN, LOW);

    Serial.println("💨 AUTO FAN STOPPED");
  }
}

// ======================================================
//                    SMS FUNCTION
// ======================================================

void sendSMS(String number, String text) {

  Serial.print("📨 SMS SENT TO : ");

  Serial.println(number);

  Serial2.print("AT+CMGS=\"");

  Serial2.print(number);

  Serial2.println("\"");

  delay(1000);

  Serial2.print(text);

  delay(500);

  Serial2.write(26);

  delay(4000);
}

// ======================================================
//                    CALLING SYSTEM
// ======================================================

void startCallingSystem() {

  Serial.println("");
  Serial.println("======================================================");
  Serial.println("📞 GSM CALLING SYSTEM STARTED");
  Serial.println("======================================================");

  bool responded[TOTAL_USERS];

  for (int i = 0; i < TOTAL_USERS; i++) {

    responded[i] = false;
  }

  bool allDone = false;

  // ===================================================
  //              WAITING LIST LOOP
  // ===================================================

  while (!allDone) {

    allDone = true;

    for (int i = 0; i < TOTAL_USERS; i++) {

      if (!responded[i]) {

        allDone = false;

        bool success =
        makeCall(userNumbers[i], userNames[i]);

        if (success) {

          responded[i] = true;

          Serial.println("✅ USER RESPONDED");

        } else {

          Serial.println("⏳ USER MOVED TO WAITING LIST");
        }
      }
    }
  }

  Serial.println("");
  Serial.println("✅ ALL USERS RESPONDED");
}

// ======================================================
//                    CALL FUNCTION
// ======================================================

bool makeCall(String number, String user) {

  Serial.println("");
  Serial.println("------------------------------------------------------");

  Serial.print("📞 CALLING : ");

  Serial.println(user);

  Serial.println(number);

  Serial.println("⏳ WAITING FOR RESPONSE");

  Serial2.print("ATD");

  Serial2.print(number);

  Serial2.println(";");

  unsigned long startCall = millis();

  while (millis() - startCall < 30000) {

    if (Serial2.available()) {

      String response =
      Serial2.readString();

      Serial.println(response);

      // USER PRESSED 1

      if (response.indexOf("+DTMF: 1") != -1) {

        Serial.println("✅ USER PRESSED 1");

        Serial2.println("ATH");

        delay(1000);

        return true;
      }

      // USER CUT / REJECT

      if (response.indexOf("BUSY") != -1 ||
          response.indexOf("NO CARRIER") != -1) {

        Serial.println("✅ USER RECEIVED ALERT");

        return true;
      }
    }

    delay(100);
  }

  // NO RESPONSE

  Serial.println("❌ NO RESPONSE");

  Serial2.println("ATH");

  delay(1000);

  return false;
}

// ======================================================
//                    DASHBOARD
// ======================================================

void dashboard(DateTime now, bool active) {

  Serial.println("");

  Serial.println("------------------------------------------------------");

  Serial.print("📅 DATE : ");

  Serial.print(now.day());

  Serial.print("/");

  Serial.print(now.month());

  Serial.print("/");

  Serial.print(now.year());

  Serial.print("   ⏰ TIME : ");

  Serial.print(now.hour());

  Serial.print(":");

  if (now.minute() < 10)
    Serial.print("0");

  Serial.print(now.minute());

  Serial.print(":");

  if (now.second() < 10)
    Serial.print("0");

  Serial.println(now.second());

  Serial.println("------------------------------------------------------");

  if (active) {

    Serial.println("🌙 NIGHT SECURITY MODE : ACTIVE");

  } else {

    Serial.println("🌞 DAY MODE ACTIVE");
  }

  Serial.println("------------------------------------------------------");
}
