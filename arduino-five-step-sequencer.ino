const int clockInPin = 2;
const int ledPin = 13;

const int rclkPin = 5;   // 74HC595 - Shift register clock
const int srclkPin = 6;  // 74HC595 - Shift register clock
const int serPin = 7;    // 74HC595 - Serial data

int currentStep = 0;
int stepLength = 4;

bool gateState = LOW;
bool lastGateState = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(clockInPin, INPUT);
  pinMode(ledPin, OUTPUT);

  pinMode(rclkPin, OUTPUT);
  pinMode(srclkPin, OUTPUT);
  pinMode(serPin, OUTPUT);
}

void loop() {
  byte readClock = gateRead(clockInPin);

  Serial.println(1 << currentStep, BIN);
  digitalWrite(rclkPin, LOW);
  shiftOut(serPin, srclkPin, LSBFIRST, 1 >> currentStep);
  digitalWrite(rclkPin, HIGH);
  currentStep++;
  if (currentStep > stepLength) {
    currentStep = 0;
  }

  delay(100);

  if (readClock != gateState) {
    gateState = readClock;
    if (gateState) {  // GATE is HIGH
      digitalWrite(ledPin, HIGH);
      digitalWrite(rclkPin, LOW);
      shiftOut(serPin, srclkPin, MSBFIRST, 1 >> currentStep);
      digitalWrite(rclkPin, HIGH);
      currentStep++;
      if (currentStep > stepLength) {
        currentStep = 0;
      }
    } else {  // GATE is LOW
      digitalWrite(ledPin, LOW);
    }
  }
  lastGateState = readClock;
}

byte gateRead(byte pin) {
  if (!pin) return;
  return !digitalRead(pin);
}
