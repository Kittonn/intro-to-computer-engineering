int num_array[10][7] = {{0, 1, 1, 0, 0, 0, 0},  // 1
                        {1, 1, 0, 1, 1, 0, 1},  // 2
                        {1, 1, 1, 1, 0, 0, 1},  // 3
                        {0, 1, 1, 0, 0, 1, 1},  // 4
                        {1, 0, 1, 1, 0, 1, 1},  // 5
                        {1, 0, 1, 1, 1, 1, 1},  // 6
                        {1, 1, 1, 0, 0, 0, 0},  // 7
                        {1, 1, 1, 1, 1, 1, 1},  // 8
                        {1, 1, 1, 1, 0, 1, 1}}; // 9

int check_array[3][7] = {{1, 1, 1, 1, 1, 1, 0},  // 0
                         {1, 0, 1, 1, 1, 1, 0},  // G
                         {0, 0, 0, 1, 1, 1, 0}}; // L

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
    digitalWrite(pin, num_array[num][i]);
    pin++;
  }
}

void check(int num)
{
  int pin = 2;
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(pin, check_array[num][i]);
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
      if (currentNumber == 10)
      {
        currentNumber = 1;
      }
      number(currentNumber - 1);
      Serial.println(currentNumber);
    }
  }

  if (debounce(1))
  {
    if (!buttonState[1])
    {
      if (currentNumber == randomNumber)
      {
        check(0);
        Serial.println("Equal");
        randomNumber = random(1, 10);
        Serial.println(randomNumber);
      }
      else if (currentNumber < randomNumber)
      {
        check(2);
        Serial.println("Less than");
      }
      else if (currentNumber > randomNumber)
      {
        check(1);
        Serial.println("More than");
      }
    }
  }
}