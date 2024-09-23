/******************************************************
 * ****************************************************
 * ****        RELOJ BALANZA DIGITAL              *****
 * ****           LCD (20x4) I2C                  *****
 * ****      CON HX711 Y 4 GALGAS DE 50KG         *****
 * ****       By: Alexander Rivas Alpizar         *****
 * ****     https://www.sysadminsdecuba.com/      *****
 * ****                                           *****
 * ****************************************************

                              +-----+
                 +------------| USB |------------+
                 |            +-----+            |
    DOUT HX711   | [X]D13/SCK        MISO/D12[X] |   CLK HX711
                 | [ ]3.3V           MOSI/D11[ ]~|
                 | [ ]V.ref     ___    SS/D10[X]~|   BACKLIGHT
    BUZZER       | [X]A0       / N \       D9[X]~|   KEYPAD
    LED 1        | [X]A1      /  A  \      D8[X] |   KEYPAD
    LED 2        | [X]A2      \  N  /      D7[X] |   KEYPAD
    LDR          | [X]A3       \_0_/       D6[X]~|   KEYPAD
    I2C          | [X]A4/SDA               D5[X]~|   KEYPAD
    I2C          | [X]A5/SCL               D4[X] |   KEYPAD
                 | [ ]A6              INT1/D3[X]~|   KEYPAD
    POWER DETECT | [X]A7              INT0/D2[X] |   KEYPAD
                 | [X]5V                  GND[X] |
                 | [ ]RST                 RST[ ] |
                 | [X]GND   5V MOSI GND   TX1[ ] |
                 | [ ]Vin   [ ] [ ] [ ]   RX1[ ] |
                 |          [ ] [ ] [ ]          |
                 |          MISO SCK RST         |
                 | NANO-V3                       |
                 +-------------------------------+
 */
// Global Libraries
#include <Arduino.h>
#include <Keypad.h>
#include <DS3231.h> //lib clock
#include <HX711.h>
#include <EEPROM.h>
#include <math.h>
#include <Wire.h>
#include <avr/pgmspace.h> //lib clock
#include <SPI.h>          //lib clock
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// intro progressBAR glyph
byte progress1[] = {B00000, B00000, B00000, B00000, B11000, B11000, B11000, B11000};
byte progress2[] = {B00000, B00000, B00000, B00000, B11011, B11011, B11011, B11011};

// DS module
DS3231 reloj;

// HX711
#define CLK 12
#define DOUT 13
HX711 hx;           // Init ADC
long ratio, offset; // define two variables, ratio is scalefactor,
double weight = 0, P = 0, M = 0, D = 0;
int P1, P2, P3, P4, D1, D2, D3, D4, N;
float weight_lb;
float weight_gr;
float weight_oz;
float weight_kg;
int known_weight[4] = {50, 100, 500, 1000}; // Change with knowning weight
bool scale_flag = false;                    // false is pricing scale,true is counting scale

// The two LEDs are connected to A1 and A2. Turning analog pin into digital pin
#define LED1 A1
#define LED2 A2

// Brightness LCD control using LDR
//#define LDR_PIN A3
//#define BACKLIGHT_PIN 10 // PWM Pin
//byte ldr, bri, bled; // LCD Bright 
const int LDR_PIN = A3;
const int BACKLIGHT_PIN = 10; // PWM Pin
int ldr, bri;
int bled; // LCD Bright AUTO mode

// Buzzer Pin
#define BUZZER A0

// KeyPad Variables
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
char keypressed;     // used to receive the key value
float key_num;       // press keys to show numbers
// define the symbols on the buttons of the keypads
char keyMap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {2, 3, 4, 5}; // Row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; // Column pinouts of the keypad
Keypad myKeypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

// Clock Variables
byte hr, mn, osec;
bool century = false;
bool h12Flag;
bool pmFlag;
bool FlagInfo = true;

byte alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits = 0x48;
byte alarmHour2, alarmMinute2;
bool alarmDy = false, alarmH12Flag = false, alarmPmFlag = false;
byte row_k = 6, col_k = 1, page = 0;

boolean K0, K1, K2, K3, K4, K5, K6, K7, K8, K9, KA, KB, KC, KD, KE, KF; // key flag
byte year, month, date, hour, minute, second, week;
int y1, y2, mon1, mon2, d1, d2, h1, h2, min1, min2, s1, s2, t1, t2, t3, t4, convertemp;
float temp;
int alarm_h1, alarm_h2, alarm_m1, alarm_m2, alarm_s1, alarm_s2;
int alarm_hh1, alarm_hh2, alarm_mm1, alarm_mm2;
bool BC_flag; // false selective value plus 1, true sselective value subtracts 1

String M_arr[12] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
String WD_arr[7] = {"Lun,", "Mar,", "Mie,", "Jue,", "Vie,", "Sab,", "Dom,"};
String day, monthName;

// Time info variables
int mode; // Time to show other info. Min 2 mint.
bool isClockInfoShown = false;
unsigned long IntervalInfo;
unsigned long now = 0;
unsigned long startCollecting = 0; // Aux variable for Clock information

// Battery
#define FREEADCPIN A6 // An analog pin that is not being used
float powervcc;

// Power detect
#define POWERPIN A7
float powersensor;
byte sensorVCC;
byte blackoutTimeH, blackoutTimeDate, blackoutTimeM, poweronTimeH, poweronTimeM, poweronTimeDate;
String blackoutTimeMonth, poweronTimeMonth, blackoutAMPM, poweronAMPM, formattedHourBlackout, formattedMinuteBlackout, formattedHourPoweron, formattedMinutePoweron;
bool powerflag; // True, there's 5v power from supply. False a blackout event happens
bool blackoutTriggered = false; // Flag to control the activation of datablackout
bool powerOnTriggered = false;   // Flag to control the activation of poweron
// Time to sense power
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long refresh = 3000UL; // 3 seg Unsigned long

#include "ReadVCC.h"
#include "Key.h"
#include "LCDBright.h"
#include "AlarmSetting.h"
#include "ClockRoutines.h"
#include "Blackout.h"
#include "Clock.h"
#include "ClockSetting.h"
#include "ScaleSetting.h"

void setup()
{
    // Start the serial interface
  Serial.begin(9600);

  // Request INTERNAL reference voltage (for ATMega328P). 
  analogReference(INTERNAL);

  // That request is not honoured until we read the analog pin
  // so force voltage reference to be turned on
  analogRead(FREEADCPIN);

  // Get value from EEPROM
  EEPROM.get(0, ratio);              // Get scale ratio.int value 2 bytes
  EEPROM.get(10, bled);              // Get time led LCD_BACKLIGHT. int value 2 bytes
  EEPROM.get(20, mode);              // Get Time info mode. int value 2 bytes
  // Blackout
  EEPROM.get(30, blackoutTimeH);     // Get hour blackout event. int value 2 bytes
  EEPROM.get(40, blackoutTimeM);     // Get minute blackout event. int value 2 bytes
  EEPROM.get(50, blackoutTimeDate);  // Get date blackout event.
  EEPROM.get(60, blackoutAMPM);      // Get AM/PM blackout event. 
  EEPROM.get(70, blackoutTimeMonth); // Get month blackout event.
  // Power its back
  EEPROM.get(80, poweronTimeH);     // Get hour power on event. int value 2 bytes
  EEPROM.get(90, poweronTimeM);     // Get minute power on event. int value 2 bytes
  EEPROM.get(100, poweronTimeDate);  // Get date power on event.
  EEPROM.get(110, poweronAMPM);      // Get AM/PM poweron event. 
  EEPROM.get(120, poweronTimeMonth); // Get month power on  event.

  // Initialize pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LDR_PIN, INPUT_PULLUP);
  pinMode(BACKLIGHT_PIN, OUTPUT);

  lcd.init();      // initialize LCD
  lcd.backlight(); // set the backlight of LCD on
  analogWrite(BACKLIGHT_PIN, 255); // Max Bright on intro.

  // Start the I2C interface
  Wire.begin();

  // Init HX
  hx.begin(DOUT, CLK);
  hx.set_scale(ratio);
  hx.tare();

  // intro
  tone(BUZZER, 1971, 100);
  delay(60);

  lcd.createChar(1, progress1);
  lcd.createChar(2, progress2);
  lcd.home();
  lcd.setCursor(2, 0);
  lcd.print("RELOJ - BALANZA");
  lcd.setCursor(4, 1);
  lcd.print("alexminator");
  lcd.setCursor(3, 2);
  lcd.print("Free MEM:");
  lcd.setCursor(13, 2);
  lcd.print(freeRam());
  lcd.print(F("B"));

  // Bar animation
  for (int index = 0; index < 20; index++)
  {
    lcd.setCursor(index, 3);
    lcd.write(1);
    delay(90);
    lcd.setCursor(index, 3);
    lcd.write(2);
    delay(90);
  }

  tone(BUZZER, 494, 100);
  delay(60);
  // intro end

  // create 8 custom characters for bigclock
  for (nb = 0; nb < 8; nb++)
  {
    for (bc = 0; bc < 8; bc++)
      bb[bc] = pgm_read_byte(&custom[nb][bc]);
    lcd.createChar(nb + 1, bb);
  }

  startMillis = millis(); // initial start time
}

void loop()
{

  /* press 'D' key to switch page
     when page is 0, display CLOCK
     when page is 1, show the page of alarm setting
     when page is 2, show the page of digital scale
     when page is 3, show clock settings
  */

  switch (page)
  {
  case 0:
    ShowBigClock();
    break;
  case 1:
    showAlarmPage();
    break;
  case 2:
    showScalePage();
    break;
  case 3:
    showDatePage();
    break;
  }
}
