const int ComPin = 2;
const int ClockPin = A5;
const int ClockDelay = 10;

void com_init()
{
  pinMode(ClockPin, INPUT);
  pinMode(ComPin, INPUT);
}



byte com_readByte(int bitCount=8)
{
  //Serial.print("data: ");
  byte output = 0;
  for(int i=0; i < bitCount; i++)
  {
    bool data = com_readBit();
    //Serial.print(data);
    
    output |= (data << i);
  }
  //Serial.println("");
  return output;
}



bool com_readBit()
{ 
 com_waitForClockVal(HIGH);
 bool value = digitalRead(ComPin);
 com_waitForClockVal(LOW);
 return value;
}


void com_waitForClockVal(bool value)
{
  while(true)
  {
    //Serial.println(value);
   if(digitalRead(ClockPin) == value)
   {
    delay(1);
    break;
   }
    
  }
}
