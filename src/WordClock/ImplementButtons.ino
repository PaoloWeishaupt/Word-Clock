/*
Implement buttons
Gabriele Alessi, Paolo Claudio Weishaupt
*/

// Buttons pins
int hourButton = 8;
int minuteButton = 11;
int confirmButton = 12;
int modeButton = 13;
// Buttons states
int hourButtonState = 0;
int lastHourButtonState = 0;
int minuteButtonState = 0;
int lastMinuteButtonState = 0;
int confirmButtonState = 0;
int lastConfirmButtonState = 0;
int modeButtonState = 0;
int lastModeButtonState = 0;
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
  // Buttons pins
  pinMode(hourButton, INPUT);
  pinMode(minuteButton, INPUT);
  pinMode(confirmButton, INPUT);
  pinMode(modeButton, INPUT);
}

void loop()
{
  waitMode(5000);
  if(modeButtonState){
    // Wait a button click for the increment
    waitIncrement();
    // Wait the confirm
    waitConfirm(5000);
  }
}

void waitMode(long waitTime){
  modeButtonState = digitalRead(modeButton);
  lastModeButtonState = modeButtonState;
  Serial << F("Change mode");
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
  Serial << F("Confirm");
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
  Serial << F("Increment hour");
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
  Serial << F("Increment minute");
}
