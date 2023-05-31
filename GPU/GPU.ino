//#define LOG_BUFSIZE

void setup() {

  draw_init();

  Serial.begin(9600);

  
  gpu_init();

  //draw_writePixel(0, 0, HIGH);
}

int lastLoggedBufferSize = -1;

unsigned long last_gpu_read_time = millis();

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

  unsigned long time = millis();
  if (gpu_buffer_filled(20)){
    last_gpu_read_time = time;
    if (!gpu_buffer_filled(3)){
        return;
    }

    Serial.print("started executing commands. buffer size: ");
    Serial.println(gpu_buffer_size());
    gpu_read_all_commands();
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
