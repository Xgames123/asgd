const int JoyXPin = A0;
const int JoyYPin = A1;
const int JoyButtonPin = A5;

void setup() {

  pinMode(JoyButtonPin, INPUT);
  pinMode(JoyXPin, INPUT);
  pinMode(JoyYPin, INPUT);

  Serial.begin(9600);

}

void loop() {
  int x = analogRead(JoyXPin);
  int y = analogRead(JoyYPin);
  int Button = analogRead(JoyButtonPin);

  Serial.println("");
  if (y > 700 && x < 200) {
    Serial.print("TopLeft");
  }
  if (y > 200 && y < 700 && x < 200) {
    Serial.print("Top");
  }
  if (x < 200 && y < 200) {
    Serial.print("TopRight");
  }
  if (x > 200 && x < 700 && y < 200) {
    Serial.print("Right");
  }
  if (y < 200 && x > 700) {
    Serial.print("BottomRight");
  }
  if (y > 200 && y < 700 && x > 700) {
    Serial.print("Down");
  }
  if (y > 700 && x > 700) {
    Serial.print("BottomLeft");
  }
  if (x > 200 && x < 700 && y > 700) {
    Serial.print("Left");
  }
  if (Button == 0 && x < 600 && x > 400 && y < 600 && y > 400) {
    Serial.print("ButtonPressed");
  }
}
