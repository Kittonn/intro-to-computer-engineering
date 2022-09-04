#define LED_1 4 // RED
#define LED_2 3 // YELLOW
#define LED_3 2 // GREEN
#define BTN_1 13
#define BTN_2 12
#define BTN_3 11

int led[3] = {LED_1, LED_2, LED_3};
int button[3] = {BTN_1, BTN_2, BTN_3};
int ledState[3] = {LOW, LOW, LOW};
int lastButtonState[3] = {LOW, HIGH, HIGH};
int buttonState[3];
unsigned long long int debounceDelay = 50;
unsigned long long int lastDebounceTime[3];
int reading[3];

int ledTime[3] = {3000, 2000, 3000};
unsigned long long int lastTime[3] = {0, 0, 0};
unsigned long long int blinkYellow = 500;
unsigned long long int lastYellowTime = 0;
int isActive = 0;
void setup()
{
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(BTN_1, INPUT);        // PULL DOWN
  pinMode(BTN_2, INPUT);        // PULL UP
  pinMode(BTN_3, INPUT_PULLUP); // PULLUP BUILTIN
  Serial.begin(9600);
}

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

void loop()
{
  if (debounce(0))
  {
    if (buttonState[0] == HIGH)
    {
      ledState[0] = !ledState[0];
      digitalWrite(led[0], ledState[0]);
      lastTime[0] = millis();
    }
  }

  if (debounce(2))
  {
    if (buttonState[2] == LOW && !ledState[0])
    {
      ledState[2] = !ledState[2];
      digitalWrite(led[2], ledState[2]);
      lastTime[2] = millis();
    }
  }

  if (debounce(1))
  {
    if (buttonState[1] == LOW  && !ledState[0] && !ledState[2])
    { 
      lastYellowTime = millis();
      lastTime[1] = millis();
      isActive = 1;
    }
  }

  if ((millis() - lastYellowTime) >= blinkYellow && isActive && (millis() - lastTime[1]) < ledTime[1]) {
    lastYellowTime = millis();  
    ledState[1] = !ledState[1];
    digitalWrite(led[1],ledState[1]);
  }

  if ((millis() - lastTime[0]) > ledTime[0] && lastTime[0] != 0)
  {
    ledState[0] = LOW;
    lastButtonState[0] = LOW;
    digitalWrite(led[0], ledState[0]);
    lastTime[0] = 0;
  }
  if ((millis() - lastTime[2]) > ledTime[2] && lastTime[2] != 0)
  {
    ledState[2] = LOW;
    lastButtonState[2] = HIGH;
    digitalWrite(led[2], ledState[2]);
    lastTime[2] = 0;
  }
  if ((millis() - lastTime[1]) > ledTime[1] && lastTime[1] != 0)
  {
    ledState[1] = LOW;
    lastButtonState[1] = HIGH;
    digitalWrite(led[1], ledState[1]);
    lastTime[1] = 0;
  }
}