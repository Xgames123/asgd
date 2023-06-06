//#define LOG_CLOCK
//#define LOG_CLOCKSPEED
#define LOG_CLOCK_DATA
const int ComPin = 3;
const int ClockPin = 2;

const float ClockSpeed = 20;  // bits per second(bps)

const int ClockDelay = (int)(1000.0 / ClockSpeed);  //Amount of delay between pulsing the clock

bool ClockValue = LOW;

void com_init() {
  pinMode(ClockPin, OUTPUT);
  pinMode(ComPin, OUTPUT);
  digitalWrite(ClockPin, LOW);

#ifdef LOG_CLOCKSPEED
  Serial.print("clock speed: ");
  Serial.print(ClockSpeed);
  Serial.println(" bps");
  #endif
}

void com_tickClock() {
  ClockValue = !ClockValue;
  digitalWrite(ClockPin, ClockValue);

#ifdef LOG_CLOCK
  Serial.println(ClockValue);
#endif
}


void com_sendBit(bool value) {
  digitalWrite(ComPin, value);
  com_tickClock();
  delay(ClockDelay);
  #ifdef LOG_CLOCK_DATA
  Serial.println(value);
  #endif
  
}

void com_sendCmd(byte cmd)
{
  com_sendByte(cmd, 4);
}


void com_sendByte(byte value, byte bitsize) {
  //Serial.print("byte: ");
  for (int i = 0; i < bitsize; i++) {
    bool data = (value >> i) & 0x01;
    //Serial.print(data);
    com_sendBit(data);
  }
  //Serial.println("");
}
