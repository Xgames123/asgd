const int JoyXPin = A0;
const int JoyYPin = A1;
const int JoyButtonPin = A5;
int Direction = 0;
int DISx = 0;
int DISy = 0;
void setup() {

  pinMode(JoyButtonPin, INPUT);
  pinMode(JoyXPin, INPUT);
  pinMode(JoyYPin, INPUT);
  //com_init();
  Serial.begin(9600);
  gpu_init();
 
  Serial.println("uploading texture 2x2");

  struct GTexture* tex = gpu_createTex(2, 2);
  gpu_uploadTex(tex);
  
}

void loop() {
  int x = analogRead(JoyXPin);
  int y = analogRead(JoyYPin);
  int Button = analogRead(JoyButtonPin);
  Serial.println("");


  if (y > 700 && x < 200) {
    Serial.print("TopLeft");
    Direction = 2;
    if (DISx > 0 && DISy > 0) {
      DISx += -1;
      DISy += -1;
    }
  }
  if (y > 200 && y < 700 && x < 200) {
    Serial.print("Top");
    Direction = 1;
    if (DISy > 0) {
      DISy += -1;
    }
  }
  if (x < 200 && y < 200) {
    Serial.print("TopRight");
    Direction = 8;
    if (DISx < 7 && DISy > 0) {
      DISx += 1;
      DISy += -1;
    }
  }
  if (x > 200 && x < 700 && y < 200) {
    Serial.print("Right");
    Direction = 7;
    if (DISx < 7) {
      DISx += 1;
    }
  }
  if (y < 200 && x > 700) {
    Serial.print("BottomRight");
    Direction = 6;
    if (DISx < 7 && DISy < 7) {
      DISx += 1;
      DISy += 1;
    }
  }
  if (y > 200 && y < 700 && x > 700) {
    Serial.print("Down");
    Direction = 5;
    if (DISy < 7) {
      DISy += 1;
    }
  }
  if (y > 700 && x > 700) {
    Serial.print("BottomLeft");
    Direction = 4;
    if (DISx > 0 && DISy < 7) {
      DISx += -1;
      DISy += 1;
    }
  }
  if (x > 200 && x < 700 && y > 700) {
    Serial.print("Left");
    Direction = 3;
    if (DISx > 0) {
      DISx += -1;
    }
  }
  if (Button == 0 && x < 600 && x > 400 && y < 600 && y > 400) {
    Serial.print("ButtonPressed");
    DISx = 0; DISy = 0;
  }


}
