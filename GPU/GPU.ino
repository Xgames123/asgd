#define LOG_BUFSIZE

int i = 0;
void setup() {

  draw_init();

  Serial.begin(9600);

  
  gpu_init();

  //draw_writePixel(0, 0, HIGH);
}

void loop() {

  draw_writeBuff();

  #ifdef LOG_BUFSIZE
  Serial.print("BufferSize: ");
  Serial.println(gpu_buffer_size());
  #endif
  if (gpu_buffer_filled(10)){
    Serial.print("started executing commands. buffer size: ");
    Serial.println(gpu_buffer_size());
    gpu_read_all_commands();
  }

  
  //draw_BuffToSerial();
  delay(10);
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
