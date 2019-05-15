// Segments pins
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
// Digits pins
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;
// Buttons pins
int hourButton = 22;
int minuteButton = 23;
int confirmButton = 24;
// Buttons states
int hourButtonState = 0;
int lastHourButtonState = 0;
int minuteButtonState = 0;
int lastMinuteButtonState = 0;
int confirmButtonState = 0;
int lastConfirmButtonState = 0;
// Numbers to display
int hourUnit = 0;
int hourTen = 0;
int minuteUnit = 0;
int minuteTen = 0;
// Hour and minute to set up
int confirmedHour = 0;
int confirmedMinute = 0;

void setup()
{
  Serial.begin(115200);
  // Segments pins to output
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  // Digits pins (displays) to output
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  // Buttons pins
  pinMode(hourButton, INPUT);
  pinMode(minuteButton, INPUT);
  pinMode(confirmButton, INPUT);
  // Turn off digit pins
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
}

void loop()
{
  // Set the display showing the hour and minute
  setDisplay(hourTen, hourUnit, minuteTen, minuteUnit);
  // Wait a button click for the increment
  waitIncrement();
  // Wait the confirm
  waitConfirm(5000);
}

void waitIncrement()
{
  incrementHour();
  incrementMinute();
}

void waitConfirm(long waitTime)
{
  confirmButtonState = digitalRead(confirmButton);
  if (confirmButtonState != lastConfirmButtonState && confirmButtonState)
  {
    // Set the time
    confirmedHour = hourTen * 10 + hourUnit;
    confirmedMinute = minuteTen * 10 + minuteUnit;
    Serial.println(String(confirmedHour) + ":" + String(confirmedMinute));
  }
  lastConfirmButtonState = confirmButtonState;
}

void incrementHour()
{
  hourButtonState = digitalRead(hourButton);
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
  }
  lastHourButtonState = hourButtonState;
}

void incrementMinute()
{
  minuteButtonState = digitalRead(minuteButton);
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
  }
  lastMinuteButtonState = minuteButtonState;
}

void setDisplay(int num1, int num2, int num3, int num4)
{
  setNum(1, num1);
  setNum(2, num2);
  setNum(3, num3);
  setNum(4, num4);
}

void setDisplay(int num)
{
  setNum(1, (num / 1000U) % 10);
  setNum(2, (num / 100U) % 10);
  setNum(3, (num / 10U) % 10);
  setNum(4, (num / 1U) % 10);
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