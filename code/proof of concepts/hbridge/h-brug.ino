// Motor pinnen
#define MotorLeftForward 5
#define MotorLeftBackward 6
#define MotorRightForward 9
#define MotorRightBackward 10

#define EEP 7 // Enable pin voor H-brug

void setup() {
  pinMode(MotorLeftForward, OUTPUT);
  pinMode(MotorLeftBackward, OUTPUT);
  pinMode(MotorRightForward, OUTPUT);
  pinMode(MotorRightBackward, OUTPUT);
  pinMode(EEP, OUTPUT);

  digitalWrite(EEP, HIGH); // H-brug inschakelen
}

void loop() {
  // --- Motor Links vooruit, traploos ---
  analogWrite(MotorLeftForward, 80);  // langzaam
  analogWrite(MotorLeftBackward, 0);
  delay(2000);

  analogWrite(MotorLeftForward, 150); // sneller
  delay(2000);

  analogWrite(MotorLeftForward, 255); // max
  delay(2000);

  // Stop
  analogWrite(MotorLeftForward, 0);
  delay(1000);

  // --- Motor Links achteruit ---
  analogWrite(MotorLeftForward, 0);
  analogWrite(MotorLeftBackward, 150);
  delay(2000);
  analogWrite(MotorLeftBackward, 0);
  delay(1000);

  // --- Motor Rechts vooruit traploos ---
  analogWrite(MotorRightForward, 80);  // langzaam
  analogWrite(MotorRightBackward, 0);
  delay(2000);

  analogWrite(MotorRightForward, 150); // sneller
  delay(2000);

  analogWrite(MotorRightForward, 255); // max
  delay(2000);

  // Stop
  analogWrite(MotorRightForward, 0);
  delay(1000);

  // --- Motor Rechts achteruit ---
  analogWrite(MotorRightBackward, 150);
  delay(2000);
  analogWrite(MotorRightBackward, 0);
  delay(2000);
}

