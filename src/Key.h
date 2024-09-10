void enter()
{
  keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
  {

    // Map key presses to buzzer tones
    int tones[] = {262, 293, 329, 349, 392, 440, 494, 523, 586, 697, 1045, 879, 987, 1971, 658, 783};
    int keyIndex = keypressed - '0'; // Convert char to index

    if (keyIndex >= 0 && keyIndex <= 9)
    {
      tone(buzzer, tones[keyIndex], 100);
      if (keyIndex == 0)
      {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
      }
    }
    else if (keypressed >= 'A' && keypressed <= 'D')
    {
      tone(buzzer, tones[keypressed - 'A' + 10], 100);
      switch (keypressed)
      {
      case 'A':
        KA = 1;
        break;
      case 'B':
        KB = 1;
        break;
      case 'C':
        KC = 1;
        break;
      case 'D':
        KD = 1;
        break;
      }
    }
    else if (keypressed == '*')
    {
      tone(buzzer, tones[14], 100); 
      KE = 1;                       
    }
    else if (keypressed == '#')
    {
      tone(buzzer, tones[15], 100); 
      KF = 1;                       
    }
  }
}