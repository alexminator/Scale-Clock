void LDR_Sensor()
{
    ldr = analogRead(LDR_PIN);
    bri = map(ldr, 0, 1023, 0, 255);
    analogWrite(BACKLIGHT_PIN, bri);
}