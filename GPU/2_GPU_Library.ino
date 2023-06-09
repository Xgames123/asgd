//#define LOG_EXECCOMMAND

// commands
// 1 start uploading texture(u4b w, u4b h)
// 2 draw texture(u4b x, u4b y)
// 3 clear()
// 4 draw point(u4b x, u4b y)
// 5 upload data 8bits (u8b data)
// 6 swap
// 7 init
// 8 swap and clear
// 9 draw texture (8b x, 8b y)

#define GPU_CMD_UPLOADTEX 1
#define GPU_CMD_DRAWTEX4 2
#define GPU_CMD_DRAWCLEAR 3
#define GPU_CMD_DRAWPOINT4 4
#define GPU_CMD_DATA8 5
#define GPU_CMD_SWAP 6
#define GPU_CMD_INIT 7
#define GPU_CMD_SWAPCLEAR 8
#define GPU_CMD_DRAWTEX8 9

int CommandSizeDict[] = { -1,
                          12, //GPU_CMD_UPLOADTEX
                          12, //GPU_CMD_DRAWTEX4
                          0, //GPU_CMD_DRAWCLEAR
                          8, //GPU_CMD_DRAWPOINT4
                          8, //GPU_CMD_DATA8
                          0, //GPU_CMD_SWAP
                          0, //GPU_CMD_INIT
                          0, //GPU_CMD_SWAPCLEAR
                          20 //GPU_CMD_DRAWTEX8
                        };

struct GTexture {

  byte Width;
  byte Height;

  bool *Data;
};

bool Initialized = false;

GTexture* TextureSlots[14];

GTexture *DownloadTexture = 0;
int Texture_index = 0;
int Texture_slot = -1;

bool CmdBuffer[200];
int Index = 0;

byte current_command = 0;

void gpu_init() {
  com_init();
  Serial.println("gpu started");
}
bool gpu_isInit()
{
  return Initialized;
}

void gpu_update() {

  bool *buff = CmdBuffer;
  int *cmdSizeDict = CommandSizeDict;

  if (com_update(buff, (sizeof(CmdBuffer) / sizeof(bool)), cmdSizeDict)) {

    gpu_exec_command(CmdBuffer);
  }
}

bool peekBit(int start) {
  if (start > Index || start < 0) {
    Serial.println("OUT OF RANGE READ");
    return 0;
  }

  return CmdBuffer[start];
}

byte peekByte(int start, int len) {
  byte output = 0;
  for (int i = 0; i < len; i++) {
    bool data = peekBit(start + i);
    // Serial.print(data);

    output |= (data << i);
  }
  // Serial.println("");
  return output;
}

int gpu_buffer_size() {
  return Index;
}

void gpu_exec_command(bool *buff) {

  byte command = com_parseByte(buff, 0, 4);

#ifdef LOG_EXECCOMMAND
  Serial.print("executing command id: ");
  Serial.println(command);
#endif

  if (command == GPU_CMD_INIT) // init
  {
    if (Initialized == true) {
      Serial.println("CANT INITIALIZE 2 TIMES");
      return;
    }

    Serial.println("Got init command");
    Initialized = true;
    return;
  }

  if (Initialized == false) {
    Serial.println("NOT INITIALIZED");
    return;
  }

  if (command == GPU_CMD_UPLOADTEX) // start uploading texture
  {

    if (DownloadTexture != 0) {
      Serial.println("CANT UPLOAD 2 TEXTURES AT THE SAME TIME");
      return;
    }

    byte slot = com_parseByte(buff, 4, 4);
    byte w = com_parseByte(buff, 8, 4);
    byte h = com_parseByte(buff, 12, 4);

    Serial.println("Started downloading texture ");
   
    GTexture *tex = gpu_create_texture(w, h);

    Serial.print(tex->Width);
    Serial.print("x");
    Serial.print(tex->Height);
    Serial.print(" slot: ");
    Serial.println(slot);

    Texture_slot = slot;
    DownloadTexture = tex;
    Texture_index = 0;
    return;
  }
  if (command == GPU_CMD_DRAWTEX4) // draw texture
  {
    Serial.println("Draw Texture");
    
    byte slot = com_parseByte(buff, 4, 4);
    byte x = com_parseByte(buff, 8, 4);
    byte y = com_parseByte(buff, 12, 4);

    if (TextureSlots[slot] == 0) {
      Serial.println("NO TEXTURE UPLOAD");
      return;
    }
    
    Serial.print("Coords: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print("slot: ");
    Serial.println(slot);

    gpu_draw_texture(TextureSlots[slot], x, y);
    return;
  }
  if (command == GPU_CMD_DRAWCLEAR) // clear
  {
    Serial.println("Clear");
    draw_clear(LOW);
    return;
  } else if (command == GPU_CMD_DRAWPOINT4) { // draw point
    Serial.print("Draw point: ");

    byte x = com_parseByte(buff, 4, 4);
    byte y = com_parseByte(buff, 8, 4);

    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);

    draw_point(x, y, HIGH);

    return;
  } else if (command == GPU_CMD_DATA8) // data 8bits
  {
    if (DownloadTexture == 0) {
      Serial.println("CANT WRITE TO A TEXTURE THAT IS NOT CREATED");
      return;
    }

    Serial.println("Writing data to texture");
    int size = DownloadTexture->Width * DownloadTexture->Height;
    for (int i = 0; i < 8; i++) {
      DownloadTexture->Data[Texture_index] = buff[4 + i];

      if (Texture_index >= size) {
        Serial.println("Texture upload done");
        gpu_print_texture(DownloadTexture);

        if (TextureSlots[Texture_slot] != 0) {
          gpu_free_texture(TextureSlots[Texture_slot]);
          TextureSlots[Texture_slot] = 0;
        }
        TextureSlots[Texture_slot] = DownloadTexture;
        DownloadTexture = 0;
        return;
      }
      Texture_index++;
    }

    return;
  } else if (command == GPU_CMD_SWAP) {
    draw_swap();
    return;
  }
  else if (command == GPU_CMD_SWAPCLEAR) {
    draw_swap();
    draw_clear(LOW);
    return;
  } else if ( command == GPU_CMD_DRAWTEX8 ) {

    Serial.println("Draw Texture 8bit");
   
    byte slot = com_parseByte(buff, 4, 4);
    int x = com_parseByte(buff, 8, 8);
    int y = com_parseByte(buff, 16, 8);
    //int  00000000 00000000 00000000 00000000
    //byte 00000000
    //     ^ sign bit
    x -= 128;
    y -= 128;

    if (TextureSlots[slot] == 0) {
      Serial.println("NO TEXTURE UPLOAD");
      return;
    }

    Serial.print("Coords: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);

    gpu_draw_texture(TextureSlots[slot], x, y);
    return;

  }

  Serial.print("INVALID COMMAND");
  Serial.println(command);
}

void gpu_print_texture(struct GTexture *tex) {
  int w = tex->Width;
  int h = tex->Height;
  int i = 0;

  for (int ih = 0; ih < h; ih++) {
    for (int iw = 0; iw < w; iw++) {
      if (tex->Data[i]) {
        Serial.print('x');
      } else {
        Serial.print('.');
      }
      i++;
    }
    Serial.println();
  }
}

void gpu_draw_texture(struct GTexture *tex, int x, int y) {
  int w = tex->Width;
  int h = tex->Height;
  int i = 0;

  for (int iy = 0; iy < h; iy++) {
    for (int ix = 0; ix < w; ix++) {
      bool bit = tex->Data[i];
      draw_point(x + ix, y + iy, bit);
      i++;
    }
  }
}

struct GTexture *gpu_create_texture(byte w, byte h) {
  struct GTexture *tex = (struct GTexture *)malloc(sizeof(struct GTexture));
  tex->Width = w;
  tex->Height = h;
  tex->Data = (bool *)malloc(sizeof(bool) * w * h);

  return tex;
}

void gpu_free_texture(struct GTexture *tex) {
  free(tex->Data);
  free(tex);
}
