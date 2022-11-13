void ShowBigClock()
{
  lcd.init();
  lcd.clear();
  while (true)
  {
    alarm(); // detect alarm
    LDR_Sensor();
    if (shouldShowInfo())
    {
      ShowInfo();
    }
    else
    {
      ShowDateInfo();
    }
    BigClock();
    enter();

    if (KE == 1) // Show other info sub screen. Temp & voltage of batt
    {
      KE = 0;
      OtherInfo();
      break;
    }
    if (KD == 1) // Power off LED 1 or LED 2 alarm and change page
    {
      KD = 0;
      page = 1;
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      break;
    }
    if (KB == 1) // Show other info sub screen. Temp & voltage of batt
    {
      KB = 0;
      HourFormat12(); //Convert to 12h, only for show on bigclock
      h12Flag = true;
      break;
    }
    if (KC == 1) // Return to 24H mode
    {
      KC = 0;
      hour = reloj.getHour(h12Flag, pmFlag); // Get hour 24h format
      break;
    }
  }
}

void BigClock()
{
  // hour, minute, and second
  if (!h12Flag)
  {
    hour = reloj.getHour(h12Flag, pmFlag);
  }
  h1 = hour / 10;
  h2 = hour % 10;
  minute = reloj.getMinute();
  min1 = minute / 10;
  min2 = minute % 10;
  second = reloj.getSecond();
  s1 = second / 10;
  s2 = second % 10;

  // Change info every mode time. Mode = 2 to 5 mints

  if (mode == 2)
  {
    IntervalInfo = 120000;
    if (millis() >= now + IntervalInfo)
    {
      now = millis();
      ShowInfo();
    }
  }
  else if (mode == 3)
  {
    IntervalInfo = 180000;
    if (millis() >= now + IntervalInfo)
    {
      now = millis();
      ShowInfo();
    }
  }
  else if (mode == 4)
  {
    IntervalInfo = 240000;
    if (millis() >= now + IntervalInfo)
    {
      now = millis();
      ShowInfo();
    }
  }
  else if (mode == 5)
  {
    IntervalInfo = 300000;
    if (millis() >= now + IntervalInfo)
    {
      now = millis();
      ShowInfo();
    }
  }

  // O'clock sound
  if (minute == 0 && page == 0 && second == 0)
  {
    tone(buzzer, 783, 100);
    delay(60);
    digitalWrite(LED2, HIGH);
    delay(60);
    digitalWrite(LED2, LOW);
  }

  // Print LCD
  if (second != osec)
  {
    printNum3(h1, 0, 1);
    printNum3(h2, 3, 1);
    printColon(6, 1);
    printNum3(min1, 7, 1);
    printNum3(min2, 10, 1);
    printColon(13, 1);
    printNum4(s1, 14);
    printNum4(s2, 17);
    osec = second;
  }
  delay(50);
}

bool shouldShowInfo()
{
  if (millis() - startCollecting >= 60000)
  {
    isClockInfoShown = false;
    return isClockInfoShown;
  }

  if (isClockInfoShown)
  {
    return true;
  }
  else
  {

    if (mode == 2)
    {
      IntervalInfo = 120000;
      if (millis() >= now + IntervalInfo)
      {
        now = millis();
        return true;
      }
    }
    else if (mode == 3)
    {
      IntervalInfo = 180000;
      if (millis() >= now + IntervalInfo)
      {
        now = millis();
        return true;
      }
    }
    else if (mode == 4)
    {
      IntervalInfo = 240000;
      if (millis() >= now + IntervalInfo)
      {
        now = millis();
        return true;
      }
    }
    else if (mode == 5)
    {
      IntervalInfo = 300000;
      if (millis() >= now + IntervalInfo)
      {
        now = millis();
        return true;
      }
    }
    return false;
  }
}

void OtherInfo()
{
  // lcd.init();
  lcd.clear();
  while (true)
  {
    alarm(); // detect alarm
    LDR_Sensor();
    enter();

    // Display the temperature
    temp = reloj.getTemperature();
    convertemp = temp * 100;
    t1 = (convertemp / 1000) % 10;
    t2 = (convertemp / 100) % 10;
    t3 = (convertemp / 10) % 10;
    // t4 = (convertemp/1) %10;

    // Print temperature
    printNum3(t1, 0, 1);
    printNum3(t2, 3, 1);
    lcd.setCursor(6, 3);
    lcd.write(pgm_read_byte(&dot[0][0]));
    printNum3(t3, 7, 1);
    lcd.setCursor(0, 0);
    lcd.print("Temp ");
    lcd.print(char(223)); // Degree ASCII
    lcd.print(char(67));  // C capital ASCII

    // Battery
    // Empty Batt
    printNum4(0, 17);

    if (KE == 1) // Back to BigClock
    {
      KE = 0;
      page = 0;
      break;
    }
    if (KD == 1) // Power off LED 1 or LED 2 alarm and change page
    {
      KD = 0;
      page = 1;
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      break;
    }
  }
}

void ShowInfo()
{
  if (!isClockInfoShown)
  {
    startCollecting = millis();
    isClockInfoShown = true;
  }
  lcd.setCursor(0, 0);
  lcd.print("             "); // Clean info

  // Display the temperature
  if (reloj.getTemperature() < 10)
  {
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.setCursor(0, 0);
    lcd.print(reloj.getTemperature(), 1);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print(reloj.getTemperature(), 1);
  }
  lcd.setCursor(4, 0);
  lcd.print(char(223)); // Degree ASCII
  lcd.print(char(67));  // C capital ASCII

  //  Show alarm status.
  lcd.setCursor(8, 0);
  lcd.print("A1");
  lcd.setCursor(11, 0);
  lcd.print("A2");

  if (reloj.checkAlarmEnabled(1))
  {
    lcd.setCursor(7, 0);
    lcd.print(char(126));
  }
  if (reloj.checkAlarmEnabled(2))
  {
    lcd.setCursor(10, 0);
    lcd.print(char(126));
  }
}

void ShowDateInfo()
{
  lcd.setCursor(0, 0);
  lcd.print("             "); // Clean info

  // Data Show
  // and the day of the week
  week = reloj.getDoW();
  lcd.setCursor(0, 0);
  lcd.print(WD_arr[week - 1]);

  // first the date
  date = reloj.getDate();
  lcd.setCursor(4, 0);
  lcd.print(date);

  // then the month
  month = reloj.getMonth(century);
  lcd.setCursor(7, 0);
  lcd.print(M_arr[month - 1]); // Minus one, arrays begins in 0

  if (h12Flag)
  {
    if (pmFlag)
    {
      lcd.setCursor(11, 0);
      lcd.print("PM");
    }
    else
    {
      lcd.setCursor(11, 0);
      lcd.print("AM");
    }
  }
  else
  {
    // dash
    lcd.setCursor(10, 0);
    lcd.print("-");
    // Year
    year = reloj.getYear();
    lcd.setCursor(11, 0);
    lcd.print(year);
  }
}

void HourFormat12()
{

  hour = reloj.getHour(h12Flag, pmFlag);
  if (hour == 0)
  {
    hour = 12; // 12 Midnight
  }
  else if (hour > 12)
  {
    hour = hour - 12;
  }
}
