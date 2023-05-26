

int i = 0;
void setup() {

  draw_init();

  Serial.begin(9600);

  
  gpu_init();

  //draw_writePixel(0, 0, HIGH);
}

void loop() {
  //TestLeds();

  draw_clear(LOW);

  draw_point(3, 3, HIGH);
  draw_point(4, 4, HIGH);
  
  //Serial.println("tick");
  //SetPixel(0, 0, HIGH);
  //Serial.println("tick2");

  //Serial.println(com_readBit());
  
  //Serial.print("data: ");
  //Serial.println(com_readByte());

  draw_writeBuff();
}


void TestLeds()
{
  for (int x=0; x < 8; x++)
  {
    for (int y=0; y < 8; y++)
    {
      draw_writePixel(x, y, HIGH);
      delay(500);
      draw_writePixel(x, y, LOW);
    } 
  }
  
}
