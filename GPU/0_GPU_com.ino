// #define LOG_CLOCK
// #define LOG_CLOCK_DATA
// #define LOG_COMMANDID
const int ComPin = A5;
const int ClockPin = 2;

bool ClockValue = LOW;
int BufferIndex = 0;
byte CommandId = 0;
int CommandSize = 0;

unsigned long last_bit_time = -1;

unsigned long last_clock_time = -1;

int max_bps = 99999999;

void com_init() {
  pinMode(ClockPin, INPUT);
  pinMode(ComPin, INPUT);

  // attachInterrupt(digitalPinToInterrupt(ClockPin), onClock, CHANGE);
}

void com_reset() {
  CommandId = 0;
  BufferIndex = 0;
}

bool com_update(bool *buff, int max_size, int *cmdSizeDict) {
  unsigned long ctime = millis();

  int bps = (1000.0 / (ctime - last_clock_time));
  last_clock_time = ctime;
  if (max_bps > bps) {
    max_bps = bps;
    Serial.print("Max clock speed: ");
    Serial.println(max_pps);
  }

  bool clock = digitalRead(ClockPin);
  if (clock != ClockValue) {
    ClockValue = clock;

    unsigned long time = millis();
    if (last_bit_time != -1 && (time - last_bit_time) > 600 &&
        BufferIndex != 0) {
      BufferIndex = 0;
      CommandId = 0;
      Serial.println("OUT OF SYNC");
    }
    last_bit_time = time;

    bool bit = digitalRead(ComPin);
#ifdef LOG_CLOCK_DATA
    Serial.println(bit);
#endif

    buff[BufferIndex] = bit;
    BufferIndex++;
    if (BufferIndex >= max_size) {
      Serial.println("TOO MUCH DAT IN COMMAND BUFFER (NU HEB JE EEN GROOT "
                     "PROBLEEM WANT DIT MAG NOOIT GEBEUREN)");
      com_reset();
      return false;
    }

    if (CommandId == 0 && BufferIndex >= 4) {
      CommandId = com_parseByte(buff, 0, 4);
      CommandSize = cmdSizeDict[CommandId] + 4;
#ifdef LOG_COMMANDID
      Serial.print("cmd size: ");
      Serial.print(CommandSize);
      Serial.print(" cmd id: ");
      Serial.println(CommandId);
#endif
    }

    if (CommandId != 0 && BufferIndex >= CommandSize) {
      com_reset();
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
