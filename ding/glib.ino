const int ResetGpu_pin = 4;

struct GPUTex
{
  int Width;
  int Height;

  bool Data[]; 
};

void gpu_init(){
 pinMode(ResetGpu_pin, OUTPUT);
 
 digitalWrite(ResetGpu_pin, HIGH);
 delay(100);
 digitalWrite(ResetGpu_pin, LOW);
 delay(3000);

 com_init();
}


void gpu_uploadTex(struct GPUTex tex)
{
  

}