#include "Wire.h"
#include "MCP4725.h"

const int clockInPin = 2;
const int ledPin = 13;

const int rclkPin = 5;   // 74HC595 - Storage register clock
const int srclkPin = 6;  // 74HC595 - Shift register clock
const int serPin = 7;    // 74HC595 - Serial data

int currentStep = 0;
int stepLength = 4;  // = 5 steps
uint8_t ledPatterns[] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000
};

bool gateState = LOW;
bool lastGateState = LOW;

MCP4725 MCP(0x61);
const int notes[] = {
  0, 2, 4, 5, 7, 9, 11,
  12, 14, 16, 17, 19, 21, 23,
  24, 26, 28, 29, 31, 33, 35,
  36, 38, 40, 41, 43, 45, 47,
  48, 50, 52, 53, 55, 57, 59, 
  60
};
const int pitchPins[] = { A0, A1, A2, A3, A6 };

void setup() {
  Serial.begin(115200);

  Wire.begin();
  MCP.begin();

  pinMode(clockInPin, INPUT);
  pinMode(ledPin, OUTPUT);

  pinMode(rclkPin, OUTPUT);
  pinMode(srclkPin, OUTPUT);
  pinMode(serPin, OUTPUT);
}

void loop() {
  int readClock = gateRead(clockInPin);
  if (readClock != gateState) {
    gateState = readClock;

    // Read analog pots and set cv voltage.
    int analogValue = analogRead(pitchPins[currentStep]);
    int note = notes[map(analogValue, 5, 1018, 0, 35)];
    
    if (gateState) {  // GATE is HIGH
      digitalWrite(ledPin, HIGH);

      Serial.println(note);
      MCP.setVoltage(0.0833 * note); 

      digitalWrite(rclkPin, LOW);
      shiftOut(serPin, srclkPin, MSBFIRST, ledPatterns[currentStep]);
      digitalWrite(rclkPin, HIGH);
      
      currentStep++;
      if (currentStep > stepLength) {
        currentStep = 0;
      }
    } else {  // GATE is LOW
      digitalWrite(ledPin, LOW);

      digitalWrite(rclkPin, LOW);
      shiftOut(serPin, srclkPin, MSBFIRST, B00000000);
      digitalWrite(rclkPin, HIGH);
    }
  }
  lastGateState = readClock;
}

int gateRead(byte pin) {
  if (!pin) return;
  return !digitalRead(pin);
}
