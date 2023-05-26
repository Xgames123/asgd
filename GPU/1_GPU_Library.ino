

//commands
//0 upload texture(3b w, 3b h, b[] data)
//1 draw texture(3b x, 3b y)

struct GTexture{

  int Width;
  int Heigt;

  bool Data[];
};

GTexture ActiveTexture;



void gpu_init()
{
  com_init();
}



int gpu_read_command()
{
  byte command = com_readByte(3);

  if(command == 0) //upload texture
  {
    Serial.println("Uploading texture");
    GTexture tex = gpu_read_texture();
    Serial.print("Done uploading texture ");
    Serial.print(tex.Width);
    Serial.print("x");
    Serial.println(tex.Heigt);
    ActiveTexture = tex;
  }
  if(command == 1) //draw texture
  {
  }
  
}

struct GTexture gpu_read_texture()
{
  byte w = com_readByte(3);
  byte h = com_readByte(3);
  bool data[w*h];
  for (int i=0; i < w*h; i++)
  {
    data[i] = com_readBit();
  }
  
  
}
