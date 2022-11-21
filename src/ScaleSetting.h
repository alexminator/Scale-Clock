void showScalePage()
{
  hx.tare();

  while (page == 2)
  {
    if (!scale_flag)
    {
      lcd.init();
      lcd.clear();
      while (true)
      {
        showScaleOne();
        enter();
        LDR_Sensor();
        if (KA == 1)
        {
          KA = 0;
          row_k = 2;
          col_k = 3;
          while (true)
          {
            enter(); // input
            if (KA == 1)
            {
              KA = 0;
              row_k += 1;
              if (row_k > 6)
              {
                row_k = 2;
              }
            }
            lcd.setCursor(row_k, col_k);
            lcd.blink();
            changeScaleOne();
            if (KF == 1)
            {
              KF = 0;
              break;
            }
          }
        }

        if (KD == 1)
        {
          KD = 0;
          page = 3;
          break;
        }
        if (KE == 1)
        { // press * to calibrate
          KE = 0;
          calibrate(); // The screen shows calibartion routine
        }
        if (KF == 1)
        { // press # to deduct tare. The screen will show 0, if 0 doesn't appear on screen
          KF = 0;
          hx.tare(); // subtract tare and read the offset
          // hx.set_offset(offset);  // set the read offset
          // EEPROM.put(8, offset);
        }
      }
    }
  }
}

void showScaleOne()
{
  // Fix text
  lcd.setCursor(6, 0);
  lcd.print("BALANZA");
  lcd.setCursor(0, 1);
  lcd.print("GR:");
  lcd.setCursor(0, 2);
  lcd.print("KG:");
  lcd.setCursor(11, 1);
  lcd.print("LB:");
  lcd.setCursor(11, 2);
  lcd.print("OZ:");

  // Read on gr
  weight = hx.get_units(30);
  hx.power_down(); // put the ADC in sleep mode
  delay(1);
  hx.power_up();

  /*while (weight>100000) {
    lcd.setCursor(0,0);
    lcd.print("     WARNING !!!    ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("  Scale Overloaded  ");
    lcd.setCursor(0,3);
    lcd.print("                    ");
    tone(buzzer, 783, 100);
    delay(100);
    tone(buzzer, 658, 100);
    delay(100);
    lcd.clear();
    weight = hx.get_units(30);
  }
  */

  // kilograms
  weight_kg = weight / 1000.0;
  char buff1[6];
  dtostrf(weight_kg, 6, 3, buff1);
  lcd.setCursor(3, 2);
  lcd.print(buff1);
  // Grams
  char buff2[6];
  dtostrf(weight, 6, 1, buff2);
  lcd.setCursor(3, 1);
  lcd.print(buff2);
  // Pounds
  weight_lb = weight * 0.002205;
  char buff3[6];
  dtostrf(weight_lb, 6, 1, buff3);
  lcd.setCursor(14, 1);
  lcd.print(buff3);
  // ounce
  weight_oz = weight * 0.035273;
  char buff4[6];
  dtostrf(weight_oz, 6, 1, buff4);
  lcd.setCursor(14, 2);
  lcd.print(buff4);

  lcd.setCursor(0, 3);
  lcd.print("P=");
  lcd.setCursor(5, 3);
  lcd.print(".");

  P1 = int(P * 10) / 1000;
  P2 = (int(P * 10) - P1 * 1000) / 100;
  P3 = (int(P * 10) - P1 * 1000 - P2 * 100) / 10;
  P4 = int(P * 10) % 10;
  lcd.setCursor(2, 3);
  lcd.print(P1);
  lcd.setCursor(3, 3);
  lcd.print(P2);
  lcd.setCursor(4, 3);
  lcd.print(P3);
  lcd.setCursor(6, 3);
  lcd.print(P4);

  lcd.setCursor(10, 3);
  lcd.print("M=");
  M = weight_lb * P;
  char buff5[6];
  dtostrf(M, -6, 1, buff5); // right alignment
  if (M >= 0)
  {
    lcd.setCursor(12, 3);
    lcd.print(buff5);
  }
  else
  {
    lcd.setCursor(12, 3);
    lcd.print("----.-");
  }
}

void changeScaleOne()
{
  if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9')
  {
    key_num = keypressed - '0';
    switch (row_k)
    {
    case 2:
      lcd.setCursor(row_k, col_k);
      lcd.print(keypressed);
      P = P - P1 * 100 + key_num * 100;
      break;

    case 3:
      lcd.setCursor(row_k, col_k);
      lcd.print(keypressed);
      P = P - P2 * 10 + key_num * 10;
      break;

    case 4:
      lcd.setCursor(row_k, col_k);
      lcd.print(keypressed);
      P = P - P3 + key_num;
      break;

    case 6:
      lcd.setCursor(row_k, col_k);
      lcd.print(keypressed);
      P = P - float(P4) / 10 + key_num / 10;
      break;

    default:
      break;
    }
  }
}

void calibrate()
{
  lcd.init(); // initialize LCD1602, avoid cursor flashing
  int i = 0, cal = 1;
  long adc_lecture;

  // Escribimos el Mensaje en el LCD
  lcd.setCursor(2, 0);
  lcd.print("Calibracion de");
  lcd.setCursor(4, 1);
  lcd.print("Balanza");
  delay(1500);
  hx.read();
  hx.set_scale(); // Default scale its 1
  hx.tare(20);    // Actual weight its consider Tare.
  lcd.clear();

  // Calibration
  while (cal == 1)
  {
    enter();

    lcd.setCursor(1, 0);
    lcd.print("Peso Conocido:");
    lcd.setCursor(1, 1);
    lcd.print(known_weight[i]);
    lcd.print(" g");

    // Search weight
    if (KA == 1)
    {
      KA = 0;                  // key flag, clear 0
      i = (i > 2) ? 0 : i + 1; // if-else en una linea
    }

    // Select weight
    if (KF == 1)
    {
      KF = 0; // key flag, clear 0

      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Ponga el Peso");
      lcd.setCursor(1, 1);
      lcd.print("y espere ...");
      delay(2000);

      // Read HX711
      adc_lecture = hx.get_value(100);

      // Calculate ratio
      ratio = adc_lecture / known_weight[i];

      // Save on EEPROM
      EEPROM.put(0, ratio);
      delay(100);
      cal = 0; // Change Flag for break the while
      lcd.print("CAL OK!");
      delay(500);
      lcd.clear();
    }
  }
}
