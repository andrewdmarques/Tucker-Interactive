// ESP32 Arduino sketch
// Pattern: HIGH 0.3s -> LOW 0.2s -> HIGH 15min -> repeat
// Randomly choose between two safe-at-boot pins each cycle.

#include <esp_system.h>  // for esp_random()

// Two output pins that are safe at boot (not strapping, not flash, not input-only)
const int PIN_A = 23;
const int PIN_B = 19;

// Durations in milliseconds
const uint32_t T_HIGH_SHORT = 300;                       // 0.3 s
const uint32_t T_LOW_SHORT  = 200;                       // 0.2 s
const uint32_t T_HIGH_LONG  = 4UL * 60UL * 1000UL;      // 15 minutes

enum Phase : uint8_t { PH_HIGH_SHORT, PH_LOW_SHORT, PH_HIGH_LONG };
Phase phase = PH_HIGH_SHORT;

uint32_t phaseStartMs = 0;
int activePin = PIN_A;    // will be randomized per cycle

void setBothLow() {
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
}

void pickRandomPinForCycle() {
  // Randomly choose PIN_A or PIN_B
  activePin = (random(0, 2) == 0) ? PIN_A : PIN_B;
  // Ensure the non-active stays LOW
  digitalWrite((activePin == PIN_A) ? PIN_B : PIN_A, LOW);
}

void enterPhase(Phase p) {
  phase = p;
  phaseStartMs = millis();

  switch (phase) {
    case PH_HIGH_SHORT:
      digitalWrite(activePin, HIGH);
      break;
    case PH_LOW_SHORT:
      digitalWrite(activePin, LOW);
      break;
    case PH_HIGH_LONG:
      digitalWrite(activePin, HIGH);
      break;
  }
}

void setup() {
  // Seed RNG with hardware RNG
  randomSeed(esp_random());

  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  setBothLow();  // defined state at boot

  pickRandomPinForCycle();
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
        // End of the full pattern on this pin:
        // hold both LOW briefly, pick a new pin, and restart
        setBothLow();
        pickRandomPinForCycle();
        enterPhase(PH_HIGH_SHORT);
      }
      break;
  }

  // Do other work here without blocking...
}

