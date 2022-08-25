#define PIN_13 13
#define PIN_12 12
#define PIN_11 11
#define PIN_10 10

void setup()
{
  pinMode(PIN_10, OUTPUT);
  pinMode(PIN_11, OUTPUT);
  pinMode(PIN_12, OUTPUT);
  pinMode(PIN_13, OUTPUT);
}

void loop()
{
  digitalWrite(PIN_10, LOW);
  digitalWrite(PIN_11, LOW);
  digitalWrite(PIN_12, LOW);
  digitalWrite(PIN_13, LOW);

  delay(500);

  digitalWrite(PIN_10, HIGH);
  digitalWrite(PIN_11, LOW);
  digitalWrite(PIN_12, LOW);
  digitalWrite(PIN_13, LOW);

  delay(500);

  digitalWrite(PIN_10, HIGH);
  digitalWrite(PIN_11, HIGH);
  digitalWrite(PIN_12, LOW);
  digitalWrite(PIN_13, LOW);

  delay(500);

  digitalWrite(PIN_10, HIGH);
  digitalWrite(PIN_11, HIGH);
  digitalWrite(PIN_12, HIGH);
  digitalWrite(PIN_13, LOW);

  delay(500);

  digitalWrite(PIN_10, HIGH);
  digitalWrite(PIN_11, HIGH);
  digitalWrite(PIN_12, HIGH);
  digitalWrite(PIN_13, HIGH);

  delay(500);
}