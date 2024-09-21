void acdetect()
{
  float Vref = VccVoltageReader();
  sensorVCC = analogRead(POWERPIN);
  powersensor = ((float)sensorVCC / 1024) * Vref;
  Serial.println("Power: " + String(powersensor));
  powerflag = (powersensor > 0.1); //sera true si es mayor que 0.1 si es 0 sera false
}
void datablackout()
{
  // Save on EEPROM
  EEPROM.put(30, hour);
  EEPROM.put(40, minute);
  EEPROM.put(50, date);
  EEPROM.put(70, monthName);
  Serial.println("Datos del evento apagon guardados en EEPROM.");
}

void datapoweron()
{
  // Save on EEPROM
  EEPROM.put(80, hour);
  EEPROM.put(90, minute);
  EEPROM.put(100, date);
  EEPROM.put(120, monthName);
  Serial.println("Datos del evento poweron guardados en EEPROM.");
}