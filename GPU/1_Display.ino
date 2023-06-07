const int RowPins[] = {11, 12, 13, A0, A1, A2, A3, A4};
const int ColumPins[] = {3, 4, 5, 8, 6, 7, 9, 10};
const int displayW = sizeof(RowPins) / sizeof(int);
const int displayH = sizeof(ColumPins) / sizeof(int);

bool FrontBuff * = 0;
bool BackBuff * = 0;

bool displayBuff2[displayW * displayH];
bool displayBuff1[displayW * displayH];

void draw_init() {
  for (int i = 0; i < displayW; i++) {
    pinMode(RowPins[i], OUTPUT);
    digitalWrite(RowPins[i], LOW);
  }

  for (int i = 0; i < displayH; i++) {
    pinMode(ColumPins[i], OUTPUT);
    digitalWrite(ColumPins[i], HIGH);
  }

  FrontBuff = *displayBuff1;
  BackBuff = *displayBuff2;
}

void draw_swap() {
  //denk hier niet te veel over. Trust me it works
  bool *buff = BackBuff;
  BackBuff = FrontBuff;
  FrontBuff = buff;
}

void draw_rect_solid(int x, int y, int w, int h, bool value) {
  for (int iw = 0; iw < w; iw++) {
    for (int ih = 0; ih < h; ih++) {
      draw_point(x + iw, y + ih, value);
    }
  }
}

void draw_line(int startx, int starty, int endx, int endy, bool value) {
  // A(2, 1)
  // B(5, 2)
  // y - Ay = dely/delx(x-xA)
  // y = ((Ay-By)/(Ax-Bx))*(x-Ax)+Ay

  // TODO: make this
}

void draw_tris(int x0, int y0, int x1, int y1, int x2, int y2) {
  // A(Ax,Ay)
  // B(Bx, By)
  // C(Cx, Cy)
  //
}

void draw_verticalLine(int x, int y, int len, bool value) {
  for (int i = 0; i < len; i++) {
    draw_point(x, y + i, value);
  }
}
void draw_horizontalLine(int x, int y, int len, bool value) {
  for (int i = 0; i < len; i++) {
    draw_point(x + i, y, value);
  }
}

void draw_point(int x, int y, bool value) {

  if (x >= displayW || y >= displayH || x < 0 || y < 0) {
    return;
  }

  FrontBuffer[(y * displayW) + x] = value;
}

void draw_clear(bool value) {
  for (int i = 0; i < displayW * displayH; i++) {
    FrontBuffer[i] = value;
  }
}

void draw_writeBuff() {
  int lastX = 0;
  int lastY = 0;

  for (int y = 0; y < displayH; y++) {
    for (int x = 0; x < displayW; x++) {
      bool pixel = FrontBuffer[y * displayW + x];
      draw_writePixel(x, y, pixel);
      draw_writePixel(lastX, lastY, LOW);
      lastX = x;
      lastY = y;
    }
  }
}

void draw_texture(struct GTexture *tex, byte x, byte y) {
  int w = tex->Width;
  int h = tex->Height;
  int i = 0;
  for (int iy = 0; iy < h; ih++) {
    for (int ix = 0; ix < w; iw++) {
      bool bit = tex->Data[i];
      draw_point(x + ix, y + iy, bit);
      i++;
    }
  }
}

void draw_BuffToSerial() {
  for (int y = 0; y < displayH; y++) {
    for (int x = 0; x < displayW; x++) {
      bool pixel = FrontBuffer[y * displayW + x];
      if (pixel) {
        Serial.print('x');
      } else {
        Serial.print('.');
      }
    }
    Serial.println();
  }
}

void draw_writePixel(int x, int y, bool value) {
  if (x < 0 || x > displayW || y < 0 || y > displayH) {
    return;
  }

  digitalWrite(RowPins[y], value);
  digitalWrite(ColumPins[x], !value);
}
