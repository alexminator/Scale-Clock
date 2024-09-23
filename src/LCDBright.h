void LDR_Sensor()
{
    ldr = analogRead(LDR_PIN);
    filteredLDR = alpha * ldr + (1 - alpha) * filteredLDR;  // Filter equation
    //Serial.println(filteredLDR);
    bri = map(filteredLDR, 0, 1023, 0, 255);
    //Serial.println(bri);
    switch (bled)
    {
    case 1:
        analogWrite(BACKLIGHT_PIN, 50);
        break;
    case 2:
        analogWrite(BACKLIGHT_PIN, 100);
        break;
    case 3:
        analogWrite(BACKLIGHT_PIN, 150);
        break;
    case 4:
        analogWrite(BACKLIGHT_PIN, 200);
        break;
    case 5:
        analogWrite(BACKLIGHT_PIN, 255);
        break;
    case 6:
        analogWrite(BACKLIGHT_PIN, bri);
        break;
    default:
        break;
    }
}