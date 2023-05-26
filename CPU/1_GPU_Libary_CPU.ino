const int ResetGpu_pin = 4;

struct GTexture
{
  byte Width;
  byte Height;

  bool* Data; 
};

void gpu_init(){
 Serial.println("initializing gpu");
 pinMode(ResetGpu_pin, OUTPUT);
 
 digitalWrite(ResetGpu_pin, HIGH);
 delay(10);
 digitalWrite(ResetGpu_pin, LOW);
 delay(10);
 digitalWrite(ResetGpu_pin, HIGH);
 delay(3000);
 Serial.println("gpu initialized");

 com_init();
}

struct GTexture *gpu_createTex(byte w, byte h)
{
  struct GTexture *tex;
  tex = (GTexture*)malloc(sizeof(struct GTexture));
  tex->Width = w;
  tex->Height = h;
  tex->Data = (bool*)malloc(sizeof(bool)*w*h);
  return tex;
}


void gpu_uploadTex(struct GTexture* tex)
{
  com_sentByte(0, 3); //Command 0 upload texture()
  com_sendByte(tex->Width, 3);
  com_sendByte(tex->Height, 3);
  for (int i=0; i<tex->Width*tex->Height; i++){
    com_sendBit(tex->Data[i]);
  }
}
