#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <TimerOne.h>

struct Time {
  int hour;
  int min;
  int sec;
};

int countDownCheck = 1;
int alarmCheck = 1;

// ==== LDR SETTING ====
#define LDR_PIN A3
unsigned int LDRvalue;

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
#define BTN_4 4
int button[4] = { BTN_1, BTN_2, BTN_3, BTN_4 };
int reading[4];
int buttonState[4];
int lastButtonState[4] = { HIGH, HIGH, HIGH, HIGH };
unsigned long long int debounceDelay = 50;
unsigned long long int lastDebounceTime[4];

// ==== BUZZER SETTING ====
#define BUZZER_PIN 8
int numTones = 10;
int tones[] = { 261, 277, 294, 311, 330, 349, 370, 392, 415, 440 };

// ==== ULTRASONIC SENSOR ====
int trigPin = 11;
int echoPin = 10;
float duration_us, distance_cm;

// ==== CLOCK SETTING ====
Time clock = { 0, 0, 0 };

// ==== COUNT UP CLOCK SETTING ====
Time countUp = { 0, 0, 0 };
int onCountUpPause = 1;

// ==== COUNT DOWN CLOCK SETTING ====
Time countDown = { 0, 0, 0 };
int onCountDown = 1, finishCountDown = 0, onStartCountDown = 0;


// ==== ALARM CLOCK SETTING ====
Time alarm = { 0, 0, 0 };
Time alarmSave = { 0, 0, 0 };
int onAlarm = 0;

// ==== MODE SETTING ====
#define CLOCK_MODE 0
#define COUNTER_UP_MODE 1
#define ALARM_MODE 2
#define COUNTER_DOWN_MODE 3
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
  change_timestart_alarm();
  countDownTime();
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration_us = pulseIn(echoPin, HIGH);
  distance_cm = 0.017 * duration_us;

  if (finishCountDown && distance_cm < 7) {
    OLED.clearDisplay();
    countDownCheck = 0;
    finishCountDown = 0;
    onStartCountDown = 0;
  } else {
    countDownCheck = 1;
  }

  if (onAlarm && distance_cm < 7) {
    OLED.clearDisplay();
    onAlarm = 0;
    alarmCheck = 0;
  } else {
    alarmCheck = 1;
  }
}

// ==== TIMER CLOCK ====
void timer() {
  clock.sec += 1;
  clock.min += clock.sec / 60;
  clock.sec %= 60;
  clock.hour += clock.min / 60;
  clock.min %= 60;
  clock.hour %= 24;
  if (clock.sec == 0) {
    EEPROM.update(20, clock.hour);
    EEPROM.update(21, clock.min);
  }
}

void change_hour() {
  clock.hour += 1;
  EEPROM.update(20, clock.hour);
  clock.hour %= 24;
}

void change_min() {
  clock.min += 1;
  clock.hour += clock.min / 60;
  clock.hour %= 24;
  EEPROM.update(21, clock.min);
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

// ==== COUNT DOWN CLOCK ====
void change_min_countdown() {
  countDown.min += 1;
  countDown.min %= 60;
}

void change_sec_countdown() {
  countDown.sec += 1;
  countDown.sec %= 60;
}

void countDownTime() {

  if (!onCountDown) {
    if (countDown.sec == 0 && countDown.min != 0) {
      if (countDown.min != 0) {
        countDown.min -= 1;
      }
      countDown.sec = 60;
    }
    countDown.sec -= 1;
    if (countDown.min == 0 && countDown.sec == 0) {
      onCountDown = 1;
      finishCountDown = 1;
    }
  }
}

void resetcountDownTime() {
  countDown.hour = 0;
  countDown.min = 0;
  countDown.sec = 0;
}

// ==== ALARM CLOCK ====
void change_hour_alarm() {
  alarm.hour += 1;
  alarm.hour %= 24;
}

void change_min_alarm() {
  alarm.min += 1;
  alarm.min %= 60;
}

void change_timestart_alarm() {
  if (clock.sec == 0) {
    alarm.min = clock.min;
  }
  if (clock.min == 0) {
    alarm.hour = clock.hour;
  }
}


// ==== CLOCK MODE ====
void change_mode() {
  MODE += 1;
  MODE %= 4;
  if (MODE == ALARM_MODE) {
    alarm.hour = clock.hour;
    alarm.min = clock.min;
  }
}

// ==== Time Text ====
String timerText = "00 : 00";
String countUpText = "00 : 00";
String countDownText = "00 : 00";
String alarmText = "00 : 00";
String saveAlarmText = "00 : 00";
void time_text() {
  timerText[0] = (clock.hour / 10) + '0';
  timerText[1] = (clock.hour % 10) + '0';

  timerText[5] = (clock.min / 10) + '0';
  timerText[6] = (clock.min % 10) + '0';

  countUpText[0] = (countUp.min / 10) + '0';
  countUpText[1] = (countUp.min % 10) + '0';

  countUpText[5] = (countUp.sec / 10) + '0';
  countUpText[6] = (countUp.sec % 10) + '0';

  countDownText[0] = (countDown.min / 10) + '0';
  countDownText[1] = (countDown.min % 10) + '0';

  countDownText[5] = (countDown.sec / 10) + '0';
  countDownText[6] = (countDown.sec % 10) + '0';

  alarmText[0] = (alarm.hour / 10) + '0';
  alarmText[1] = (alarm.hour % 10) + '0';

  alarmText[5] = (alarm.min / 10) + '0';
  alarmText[6] = (alarm.min % 10) + '0';

  saveAlarmText[0] = (alarmSave.hour / 10) + '0';
  saveAlarmText[1] = (alarmSave.hour % 10) + '0';

  saveAlarmText[5] = (alarmSave.min / 10) + '0';
  saveAlarmText[6] = (alarmSave.min % 10) + '0';
}

void display_text(int x1, int y1, String name, int x2, int y2, String timer, int size1, int size2) {
  OLED.setTextSize(size1);
  OLED.setCursor(x1, y1);
  OLED.println(name);
  OLED.setTextSize(size2);

  OLED.setCursor(x2, y2);
  OLED.println(timer);
}

void popup_text(String text) {
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  if (LDRvalue > 900) {
    OLED.dim(true);
  } else {
    OLED.dim(false);
  }
  OLED.setCursor(10, 10);
  OLED.setTextSize(2);
  OLED.print(text);
  OLED.display();
}

void setup() {
  Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("All OLED Start Work !!!");
  }
  for (int i = 0; i < 4; i++) {
    pinMode(button[i], INPUT_PULLUP);
  }
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(interruptClock);
  // ==== WRITE EEPROM ====
  // EEPROM.write(20, clock.hour);
  // EEPROM.write(21, clock.min);

  // ==== READ EEPROM ====
  clock.hour = EEPROM.read(20);
  clock.min = EEPROM.read(21);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {


  time_text();

  LDRvalue = analogRead(LDR_PIN);

  OLED.clearDisplay();
  OLED.setTextColor(WHITE);

  if (LDRvalue > 900) {
    OLED.dim(true);
  } else {
    OLED.dim(false);
  }

  if (debounce(0)) {
    if (!digitalRead(button[0])) {
      switch (MODE) {
        case CLOCK_MODE:
          change_hour();
          break;
        case COUNTER_UP_MODE:
          onCountUpPause = onCountUpPause == 1 ? 0 : 1;
          break;
        case ALARM_MODE:
          change_hour_alarm();
          break;
        case COUNTER_DOWN_MODE:
          if (!onStartCountDown) {
            change_min_countdown();
          }
          break;
      }
    }
  }

  if (debounce(1)) {
    if (!digitalRead(button[1])) {
      switch (MODE) {
        case CLOCK_MODE:
          change_min();
          break;
        case COUNTER_UP_MODE:
          onCountUpPause = 1;
          resetCountUpTime();
          break;
        case ALARM_MODE:
          change_min_alarm();
          break;
        case COUNTER_DOWN_MODE:
          if (!onStartCountDown) {
            change_sec_countdown();
          } else if (onStartCountDown) {
            resetcountDownTime();
            onStartCountDown = 0;
            onCountDown = 1;
          }
          break;
      }
    }
  }

  if (debounce(2)) {
    if (!digitalRead(button[2])) {
      change_mode();
    }
  }

  if (debounce(3)) {
    if (!digitalRead(button[3])) {
      switch (MODE) {
        case ALARM_MODE:
          onAlarm = onAlarm == 0 ? 1 : 0;
          if (onAlarm) {
            alarmSave.hour = alarm.hour;
            alarmSave.min = alarm.min;
          }
          break;
        case COUNTER_DOWN_MODE:
          if (!(countDown.min == 0 && countDown.sec == 0)) {
            onCountDown = onCountDown == 1 ? 0 : 1;
            onStartCountDown = 1;
          }
          break;
      }
    }
  }




  if (onAlarm && alarmSave.hour == clock.hour && alarmSave.min == clock.min && clock.sec == 0) {
    while (alarmCheck) {
      tone(BUZZER_PIN, 440, 100);
      popup_text("WAKE UP!!");
      if (debounce(0)) {
        if (!digitalRead(button[0])) {
          OLED.clearDisplay();
          onAlarm = 0;
          break;
        }
      }
    }
  }

  if (finishCountDown) {
    while (countDownCheck) {
      tone(BUZZER_PIN, 440, 100);
      popup_text("FINISH!!");
      if (debounce(0)) {
        if (!digitalRead(button[0])) {
          OLED.clearDisplay();
          finishCountDown = 0;
          onStartCountDown = 0;
          break;
        }
      }
    }
  }

  if (get_x() > 360) {
    OLED.setRotation(2);
  } else {
    OLED.setRotation(0);
  }

  switch (MODE) {
    case CLOCK_MODE:
      display_text(10, 0, "Clock", 10, 12, timerText, 1, 2);
      break;
    case COUNTER_UP_MODE:
      display_text(10, 0, "Stopwatch", 10, 12, countUpText, 1, 2);
      break;
    case ALARM_MODE:
      display_text(10, 0, "Alarm", 10, 9, alarmText, 1, 2);
      display_text(10, 25, saveAlarmText, 80, 0, onAlarm == 0 ? "OFF" : "ON", 1, 1);
      break;
    case COUNTER_DOWN_MODE:
      display_text(10, 0, "Timer", 10, 12, countDownText, 1, 2);
      break;
  }

  OLED.display();
}