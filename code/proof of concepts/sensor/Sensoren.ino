// HY-301 sensorarray testcode
const int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};

void setup() {
  Serial.begin(9600); // Start seriële communicatie
}

void loop() {
  Serial.print("Sensorwaarden: ");
  for (int i = 0; i < 8; i++) {
    int value = analogRead(sensorPins[i]); // Lees sensorwaarde
    Serial.print(value);
    if (i < 7) Serial.print(", "); // Scheidingsteken
  }
  Serial.println(); // Nieuwe regel
  delay(200); // Kleine pauze om de monitor leesbaar te houden
}
