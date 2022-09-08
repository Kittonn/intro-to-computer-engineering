int new_num_array[10][7] = {{1, 0, 0, 1, 1, 1, 1}, // 0
                            {0, 0, 1, 0, 0, 1, 0}, // 21
                            {0, 0, 0, 0, 1, 1, 0}, // 3
                            {1, 0, 0, 1, 1, 0, 0}, // 4
                            {0, 1, 0, 0, 1, 0, 0}, // 5
                            {0, 1, 0, 0, 0, 0, 0}, // 6
                            {0, 0, 0, 1, 1, 1, 1}, // 7
                            {0, 0, 0, 0, 0, 0, 0}, // 8
                            {0, 0, 0, 0, 1, 0, 0},
                            {1, 1, 1, 1, 1, 1, 1}}; // 9

int new_check_array[3][7] = {{0, 0, 0, 0, 0, 0, 1},  // 1
                             {0, 1, 0, 0, 0, 0, 1},  // G
                             {1, 1, 1, 0, 0, 0, 1}}; // L

int currentNumber = 0;
int button[2] = {13, 12};
int randomNumber;

int reading[2];
int lastButtonState[2] = {HIGH, HIGH};
int buttonState[2];

unsigned long long int debounceDelay = 50;
unsigned long long int lastDebounceTime[2];

void setup()
{
  for (int i = 0; i < 2; i++)
  {
    pinMode(button[i], INPUT_PULLUP);
  }
  for (int i = 2; i < 9; i++)
  {
    pinMode(i, OUTPUT);
  }

  randomSeed(analogRead(A0));
  randomNumber = random(1, 10);
  Serial.begin(9600);
  Serial.println(randomNumber);
  number(currentNumber);
  Serial.println(currentNumber);
}

int debounce(int i)
{
  int isChange = 0;
  reading[i] = digitalRead(button[i]);
  if (reading[i] != lastButtonState[i])
  {
    lastDebounceTime[i] = millis();
  }
  if ((millis() - lastButtonState[i]) > debounceDelay)
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

void number(int num)
{
  int pin = 2;
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(pin, (new_num_array[num][i]));
    pin++;
  }
}

void check(int num)
{
  int pin = 2;
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(pin, (new_check_array[num][i]));
    pin++;
  }
}

void loop()
{

  if (debounce(0))
  {

    if (!buttonState[0])
    {
      currentNumber++;
      if (currentNumber == 9)
      {
        currentNumber = 0;
      }
      delay(500);
      Serial.println(currentNumber);
      number(currentNumber);
    }
  }

  if (debounce(1))
  {
    if (!buttonState[1])
    {
      if (currentNumber + 1 == randomNumber)
      {
        delay(500);
        check(0);
        Serial.println("Equal");
        randomNumber = random(1, 10);
        Serial.println(randomNumber);
      }
      else if (currentNumber + 1 < randomNumber)
      {
        delay(500);
        check(2);
        Serial.println("Less than");
      }
      else if (currentNumber + 1 > randomNumber)
      {
        delay(500);
        check(1);
        Serial.println("More than");
      }
    }
  }
}