

//commands
//0 upload texture

struct GPUTTex{

  int Width;
  int Heigt;

  bool[] Data;
}


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
    GPUTex tex = gpu_read_texture();
    Serial.println("Done uploading texture {0}x{1}", tex.Width, tex.Heigt);
    
  }
  
}

GPUTTex gpu_read_texture()
{
  byte w = com_readByte();
  
  
  
}
