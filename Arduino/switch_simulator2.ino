
#define pinSW1  PB4
#define pinSW2  PB5

int splitString(String input, char delimiter, String parts[], int maxParts);

HardwareSerial Serial2(USART1);

int sw1 = 0;
int sw2 = 0;

String inputLine;
String commands[5];

void setup() {
  // start serial port at 9600 bps:
  Serial2.begin(115200);
  while (!Serial2) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(pinSW1, OUTPUT);
  pinMode(pinSW2, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(pinSW1, HIGH);
  digitalWrite(pinSW2, HIGH);

  digitalWrite(LED_BUILTIN, HIGH);

  Serial2.println("READY");
}

void loop() {

  int partCount, i;

  // DEBUG
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(100);
  
  // if we get a valid byte, read analog ins:
  if (Serial2.available() > 0) {
    // get incoming byte:
    inputLine = Serial2.readString();

    switch(inputLine[0])
    {
      case 'a':
        
        digitalWrite(pinSW1, LOW);
        delay(220);
        digitalWrite(pinSW2, LOW);
        delay(100);
        digitalWrite(pinSW1, HIGH);
        delay(281);
        digitalWrite(pinSW2, HIGH);

      break;

      case 'b':

        digitalWrite(pinSW1, LOW);
        delay(281);
        digitalWrite(pinSW2, LOW);
        delay(100);
        digitalWrite(pinSW2, HIGH);
        delay(220);
        digitalWrite(pinSW1, HIGH);

      break;
      
      case 'c':

        digitalWrite(pinSW2, LOW);
        delay(220);
        digitalWrite(pinSW1, LOW);
        delay(100);
        digitalWrite(pinSW1, HIGH);
        delay(281);
        digitalWrite(pinSW2, HIGH);

      break;

      case 'd':

        digitalWrite(pinSW2, LOW);
        delay(281);
        digitalWrite(pinSW1, LOW);
        delay(100);
        digitalWrite(pinSW2, HIGH);
        delay(220);
        digitalWrite(pinSW1, HIGH);

      break;

    }

    // DEBUG
    // Serial2.print("Partes: ");
    // Serial2.print(partCount);
    // Serial2.print('\n');


  }

  // DEBUG
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(100);

}

