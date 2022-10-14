#include <Wire.h>

#define SLAVE_1 1
#define SLAVE_2 2

char text[] = "HELLO1";
  
int count = 0, slaveState;

void setup() {
	
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  slaveState = count % 2 == 0 ? SLAVE_1 : SLAVE_2;
  Wire.beginTransmission(slaveState);
  Wire.write(text);
  Wire.endTransmission();
  Serial.println(text);
  int len = Wire.requestFrom(slaveState, 6);
  count++;
  for(int i = 0; i < len; i++){
  	text[i] = Wire.read();
  } 
  delay(1000);
}
