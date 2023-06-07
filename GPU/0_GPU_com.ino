// #define LOG_CLOCK
// #define LOG_CLOCK_DATA
#define LOG_WAITING_ON_BIT
const int ComPin = A5;
const int ClockPin = 2;

bool ClockValue = LOW;
int BufferIndex = 0;
byte CommandId = 0;
int CommandSize = 0;

unsigned long last_bit_time = millis();

void com_init() {
  pinMode(ClockPin, INPUT);
  pinMode(ComPin, INPUT);

  // attachInterrupt(digitalPinToInterrupt(ClockPin), onClock, CHANGE);
}

bool com_update(bool *buff, int max_size, int *cmdSizeDict) {
  bool clock = digitalRead(ClockPin);
  if (clock != ClockValue) {
    ClockValue = clock;

    unsigned long time = millis();
    if ((time - last_bit_time) > 600 && BufferIndex != 0) {
      BufferIndex = 0;
      CommandId = 0;
      Serial.println("OUT OF SYNC");
    }
    last_bit_time = time;

    bool bit = digitalRead(ComPin);
    buff[BufferIndex] = bit;
    BufferIndex++;
    if (BufferIndex >= max_size) {
      Serial.println("TOO MUCH DAT IN COMMAND BUFFER (NU HEB JE EEN GROOT "
                     "PROBLEEM WANT DIT MAG NOOIT GEBEUREN)");
      BufferIndex = 0;
      CommandId = 0;
      return false;
    }

    if (CommandId == 0 && BufferIndex >= 4) {
      CommandId = com_parseByte(buff, 0, 4);
      CommandSize = cmdSizeDict[CommandId] + 4;
      return false;
    }

    if (CommandId != 0 && BufferIndex >= CommandSize) {
      CommandId = 0;
      BufferIndex = 0;
      return true;
    }
  }
  return false;
}

byte com_parseByte(bool *buff, int start, int len) {
  byte output = 0;
  for (int i = 0; i < len; i++) {
    bool data = buff[start + i];
    // Serial.print(data);

    output |= (data << i);
  }
  // Serial.println("");
  return output;
}
