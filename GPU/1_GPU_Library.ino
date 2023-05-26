

//commands
//0 upload texture(3b w, 3b h, b[] data)
//1 draw texture(3b x, 3b y)

struct GTexture{

  byte Width;
  byte Heigt;

  bool* Data;
};

GTexture* ActiveTexture;



void gpu_init()
{
  com_init();
}



void gpu_read_command()
{
  Serial.println("waiting on command...");
  byte command = com_readByte(3);
  if(command == 0) //upload texture
  {
    Serial.println("Uploading texture");
    GTexture* tex = gpu_read_texture();
    Serial.print("Done uploading texture ");
    Serial.print(tex->Width);
    Serial.print("x");
    Serial.println(tex->Heigt);
    ActiveTexture = tex;
    return;
  }
  if(command == 1) //draw texture
  {
    Serial.println("Draw Texture");
    return;
  }

  Serial.print("INVALID COMMAND ");
  Serial.println(command);
  
}

struct GTexture* gpu_read_texture()
{
  byte w = com_readByte(3);
  byte h = com_readByte(3);

  Serial.print("tex w: ");
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
