#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

/* ================== PINS ================== */
// RFID (same as your working test)
#define SS_PIN   10
#define RST_PIN  2

// Servo
#define SERVO_PIN 3

// LEDs
#define LED_OK   8   // green
#define LED_BAD  9   // orange

// Buzzer
#define BUZZER_PIN 21

// Sensors
#define PIR_PIN     7
#define GAS_DO_PIN  1

/* ================== RFID ================== */
MFRC522 rfid(SS_PIN, RST_PIN);

// Your accepted card UID
byte ACCEPT_UID[4] = { 0x60, 0x80, 0x0E, 0x61 };

/* ================== SERVO ================== */
Servo door;

/*
  Continuous servo rules:
  - STOP is the "neutral" pulse (often ~1500 but can be different!)
  - Open/Close are made by adding/subtracting speed from STOP
*/
#define SERVO_STOP_US 1500     // if directions act weird, try 1470 or 1530

// SPEED (bigger number = faster spin). 120..300 typical.
#define SERVO_SPEED_US 250

// CHANGE THESE TIMES to increase movement (your "door angle")
#define OPEN_TIME_MS   900     // ⬅ increase for bigger opening
#define CLOSE_TIME_MS  900     // ⬅ increase for bigger closing

// If servo moves the wrong direction, flip this from 0 to 1
#define INVERT_DIR 0

int servoOpenUS() {
  // Anti-clockwise vs clockwise depends on your servo
  // This formula guarantees opposite directions around STOP
  return INVERT_DIR ? (SERVO_STOP_US + SERVO_SPEED_US) : (SERVO_STOP_US - SERVO_SPEED_US);
}
int servoCloseUS() {
  return INVERT_DIR ? (SERVO_STOP_US - SERVO_SPEED_US) : (SERVO_STOP_US + SERVO_SPEED_US);
}

/* ================== HELPERS ================== */
void beep(int ms) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

bool uidMatch(byte *uid) {
  for (int i = 0; i < 4; i++) {
    if (uid[i] != ACCEPT_UID[i]) return false;
  }
  return true;
}

void doorStop() {
  door.writeMicroseconds(SERVO_STOP_US);
}

void doorOpen() {
  door.writeMicroseconds(servoOpenUS());
  delay(OPEN_TIME_MS);
  doorStop();
}

void doorClose() {
  door.writeMicroseconds(servoCloseUS());
  delay(CLOSE_TIME_MS);
  doorStop();
}

/* ================== SETUP ================== */
void setup() {
  Serial.begin(115200);

  pinMode(LED_OK, OUTPUT);
  pinMode(LED_BAD, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(GAS_DO_PIN, INPUT);

  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_BAD, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Servo
  door.setPeriodHertz(50);
  door.attach(SERVO_PIN, 500, 2400);
  doorStop();

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("SYSTEM READY");
}

/* ================== LOOP ================== */
void loop() {

  /* ---------- PIR (beep once per motion) ---------- */
  if (digitalRead(PIR_PIN) == HIGH) {
    beep(120);
    delay(1000); // cooldown so it doesn't spam
  }

  /* ---------- GAS (3 short beeps) ---------- */
  if (digitalRead(GAS_DO_PIN) == LOW) {
    beep(80); delay(80);
    beep(80); delay(80);
    beep(80);
    delay(1500); // cooldown
  }

  /* ---------- RFID ---------- */
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    bool ok = uidMatch(rfid.uid.uidByte);

    if (ok) {
      digitalWrite(LED_OK, HIGH);
      beep(100); delay(100); beep(100);

      doorOpen();
      delay(1500);
      doorClose();

      digitalWrite(LED_OK, LOW);

    } else {
      digitalWrite(LED_BAD, HIGH);
      beep(400);
      digitalWrite(LED_BAD, LOW);
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
