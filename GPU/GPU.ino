#include <1_Display.ino>
#include <2_GPU_Library.ino>
#include <Arduino.h>

// #define LOG_BUFSIZE

void setup() {

  draw_init();

  Serial.begin(9600);

  gpu_init();

  // draw_writePixel(0, 0, HIGH);
}

int lastLoggedBufferSize = -1;

unsigned long last_gpu_read_time = millis();

void loop() {

  draw_writeBuff();

#ifdef LOG_BUFSIZE
  int buffSize = gpu_buffer_size();
  if (lastLoggedBufferSize != buffSize) {
    Serial.print("BufferSize: ");
    Serial.println(buffSize);
    lastLoggedBufferSize = buffSize;
  }
#endif

  gpu_update();
}

void TestLeds() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      draw_writePixel(x, y, HIGH);
      delay(500);
      draw_writePixel(x, y, LOW);
    }
  }
}
