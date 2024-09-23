void acdetect()
{
  float Vref = VccVoltageReader();
  sensorVCC = analogRead(POWERPIN);
  powersensor = ((float)sensorVCC * Vref) / 1024;
  //Serial.println(powersensor);
  powerflag = (powersensor > 0.1); //It will be true if it is greater than 0.1 if it is 0 it will be false
}

void datablackout()
{ 
  // Save on EEPROM
  EEPROM.put(30, hour);
  EEPROM.put(40, minute);
  EEPROM.put(50, date);
  EEPROM.put(60, blackoutAMPM);
  EEPROM.put(70, monthName);
}

void datapoweron()
{
  // Save on EEPROM
  EEPROM.put(80, hour);
  EEPROM.put(90, minute);
  EEPROM.put(100, date);
  EEPROM.put(110, poweronAMPM);
  EEPROM.put(120, monthName);
}