//#define LOG_BUFSIZE

int i = 0;
void setup() {

  draw_init();

  Serial.begin(9600);

  
  gpu_init();

  //draw_writePixel(0, 0, HIGH);
}

int lastLoggedBufferSize = -1;

void loop() {

  draw_writeBuff();


  com_update();

  #ifdef LOG_BUFSIZE
  int buffSize = gpu_buffer_size();
  if(lastLoggedBufferSize != buffSize){
    Serial.print("BufferSize: ");
    Serial.println(buffSize);
    lastLoggedBufferSize = buffSize;
  }
  #endif

  if (gpu_buffer_filled(10)){
    Serial.print("started executing commands. buffer size: ");
    Serial.println(gpu_buffer_size());
    gpu_read_all_commands();
  }

  
  //draw_BuffToSerial();
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
