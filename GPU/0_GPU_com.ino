//#define LOG_CLOCK
#define LOG_CLOCK_DATA
#define LOG_WAITING_ON_BIT
const int ComPin = A5;
const int ClockPin = 2;

bool Buffer[500];
int Index = 0;

bool ClockValue = LOW;

void com_init() {
  pinMode(ClockPin, INPUT);
  pinMode(ComPin, INPUT);

  //attachInterrupt(digitalPinToInterrupt(ClockPin), onClock, CHANGE);
}

void com_update() {
  bool clock = digitalRead(ClockPin);
  if (clock != ClockValue) {
    ClockValue = clock;
    onClock(clock);
  }
}


void onClock(bool clock) {
#ifdef LOG_CLOCK
  Serial.println(ClockValue);
#endif

  bool bit = digitalRead(ComPin);
  Buffer[Index] = bit;
  Index++;
  if (Index >= sizeof(Buffer) / sizeof(bool)) {
    Serial.println("TOO MUCH DAT IN DATBUFFER (OVERFLOW)");
    Index = 0;
  }

#ifdef LOG_CLOCK_DATA
  Serial.println(bit);
#endif
}


byte com_readByte(int bitCount = 8) {
  //Serial.print("data: ");
  byte output = 0;
  for (int i = 0; i < bitCount; i++) {
    bool data = com_readBit();
    //Serial.print(data);

    output |= (data << i);
  }
  //Serial.println("");
  return output;
}


bool com_hasBits(int count) {
  if (Index >= count) {
    return true;
  }
  return false;
}


bool com_readBit() {
  if (!com_hasBits(1)) {
#ifdef LOG_WAITING_ON_BIT
    Serial.println("STARTED WAITING ON BIT");
#endif
    while (!com_hasBits(1)) {
      com_update();
    }
#ifdef LOG_WAITING_ON_BIT
    Serial.println("ENDED WAITING ON BIT");
#endif
  }

  Index--;
  bool bit = Buffer[Index];

  return bit;
}
