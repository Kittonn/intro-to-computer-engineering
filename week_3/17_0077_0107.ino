#define PIN_11 11
#define PIN_12 12
#define PIN_13 13

void setup()
{
}

void loop()
{
  // LED 1
  pinMode(PIN_12, OUTPUT);
  digitalWrite(PIN_12, HIGH);
  pinMode(PIN_13, OUTPUT);
  digitalWrite(PIN_13, LOW);
  pinMode(PIN_11, INPUT);

  delay(1000);

  // LED 2
  pinMode(PIN_12, OUTPUT);
  digitalWrite(PIN_12, LOW);
  pinMode(PIN_13, OUTPUT);
  digitalWrite(PIN_13, HIGH);
  pinMode(PIN_11, INPUT);

  delay(1000);

  // LED 3
  pinMode(PIN_11, OUTPUT);
  digitalWrite(PIN_11, HIGH);
  pinMode(PIN_12, OUTPUT);
  digitalWrite(PIN_12, LOW);
  pinMode(PIN_13, INPUT);

  delay(1000);

  // LED 4
  pinMode(PIN_11, OUTPUT);
  digitalWrite(PIN_11, LOW);
  pinMode(PIN_12, OUTPUT);
  digitalWrite(PIN_12, HIGH);
  pinMode(PIN_13, INPUT);

  delay(1000);

  // LED 5
  pinMode(PIN_11, OUTPUT);
  digitalWrite(PIN_11, HIGH);
  pinMode(PIN_13, OUTPUT);
  digitalWrite(PIN_13, LOW);
  pinMode(PIN_12, INPUT);

  delay(1000);

  // LED 6
  pinMode(PIN_11, OUTPUT);
  digitalWrite(PIN_11, LOW);
  pinMode(PIN_13, OUTPUT);
  digitalWrite(PIN_13, HIGH);
  pinMode(PIN_12, INPUT);

  delay(1000);
}