const int ComPin = 2;
const int ClockPin = 3;
const int ClockDelay = 10;

void com_init()
{
  pinMode(ClockPin, OUTPUT);
  pinMode(ComPin, OUTPUT);
  digitalWrite(ClockPin, LOW);

  //delay so you can more easily get them in sync
  delay(1000);
}


void com_sendBit(bool value)
{
  digitalWrite(ComPin, value);
  digitalWrite(ClockPin, HIGH);
  delay(ClockDelay);
  digitalWrite(ClockPin, LOW);
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
