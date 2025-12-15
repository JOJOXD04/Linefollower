#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial
#define Baudrate 115200

SerialCommand sCmd(SerialPort);
bool debug;
unsigned long previous, calculationTime;

struct param_t
{
  unsigned long cycleTime;
  /* andere parameters die in het eeprom geheugen moeten opgeslagen worden voeg je hier toe ... */
} params;

//TOEGEVOEGD: run-state en knop
const uint8_t BUTTON_PIN = 4;       // hardware drukknop
volatile bool running = false;      // run/stop status

// eenvoudige debounce
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // ms
int lastButtonReading = HIGH;       // omdat INPUT_PULLUP: HIGH = niet ingedrukt
int stableButtonState = HIGH;       // huidige stabiele toestand


void setup()
{
  SerialPort.begin(Baudrate);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.setDefaultHandler(onUnknownCommand);


  //TOEGEVOEGD: extra start/stop commands 
  sCmd.addCommand("start", onStart);  // via Bluetooth/Serial starten
  sCmd.addCommand("stop", onStop);    // via Bluetooth/Serial stoppen
  

  EEPROM_readAnything(0, params);

//TOEGEVOEGD: knop init
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // interne pull-up, knop naar GND
 

  pinMode(13, OUTPUT);
  SerialPort.println("ready");
}

void loop()
{
  sCmd.readSerial();

  //TOEGEVOEGD: hardware knop debounce en toggle
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();      // ingang veranderde: start debounce timer
  }
  lastButtonReading = reading;

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // stabiele toestand bereikt
    if (reading != stableButtonState) {
      stableButtonState = reading;
      // detecteer een knopdruk (falling edge: van HIGH naar LOW)
      if (stableButtonState == LOW) {
        running = !running;           // toggle run/stop
        digitalWrite(13, running ? HIGH : LOW);
        SerialPort.print("state: ");
        SerialPort.println(running ? "running" : "stopped");
      }
    }
  }
  /* ========================================================================================= */

  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;

    if (running) {
      /* code die cyclisch moet uitgevoerd worden programmeer je hier ... */
    }
  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "cycle") == 0) params.cycleTime = atol(value);
  /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  
  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  
  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

/* ===================== TOEGEVOEGD: start/stop handlers ===================== */
void onStart() {
  running = true;
  digitalWrite(13, HIGH);
  SerialPort.println("started");
}

void onStop() {
  running = false;
  digitalWrite(13, LOW);
  SerialPort.println("stopped");
}

