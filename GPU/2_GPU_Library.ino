

//commands
//1 upload texture(3b w, 3b h, b[] data)
//2 draw texture(3b x, 3b y)
//3 clear()
//4 draw point(3b x, 3b y)

struct GTexture{

  byte Width;
  byte Heigt;

  bool* Data;
};

GTexture* ActiveTexture = 0;

bool CmdBuffer[200];
int Index = 0;

byte current_command = 0;

void gpu_init()
{
  com_init();
  Serial.println("gpu initialized");
}

void gpu_update(){
  
  if(com_update()){

    unsigned long time = millis();
    if((time - last_bit_time) > 600 && Index != 0){
      Index = 0;
      current_command = 0;
      Serial.println("Too much time between bits clearing command buffer");
    }
    last_bit_time = time;

    bool bit = digitalRead(ComPin);
    Buffer[Index] = bit;
    Index++;
    if (Index >= sizeof(Buffer) / sizeof(bool)) {
      Serial.println("TOO MUCH DAT IN DATBUFFER (OVERFLOW)");
      Index = 0;
    }

  }
}

int get_command_size(byte command){
  if (command == 1){
    return 
  }


}



bool gpu_buffer_filled(int bits){
  
  return com_hasBits(bits);
}

int gpu_buffer_size(){
  return Index;
}



void gpu_read_command()
{


  byte command = com_readByte(3);
  if(command == 0) //upload texture
  {
    Serial.println("Downloading texture");
    GTexture* tex = gpu_read_texture();
    Serial.print("Done downloading texture ");
    Serial.print(tex->Width);
    Serial.print("x");
    Serial.println(tex->Heigt);
    if (ActiveTexture != 0)
    {
      free(ActiveTexture->Data);
      free(ActiveTexture);
      ActiveTexture = 0;
    }
    ActiveTexture = tex;
    return;
  }
  if(command == 1) //draw texture
  {
    Serial.println("Draw Texture");
    if(ActiveTexture == 0)
    {
      Serial.println("NO TEXTURE UPLOAD");
      return;
    }
    byte x = com_readByte(3);
    byte y = com_readByte(3);

    Serial.print("Coords: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);

    gpu_draw_texture(ActiveTexture, x, y);
    return;
  }
  if (command == 2) // clear
  {
    Serial.println("Clear");
    draw_clear(LOW);
    return;
  }
  if (command == 3){ //draw point
    Serial.print("Draw point: ");
    
    byte x = com_readByte(3);
    byte y = com_readByte(3);
    
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);

    draw_point(x, y, HIGH);
    
    return;
  } 

  Serial.print("INVALID COMMAND ");
  Serial.println(command);
  
}


void gpu_draw_texture(struct GTexture* tex, byte x, byte y)
{
  int w = tex->Width;
  int h = tex->Heigt;
  int i = 0;
  for (int iw =0; iw < w; iw++)
  {
    for (int ih =0; ih < h; ih++)
    {
      draw_point(x+iw, y+ih, tex->Data[i]);
      i++;
    }
  }
  
}



struct GTexture* gpu_read_texture()
{
  byte w = com_readByte(3);
  byte h = com_readByte(3);

Serial.print("gpu_read_tex() w: ");
  Serial.print(w);
  Serial.print(" h: ");
  Serial.println(h);
  
  struct GTexture* tex = (struct GTexture*)malloc(sizeof(struct GTexture));
  tex->Width = w;
  tex->Heigt = h;
  tex->Data = (bool*)malloc(sizeof(bool)*w*h);
  
  for (int i=0; i < w*h; i++)
  {
    tex->Data[i] = com_readBit();
  }

  return tex;
}
