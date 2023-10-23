
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

  digitalWrite(pinSW1, LOW);
  digitalWrite(pinSW2, LOW);

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
 
    partCount = splitString(inputLine, ' ', commands, 5);

    for(i = 0;i < partCount;i++)
    {
      switch(commands[i][0])
      {
        case 'a':
          
          if(commands[i][1] == '1')
            sw1 = 1;
          else
          {
            if(commands[i][1] == '0')
              sw1 = 0;
          };

        break;

        case 'b':

          if(commands[i][1] == '1')
            sw2 = 1;
          else
          {
            if(commands[i][1] == '0')
              sw2 = 0;
          };

        break;
        
      }

    }

    // DEBUG
    // Serial2.print("Partes: ");
    // Serial2.print(partCount);
    // Serial2.print('\n');

      if(sw1)
        digitalWrite(pinSW1, HIGH);
      else
        digitalWrite(pinSW1, LOW);

      if(sw2)
        digitalWrite(pinSW2, HIGH);
      else
        digitalWrite(pinSW2, LOW);

      Serial2.print("SW1: ");
      if(sw1)
        Serial2.print("ON ");
      else
        Serial2.print("OFF");;

      Serial2.print("\tSW2: ");
      if(sw2)
        Serial2.print("ON ");
      else
        Serial2.print("OFF");;

      Serial2.print('\n');

  }

  // DEBUG
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(100);

}

int splitString(String input, char delimiter, String parts[], int maxParts)
{
  int partCount = 0;
  int lastIndex = 0;
  
  for (int i = 0; i < input.length(); i++) {
    if (input.charAt(i) == delimiter || i == input.length() - 1) {
      // Si se encuentra un delimitador o llegamos al final de la cadena, se agrega una parte
      if (i == input.length() - 1) {
        parts[partCount] = input.substring(lastIndex, i + 1);
      } else {
        parts[partCount] = input.substring(lastIndex, i);
      }
      partCount++;
      lastIndex = i + 1;
    }
  }

  return partCount;
}
