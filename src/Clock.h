void HourFormat12()
{
  hour = reloj.getHour(h12Flag, pmFlag);
  if (hour == 0)
  {
    hour = 12;     // 12 Midnight
    pmFlag = true; // Set AM
  }
  else if (hour < 12)
  {
    pmFlag = true; // Set AM
  }
  else
  {
    hour -= 12;     // Convert to 12-hour format
    pmFlag = false; // Set PM
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

void BigClock()
{
  // hour, minute, and second
  if (!h12Flag)
  {
    hour = reloj.getHour(h12Flag, pmFlag);
  }
  else
  {
    HourFormat12(); // Convert to 12H, only for show on bigclock
    h12Flag = true;
  }

  h1 = hour / 10;
  h2 = hour % 10;
  minute = reloj.getMinute();
  min1 = minute / 10;
  min2 = minute % 10;
  second = reloj.getSecond();
  s1 = second / 10;
  s2 = second % 10;

  // info for save blackout event
  month = reloj.getMonth(century);
  monthName = M_arr[month - 1];

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
    tone(BUZZER, 783, 100);
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
  lcd.clear();
  while (true)
  {
    alarm(); // detect alarm
    LDR_Sensor();
    enter();
    VccVoltageReader();

    // Battery
    lcd.setCursor(0, 2);
    lcd.print("VCC:");
    lcd.setCursor(5, 2);
    lcd.print(powervcc);
    lcd.setCursor(9, 2);
    lcd.print("V");
    lcd.setCursor(11, 2);
    lcd.print(!powerflag ? "Using BAT" : "Charging");

    // Blackout data
    EEPROM.get(30, blackoutTimeH);
    EEPROM.get(40, blackoutTimeM);
    EEPROM.get(50, blackoutTimeDate);
    EEPROM.get(60, blackoutAMPM);
    EEPROM.get(70, blackoutTimeMonth);
    lcd.setCursor(0, 0);
    lcd.print("OFF:");
    formattedHourBlackout = (blackoutTimeH < 10) ? "0" + String(blackoutTimeH) : String(blackoutTimeH);
    lcd.setCursor(5, 0);
    lcd.print(formattedHourBlackout);
    lcd.setCursor(7, 0);
    lcd.print(":");
    formattedMinuteBlackout = (blackoutTimeM < 10) ? "0" + String(blackoutTimeM) : String(blackoutTimeM);
    lcd.setCursor(8, 0);
    lcd.print(formattedMinuteBlackout);
    lcd.setCursor(11, 0);
    lcd.print(blackoutAMPM);
    lcd.setCursor(14, 0);
    lcd.print(blackoutTimeDate);
    lcd.setCursor(16, 0);
    lcd.print(",");
    lcd.setCursor(17, 0);
    lcd.print(blackoutTimeMonth);
    // Power on data
    EEPROM.get(80, poweronTimeH);
    EEPROM.get(90, poweronTimeM);
    EEPROM.get(100, poweronTimeDate);
    EEPROM.get(110, poweronAMPM);
    EEPROM.get(120, poweronTimeMonth);
    lcd.setCursor(0, 1);
    lcd.print("ON :");
    formattedHourPoweron = (poweronTimeH < 10) ? "0" + String(poweronTimeH) : String(poweronTimeH);
    lcd.setCursor(5, 1);
    lcd.print(formattedHourPoweron);
    lcd.setCursor(7, 1);
    lcd.print(":");
    formattedMinutePoweron = (poweronTimeM < 10) ? "0" + String(poweronTimeM) : String(poweronTimeM);
    lcd.setCursor(8, 1);
    lcd.print(formattedMinutePoweron);
    lcd.setCursor(11, 1);
    lcd.print(poweronAMPM);
    lcd.setCursor(14, 1);
    lcd.print(poweronTimeDate);
    lcd.setCursor(16, 1);
    lcd.print(",");
    lcd.setCursor(17, 1);
    lcd.print(poweronTimeMonth);

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

void ShowDateInfo()
{
  lcd.setCursor(0, 0);
  lcd.print("             "); // Clean info

  // Data Show
  // and the day of the week
  week = reloj.getDoW();
  day = WD_arr[week - 1];
  lcd.setCursor(0, 0);
  lcd.print(day);

  // first the date
  date = reloj.getDate();
  lcd.setCursor(4, 0);
  lcd.print(date);

  // then the month
  month = reloj.getMonth(century);
  monthName = M_arr[month - 1];
  lcd.setCursor(7, 0);
  lcd.print(monthName); // Minus one, arrays begins in 0

  if (h12Flag) {
    lcd.setCursor(11, 0);
    lcd.print(!pmFlag ? "PM" : "AM");
  } else {
    // dash
    lcd.setCursor(10, 0);
    lcd.print("-");
    // Year
    year = reloj.getYear();
    lcd.setCursor(11, 0);
    lcd.print(year);
  }
}

void ShowBigClock()
{
  lcd.clear();
  while (true)
  {
    alarm(); // detect alarm
    LDR_Sensor();
    BigClock();
    enter();

    currentMillis = millis();
    // Check if the update time has passed
    if (currentMillis - startMillis >= refresh)
    {
      startMillis = currentMillis; // Update previous time
      acdetect();                  // Call function to detect power
      // Only calls datablackout if powerflag is false and has not been called before
      if (!powerflag && !blackoutTriggered)
      {
        blackoutAMPM = !pmFlag ? "PM" : "AM"; // Set blackoutAMPM based on pmFlag
        datablackout();
        blackoutTriggered = true; // Marks that datablackout has been activated
        powerOnTriggered = false; // Reset the poweron flag
        // Serial.println("Blackout event at: " + String(hour) + ":" + String(minute) + " " + poweronAMPM);
      }
      else if (powerflag && !powerOnTriggered)
      {
        poweronAMPM = !pmFlag ? "PM" : "AM"; // Set poweronAMPM based on pmFlag
        datapoweron();
        powerOnTriggered = true;   // Marks that datapoweron has been activated
        blackoutTriggered = false; // Reset the blackout flag
        // Serial.println("Power on event at: " + String(hour) + ":" + String(minute) + " " + poweronAMPM);
      }
    }

    if (shouldShowInfo())
    {
      ShowInfo();
    }
    else
    {
      ShowDateInfo();
    }

    if (KE == 1) // Show other info sub screen. Temp & power events
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
        if (KB == 1) // Show hour in 12H mode
    {
      KB = 0;
      HourFormat12(); // Convert to 12H, only for show on bigclock
      h12Flag = true;
      break;
    }
    if (KC == 1) // Return to 24H mode
    {
      KC = 0;
      hour = reloj.getHour(h12Flag, pmFlag); // Get hour 24H format
      break;
    }
  }
}
