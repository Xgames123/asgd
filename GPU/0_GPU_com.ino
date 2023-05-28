const int ComPin = A5;
const int ClockPin = 2;

bool Buffer[100];
int Index=0;

unsigned long lastClockPulse;

void com_init()
{
  pinMode(ClockPin, INPUT);
  pinMode(ComPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(ClockPin), onClock, RISING);
  attachInterrupt(digitalPinToInterrupt(ClockPin), onClock, FALLING);
}


void onClock(){
 
  unsigned long time = micros();
  Serial.print("clock: ");
  Serial.println(time-lastClockPulse);
  lastClockPulse = time;

  bool bit = digitalRead(ComPin);
  Buffer[Index] = bit;
  Index++;
  if(Index >= 100){
    Serial.println("RESIVE BUFFER OVERFLOW");
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


bool com_hasBits(int count=1){
  if (Index >= count){
    return true;
  }
  return false;
}


bool com_readBit()
{ 
  if(!com_hasBits())
  {
     Serial.println("NOTE: waiting on bit");
     while(!com_hasBits()){
       
     }
  }

  Index--;
  return Buffer[Index];
}
