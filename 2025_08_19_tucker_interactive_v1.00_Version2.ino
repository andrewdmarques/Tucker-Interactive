// ESP32 Arduino sketch
// Pattern: HIGH 0.3s -> LOW 0.2s -> HIGH 15min -> repeat

// Choose a pin that doesn't interfere with boot:
const int PIN_OUT = 23;  // safe at boot on ESP32

// Durations in milliseconds
const uint32_t T_HIGH_SHORT = 300;        // 0.3 s
const uint32_t T_LOW_SHORT  = 200;        // 0.2 s
const uint32_t T_HIGH_LONG  = 15UL * 60UL * 1000UL;  // 15 minutes = 900,000 ms

enum Phase : uint8_t { PH_HIGH_SHORT, PH_LOW_SHORT, PH_HIGH_LONG };
Phase phase = PH_HIGH_SHORT;

uint32_t phaseStartMs = 0;

void enterPhase(Phase p) {
  phase = p;
  phaseStartMs = millis();

  switch (phase) {
    case PH_HIGH_SHORT:
      digitalWrite(PIN_OUT, HIGH);
      break;
    case PH_LOW_SHORT:
      digitalWrite(PIN_OUT, LOW);
      break;
    case PH_HIGH_LONG:
      digitalWrite(PIN_OUT, HIGH);
      break;
  }
}

void setup() {
  pinMode(PIN_OUT, OUTPUT);
  digitalWrite(PIN_OUT, LOW);  // defined state at boot
  enterPhase(PH_HIGH_SHORT);
}

void loop() {
  uint32_t now = millis();

  switch (phase) {
    case PH_HIGH_SHORT:
      if (now - phaseStartMs >= T_HIGH_SHORT) {
        enterPhase(PH_LOW_SHORT);
      }
      break;

    case PH_LOW_SHORT:
      if (now - phaseStartMs >= T_LOW_SHORT) {
        enterPhase(PH_HIGH_LONG);
      }
      break;

    case PH_HIGH_LONG:
      if (now - phaseStartMs >= T_HIGH_LONG) {
        // Cycle complete; restart
        enterPhase(PH_HIGH_SHORT);
      }
      break;
  }

  // Do other work here without blocking...
}