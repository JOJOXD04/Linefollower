
const int BUTTON_PIN = 4;     // knop naar GND
bool ledState = false;        // status die we togglen
int lastReading = HIGH;       // INPUT_PULLUP -> rust = HIGH
unsigned long t0 = 0;
const unsigned long db = 20;  // debounce in ms

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);  // gebruik ingebouwde LED (pin 13)
}

void loop() {
  int r = digitalRead(BUTTON_PIN);

  // detecteer veranderend signaal en start debounce-timer
  if (r != lastReading) t0 = millis();

  // als het signaal langer dan db ms stabiel is, check op neergaande flank
  if ((millis() - t0) > db) {
    static int stablePrev = HIGH;      // vorige stabiele waarde
    if (r != stablePrev) {             // er is een stabiele verandering
      stablePrev = r;
      if (r == LOW) {                  // alleen op druk (HIGH -> LOW)
        ledState = !ledState;          // toggle
      }
    }
  }

  lastReading = r;
  digitalWrite(LED_BUILTIN, ledState);
}

