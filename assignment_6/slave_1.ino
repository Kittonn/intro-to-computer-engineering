#include <Wire.h>

#define SLAVE_1 1

char text[7];

void setup() {

  Wire.begin(SLAVE_1);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {}

void receiveEvent(int byte) {
  for(int i = 0; i < byte; i++){
    text[i] = Wire.read();
  }
  Serial.println(text);
}

void requestEvent() {
  text[5]++;
  if (text[5] == ':') {
  	text[5] = '1';
  }
  Wire.write(text);
}