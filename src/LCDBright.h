void LDR_Sensor()
{
    ldr = analogRead(LDR_PIN);
    filteredLDR = alpha * ldr + (1 - alpha) * filteredLDR;  // Filter equation
    //Serial.println(filteredLDR);
    bri = map(filteredLDR, 0, 1023, 0, 255);
    
    // Limitar el brillo mínimo para evitar apagado total
    if (bri < 50) bri = 50;
    
    // Selección de modo de brillo
    int pwmValue = bri;
    switch (bled)
    {
        case 1: pwmValue = 50; break;
        case 2: pwmValue = 100; break;
        case 3: pwmValue = 150; break;
        case 4: pwmValue = 200; break;
        case 5: pwmValue = 255; break;
        case 6: /* pwmValue ya es bri */ break;
        default: return;
    }
    analogWrite(BACKLIGHT_PIN, pwmValue);
}