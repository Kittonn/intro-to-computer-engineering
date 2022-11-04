#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <TimerOne.h>

#define DEBUG 1

struct Time {
  int hour;
  int min;
  int sec;
};



// ==== OLED SETTING ====
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==== ACCELEROMETER SETTING ====
const unsigned int X_AXIS_PIN = 0;
const unsigned int Y_AXIS_PIN = 1;
const unsigned int Z_AXIS_PIN = 2;
const unsigned int NUM_AXES = 3;
const unsigned int PINS[NUM_AXES] = {
  X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN
};
const unsigned int BUFFER_SIZE = 16;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };

// ==== BUTTON SETTING ====
#define BTN_1 7
#define BTN_2 6
#define BTN_3 5
int button[3] = { BTN_1, BTN_2, BTN_3 };
int reading[3];
int buttonState[3];
int lastButtonState[3] = { HIGH, HIGH, HIGH };
unsigned long long int debounceDelay = 50;
unsigned long long int lastDebounceTime[3];

// ==== CLOCK SETTING ====
Time clock = { 0, 0, 0 };

// ==== COUNT UP CLOCK SETTING ====
Time countUp = { 0, 0, 0 };
int onCountUpPause = 1;

// ==== COUNT DOWN CLOCK SETTING ====
Time countDown = { 0, 0, 0 };
int onCountDownStart = 0, onCountDownPause = 0;

// ==== MODE SETTING ====
#define CLOCK_MODE 0
#define COUNTER_UP_MODE 1
#define COUNTER_DOWN_MODE 2
int MODE = CLOCK_MODE;

// ==== ACCELEROMETER ====
int get_axis(const int axis) {
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  long sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
    sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
}

int get_x() {
  return get_axis(0);
}
int get_y() {
  return get_axis(1);
}
int get_z() {
  return get_axis(2);
}

// ==== DEBOUNCE BUTTON ====
int debounce(int i) {
  int isChange = 0;
  reading[i] = digitalRead(button[i]);
  if (reading[i] != lastButtonState[i]) {
    lastDebounceTime[i] = millis();
  }
  if ((millis() - lastDebounceTime[i]) >= debounceDelay) {
    if (reading[i] != buttonState[i]) {
      buttonState[i] = reading[i];
      isChange = 1;
    }
  }
  lastButtonState[i] = reading[i];
  return isChange;
}

// ==== TIMER ONE ====
void interruptClock() {
  timer();
  countUpTime();
}

// ==== TIMER CLOCK ====
void timer() {
  clock.sec += 1;
  clock.min += clock.sec / 60;
  clock.sec %= 60;
  clock.hour += clock.min / 60;
  clock.min %= 60;
  clock.hour %= 24;
}

void change_hour() {
  clock.hour += 1;
  clock.hour %= 24;
}

void change_min() {
  clock.min += 1;
  clock.hour += clock.min / 60;
  clock.min %= 60;
}

// ==== COUNT UP CLOCK ====
void countUpTime() {
  if (!onCountUpPause) {
    countUp.sec += 1;
    countUp.min += countUp.sec / 60;
    countUp.sec %= 60;
    countUp.hour += countUp.min / 60;
    countUp.min %= 60;
  }
}

void resetCountUpTime() {
  countUp.hour = 0;
  countUp.min = 0;
  countUp.sec = 0;
}


// ==== PRINT DEBUG ====
void print_timer() {
  Serial.print(clock.hour);
  Serial.print(" : ");
  Serial.print(clock.min);
  Serial.print(" : ");
  Serial.println(clock.sec);
}

void print_countUp() {
  Serial.print(countUp.hour);
  Serial.print(" : ");
  Serial.print(countUp.min);
  Serial.print(" : ");
  Serial.println(countUp.sec);
}

// ==== CLOCK MODE ====
void change_mode() {
  MODE += 1;
  MODE %= 3;
}

String timerText = "00 : 00 : 00";
String countUpText = "00 : 00 : 00";
void timer_text() {
  timerText[0] = (clock.hour / 10) + '0';
  timerText[1] = (clock.hour % 10) + '0';

  timerText[5] = (clock.min / 10) + '0';
  timerText[6] = (clock.min % 10) + '0';

  timerText[10] = (clock.sec / 10) + '0';
  timerText[11] = (clock.sec % 10) + '0';
}

void setup() {
  Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("All OLED Start Work !!!");
  }
  for (int i = 0; i < 3; i++) {
    pinMode(button[i], INPUT_PULLUP);
  }
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(interruptClock);
}

void loop() {


  if (debounce(0)) {
    if (!digitalRead(button[0])) {
      if (MODE == 0) {
        change_hour();
      } else if (MODE == 1) {
        onCountUpPause = onCountUpPause == 1 ? 0 : 1;
      }
    }
  }

  if (debounce(1)) {
    if (!digitalRead(button[1])) {
      if (MODE == 0) {
        change_min();
      } else if (MODE == 1) {
        onCountUpPause = 1;
        resetCountUpTime();
      }
    }
  }

  if (debounce(2)) {
    if (!digitalRead(button[2])) {
      change_mode();
      // Serial.println(MODE);
    }
  }


  if (DEBUG) {
    if (MODE == 0) {
      // print_timer();
    } else if (MODE == 1) {
      // print_countUp();
    }
    // Serial.println(MODE);
  }



  // Serial.print(get_x());
  // Serial.print(" ");
  // Serial.print(get_y());
  // Serial.print(" ");
  // Serial.println(get_z());
  timer_text();
  Serial.println(timerText);
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(10, 0);
  OLED.setTextSize(1);
  OLED.println(timerText);
  // OLED.setCursor(10, 15);
  // OLED.println("World!");
  OLED.display();
}