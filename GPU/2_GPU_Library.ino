#define LOG_EXECCOMMAND
// commands
// 1 start uploading texture(4b w, 4b h)
// 2 draw texture(4b x, 4b y)
// 3 clear()
// 4 draw point(4b x, 4b y)
// 5 upload data 8bits (8b data)
// 6 (unused)
// 7 init

#define GPU_CMD_UPLOADTEX 1
#define GPU_CMD_DRAWTEX 2
#define GPU_CMD_DRAWCLEAR 3
#define GPU_CMD_DRAWPOINT 4
#define GPU_CMD_DATA8 5

#define GPU_CMD_INIT 7

int CommandSizeDict[] = {-1, 8, 8, 0, 8, 8, -1, 0};

struct GTexture {

  byte Width;
  byte Height;

  bool *Data;
};

bool Initialized = false;

GTexture *ActiveTexture = 0;
GTexture *DownloadTexture = 0;
int Texture_index = 0;

bool CmdBuffer[200];
int Index = 0;

byte current_command = 0;

void gpu_init() {
  com_init();
  Serial.println("gpu started");
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

int gpu_buffer_size() { return Index; }

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

    byte w = com_parseByte(buff, 4, 4);
    byte h = com_parseByte(buff, 8, 4);

    Serial.println("Started downloading texture ");
    GTexture *tex = gpu_create_texture(w, h);

    Serial.print(tex->Width);
    Serial.print("x");
    Serial.println(tex->Height);

    DownloadTexture = tex;
    return;
  }
  if (command == GPU_CMD_DRAWTEX) // draw texture
  {
    Serial.println("Draw Texture");
    if (ActiveTexture == 0) {
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
  if (command == GPU_CMD_DRAWCLEAR) // clear
  {
    Serial.println("Clear");
    draw_clear(LOW);
    return;
  }
  if (command == GPU_CMD_DRAWPOINT) { // draw point
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
      DownloadTexture->Data[Texture_index + i] = buff[4 + i];
      Texture_index++;
      if (Texture_index >= size) {
        Serial.println("Texture upload done");
        gpu_print_texture(DownloadTexture);

        if (ActiveTexture != 0) {
          gpu_free_texture(ActiveTexture);
          ActiveTexture = 0;
        }
        ActiveTexture = DownloadTexture;
        DownloadTexture = 0;
        return;
      }
    }

    return;
  }

  Serial.print("INVALID COMMAND ");
  Serial.println(command);
}

void gpu_print_texture(struct GTexture *tex) {
  int w = tex->Width;
  int h = tex->Height;
  int i = 0;
  for (int iw = 0; iw < w; iw++) {
    for (int ih = 0; ih < h; ih++) {
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

void gpu_draw_texture(struct GTexture *tex, byte x, byte y) {
  int w = tex->Width;
  int h = tex->Height;
  int i = 0;
  for (int iw = 0; iw < w; iw++) {
    for (int ih = 0; ih < h; ih++) {
      draw_point(x + iw, y + ih, tex->Data[i]);
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
