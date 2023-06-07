#define LOG_TEST1
const int JoyXPin = A0;
const int JoyYPin = A1;
const int JoyButtonPin = A5;
int Direction = 0;
int DISx = 0;
int DISy = 0;
void setup() {
  Serial.begin(9600);

  create_textures();

  pinMode(JoyButtonPin, INPUT);
  pinMode(JoyXPin, INPUT);
  pinMode(JoyYPin, INPUT);
  // com_init();

  gpu_init();

#ifdef LOG_TEST1
  Serial.println("Uploading player texture");
#endif
  struct GTexture *tex = Tex_player();
  gpu_uploadTex(tex);

  on_input();
}

void loop() {
  int x = analogRead(JoyXPin);
  int y = analogRead(JoyYPin);
  int Button = analogRead(JoyButtonPin);

  if (y > 700 && x < 200) {

    Direction = 2;
    if (DISx > 0 && DISy > 0) {
      Serial.print("TopLeft");
      DISx += -1;
      DISy += -1;
      on_input();
    } else {
      Serial.println("TopLeft limit reached");
    }
  }
  if (y > 200 && y < 700 && x < 200) {

    Direction = 1;
    if (DISy > 0) {
      Serial.print("Top");
      DISy += -1;
      on_input();
    } else {
      Serial.println("Top limit reached");
    }
  }
  if (x < 200 && y < 200) {

    Direction = 8;
    if (DISx < 7 && DISy > 0) {
      Serial.print("TopRight");
      DISx += 1;
      DISy += -1;
      on_input();
    } else {
      Serial.println("TopRight limit reached");
    }
  }
  if (x > 200 && x < 700 && y < 200) {

    Direction = 7;
    if (DISx < 7) {
      Serial.print("Right");
      DISx += 1;
      on_input();
    } else {
      Serial.println("Right limit reached");
    }
  }
  if (y < 200 && x > 700) {

    Direction = 6;
    if (DISx < 7 && DISy < 7) {
      Serial.print("BottomRight");
      DISx += 1;
      DISy += 1;
      on_input();
    } else {
      Serial.println("BottomRight limit reached");
    }
  }
  if (y > 200 && y < 700 && x > 700) {

    Direction = 5;
    if (DISy < 7) {
      Serial.print("Down");
      DISy += 1;
      on_input();
    } else {
      Serial.println("Down limit reached");
    }
  }
  if (y > 700 && x > 700) {

    Direction = 4;
    if (DISx > 0 && DISy < 7) {
      Serial.print("BottomLeft");
      DISx += -1;
      DISy += 1;
      on_input();
    } else {
      Serial.println("BottomLeft limit reached");
    }
  }
  if (x > 200 && x < 700 && y > 700) {

    Direction = 3;
    if (DISx > 0) {
      Serial.print("Left");
      DISx += -1;
      on_input();
    } else {
      Serial.println("Left limit reached");
    }
  }
  if (Button == 0 && x < 600 && x > 400 && y < 600 && y > 400) {
    Serial.print("ButtonPressed");
    DISx = 0;
    DISy = 0;
    gpu_clear();
    testLeds();
    gpu_clear();
    on_input();
  }
}

void on_input() {
  Serial.print("x: ");
  Serial.print(DISx);
  Serial.print("  y: ");
  Serial.println(DISy);
  gpu_drawTex(DISx, DISy);
  gpu_swapclear();
}

void testLeds() {
  gpu_clear();

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      gpu_drawPoint(x, y);
    }
  }
}
