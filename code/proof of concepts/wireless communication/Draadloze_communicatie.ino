
#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3);  // D2=RX, D3=TX

const int LedBlauw = 12;
char ch = 0;

void setup() {
  pinMode(LedBlauw, OUTPUT);
  Serial.begin(9600);  // PC-monitor
  BT.begin(9600);      // HC-05 data-mode

  Serial.println("BT klaar @9600. Stuur 0/1/2 via Bluetooth app.");
  BT.println("Verbonden. Stuur 0/1/2.");
}

void loop() {
  // Ontvang van telefoon (BT)
  if (BT.available()) {
    ch = BT.read();
    BT.println(ch);          // echo naar telefoon
    Serial.print("BT-> ");   // log naar PC
    Serial.println(ch);

    if (ch == '0') {
      digitalWrite(LedBlauw, LOW);
      BT.println("LED UIT");
    } else if (ch == '1') {
      digitalWrite(LedBlauw, HIGH);
      BT.println("LED AAN");
    } else if (ch == '2') {
      BT.println("LED Knipperen");
      for (int i = 0; i < 3; i++) {
        digitalWrite(LedBlauw, HIGH); delay(500);
        digitalWrite(LedBlauw, LOW);  delay(500);
      }
    } else {
      BT.println("Onbekend: stuur 0/1/2");
    }
  }
}