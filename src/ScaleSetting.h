void showScaleOne() {
  // Constantes de conversión
  const float GRAM_TO_KG = 1.0 / 1000.0;
  const float GRAM_TO_LB = 1.0 / 453.59237;
  const float GRAM_TO_OZ = 1.0 / 28.3495231;

  // Etiquetas
  lcd.setCursor(6, 0); lcd.print("BALANZA");
  lcd.setCursor(0, 1); lcd.print("GR:");
  lcd.setCursor(0, 2); lcd.print("KG:");
  lcd.setCursor(11, 1); lcd.print("LB:");
  lcd.setCursor(11, 2); lcd.print("OZ:");

  // Lectura de peso en gramos
  weight = hx.get_units(30);
  hx.power_down(); delay(1); hx.power_up();

  // Conversiones
  weight_kg = weight * GRAM_TO_KG;
  weight_lb = weight * GRAM_TO_LB;
  weight_oz = weight * GRAM_TO_OZ;

  // Mostrar valores
  char buff[10];
  dtostrf(weight, 6, 1, buff); lcd.setCursor(3, 1); lcd.print(buff);      // Gramos
  dtostrf(weight_kg, 6, 3, buff); lcd.setCursor(3, 2); lcd.print(buff);   // Kilogramos
  dtostrf(weight_lb, 6, 2, buff); lcd.setCursor(14, 1); lcd.print(buff);  // Libras
  dtostrf(weight_oz, 6, 2, buff); lcd.setCursor(14, 2); lcd.print(buff);  // Onzas

  // Show Price
  lcd.setCursor(0, 3); lcd.print("P=");
  lcd.setCursor(5, 3); lcd.print(".");

  int p_int = int(P * 10 + 0.5); // Redondeo correcto
  P1 = (p_int / 1000) % 10;
  P2 = (p_int / 100) % 10;
  P3 = (p_int / 10) % 10;
  P4 = p_int % 10;

  lcd.setCursor(2, 3); lcd.print(P1);
  lcd.setCursor(3, 3); lcd.print(P2);
  lcd.setCursor(4, 3); lcd.print(P3);
  lcd.setCursor(6, 3); lcd.print(P4);

  // Show Total
  lcd.setCursor(10, 3); lcd.print("T=");
  T = weight_lb * P;
  if (T >= 0)
  {
    dtostrf(T, 6, 1, buff);
    lcd.setCursor(12, 3); lcd.print(buff);
  }
  else
  {
    lcd.setCursor(12, 3); lcd.print("----.-");
  }
}

void changeScaleOne() {
  if (keypressed >= '0' && keypressed <= '9')
  {
    int key_num = keypressed - '0';
    int p_int = int(P * 10 + 0.5);

    switch (row_k)
    {
      case 2: // Centena
        p_int = (key_num * 1000) + (p_int % 1000);
        break;
      case 3: // Decena
        p_int = ((p_int / 1000) * 1000) + (key_num * 100) + (p_int % 100);
        break;
      case 4: // Unidad
        p_int = ((p_int / 100) * 100) + (key_num * 10) + (p_int % 10);
        break;
      case 6: // Decimal
        p_int = (p_int / 10) * 10 + key_num;
        break;
      default:
        break;
    }
    P = p_int / 10.0;
    lcd.setCursor(row_k, col_k);
    lcd.print(keypressed);
  }
}

void calibrate() {
  lcd.init(); // initialize LCD, avoid cursor flashing
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
    lcd.print(pgm_read_word(&known_weight[i]));
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
      ratio = adc_lecture / pgm_read_word(&known_weight[i]);

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

void showScalePage() {
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