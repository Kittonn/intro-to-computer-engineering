#include <Wire.h>
#define BOARD1 1
#define BOARD2 2
#define BOARD3 3
#define BOARD4 4
int debug = 1;
int currentBoard = BOARD2;
int nextBoard = BOARD1;
int numBoard = 3;
char receiveMessage[64] = "\0";
String serialInput;
String message;
String token;
int i = 0;
long time = 0;
bool haveReceived = 0;
int n = 0;
int checkMessage = 0;
int state = 0;

void setup()
{
  Wire.begin(currentBoard);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

bool cooldown()
{
  if (millis() - time > 200)
  {
    haveReceived = 0;
    return 1;
  }
  return 0;
}

void createToken()
{
  if (checkMessage == 2 && state == 0)
  {
    token = "Token#1#";
    token += n;
    token += "#";
    token += message;
    n = 0;
    checkMessage = 0;
    message = "";
  }
  else if (state == 1)
  {
    token = receiveMessage;
  }
  else
  {
    token = "Token#0#";
  }
  if (debug)
  {
    Serial.print("Send Token : ");
    Serial.println(token);
  }
}

void sendToken()
{
  Wire.beginTransmission(nextBoard);
  createToken();
  Wire.write(&token[0]);
  Wire.endTransmission();
}

void printMessage()
{
  for (int j = 10; j < i; ++j)
  {
    Serial.print(receiveMessage[j]);
  }
}

void receiveEvent(int lenByte)
{

  for (i = 0; i < lenByte; i++)
  {
    receiveMessage[i] = Wire.read();
  }
  receiveMessage[i] = '\0';
  if (debug)
  {
    Serial.print("Receive : ");
    Serial.println(receiveMessage);
  }
  state = receiveMessage[6] - '0';
  if (receiveMessage[8] - '0' == currentBoard)
  {
    state = 0;
    printMessage();
  }
  time = millis();
  haveReceived = 1;
}

void loop()
{
  // delay(100);
  while (Serial.available())
  {
    serialInput = Serial.readString();

    if (checkMessage == 0)
    {
      n = serialInput.toInt();
      if (n > numBoard || n <= 0)
      {
        Serial.println("Invalid Input Board");
        Serial.println("Available Board : 1 2 3");
        n = 0;
      }
      else
      {
        checkMessage = 1;
      }
    }
    else if (checkMessage == 1)
    {
      message = serialInput;
      checkMessage = 2;

      Serial.print("Create Token : Token#1#");
      Serial.print(n);
      Serial.print("#");
      Serial.println(message);
    }
    else
    {
      Serial.println("Can't queue more than 1 message");
    }
  }
  if (haveReceived)
  {
    if (cooldown())
    {
      sendToken();
    }
  }
}