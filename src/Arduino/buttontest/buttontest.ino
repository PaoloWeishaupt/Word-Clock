int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;

int hourButton = 22;
int hourButtonState = 0;
int lastHourButtonState = 0;
int minuteButton = 23;
int minuteButtonState = 0;
int lastMinuteButtonState = 0;

int hourUnit = 0;
int hourTen = 0;
int minuteUnit = 0;
int minuteTen = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);

  pinMode(hourButton, INPUT);
  pinMode(minuteButton, INPUT);
}

void loop()
{
  setNum(1, hourTen);
  setNum(2, hourUnit);
  setNum(3, minuteTen);
  setNum(4, minuteUnit);
  waitIncrement();
}

void waitIncrement()
{
  incrementHour();
  incrementMinute();
}

void incrementHour()
{
  hourButtonState = isClicked(hourButton);
  if (hourButtonState != lastHourButtonState && hourButtonState)
  {
    if (hourUnit == 9)
    {
      hourUnit = 0;
      hourTen++;
    }
    else if (hourTen == 2 && hourUnit == 3)
    {
      hourUnit = 0;
      hourTen = 0;
    }
    else
    {
      hourUnit++;
    }
    delay(10);
  }
  lastHourButtonState = hourButtonState;
}

void incrementMinute()
{
  minuteButtonState = isClicked(minuteButton);
  if (minuteButtonState != lastMinuteButtonState && minuteButtonState)
  {
    if (minuteUnit == 9)
    {
      if (minuteTen == 5)
      {
        minuteTen = 0;
      }
      else
      {
        minuteTen++;
      }
      minuteUnit = 0;
    }
    else if (minuteTen == 5 && minuteUnit == 9)
    {
      minuteUnit = 0;
      minuteTen = 0;
    }
    else
    {
      minuteUnit++;
    }
    delay(10);
  }
  lastMinuteButtonState = minuteButtonState;
}

bool isClicked(int button)
{
  return digitalRead(button);
}

void setNum(int pos, int num)
{
  resetNum();
  resetPos();
  switch (pos)
  {
  case 1:
    pos1();
    break;
  case 2:
    pos2();
    break;
  case 3:
    pos3();
    break;
  case 4:
    pos4();
    break;
  }

  switch (num)
  {
  case 0:
    zero();
    break;
  case 1:
    one();
    break;
  case 2:
    two();
    break;
  case 3:
    three();
    break;
  case 4:
    four();
    break;
  case 5:
    five();
    break;
  case 6:
    six();
    break;
  case 7:
    seven();
    break;
  case 8:
    eight();
    break;
  case 9:
    nine();
    break;
  }
}

void resetNum()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void resetPos()
{
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
}

void pos1()
{
  digitalWrite(D1, LOW);
}

void pos2()
{
  digitalWrite(D2, LOW);
}

void pos3()
{
  digitalWrite(D3, LOW);
}

void pos4()
{
  digitalWrite(D4, LOW);
}

void zero()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
}

void one()
{
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
}

void two()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(g, HIGH);
}

void three()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(g, HIGH);
}

void four()
{
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five()
{
  digitalWrite(a, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six()
{
  digitalWrite(a, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
}

void eight()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}