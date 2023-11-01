const int clockInPin = 2;
const int ledPin = 13;

const int rclkPin = 5;   // 74HC595 - Shift register clock
const int srclkPin = 6;  // 74HC595 - Shift register clock
const int serPin = 7;    // 74HC595 - Serial data

int currentStep = 0;
int stepLength = 4;
uint8_t ledPatterns[] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000
};

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
  int readClock = gateRead(clockInPin);

  if (readClock != gateState) {
    gateState = readClock;
    if (gateState) {  // GATE is HIGH
      digitalWrite(ledPin, HIGH);

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
