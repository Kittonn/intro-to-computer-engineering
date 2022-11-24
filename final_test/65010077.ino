int LED[5] = {9, 10, 11, 12, 13};
int BTN[5] = {2, 3, 4, 5, 6};

bool reading[5];
bool lastButtonState[5] = {LOW, LOW, LOW, LOW, LOW};
bool buttonState[5] = {LOW, LOW, LOW, LOW, LOW};
unsigned long long int lastDebounceTime[5];
int debounceDelay = 50;

bool ledState[5] = {LOW, LOW, LOW, LOW, LOW};
int num = 0;
bool mem[4] = {0, 0, 0, 0};
int step = 0;
bool copy[4] = {0, 0, 0, 0};

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 5; i++)
  {
    pinMode(LED[i], OUTPUT);
    pinMode(BTN[i], INPUT_PULLUP);
  }
}

bool debounce(int i)
{
  bool isChange = 0;
  reading[i] = !digitalRead(BTN[i]);
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

void resetLED()
{
  for (int i = 0; i < 5; i++)
  {
    ledState[i] = LOW;
    digitalWrite(LED[i], ledState[i]);
  }
}

void resetMem()
{
  for (int i = 0; i < 4; i++)
  {
    mem[i] = 0;
  }
}

void loop()
{
  for (int i = 0; i < 3; i++)
  {
    if (debounce(i))
    {
      if (buttonState[i] == HIGH)
      {
        ledState[i + 1] = !ledState[i + 1];
        digitalWrite(LED[i + 1], ledState[i + 1]);
      }
    }
  }

  if (debounce(3))
  {
    if (buttonState[3] == HIGH)
    {
      if (step == 0)
      {
        mem[1] = ledState[1];
        mem[2] = ledState[2];
        mem[3] = ledState[3];
        step++;
        Serial.print("Mem : ");
        Serial.print(mem[0]);
        Serial.print(mem[1]);
        Serial.print(mem[2]);
        Serial.println(mem[3]);
        resetLED();
      }
      else if (step == 1)
      {
        for (int i = 0; i < 3; i++)
        {
          num *= 2;
          num += ledState[i + 1];
        }
        Serial.print("Num : ");
        Serial.println(num);
        step++;
        resetLED();
      }
      else if (step == 2)
      {

        for (int i = 0; i < num; i++)
        {
          for (int i = 0; i < 4; i++)
          {
            copy[i] = mem[i];
          }
          mem[0] = copy[1];
          mem[1] = copy[2];
          mem[2] = copy[3];
          mem[3] = copy[0];
          Serial.print("Mem : ");
          Serial.print(mem[0]);
          Serial.print(mem[1]);
          Serial.print(mem[2]);
          Serial.println(mem[3]);
        }
        for (int i = 0; i < 4; i++)
        {
          ledState[i] = mem[i];
          digitalWrite(LED[i], ledState[i]);
        }
        step++;
      }
      else if (step == 3)
      {
        resetMem();
        resetLED();
        step = 0;
        num = 0;
      }
    }
  }
}
