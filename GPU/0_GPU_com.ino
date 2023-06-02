//#define LOG_CLOCK
//#define LOG_CLOCK_DATA
#define LOG_WAITING_ON_BIT
const int ComPin = A5;
const int ClockPin = 2;

bool ClockValue = LOW;
int BufferIndex = 0;
byte CommandId = 0;

unsigned long last_bit_time = millis();

void com_init() {
  pinMode(ClockPin, INPUT);
  pinMode(ComPin, INPUT);

  //attachInterrupt(digitalPinToInterrupt(ClockPin), onClock, CHANGE);
}

void com_update(bool* buff, int max_size) {
  bool clock = digitalRead(ClockPin);
  if (clock != ClockValue) {
    ClockValue = clock;
    
    unsigned long time = millis();
    if((time - last_bit_time) > 600 && Index != 0){
      Index = 0;
      CommandId = 0;
      Serial.println("OUT OF SYNC");
    }
    last_bit_time = time;

    bool bit = digitalRead(ComPin);
    buff[Index] = bit;
    Index++;
    if (Index >= max_size) {
      Serial.println("TOO MUCH DAT IN COMMAND BUFFER (NU HEB JE EEN GROOT PROBLEEM WANT DIT MAG NOOIT GEBEUREN)");
      Index = 0;
      CommandId = 0;
      return false;
    }

    if (CommandId == 0 && Index >= 4)
    {
      CommandId = com_parseByte(buff, 0, 4);
      
    }
    

    
  }
  return false;
}

byte com_parseByte(bool* buff, int start, int len)
{
  byte output = 0;
  for (int i = 0; i < len; i++) {
    bool data = buff[start+i];
    //Serial.print(data);

    output |= (data << i);
  }
  //Serial.println("");
  return output;
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
