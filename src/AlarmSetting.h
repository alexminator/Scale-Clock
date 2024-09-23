void showAlarmStatus()
{
  // fix text
  lcd.setCursor(3, 0);
  lcd.print("Alarm Settings");

  lcd.setCursor(0, 1);
  lcd.print("Alarm 1:");
  if (reloj.checkAlarmEnabled(1))
  {
    lcd.setCursor(9, 1);
    lcd.print("Y");
  }
  else
  {
    lcd.setCursor(9, 1);
    lcd.print("N");
  }

  lcd.setCursor(0, 2);
  lcd.print("Alarm 2:");
  if (reloj.checkAlarmEnabled(2))
  {
    lcd.setCursor(9, 2);
    lcd.print("Y");
  }
  else
  {
    lcd.setCursor(9, 2);
    lcd.print("N");
  }
}

void showAlarm1()
{
  reloj.getA1Time(alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Set Alarm 1   ");

  alarm_h1 = alarmHour1 / 10;
  alarm_h2 = alarmHour1 % 10;
  if (alarmHour1 < 10)
  {
    lcd.setCursor(4, 1);
    lcd.print("0");
    lcd.setCursor(5, 1);
    lcd.print(alarmHour1);
  }
  else
  {
    lcd.setCursor(4, 1);
    lcd.print(alarmHour1);
  }

  lcd.setCursor(6, 1);
  lcd.print(":");

  alarm_m1 = alarmMinute1 / 10;
  alarm_m2 = alarmMinute1 % 10;
  if (alarmMinute1 < 10)
  {
    lcd.setCursor(7, 1);
    lcd.print("0");
    lcd.setCursor(8, 1);
    lcd.print(alarmMinute1);
  }
  else
  {
    lcd.setCursor(7, 1);
    lcd.print(alarmMinute1);
  }

  lcd.setCursor(9, 1);
  lcd.print(":");

  alarm_s1 = alarmSecond1 / 10;
  alarm_s2 = alarmSecond1 % 10;
  if (alarmSecond1 < 10)
  {
    lcd.setCursor(10, 1);
    lcd.print("0");
    lcd.setCursor(11, 1);
    lcd.print(alarmSecond1);
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.print(alarmSecond1);
  }
}

void showAlarm2()
{
  reloj.getA2Time(alarmDay, alarmHour2, alarmMinute2, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Set Alarm 2   ");

  alarm_hh1 = alarmHour2 / 10;
  alarm_hh2 = alarmHour2 % 10;
  if (alarmHour2 < 10)
  {
    lcd.setCursor(4, 1);
    lcd.print("0");
    lcd.setCursor(5, 1);
    lcd.print(alarmHour2);
  }
  else
  {
    lcd.setCursor(4, 1);
    lcd.print(alarmHour2);
  }

  lcd.setCursor(6, 1);
  lcd.print(":");

  alarm_mm1 = alarmMinute2 / 10;
  alarm_mm2 = alarmMinute2 % 10;
  if (alarmMinute2 < 10)
  {
    lcd.setCursor(7, 1);
    lcd.print("0");
    lcd.setCursor(8, 1);
    lcd.print(alarmMinute2);
  }
  else
  {
    lcd.setCursor(7, 1);
    lcd.print(alarmMinute2);
  }
}

void changeAlarmOne()
{
  switch (row_k)
  {
  case 4:
    if (!BC_flag)
    {
      alarm_h1 += 1;
    }
    else
    {
      alarm_h1 -= 1;
    }
    if (alarm_h1 > 2)
      alarm_h1 = 0;
    if (alarm_h1 < 0)
      alarm_h1 = 2;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_h1);
    alarmHour1 = alarm_h1 * 10 + alarm_h2;
    break;

  case 5:
    if (!BC_flag)
    {
      alarm_h2 += 1;
    }
    else
    {
      alarm_h2 -= 1;
    }
    if (alarm_h2 > 9)
      alarm_h2 = 0;
    if (alarm_h2 < 0)
      alarm_h2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_h2);
    alarmHour1 = alarm_h1 * 10 + alarm_h2;
    break;

  case 7:
    if (!BC_flag)
    {
      alarm_m1 += 1;
    }
    else
    {
      alarm_m1 -= 1;
    }
    if (alarm_m1 > 5)
      alarm_m1 = 0;
    if (alarm_m1 < 0)
      alarm_m1 = 5;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_m1);
    alarmMinute1 = alarm_m1 * 10 + alarm_m2;
    break;

  case 8:
    if (!BC_flag)
    {
      alarm_m2 += 1;
    }
    else
    {
      alarm_m2 -= 1;
    }
    if (alarm_m2 > 9)
      alarm_m2 = 0;
    if (alarm_m2 < 0)
      alarm_m2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_m2);
    alarmMinute1 = alarm_m1 * 10 + alarm_m2;
    break;

  case 10:
    if (!BC_flag)
    {
      alarm_s1 += 1;
    }
    else
    {
      alarm_s1 -= 1;
    }
    if (alarm_s1 > 5)
      alarm_s1 = 0;
    if (alarm_s1 < 0)
      alarm_s1 = 5;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_s1);
    alarmSecond1 = alarm_s1 * 10 + alarm_s2;
    break;

  case 11:
    if (!BC_flag)
    {
      alarm_s2 += 1;
    }
    else
    {
      alarm_s2 -= 1;
    }
    if (alarm_s2 > 9)
      alarm_s2 = 0;
    if (alarm_s2 < 0)
      alarm_s2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_s2);
    alarmSecond1 = alarm_s1 * 10 + alarm_s2;
    break;

  default:
    break;
  }
  reloj.setA1Time(alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
}

void changeAlarmTwo()
{
  switch (row_k)
  {
  case 4:
    if (!BC_flag)
    {
      alarm_hh1 += 1;
    }
    else
    {
      alarm_hh1 -= 1;
    }
    if (alarm_hh1 > 2)
      alarm_hh1 = 0;
    if (alarm_hh1 < 0)
      alarm_hh1 = 2;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_hh1);
    alarmHour2 = alarm_hh1 * 10 + alarm_hh2;
    break;

  case 5:
    if (!BC_flag)
    {
      alarm_hh2 += 1;
    }
    else
    {
      alarm_hh2 -= 1;
    }
    if (alarm_hh2 > 9)
      alarm_hh2 = 0;
    if (alarm_hh2 < 0)
      alarm_hh2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_hh2);
    alarmHour2 = alarm_hh1 * 10 + alarm_hh2;
    break;

  case 7:
    if (!BC_flag)
    {
      alarm_mm1 += 1;
    }
    else
    {
      alarm_mm1 -= 1;
    }
    if (alarm_mm1 > 5)
      alarm_mm1 = 0;
    if (alarm_mm1 < 0)
      alarm_mm1 = 5;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_mm1);
    alarmMinute2 = alarm_mm1 * 10 + alarm_mm2;
    break;

  case 8:
    if (!BC_flag)
    {
      alarm_mm2 += 1;
    }
    else
    {
      alarm_mm2 -= 1;
    }
    if (alarm_mm2 > 9)
      alarm_mm2 = 0;
    if (alarm_mm2 < 0)
      alarm_mm2 = 9;
    lcd.setCursor(row_k, col_k);
    lcd.print(alarm_mm2);
    alarmMinute2 = alarm_mm1 * 10 + alarm_mm2;
    break;

  default:
    break;
  }
  reloj.setA2Time(alarmDay, alarmHour2, alarmMinute2, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
}

void alarm()
{
  // Indicate whether an alarm went off
  if (reloj.checkAlarmEnabled(1) && reloj.checkIfAlarm(1))
  { // clock 1 detects alarm
    tone(BUZZER, 2000);
    digitalWrite(LED1, HIGH);
  }

  if (reloj.checkAlarmEnabled(2) && reloj.checkIfAlarm(2))
  { // clock 2 detects alarm
    tone(BUZZER, 1000);
    digitalWrite(LED2, HIGH);
  }
}


void showAlarmPage(void)
{

  lcd.clear();

  // fix text
  lcd.setCursor(3, 0);
  lcd.print("Alarm Settings");

  while (true)
  {
    showAlarmStatus(); // display the status of alarm
    enter();
    LDR_Sensor();
    KB = 0;      // B is invalid before A is pressed
    KC = 0;      // C is invalid before A is pressed
    if (KA == 1) // check alarm, move cursor
    {
      KA = 0;
      row_k = 9;
      col_k = 1;
      while (true)
      {
        enter();
        if (KA == 1)
        {
          KA = 0;
          col_k += 1;
          if (col_k > 2)
          {
            col_k = 1;
          }
        }
        lcd.setCursor(row_k, col_k);
        lcd.blink();

        if (KB == 1)
        {
          KB = 0;
          switch (col_k)
          {
          case 1:
            showAlarm1();
            row_k = 4;
            col_k = 1;
            while (true)
            {
              enter();
              if (KA == 1)
              {
                KA = 0;
                row_k += 1;
                if (row_k > 11)
                {
                  row_k = 4;
                }
              }
              lcd.setCursor(row_k, col_k);
              lcd.blink();
              if (KB == 1)
              {
                KB = 0;
                BC_flag = false;
                changeAlarmOne();
              }
              if (KC == 1)
              {
                KC = 0;
                BC_flag = true;
                changeAlarmOne();
              }
              if (KF == 1)
              {
                KF = 0;
                reloj.turnOnAlarm(1);
                // alarm1Flag = true;
                break;
              }
            }
            row_k = 9;
            col_k = 1;
            lcd.clear();
            showAlarmStatus();
            break;

          case 2:
            showAlarm2();
            row_k = 4;
            col_k = 1;
            while (true)
            {
              enter();
              if (KA == 1)
              {
                KA = 0;
                row_k += 1;
                if (row_k > 8)
                {
                  row_k = 4;
                }
              }
              lcd.setCursor(row_k, col_k);
              lcd.blink();
              if (KB == 1)
              {
                KB = 0;
                BC_flag = false;
                changeAlarmTwo();
              }
              if (KC == 1)
              {
                KC = 0;
                BC_flag = true;
                changeAlarmTwo();
              }
              if (KF == 1)
              {
                KF = 0;
                reloj.turnOnAlarm(2);
                // alarm2Flag = true;
                break;
              }
            }
            row_k = 9;
            col_k = 1;
            lcd.clear();
            showAlarmStatus();
            break;

          default:
            break;
          }
        }
        if (KC == 1)
        {
          KC = 0;
          switch (col_k)
          {
          case 1:
            reloj.turnOffAlarm(1);
            row_k = 9;
            col_k = 1;
            // lcd.clear();
            showAlarmStatus();
            break;

          case 2:
            reloj.turnOffAlarm(2);
            row_k = 9;
            col_k = 2;
            // lcd.clear();
            showAlarmStatus();
            break;

          default:
            break;
          }
        }
        if (KF == 1)
        {
          KF = 0;
          break;
        }
      }
      KD = 0; // D is invalid before # is pressed
    }
    if (KD == 1)
    {
      KD = 0;
      break;
    }
    page = 2;
  }
}
