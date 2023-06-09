#define LOG_UPLOADTEX
// #define LOG_CREATETEX
#define LOG_GPUINIT

#define GPU_CMD_UPLOADTEX 1
#define GPU_CMD_DRAWTEX 2
#define GPU_CMD_DRAWCLEAR 3
#define GPU_CMD_DRAWPOINT 4
#define GPU_CMD_DATA8 5
#define GPU_CMD_SWAP 6
#define GPU_CMD_INIT 7
#define GPU_CMD_SWAPCLEAR 8
#define GPU_CMD_DRAWTEX8 9

const int ResetGpu_pin = 4;

struct GTexture {
  byte Width;
  byte Height;

  bool *Data;
};

void gpu_init() {
  com_init();
#ifdef LOG_GPUINIT
  Serial.println("Resetting gpu....");
#endif
  pinMode(ResetGpu_pin, OUTPUT);

  digitalWrite(ResetGpu_pin, HIGH);
  delay(10);
  digitalWrite(ResetGpu_pin, LOW);
  delay(10);
  digitalWrite(ResetGpu_pin, HIGH);
  delay(4000);
#ifdef LOG_GPUINIT
  Serial.println("Sending gpu init command...");
#endif
  com_sendCmd(GPU_CMD_INIT);
  delay(100);
#ifdef LOG_GPUINIT
  Serial.println("Gpu should be initialized by now");
#endif
}

struct GTexture *gpu_createTex(byte w, byte h) {
#ifdef LOG_CREATETEX
  Serial.print("creating texture (");
  Serial.print("w: ");
  Serial.print(w);
  Serial.print(", h: ");
  Serial.print(h);
  Serial.println(" )");
#endif

  struct GTexture *tex = (GTexture *)malloc(sizeof(struct GTexture));
  tex->Width = w;
  tex->Height = h;
  tex->Data = (bool *)malloc(sizeof(bool) * w * h);

  return tex;
}

void gpu_uploadTex(byte slot, struct GTexture *tex) {
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

  com_sendCmd(GPU_CMD_UPLOADTEX);
  com_sendByte(slot, 4);
  com_sendByte(w, 4);
  com_sendByte(h, 4);
  for (int i = 0; i < tex->Width * tex->Height; i += 8) {
    com_sendCmd(GPU_CMD_DATA8);
    com_sendBit(tex->Data[i]);
    com_sendBit(tex->Data[i + 1]);
    com_sendBit(tex->Data[i + 2]);
    com_sendBit(tex->Data[i + 3]);
    com_sendBit(tex->Data[i + 4]);
    com_sendBit(tex->Data[i + 5]);
    com_sendBit(tex->Data[i + 6]);
    com_sendBit(tex->Data[i + 7]);
  }
}

void gpu_clear() { com_sendCmd(GPU_CMD_DRAWCLEAR); }

void gpu_swap(){ com_sendCmd(GPU_CMD_SWAP);}

void gpu_swapclear(){
  com_sendCmd(GPU_CMD_SWAPCLEAR);
}

void gpu_drawTex(byte slot, byte x, byte y) {
  com_sendCmd(GPU_CMD_DRAWTEX);
  com_sendByte(slot, 4);
  com_sendByte(x, 4);
  com_sendByte(y, 4);
}
void gpu_drawTex8(byte slot, int x, int y)
{
  byte bx = (x+128);
  byte by = (y+128);
  
  com_sendCmd(GPU_CMD_DRAWTEX8);
  com_sendByte(slot, 4);
  com_sendByte(bx, 8);
  com_sendByte(by, 8);
}

void gpu_drawPoint(byte x, byte y) {
  com_sendCmd(GPU_CMD_DRAWPOINT);
  com_sendByte(x, 4);
  com_sendByte(y, 4);
}
