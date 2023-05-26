

int i = 0;
void setup() {

  draw_init();

  Serial.begin(9600);

  
  gpu_init();

  //draw_writePixel(0, 0, HIGH);
}

void loop() {

  gpu_read_command();
  
  //draw_writeBuff();
 for (int y=0; y < displayH; y++)
 {
  for (int x=0; x < displayW; x++)
  {
    bool pixel = displayBuff[y*displayW+x];
    if(pixel){
 Serial.print('x');
    }else{
      Serial.print('.');
    }
   

   
  }
  Serial.println();
 }
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
