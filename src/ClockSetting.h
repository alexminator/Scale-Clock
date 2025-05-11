void showDate()
{
  // Mostrar año
  year = reloj.getYear();
  y1 = year / 10;
  y2 = year % 10;
  lcd.setCursor(7, 1);
  if (year < 10) lcd.print("0");
  lcd.print(year);

  lcd.setCursor(9, 1); lcd.print("-");

  // Mostrar mes
  month = reloj.getMonth(century);
  mon1 = month / 10;
  mon2 = month % 10;
  lcd.setCursor(10, 1);
  if (month < 10) lcd.print("0");
  lcd.print(month);

  lcd.setCursor(12, 1); lcd.print("-");

  // Mostrar día
  date = reloj.getDate();
  d1 = date / 10;
  d2 = date % 10;
  lcd.setCursor(13, 1);
  if (date < 10) lcd.print("0");
  lcd.print(date);

  lcd.setCursor(15, 1); lcd.print(" ");

  // Día de la semana
  week = reloj.getDoW();
  lcd.setCursor(11, 3); lcd.print(week);

  // Hora
  hour = reloj.getHour(h12Flag, pmFlag);
  h1 = hour / 10;
  h2 = hour % 10;
  lcd.setCursor(5, 2);
  if (hour < 10) lcd.print("0");
  lcd.print(hour);

  lcd.setCursor(7, 2); lcd.print(":");

  // Minuto
  minute = reloj.getMinute();
  min1 = minute / 10;
  min2 = minute % 10;
  lcd.setCursor(8, 2);
  if (minute < 10) lcd.print("0");
  lcd.print(minute);

  lcd.setCursor(10, 2); lcd.print(":");

  // Segundo
  second = reloj.getSecond();
  s1 = second / 10;
  s2 = second % 10;
  lcd.setCursor(11, 2);
  if (second < 10) lcd.print("0");
  lcd.print(second);

  lcd.setCursor(13, 2); lcd.print(" ");
}

// Optimización: uso de funciones auxiliares para reducir repetición y errores
int incrementDigit(int digit, int max, int min = 0) {
  digit++;
  if (digit > max) digit = min;
  return digit;
}
int decrementDigit(int digit, int max, int min = 0) {
  digit--;
  if (digit < min) digit = max;
  return digit;
}

void changeTwo()
{
  switch (row_k)
  {
    case 7: // Año decenas
      y1 = (!BC_flag) ? incrementDigit(y1, 9) : decrementDigit(y1, 9);
      lcd.setCursor(row_k, col_k); lcd.print(y1);
      year = y1 * 10 + y2;
      break;
    case 8: // Año unidades
      y2 = (!BC_flag) ? incrementDigit(y2, 9) : decrementDigit(y2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(y2);
      year = y1 * 10 + y2;
      break;
    case 10: // Mes decenas
      mon1 = (!BC_flag) ? incrementDigit(mon1, 1) : decrementDigit(mon1, 1);
      lcd.setCursor(row_k, col_k); lcd.print(mon1);
      month = mon1 * 10 + mon2;
      break;
    case 11: // Mes unidades
      mon2 = (!BC_flag) ? incrementDigit(mon2, 9) : decrementDigit(mon2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(mon2);
      month = mon1 * 10 + mon2;
      break;
    case 13: // Día decenas
      d1 = (!BC_flag) ? incrementDigit(d1, 3) : decrementDigit(d1, 3);
      lcd.setCursor(row_k, col_k); lcd.print(d1);
      date = d1 * 10 + d2;
      break;
    case 14: // Día unidades
      d2 = (!BC_flag) ? incrementDigit(d2, 9) : decrementDigit(d2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(d2);
      date = d1 * 10 + d2;
      break;
    default:
      break;
  }
}

void changeThree()
{
  switch (row_k)
  {
    case 5: // Hora decenas
      h1 = (!BC_flag) ? incrementDigit(h1, 2) : decrementDigit(h1, 2);
      lcd.setCursor(row_k, col_k); lcd.print(h1);
      hour = h1 * 10 + h2;
      break;
    case 6: // Hora unidades
      h2 = (!BC_flag) ? incrementDigit(h2, 9) : decrementDigit(h2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(h2);
      hour = h1 * 10 + h2;
      break;
    case 8: // Minuto decenas
      min1 = (!BC_flag) ? incrementDigit(min1, 5) : decrementDigit(min1, 5);
      lcd.setCursor(row_k, col_k); lcd.print(min1);
      minute = min1 * 10 + min2;
      break;
    case 9: // Minuto unidades
      min2 = (!BC_flag) ? incrementDigit(min2, 9) : decrementDigit(min2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(min2);
      minute = min1 * 10 + min2;
      break;
    case 11: // Segundo decenas
      s1 = (!BC_flag) ? incrementDigit(s1, 5) : decrementDigit(s1, 5);
      lcd.setCursor(row_k, col_k); lcd.print(s1);
      second = s1 * 10 + s2;
      break;
    case 12: // Segundo unidades
      s2 = (!BC_flag) ? incrementDigit(s2, 9) : decrementDigit(s2, 9);
      lcd.setCursor(row_k, col_k); lcd.print(s2);
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
    case 5:
      mode = (!BC_flag) ? incrementDigit(mode, 5, 2) : decrementDigit(mode, 5, 2);
      lcd.setCursor(row_k, col_k); lcd.print(mode);
      EEPROM.put(20, mode);
      break;
    case 11:
      week = (!BC_flag) ? incrementDigit(week, 7, 1) : decrementDigit(week, 7, 1);
      lcd.setCursor(row_k, col_k); lcd.print(week);
      break;
    case 17:
      bled = (!BC_flag) ? incrementDigit(bled, 6, 1) : decrementDigit(bled, 6, 1);
      lcd.setCursor(row_k, col_k);
      if (bled == 6) lcd.print("A");
      else lcd.print(bled);
      EEPROM.put(10, bled);
      break;
    default:
      break;
  }
}

void setTime()
{
  reloj.setSecond(second);
  reloj.setMinute(minute);
  reloj.setHour(hour);
  reloj.setDoW(week);
  reloj.setDate(date);
  reloj.setMonth(month);
  reloj.setYear(year);
  reloj.setClockMode(false); // 24h
}

void showDatePage(void)
{
  lcd.clear();

  // Texto fijo
  lcd.setCursor(3, 0); lcd.print("Clock Settings");
  lcd.setCursor(0, 1); lcd.print("Date:");
  lcd.setCursor(0, 2); lcd.print("Time:");
  lcd.setCursor(0, 3); lcd.print("Info:");
  lcd.setCursor(7, 3); lcd.print("Day:");
  lcd.setCursor(13, 3); lcd.print("LCD:");
  lcd.setCursor(5, 1); lcd.print("2");
  lcd.setCursor(6, 1); lcd.print(century ? "1" : "0");
  lcd.setCursor(5, 3); lcd.print(mode);
  lcd.setCursor(17, 3); lcd.print(bled == 6 ? "A" : String(bled));

  while (true)
  {
    alarm();
    showDate();
    enter();
    LDR_Sensor();
    if (KA == 1)
    {
      KA = 0;
      while (true)
      {
        enter();
        if (KA == 1)
        {
          KA = 0;
          row_k += 1;
          if (row_k > 17)
          {
            row_k = 5;
            col_k += 1;
            if (col_k > 3) col_k = 1;
          }
        }
        lcd.setCursor(row_k, col_k);
        lcd.blink();

        if (KB == 1)
        {
          KB = 0;
          BC_flag = false;
          if (col_k == 1) changeTwo();
          else if (col_k == 2) changeThree();
          else changeFour();
        }
        if (KC == 1)
        {
          KC = 0;
          BC_flag = true;
          if (col_k == 1) changeTwo();
          else if (col_k == 2) changeThree();
          else changeFour();
        }
        if (KF == 1)
        {
          KF = 0;
          setTime();
          break;
        }
      }
      KD = 0;
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