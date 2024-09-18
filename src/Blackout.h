void acdetect(){
    sensorVCC = analogRead(POWERPIN);
    powersensor = sensorVCC * (5.00 / 1023);
    HourFormat12();
    blackoutTimeH = hour;
    Serial.println(blackoutTimeH);
    blackoutTimeM = minute;
    Serial.println(blackoutTimeM);
    blackoutTimeDate = day;
    Serial.println(blackoutTimeDate);
    blackoutTimeMonth = monthName;
    Serial.println(blackoutTimeMonth);
    if (powersensor == 0)
    {
     // Save on EEPROM
      EEPROM.put(30, blackoutTimeH);
      delay(100);
      EEPROM.put(40, blackoutTimeM);
      delay(100);
    }
    
}