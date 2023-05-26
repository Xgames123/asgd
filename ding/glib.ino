const int ResetGpu_pin = 4;

struct GTexture
{
  int Width;
  int Height;

  bool Data[]; 
};

void gpu_init(){
 Serial.println("initializing gpu");
 pinMode(ResetGpu_pin, OUTPUT);
 
 digitalWrite(ResetGpu_pin, HIGH);
 delay(100);
 digitalWrite(ResetGpu_pin, LOW);
 delay(3000);
 Serial.println("gpu initialized");

 com_init();
}


void gpu_uploadTex(struct GTexture tex)
{
  com_sendByte(tex.Width, 3);
  com_sendByte(tex.Height, 3);
  for (int i=0; i<tex.Width*tex.Height; i++){
    com_sendBit(tex.Data[i]);
  }
}
