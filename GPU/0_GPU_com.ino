const int ComPin = A5;
const int ClockPin = 2;

bool Buffer[500];
int Index=0;

bool ClockValue = LOW;

void com_init()
{
  pinMode(ClockPin, INPUT);
  pinMode(ComPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(ClockPin), onClock, CHANGE);
}

void onClock(){
 
  Serial.println(ClockValue);
  ClockValue = !ClockValue;

  bool bit = digitalRead(ComPin);
  Buffer[Index] = bit;
  Index++;
  if(Index >= sizeof(Buffer)/sizeof(bool)){
    Serial.println("TOO MUCH DAT IN DATABUFFER (OVERFLOW)");
    Index = 0;
  }
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


bool com_hasBits(int count){
  if (Index >= count){
    return true;
  }
  return false;
}


bool com_readBit()
{ 
  if(!com_hasBits(1))
  {
     Serial.println("NOTE: waiting on bit");
     while(!com_hasBits(1)){
       
     }
  }

  Index--;
  return Buffer[Index];
}
