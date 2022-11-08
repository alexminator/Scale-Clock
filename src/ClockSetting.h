void showDatePage(void)
{
  lcd.clear();

  // Fixed text
  lcd.setCursor(3, 0);
  lcd.print("Clock Settings");
  lcd.setCursor(0, 1);
  lcd.print("Date:");
  lcd.setCursor(0, 2);
  lcd.print("Time:");
  lcd.setCursor(0, 3);
  lcd.print("Info:");
  lcd.setCursor(10, 3);
  lcd.print("Day:");
  lcd.setCursor(6, 1);
  lcd.print("2");
  lcd.setCursor(7, 1);
  if (century)
  {
    lcd.print("1");
  }
  else
  {
    lcd.print("0");
  }
  lcd.setCursor(6, 3);
  lcd.print(mode);

  while (true)
  {
    // alarm();    // Activate if you want detect alarm on setting
    showDate(); // show date
    enter();    // input
    LDR_Sensor();
    if (KA == 1) // time delayed, move cursor
    {
      KA = 0; // key flag, clear 0
      while (true)
      {
        enter(); // input
        if (KA == 1)
        {
          KA = 0;
          row_k += 1;
          if (row_k > 15)
          {
            row_k = 6;
            col_k += 1;
            if (col_k > 3)
            {
              col_k = 1;
            }
          }
        }
        lcd.setCursor(row_k, col_k);
        lcd.blink();

        if (KB == 1)
        {         // the value plus 1
          KB = 0; // key flag, clear 0
          BC_flag = false;
          if (col_k == 1)
          { // the second row
            changeTwo();
          }
          else if (col_k == 2)
          {
            changeThree(); // the third row
          }
          else
          {
            changeFour(); // the four row
          }
        }
        if (KC == 1)
        {         // the vlaue subtracts 1
          KC = 0; // key flag, clear 0
          BC_flag = true;
          if (col_k == 1)
          { // the second row
            changeTwo();
          }
          else if (col_k == 2)
          {
            changeThree(); // the third row
          }
          else
          {
            changeFour(); // the four row
          }
        }
        if (KF == 1)
        {
          KF = 0; // key flag, clear 0
          setTime();
          break;
        }
      }
      KD = 0; // D is invalid before exiting
    }
    if (KD == 1)
    {
      KD = 0;
      page = 0;
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      break;
    }
  }
}

void showDate()
{
  // Show Clock of settings.
  lcd.setCursor(8, 1);
  year = reloj.getYear();
  y1 = year / 10;
  y2 = year % 10;
  if (year < 10)
  {
    lcd.setCursor(8, 1);
    lcd.print("0");
    lcd.setCursor(9, 1);
    lcd.print(year);
  }
  else
  {
    lcd.setCursor(8, 1);
    lcd.print(year);
  }

  lcd.setCursor(10, 1);
  lcd.print("-");

  // then the month
  month = reloj.getMonth(century);
  mon1 = month / 10;
  mon2 = month % 10;
  if (month < 10)
  {
    lcd.setCursor(11, 1);
    lcd.print("0");
    lcd.setCursor(12, 1);
    lcd.print(month);
  }
  else
  {
    lcd.setCursor(11, 1);
    lcd.print(month);
  }

  lcd.setCursor(13, 1);
  lcd.print("-");

  // then the date
  date = reloj.getDate();
  d1 = date / 10;
  d2 = date % 10;
  if (date < 10)
  {
    lcd.setCursor(14, 1);
    lcd.print("0");
    lcd.setCursor(15, 1);
    lcd.print(date);
  }
  else
  {
    lcd.setCursor(14, 1);
    lcd.print(date);
  }

  lcd.setCursor(16, 1);
  lcd.print(" ");

  // and the day of the week
  week = reloj.getDoW();
  lcd.setCursor(15, 3);
  lcd.print(week);

  // Finally the hour, minute, and second
  hour = reloj.getHour(h12Flag, pmFlag);

  h1 = hour / 10;
  h2 = hour % 10;
  if (hour < 10)
  {
    lcd.setCursor(6, 2);
    lcd.print("0");
    lcd.setCursor(7, 2);
    lcd.print(hour);
  }
  else
  {
    lcd.setCursor(6, 2);
    lcd.print(hour);
  }

  lcd.setCursor(8, 2);
  lcd.print(":");

  minute = reloj.getMinute();
  min1 = minute / 10;
  min2 = minute % 10;
  if (minute < 10)
  {
    lcd.setCursor(9, 2);
    lcd.print("0");
    lcd.setCursor(10, 2);
    lcd.print(minute);
  }
  else
  {
    lcd.setCursor(9, 2);
    lcd.print(minute);
  }

  lcd.setCursor(11, 2);
  lcd.print(":");

  second = reloj.getSecond();
  s1 = second / 10;
  s2 = second % 10;
  if (second < 10)
  {
    lcd.setCursor(12, 2);
    lcd.print("0");
    lcd.setCursor(13, 2);
    lcd.print(second);
  }
  else
  {
    lcd.setCursor(12, 2);
    lcd.print(second);
  }
  lcd.setCursor(14, 2);
  lcd.print(" ");

  // Add AM/PM indicator
  if (h12Flag)
  {
    if (pmFlag)
    {
      lcd.setCursor(15, 2);
      lcd.print("PM");
    }
    else
    {
      lcd.setCursor(15, 2);
      lcd.print("AM");
    }
  }
  else
  {
    lcd.setCursor(15, 2);
    lcd.print("24H");
  }
}

void changeTwo()
{ // modify year, month, day, week at the first row
  switch (row_k)
  { // select row
  case 8:
    if (!BC_flag)
    {
      y1 += 1;
    }
    else
    {
      y1 -= 1;
    }
    if (y1 > 9)
      y1 = 0;
    if (y1 < 0)
      y1 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(y1);
    year = y1 * 10 + y2;
    break;

  case 9:
    if (!BC_flag)
    {
      y2 += 1;
    }
    else
    {
      y2 -= 1;
    }
    if (y2 > 9)
      y2 = 0;
    if (y2 < 0)
      y2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(y2);
    year = y1 * 10 + y2;
    break;

  case 11:
    if (!BC_flag)
    {
      mon1 += 1;
    }
    else
    {
      mon1 -= 1;
    }
    if (mon1 > 1)
      mon1 = 0;
    if (mon1 < 0)
      mon1 = 1;
    lcd.setCursor(row_k, col_k);
    lcd.print(mon1);
    month = mon1 * 10 + mon2;
    break;

  case 12:
    if (!BC_flag)
    {
      mon2 += 1;
    }
    else
    {
      mon2 -= 1;
    }
    if (mon2 > 9)
      mon2 = 0;
    if (mon2 < 0)
      mon2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(mon2);
    month = mon1 * 10 + mon2;
    break;

  case 14:
    if (!BC_flag)
    {
      d1 += 1;
    }
    else
    {
      d1 -= 1;
    }
    if (d1 > 3)
      d1 = 0;
    if (d1 < 0)
      d1 = 3;
    lcd.setCursor(row_k, col_k);
    lcd.print(d1);
    date = d1 * 10 + d2;
    break;

  case 15:
    if (!BC_flag)
    {
      d2 += 1;
    }
    else
    {
      d2 -= 1;
    }
    if (d2 > 9)
      d2 = 0;
    if (d2 < 0)
      d2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(d2);
    date = d1 * 10 + d2;
    break;

  default:
    break;
  }
}

void changeThree()
{ // modify time
  switch (row_k)
  { // select column
  case 6:
    if (!BC_flag)
    {
      h1 += 1;
    }
    else
    {
      h1 -= 1;
    }
    if (h1 > 2)
      h1 = 0;
    if (h1 < 0)
      h1 = 2;
    lcd.setCursor(row_k, col_k);
    lcd.print(h1);
    hour = h1 * 10 + h2;
    break;

  case 7:
    if (!BC_flag)
    {
      h2 += 1;
    }
    else
    {
      h2 -= 1;
    }
    if (h2 > 9)
      h2 = 0;
    if (h2 < 0)
      h2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(h2);
    hour = h1 * 10 + h2;
    break;

  case 9:
    if (!BC_flag)
    {
      min1 += 1;
    }
    else
    {
      min1 -= 1;
    }
    if (min1 > 5)
      min1 = 0;
    if (min1 < 0)
      min1 = 5;
    lcd.setCursor(row_k, col_k);
    lcd.print(min1);
    minute = min1 * 10 + min2;
    break;

  case 10:
    if (!BC_flag)
    {
      min2 += 1;
    }
    else
    {
      min2 -= 1;
    }
    if (min2 > 9)
      min2 = 0;
    if (min2 < 0)
      min2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(min2);
    minute = min1 * 10 + min2;
    break;

  case 12:
    if (!BC_flag)
    {
      s1 += 1;
    }
    else
    {
      s1 -= 1;
    }
    if (s1 > 5)
      s1 = 0;
    if (s1 < 0)
      s1 = 5;
    lcd.setCursor(row_k, col_k);
    lcd.print(s1);
    second = s1 * 10 + s2;
    break;

  case 13:
    if (!BC_flag)
    {
      s2 += 1;
    }
    else
    {
      s2 -= 1;
    }
    if (s2 > 9)
      s2 = 0;
    if (s2 < 0)
      s2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(s2);
    second = s1 * 10 + s2;
    break;

  default:
    break;
  }
}

void changeFour()
{
  switch (row_k)
  {
  case 6:
    if (!BC_flag)
    {
      mode += 1;
    }
    else
    {
      mode -= 1;
    }

    if (mode > 5)
      mode = 2;
    if (mode < 2)
      mode = 5;
    lcd.setCursor(row_k, col_k);
    lcd.print(mode);
    break;

  case 15:
    if (!BC_flag)
    {
      week += 1;
    }
    else
    {
      week -= 1;
    }
    if (week > 7)
      week = 1;
    if (week < 1)
      week = 7;
    lcd.setCursor(row_k, col_k);
    lcd.print(week);
    break;

  default:
    break;
  }
}

void setTime()
{
  reloj.setSecond(second);  // set second
  reloj.setMinute(minute);  // set minute
  reloj.setHour(hour);      // set hour
  reloj.setDoW(week);       // set week
  reloj.setDate(date);      // set day
  reloj.setMonth(month);    // set month
  reloj.setYear(year);      // set year
  reloj.setClockMode(ampm); // set 12h(true) or 24 h(false)
}