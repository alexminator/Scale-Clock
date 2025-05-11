void handleBlink()
{
  if (!ledOverride) return;

  if (millis() - blinkMillis >= 30) {
    blinkMillis = millis();
    blinkState = !blinkState;
    switch (blinkType) {
      case 1: // Alarma 1: rojo
        digitalWrite(LED1, blinkState ? HIGH : LOW);
        digitalWrite(LED2, LOW);
        if (blinkState) tone(BUZZER, 2000, 200); else noTone(BUZZER);
        break;
      case 2: // Alarma 2: verde
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, blinkState ? HIGH : LOW);
        if (blinkState) tone(BUZZER, 1000, 200); else noTone(BUZZER);
        break;
      case 3: // Hora exacta: verde
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, blinkState ? HIGH : LOW);
        if (blinkState) tone(BUZZER, 783, 100); else noTone(BUZZER);
        // Solo la hora exacta usa el contador
        if (!blinkState) blinkCount++;
        if (blinkCount >= 2) {
          ledOverride = false;
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          noTone(BUZZER);
          blinkType = 0;
        }
        break;
    }
  }
}

void showAlarmStatus()
{
  lcd.setCursor(3, 0); lcd.print("Alarm Settings");

  for (uint8_t i = 1; i <= 2; i++) {
    lcd.setCursor(0, i);
    lcd.print("Alarm ");
    lcd.print(i);
    lcd.print(":");
    lcd.setCursor(9, i);
    lcd.print(reloj.checkAlarmEnabled(i) ? "Y" : "N");
  }
}

void showAlarm1()
{
  reloj.getA1Time(alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("  Set Alarm 1   ");

  alarm_h1 = alarmHour1 / 10;
  alarm_h2 = alarmHour1 % 10;
  alarm_m1 = alarmMinute1 / 10;
  alarm_m2 = alarmMinute1 % 10;
  alarm_s1 = alarmSecond1 / 10;
  alarm_s2 = alarmSecond1 % 10;

  lcd.setCursor(4, 1); lcd.print(alarmHour1 < 10 ? "0" : ""); lcd.print(alarmHour1);
  lcd.setCursor(6, 1); lcd.print(":");
  lcd.setCursor(7, 1); lcd.print(alarmMinute1 < 10 ? "0" : ""); lcd.print(alarmMinute1);
  lcd.setCursor(9, 1); lcd.print(":");
  lcd.setCursor(10, 1); lcd.print(alarmSecond1 < 10 ? "0" : ""); lcd.print(alarmSecond1);
}

void showAlarm2()
{
  reloj.getA2Time(alarmDay, alarmHour2, alarmMinute2, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("  Set Alarm 2   ");

  alarm_hh1 = alarmHour2 / 10;
  alarm_hh2 = alarmHour2 % 10;
  alarm_mm1 = alarmMinute2 / 10;
  alarm_mm2 = alarmMinute2 % 10;

  lcd.setCursor(4, 1); lcd.print(alarmHour2 < 10 ? "0" : ""); lcd.print(alarmHour2);
  lcd.setCursor(6, 1); lcd.print(":");
  lcd.setCursor(7, 1); lcd.print(alarmMinute2 < 10 ? "0" : ""); lcd.print(alarmMinute2);
}

void changeAlarmOne()
{
  auto wrap = [](int &val, int max) { if (++val > max) val = 0; };
  auto wrapDec = [](int &val, int max) { if (--val < 0) val = max; };

  switch (row_k)
  {
    case 4:
      (!BC_flag) ? wrap(alarm_h1, 2) : wrapDec(alarm_h1, 2);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_h1);
      alarmHour1 = alarm_h1 * 10 + alarm_h2;
      break;
    case 5:
      (!BC_flag) ? wrap(alarm_h2, 9) : wrapDec(alarm_h2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_h2);
      alarmHour1 = alarm_h1 * 10 + alarm_h2;
      break;
    case 7:
      (!BC_flag) ? wrap(alarm_m1, 5) : wrapDec(alarm_m1, 5);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_m1);
      alarmMinute1 = alarm_m1 * 10 + alarm_m2;
      break;
    case 8:
      (!BC_flag) ? wrap(alarm_m2, 9) : wrapDec(alarm_m2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_m2);
      alarmMinute1 = alarm_m1 * 10 + alarm_m2;
      break;
    case 10:
      (!BC_flag) ? wrap(alarm_s1, 5) : wrapDec(alarm_s1, 5);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_s1);
      alarmSecond1 = alarm_s1 * 10 + alarm_s2;
      break;
    case 11:
      (!BC_flag) ? wrap(alarm_s2, 9) : wrapDec(alarm_s2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_s2);
      alarmSecond1 = alarm_s1 * 10 + alarm_s2;
      break;
    default:
      break;
  }
  reloj.setA1Time(alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
}

void changeAlarmTwo()
{
  auto wrap = [](int &val, int max) { if (++val > max) val = 0; };
  auto wrapDec = [](int &val, int max) { if (--val < 0) val = max; };

  switch (row_k)
  {
    case 4:
      (!BC_flag) ? wrap(alarm_hh1, 2) : wrapDec(alarm_hh1, 2);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_hh1);
      alarmHour2 = alarm_hh1 * 10 + alarm_hh2;
      break;
    case 5:
      (!BC_flag) ? wrap(alarm_hh2, 9) : wrapDec(alarm_hh2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_hh2);
      alarmHour2 = alarm_hh1 * 10 + alarm_hh2;
      break;
    case 7:
      (!BC_flag) ? wrap(alarm_mm1, 5) : wrapDec(alarm_mm1, 5);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_mm1);
      alarmMinute2 = alarm_mm1 * 10 + alarm_mm2;
      break;
    case 8:
      (!BC_flag) ? wrap(alarm_mm2, 9) : wrapDec(alarm_mm2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(alarm_mm2);
      alarmMinute2 = alarm_mm1 * 10 + alarm_mm2;
      break;
    default:
      break;
  }
  reloj.setA2Time(alarmDay, alarmHour2, alarmMinute2, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
}

void alarm()
{
 // Alarma 1
if (!ledOverride && reloj.checkAlarmEnabled(1) && reloj.checkIfAlarm(1)) {
  ledOverride = true;
  blinkType = 1;
  blinkCount = 0;
  blinkMillis = millis();
  blinkState = false;
}

// Alarma 2
if (!ledOverride && reloj.checkAlarmEnabled(2) && reloj.checkIfAlarm(2)) {
  ledOverride = true;
  blinkType = 2;
  blinkCount = 0;
  blinkMillis = millis();
  blinkState = false;
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
