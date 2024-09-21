void acdetect()
{
  float Vref = VccVoltageReader();
  sensorVCC = analogRead(POWERPIN);
  powersensor = ((float)sensorVCC / 1024) * Vref;
  Serial.println("Power: " + String(powersensor));
  powerflag = (powersensor > 0.1); //It will be true if it is greater than 0.1 if it is 0 it will be false
}
void datablackout()
{
  // Save on EEPROM
  EEPROM.put(30, hour);
  EEPROM.put(40, minute);
  EEPROM.put(50, date);
  EEPROM.put(70, monthName);
  Serial.println("Blackout event data saved in EEPROM.");
}

void datapoweron()
{
  // Save on EEPROM
  EEPROM.put(80, hour);
  EEPROM.put(90, minute);
  EEPROM.put(100, date);
  EEPROM.put(120, monthName);
  Serial.println("Power On event data saved in EEPROM.");
}