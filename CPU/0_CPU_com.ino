const int ComPin = 3;
const int ClockPin = 2;

const float ClockSpeed = 30; // bits per second(bps)

const int ClockDelay = (int)(1000.0/ClockSpeed); //Amount of delay between pulsing the clock

bool ClockValue = LOW;

void com_init()
{
  pinMode(ClockPin, OUTPUT);
  pinMode(ComPin, OUTPUT);
  digitalWrite(ClockPin, LOW);

  Serial.print("clock speed: ");
  Serial.print(ClockSpeed);
  Serial.print(" bps");
}

void com_tickClock(){
  ClockValue = !ClockValue;
  digitalWrite(ClockPin, ClockValue);
  //Serial.println(ClockValue);
}


void com_sendBit(bool value)
{
  digitalWrite(ComPin, value);
  com_tickClock();
  Serial.println(value);
  delay(ClockDelay);
}
void com_sendByte(byte value, byte bitsize=8)
{
  //Serial.print("byte: ");
  for (int i=0; i < bitsize; i++)
  {
    bool data = (value >> i) & 0x01;
    //Serial.print(data);
    com_sendBit(data);
  }
  //Serial.println("");
}
