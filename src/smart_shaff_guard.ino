///*
#include <Servo.h>

// === Ultrasonic A (Rakaat Counter) ===
#define trigA 10
#define echoA 9

// === Ultrasonic B (Orang Lewat + Alarm) ===
#define trigB 12
#define echoB 11

// === Servo ===
#define servoPin A5
Servo gate;

// === Buzzer ===
#define buzzerPin A4

// === 7-Segment Pins (Common Anode, D2–D8) ===
#define segA 2
#define segB 3
#define segC 4
#define segD 5
#define segE 6
#define segF 7
#define segG 8

// === Variabel ===
int rakaat = 0;
int sujudCount = 0;
bool sedangSujud = false;

// Threshold sujud
const int berdiriThresh = 25; 
const int sujudThresh  = 15;  

// Threshold alarm
const int alarmThresh = 60;   

// Fungsi baca ultrasonic
long readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long dur = pulseIn(echo, HIGH);
  long dist = dur * 0.034 / 2;
  return dist;
}

// ========== FUNCTION 7-SEG (Common Anode) ==========
void displayNumber(int num) {
  // LOW = ON, HIGH = OFF
  const byte digits[10][7] = {
    {0,0,0,0,0,0,1}, // 0
    {1,0,0,1,1,1,1}, // 1
    {0,0,1,0,0,1,0}, // 2
    {0,0,0,0,1,1,0}, // 3
    {1,0,0,1,1,0,0}, // 4
    {0,1,0,0,1,0,0}, // 5
    {0,1,0,0,0,0,0}, // 6
    {0,0,0,1,1,1,1}, // 7
    {0,0,0,0,0,0,0}, // 8
    {0,0,0,0,1,0,0}  // 9
  };

  digitalWrite(segA, digits[num][0]);
  digitalWrite(segB, digits[num][1]);
  digitalWrite(segC, digits[num][2]);
  digitalWrite(segD, digits[num][3]);
  digitalWrite(segE, digits[num][4]);
  digitalWrite(segF, digits[num][5]);
  digitalWrite(segG, digits[num][6]);
}

void setup() {
  Serial.begin(9600);

  pinMode(trigA, OUTPUT);
  pinMode(echoA, INPUT);
  pinMode(trigB, OUTPUT);
  pinMode(echoB, INPUT);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Servo
  gate.attach(servoPin);
  gate.write(0);

  // 7-Segment
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  displayNumber(0);
}

void loop() {

  // ================= SUJUD COUNTER =================
  long jarakA = readUltrasonic(trigA, echoA);

  if (jarakA < sujudThresh && !sedangSujud) {
    sedangSujud = true;
    sujudCount++;

    Serial.print("Sujud ke: ");
    Serial.println(sujudCount);

    if (sujudCount == 2) {
      rakaat++;
      if (rakaat > 9) rakaat = 0;

      sujudCount = 0;

      Serial.print("Rakaat bertambah: ");
      Serial.println(rakaat);

      displayNumber(rakaat);
    }

    delay(500);
  }

  if (jarakA > berdiriThresh && sedangSujud) {
    sedangSujud = false;
  }

  // ================= DETEKSI ORANG LEWAT =================
  long jarakB = readUltrasonic(trigB, echoB);

  if (jarakB > 0 && jarakB < alarmThresh) {
    bukaGateDenganAlarm();
  }

  delay(50);
}

void bukaGateDenganAlarm() {

  digitalWrite(buzzerPin, HIGH);
  gate.write(90);

  delay(2000);

  gate.write(0);
  digitalWrite(buzzerPin, LOW);

}



