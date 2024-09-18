void anibatt()
{
  float units_per_pixel = (gauge_size_chars * 5.0) / 100.0;    //  every character is 5px wide, we want to count from 0-100
  int value_in_pixels = round(batt_gauge * units_per_pixel);    // batt_gauge value converted to pixel width

  int tip_position = 0;      // 0= not set, 1=tip in first char, 2=tip in middle, 3=tip in last char

  if (value_in_pixels < 5) {
    tip_position = 1; // tip is inside the first character
  }
  else if (value_in_pixels > gauge_size_chars * 5.0 - 5) {
    tip_position = 3; // tip is inside the last character
  }
  else {
    tip_position = 2; // tip is somewhere in the middle
  }

  move_offset = 4 - ((value_in_pixels - 1) % 5);    // value for offseting the pixels for the smooth filling

  for (int i = 0; i < 8; i++) { // dynamically create left part of the gauge
    if (tip_position == 1) {
      gauge_left_dynamic[i] = (gauge_fill_5[i] << move_offset) | gauge_left[i]; // tip on the first character
    }
    else {
      gauge_left_dynamic[i] = gauge_fill_5[i]; // tip not on the first character
    }

    gauge_left_dynamic[i] = gauge_left_dynamic[i] & gauge_mask_left[i];                                 // apply mask for rounded corners
  }

  for (int i = 0; i < 8; i++) { // dynamically create right part of the gauge
    if (tip_position == 3) {
      gauge_right_dynamic[i] = (gauge_fill_5[i] << move_offset) | gauge_right[i]; // tip on the last character
    }
    else {
      gauge_right_dynamic[i] = gauge_right[i]; // tip not on the last character
    }

    gauge_right_dynamic[i] = gauge_right_dynamic[i] & gauge_mask_right[i];                                // apply mask for rounded corners
  }

  lcd.createChar(5, gauge_left_dynamic);     // create custom character for the left part of the gauge
  lcd.createChar(6, gauge_right_dynamic);    // create custom character for the right part of the gauge

  for (int i = 0; i < gauge_size_chars; i++) { // set all the characters for the gauge
    if (i == 0) {
      gauge_string[i] = byte(5); // first character = custom left piece
    }
    else if (i == gauge_size_chars - 1) {
      gauge_string[i] = byte(6); // last character = custom right piece
    }
    else {                                                        // character in the middle, could be empty, tip or fill
      if (value_in_pixels <= i * 5) {
        gauge_string[i] = byte(7); // empty character
      }
      else if (value_in_pixels > i * 5 && value_in_pixels < (i + 1) * 5) {
        gauge_string[i] = byte(5 - move_offset); // tip
      }
      else {
        gauge_string[i] = byte(255); // filled character
      }
    }
  }

  // gauge drawing
  lcd.setCursor(0, 2);                        // move cursor to top left
  sprintf(buffer, "Battery:%3d%% ", batt_gauge);    // set a string as CPU: XX%, with the number always taking at least 3 character
  lcd.print(buffer);                          // print the string on the display
  lcd.write(byte(0));                         // print warning character
  lcd.print("  ");

  lcd.setCursor(0, 1);             // move the cursor to the next line
  lcd.print(gauge_string);         // display the gauge

}