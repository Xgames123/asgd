

//commands
//0 upload texture(3b w, 3b h, b[] data)
//1 draw texture(3b x, 3b y)
//2 clear()
//3 draw point(3b x, 3b y)

struct GTexture{

  byte Width;
  byte Heigt;

  bool* Data;
};

GTexture* ActiveTexture = 0;



void gpu_init()
{
  com_init();
}

bool gpu_buffer_filled(int bits){
  
  return com_hasBits(bits);
}


void gpu_read_all_commands(){
  
  while(true){
    if (!com_hasBits(3)){
      return;
    }
    gpu_read_command();
  }

}


bool gpu_read_command()
{
  
  if(!com_hasBits(3)){
    return;
  }

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
