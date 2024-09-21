
float VccVoltageReader() {

const float InternalReferenceVoltage = 1.035; // <- as measured (or 1v1 by default)

  // turn ADC on
  ADCSRA =  bit (ADEN);

  // Prescaler of 128
  ADCSRA |= bit (ADPS0) |  bit (ADPS1) | bit (ADPS2);

  // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG) - Selects channel 14, bandgap voltage, to measure
  ADMUX = bit (REFS0) | bit (MUX3) | bit (MUX2) | bit (MUX1);

  // let it stabilize
  delay (10);

  // start a conversion
  bitSet (ADCSRA, ADSC);

  // Wait for the conversion to finish
  while (bit_is_set(ADCSRA, ADSC))
  {
    ;
  }

  // Float normally reduces precion but works OK here. Add 0.5 for rounding not truncating.
  powervcc = InternalReferenceVoltage / float (ADC + 0.5) * 1024.0;
  //Serial.println("Batt voltage is " + String(powervcc));
  return powervcc;
}