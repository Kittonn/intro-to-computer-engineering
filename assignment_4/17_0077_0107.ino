#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C

#define BTN_1 2
#define BTN_2 3
#define BTN_3 4
#define BTN_4 5
#define BUZZER_PIN 8

int button[4] = {BTN_1, BTN_2, BTN_3, BTN_4};
int lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
int buttonState[4];
int debounceDelay = 50;
unsigned long long int lastDebounceTime[4];
int reading[4];
int randomNumber[2];

int bar_pos[2][2] = {{127, 0}, {0, 0}};
int ball_pos[2] = {31, 31};
int ball_direction[2] = {1, 1};
int start_menu = 1, score_menu = 0, gameover_menu = 0, gameplay_menu = 0;
int score[2] = {0, 0};
unsigned long long int lastTimeScore;
unsigned long long int lastTimeGameOver;

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int debounce(int i)
{
  int isChange = 0;
  reading[i] = digitalRead(button[i]);
  if (reading[i] != lastButtonState[i])
  {
    lastDebounceTime[i] = millis();
  }
  if ((millis() - lastDebounceTime[i]) > debounceDelay)
  {
    if (reading[i] != buttonState[i])
    {
      buttonState[i] = reading[i];
      isChange = 1;
    }
  }
  lastButtonState[i] = reading[i];
  return isChange;
}

void welcome()
{
  OLED.setTextColor(WHITE);
  OLED.setTextSize(1);
  OLED.setCursor(1, 1);
  OLED.println("PONG!!!");
  OLED.setCursor(1, 13);
  OLED.println("Pressed to start.");
}

void move_ball(int x, int y)
{
  OLED.drawPixel(x, y, WHITE);
}

void move_bar(int x, int y)
{
  OLED.drawFastVLine(x, y, 13, WHITE);
}

void show_score()
{
  OLED.setTextColor(WHITE);
  OLED.setTextSize(1);
  OLED.setCursor(32, 16);
  OLED.println(score[1]);
  OLED.setCursor(96, 16);
  OLED.println(score[0]);
}

void show_gameover()
{
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setTextSize(1);
  OLED.setCursor(1, 1);
  OLED.println("Game over!!!!");
  OLED.setCursor(1, 13);
  if (score[0] > score[1])
  {
    OLED.println("Player 1 win!!!");
  }
  else if (score[1] > score[0])
  {
    OLED.println("Player 2 win!!!");
  }
}

void gameplay()
{
  if (debounce(0))
  {
    if (buttonState[0] == LOW && bar_pos[0][1] != 0)
    {
      bar_pos[0][1] -= 5;
    }
  }
  if (debounce(1))
  {
    if (buttonState[1] == LOW && bar_pos[0][1] != 20)
    {
      bar_pos[0][1] += 5;
    }
  }
  if (debounce(2))
  {
    if (buttonState[2] == LOW && bar_pos[1][1] != 0)
    {
      bar_pos[1][1] -= 5;
    }
  }
  if (debounce(3))
  {
    if (buttonState[3] == LOW && bar_pos[1][1] != 20)
    {
      bar_pos[1][1] += 5;
    }
  }
  if (ball_pos[1] == 31)
  {
    ball_direction[1] = -1;
  }
  if (ball_pos[0] == 0)
  {
    if (ball_pos[1] >= bar_pos[1][1] && ball_pos[1] <= bar_pos[1][1] + 13)
    {
      ball_direction[0] = 1;
    }
    else
    {
      tone(BUZZER_PIN, 440, 100);
      Serial.println("player 1 get score");
      score[0] += 1;
      score_menu = 1;
      lastTimeScore = millis();
    }
  }
  if (ball_pos[1] == 0)
  {
    ball_direction[1] = 1;
  }
  if (ball_pos[0] == 127)
  {
    if (ball_pos[1] >= bar_pos[0][1] && ball_pos[1] <= bar_pos[0][1] + 13)
    {
      ball_direction[0] = -1;
    }
    else
    {
      tone(BUZZER_PIN, 440, 100);
      Serial.println("player 2 get score");
      score[1] += 1;
      score_menu = 1;
      lastTimeScore = millis();
    }
  }

  ball_pos[0] += ball_direction[0];
  ball_pos[1] += ball_direction[1];

  move_ball(ball_pos[0], ball_pos[1]);
  move_bar(bar_pos[1][0], bar_pos[1][1]);
  move_bar(bar_pos[0][0], bar_pos[0][1]);
}

void setup()
{
  Serial.begin(9600);
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_4, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);

  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 allocation failed");
  }
  else
  {
    Serial.println("All OLED Start Work !!!");
  }
  randomSeed(analogRead(A0));
}

void loop()
{
  OLED.clearDisplay();
  if (start_menu && gameover_menu == 0)
  {
    welcome();
    for (int i = 0; i < 4; i++)
    {
      if (debounce(i))
      {
        if (buttonState[i] == LOW)
        {
          start_menu = 0;
        }
      }
    }
  }
  else
  {
    gameplay_menu = 1;
  }
  if (gameplay_menu && gameover_menu == 0 && start_menu == 0)
  {
    gameplay();
  }
  if (score_menu && gameover_menu == 0)
  {
    show_score();
  }
  if ((millis() - lastTimeScore) > 2000 && score_menu == 1 && gameover_menu == 0)
  {
    score_menu = 0;
    ball_pos[0] = random(50, 80);
    ball_pos[1] = random(12, 25);
  }
  if ((score[0] == 5 || score[1] == 5) & gameover_menu == 0)
  {
    gameover_menu = 1;
    score_menu = 0;
    gameplay_menu = 0;
    lastTimeGameOver = millis();
  }
  if (gameover_menu)
  {
    show_gameover();
    for (int i = 0; i < 4; i++)
    {
      if (debounce(i))
      {
        if (buttonState[i] == LOW && (millis() - lastTimeGameOver) > 2000)
        {
          start_menu = 1;
          gameover_menu = 0;
          score[0] = 0;
          score[1] = 0;
          ball_pos[0] = random(50, 80);
          ball_pos[1] = random(12, 25);
        }
      }
    }
  }

  OLED.display();
}