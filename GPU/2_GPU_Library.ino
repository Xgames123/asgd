//commands
//1 start uploading texture(3b w, 3b h)
//2 draw texture(3b x, 3b y)
//3 clear()
//4 draw point(3b x, 3b y)
//5 upload data 8bits (8b data)
//6 (unused)
//7 init

struct GTexture{

  byte Width;
  byte Heigt;

  bool* Data;
};

bool Initialized = false;

GTexture* ActiveTexture = 0;
GTexture* DownloadTexture = 0;
int Texture_index = 0;

bool CmdBuffer[200];
int Index = 0;

byte current_command = 0;

void gpu_init()
{
  com_init();
  Serial.println("gpu initialized");
}

void gpu_update(){
  
  if(com_update(*CmdBuffer, sizeof(CmdBuffer)/sizeof(bool)){
    
    gpu_exec_command(CmdBuffer);
  }
}

bool peekBit(int start)
{
  if (start > Index || start < 0){
    Serial.println("OUT OF RANGE READ");
    return 0;
  }
  
  return CmdBuffer[start];
}

byte peekByte(int start, int len)
{
   byte output = 0;
  for (int i = 0; i < len; i++) {
    bool data = peekBit(start+i);
    //Serial.print(data);

    output |= (data << i);
  }
  //Serial.println("");
  return output;
}



bool gpu_buffer_filled(int bits){
  
  return com_hasBits(bits);
}

int gpu_buffer_size(){
  return Index;
}



void gpu_exec_command(bool* buff)
{
  
  byte command = com_parseByte(buff, 0, 4);
  
  if (Command == 7) //init
  {
    if (Initialized == true)
    {
      Serial.println("CANT INITIALIZE 2 TIMES");
      return;
    }
    
    Serial.println("Got init command");
    Initialized = true;
    return;
  }

  if (Initialized == false)
  {
    Serial.println("NOT INITIALIZED");
    return;
  }
  
  if(command == 1) //start uploading texture
  {

    if (DownloadTexture != 0)
    {
      Serial.println("CANT UPLOAD 2 TEXTURES AT THE SAME TIME");
      return;
    }
    
    byte w = com_parseByte(4, 4);
    byte h = com_parseByte(8, 4);
    
    Serial.println("Started downloading texture ");
    GTexture* tex = gpu_create_texture(w, h);

    Serial.print(tex->Width);
    Serial.print("x");
    Serial.println(tex->Heigt);
    
    DownloadTexture = tex;
    return;
  }
  if(command == 2) //draw texture
  {
    Serial.println("Draw Texture");
    if(ActiveTexture == 0)
    {
      Serial.println("NO TEXTURE UPLOAD");
      return;
    }
    byte x = com_parseByte(buff, 4, 4);
    byte y = com_parseByte(buff, 8, 4);

    Serial.print("Coords: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);

    gpu_draw_texture(ActiveTexture, x, y);
    return;
  }
  if (command == 3) // clear
  {
    Serial.println("Clear");
    draw_clear(LOW);
    return;
  }
  if (command == 4){ //draw point
    Serial.print("Draw point: ");
    
    byte x = com_parseByte(buff,4, 4);
    byte y = com_parseByte(buff, 8, 4);
    
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);

    draw_point(x, y, HIGH);
    
    return;
  }else if (command == 5) //upload data 8bits
  {
    byte data = com_parseByte(buff, 4, 8);
    //TODO: everything
    if (DownloadTexture == 0)
    {
      return;
    }
    
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


struct GTexture* gpu_create_texture(byte w, byte h)
{
  struct GTexture* tex = (struct GTexture*)malloc(sizeof(struct GTexture));
  tex->Width = w;
  tex->Heigt = h;
  tex->Data = (bool*)malloc(sizeof(bool)*w*h);

  return tex;
}

void gpu_free_texture(struct GTexture* tex)
{
  free(tex->Data);
  free(tex);
}

void gpu_read_texture_data(struct GTexture* tex)
{
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
    tex->Data[i] = peekBit();
  }

  return tex;
}
