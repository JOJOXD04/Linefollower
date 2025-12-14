#include "SerialCommand.h"
#include "EEPROMAnything.h"
#include <SoftwareSerial.h>

// HC-05 op D2 (RX van Arduino) en D3 (TX van Arduino)
SoftwareSerial BT(2, 3); // RX, TX

#define SerialPort BT
#define Baudrate 9600

// Motor pinnen
#define MotorLeftForward 5
#define MotorLeftBackward 6
#define MotorRightForward 9
#define MotorRightBackward 10

// IR LED en enable pin
const int IR = 12;
const int EEP = 7;

// SerialCommand object
SerialCommand sCmd(SerialPort);

// PID parameters en variabelen
struct param_t {
  unsigned long cycleTime;
  int black[8];
  int white[8];
  float diff;
  float kp;
  int power;
  float ki;
  float kd;
} params;

const int sensor[] = {A0, A1, A2, A3, A4, A5, A6, A7};// omgedraaid 
int normalised[8];
float debugposition;
float output;
float error;
float iTerm = 0;
float lastErr;

bool run = false;
unsigned long previous, calculationTime;

// Power ramp variabele
int powerCmd = 0;

void setup() {
  pinMode(MotorLeftForward, OUTPUT);
  pinMode(MotorLeftBackward, OUTPUT);
  pinMode(MotorRightForward, OUTPUT);
  pinMode(MotorRightBackward, OUTPUT);

  pinMode(EEP, OUTPUT);
  digitalWrite(EEP, HIGH); // enable H-brug

  pinMode(IR, OUTPUT);
  digitalWrite(IR, HIGH); // IR LED aan

  SerialPort.begin(Baudrate);

  // Commando's registreren
  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("run", onRun);
  sCmd.addCommand("stop", onStop);
  sCmd.addCommand("toggle", onToggle);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);
  SerialPort.println("ready");
}

void loop() {
  sCmd.readSerial();

  unsigned long current = micros();
  if (current - previous >= params.cycleTime) {
    previous = current;

    // Normaliseer sensoren
    for (int i = 0; i < 8; i++) {
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);
    }

    // Positie bepalen
    int index = 0;
    for (int i = 1; i < 8; i++) if (normalised[i] < normalised[index]) index = i;

    if (normalised[index] > 950) run = false; // lijnverlies-drempel

    float position;
    if (index == 0) position = -30;
    else if (index == 7) position = 30;
    else {
      int sNul = normalised[index];
      int sMinEen = normalised[index-1];
      int sPlusEen = normalised[index+1];

      float b = (sPlusEen - sMinEen) / 2.0;
      float a = sPlusEen - b - sNul;

      position = -b / (2 * a);
      position += index;
      position -= 3.5;
      position *= 9.525; // afstand sensoren
    }
    debugposition = position;

    // PID-regelaar
    error = -position;
    output = error * params.kp;

    iTerm += params.ki * error;
    iTerm = constrain(iTerm, -510, 510);
    output += iTerm;

    output += params.kd * (error - lastErr);
    lastErr = error;

    output = constrain(output, -510, 510);

    // Power ramp
    int target = run ? params.power : 0;
    int step = 3; // max verandering per cyclus
    if (powerCmd < target) powerCmd = min(powerCmd + step, target);
    else if (powerCmd > target) powerCmd = max(powerCmd - step, target);

    // Turn-cap
    int turn = (int)(params.diff * output);
    turn = constrain(turn, -120, 120);

    // Nieuwe motor-mix
    int powerLeft  = run ? constrain(powerCmd + turn, -255, 255) : 0;
    int powerRight = run ? constrain(powerCmd - turn, -255, 255) : 0;

    // Motoren aansturen
    analogWrite(MotorLeftForward,  powerLeft  > 0 ? powerLeft  : 0);
    analogWrite(MotorLeftBackward, powerLeft  < 0 ? -powerLeft : 0);
    analogWrite(MotorRightForward, powerRight > 0 ? powerRight : 0);
    analogWrite(MotorRightBackward,powerRight < 0 ? -powerRight: 0);
  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

// Onbekend commando
void onUnknownCommand(char *command) {
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

// SET-commando
void onSet() {
  char* param = sCmd.next();
  char* value = sCmd.next();  

  if (!param || !value) {
    SerialPort.println("set: missing param or value");
    return;
  }

  if (strcmp(param, "cycle") == 0) {
    long newCycleTime = atol(value);
    if (newCycleTime <= 0) { SerialPort.println("cycle must be > 0"); return; }
    float ratio = ((float)newCycleTime) / ((float)params.cycleTime == 0 ? 1.0f : (float)params.cycleTime);
    params.ki *= ratio;
    params.kd /= ratio;
    params.cycleTime = newCycleTime;
  }
  else if (strcmp(param, "ki") == 0) {
    float cycleTimeInSec = ((float)params.cycleTime) / 1000000.0f;
    params.ki = atof(value) * (cycleTimeInSec > 0 ? cycleTimeInSec : 1.0f);
  }
  else if (strcmp(param, "kd") == 0) {
    float cycleTimeInSec = ((float)params.cycleTime) / 1000000.0f;
    params.kd = atof(value) / (cycleTimeInSec > 0 ? cycleTimeInSec : 1.0f);
  }
  else if (strcmp(param, "power") == 0) params.power = atol(value);
  else if (strcmp(param, "diff") == 0) params.diff = atof(value);
  else if (strcmp(param, "kp") == 0) params.kp = atof(value);
  else {
    SerialPort.println("unknown set param");
    return;
  }

  EEPROM_writeAnything(0, params);
  SerialPort.println("ready 1");
}

// DEBUG-commando
void onDebug() {
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);

  SerialPort.print("black: ");
  for (int i = 0; i < 8; i++) { SerialPort.print(params.black[i]); SerialPort.print(" "); }
  SerialPort.println();

  SerialPort.print("white: ");
  for (int i = 0; i < 8; i++) { SerialPort.print(params.white[i]); SerialPort.print(" "); }
  SerialPort.println();

  SerialPort.print("Normalised: ");
  for (int i = 0; i < 8; i++) { SerialPort.print(normalised[i]); SerialPort.print(" "); }
  SerialPort.println();

  SerialPort.print("position: "); SerialPort.println(debugposition);
  SerialPort.print("Power: "); SerialPort.println(params.power);
  SerialPort.print("diff: "); SerialPort.println(params.diff);
  SerialPort.print("kp: "); SerialPort.println(params.kp);

  float cycleTimeInSec = ((float)params.cycleTime) / 1000000.0f;
  float ki = cycleTimeInSec > 0 ? (params.ki / cycleTimeInSec) : 0;
  SerialPort.print("Ki: "); SerialPort.println(ki);

  float kd = cycleTimeInSec > 0 ? (params.kd * cycleTimeInSec) : 0;
  SerialPort.print("Kd: "); SerialPort.println(kd);

  SerialPort.print("RUN: "); SerialPort.println(run ? "ON" : "OFF");

  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

// CALIBRATE-commando
void onCalibrate() {
  char* param = sCmd.next();
  if (!param) {
    SerialPort.println("calibrate: missing 'black' or 'white'");
    return;
  }

  if (strcmp(param, "black") == 0) {
    SerialPort.print("calibrating black...");
    for (int i = 0; i < 8; i++) params.black[i] = analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0) {
    SerialPort.print("calibrating white...");
    for (int i = 0; i < 8; i++) params.white[i] = analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else {
    SerialPort.println("calibrate: unknown arg (use 'black' or 'white')");
    return;
  }

  EEPROM_writeAnything(0, params);
  SerialPort.println("ready 2");
}

// RUN-commando
void onRun() {
  run = true;
  iTerm = 0; // integrator reset bij start
  SerialPort.println("RUN ON");
}

// STOP-commando
void onStop() {
  run = false;
  SerialPort.println("RUN OFF");
}

// TOGGLE-commando
void onToggle() {
  run = !run;
  iTerm = 0;
  SerialPort.print("Run state: ");
  SerialPort.println(run ? "ON" : "OFF");
}
