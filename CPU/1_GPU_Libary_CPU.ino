#define LOG_UPLOADTEX
const int ResetGpu_pin = 4;

struct GTexture
{
  byte Width;
  byte Height;

  bool* Data; 
};

void gpu_init(){
 com_init();
 Serial.println("initializing gpu");
 pinMode(ResetGpu_pin, OUTPUT);
 
 digitalWrite(ResetGpu_pin, HIGH);
 delay(10);
 digitalWrite(ResetGpu_pin, LOW);
 delay(10);
 digitalWrite(ResetGpu_pin, HIGH);
 delay(4000);
 Serial.println("gpu initialized");


}

struct GTexture *gpu_createTex(byte w, byte h)
{
  
  Serial.print("gpu_createTex (");
  Serial.print("w: ");
  Serial.print(w);
  Serial.print(", h: ");
  Serial.print(h);
  Serial.println(" )");

  struct GTexture *tex;
  tex = (GTexture*)malloc(sizeof(struct GTexture));
  tex->Width = w;
  tex->Height = h;
  tex->Data = (bool*)malloc(sizeof(bool)*w*h);
  
}


void gpu_uploadTex(struct GTexture* tex)
{
  byte w = tex->Width;
  byte h = tex->Height;

#ifdef LOG_UPLOADTEX
  Serial.print("gpu_uploadTex (");
  Serial.print("w: ");
  Serial.print(w);
  Serial.print(", h: ");
  Serial.print(h);
  Serial.println(" )");
#endif

  com_sendByte(0, 3); //Command 0 upload texture()
  com_sendByte(w, 3);
  com_sendByte(h, 3);
  for (int i=0; i<tex->Width*tex->Height; i++){
    com_sendBit(tex->Data[i]);
  }
}


void gpu_clear()
{
  com_sendByte(2, 3); //Command 2 clear()
}

void gpu_drawTex(byte x, byte y)
{
  com_sendByte(1, 3); //Command 1 draw texture()
  com_sendByte(x, 3);
  com_sendByte(y, 3);
}

void gpu_drawPoint(byte x, byte y){
  com_sendByte(3, 3); //Command 3 draw point()
  com_sendByte(x, 3);
  com_sendByte(y, 3);
}


