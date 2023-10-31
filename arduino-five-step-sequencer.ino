const byte clockInPin = 2;
const byte ledPin = 13;

bool gateState = LOW;
bool lastGateState = LOW;

void setup() {
  pinMode(clockInPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  byte readClock = gateRead(clockInPin);

  if (readClock != gateState) {
    gateState = readClock;
    if (gateState) {  // GATE is HIGH
      digitalWrite(ledPin, HIGH);
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
