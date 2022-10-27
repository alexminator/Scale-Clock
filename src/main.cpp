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
                 | [ ]A7              INT0/D2[X] |   KEYPAD
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

// The two LEDs are connected to A1 and A2. Turning analog pin into digital pin
const int LED1 = A1;
const int LED2 = A2;

// Brightness LCD control using LDR
const int LDR_PIN = A3;
const int BACKLIGHT_PIN = 10;
int ldr, bri;

// Buzzer Pin
const int buzzer = A0;

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
bool ampm;
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
int mode = 1;            // Time to show other info
bool BC_flag;            // false selective value plus 1, true sselective value subtracts 1
bool scale_flag = false; // false is pricing scale,true is counting scale

String Mode_arr[2] = {"12H", "24H"};
String M_arr[12] = {"Ene-", "Feb-", "Mar-", "Abr-", "May-", "Jun-", "Jul-", "Ago-", "Sep-", "Oct-", "Nov-", "Dic-"};
String WD_arr[7] = {"Lun,", "Mar,", "Mie,", "Jue,", "Vie,", "Sab,", "Dom,"};

void showDatePage(void);
void showDate();
void showAlarmPage(void);
void showAlarmStatus();
void showAlarm1();
void showAlarm2();
void showScalePage();
void showScaleOne();
void changeScaleOne();
void calibrate();
void changeThree();
void changeTwo();
void changeFour();
void changeAlarmOne();
void changeAlarmTwo();
void alarm();
void setTime();
void enter();
void ShowBigClock();
void BigClock();
void LDR_Sensor();
void info();

#include "ClockRoutines.h"
#include "Clock.h"
#include "ClockSetting.h"
#include "AlarmSetting.h"
#include "ScaleSetting.h"
#include "LCDBright.h"

void setup()
{
  EEPROM.get(0, ratio);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LDR_PIN, INPUT_PULLUP);
  pinMode(BACKLIGHT_PIN, OUTPUT);

  lcd.init();      // initialize LCD
  lcd.backlight(); // set the backlight of LCD on

  analogWrite(BACKLIGHT_PIN, 255); // Max Bright on intro.

  // Start the I2C interface
  Wire.begin();

  // Start the serial interface
  Serial.begin(9600);

  // Init HX
  hx.begin(DOUT, CLK);
  hx.set_scale(ratio);
  hx.tare();

  // intro
  tone(buzzer, 1971, 100);
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

  tone(buzzer, 494, 100);
  delay(60);
  // intro end

  // create 8 custom characters for bigclock
  for (nb = 0; nb < 8; nb++)
  {
    for (bc = 0; bc < 8; bc++)
      bb[bc] = pgm_read_byte(&custom[nb][bc]);
    lcd.createChar(nb + 1, bb);
  }
}

void loop()
{

  /* press 'D' key to switch page
     when page is 0, display CLOCK
     when page is 1, show the page of alarm setting
     when page is 2, show the page of digital scale
     when page is 3, show clock settings
  */

  if (page == 0)
  {
    ShowBigClock();
  }
  else if (page == 1)
  {
    showAlarmPage();
  }
  else if (page == 2)
  {
    showScalePage();
  }
  else if (page == 3)
  {
    showDatePage();
  }
}

void enter()
{
  keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
  {
    switch (keypressed)
    {
    case '1':
      tone(buzzer, 262, 100);
      break;

    case '2':
      tone(buzzer, 293, 100);
      break;

    case '3':
      tone(buzzer, 329, 100);
      break;

    case '4':
      tone(buzzer, 349, 100);
      break;

    case '5':
      tone(buzzer, 392, 100);
      break;

    case '6':
      tone(buzzer, 440, 100);
      break;

    case '7':
      tone(buzzer, 494, 100);
      break;

    case '8':
      tone(buzzer, 523, 100);
      break;

    case '9':
      tone(buzzer, 586, 100);
      break;

    case '0':
      tone(buzzer, 697, 100);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      break;

    case 'A':
      tone(buzzer, 1045, 100);
      KA = 1;
      break;

    case 'B':
      tone(buzzer, 879, 100);
      KB = 1;
      break;

    case 'C':
      tone(buzzer, 987, 100);
      KC = 1;
      break;

    case 'D':
      tone(buzzer, 1971, 100);
      KD = 1;
      break;

    case '*':
      tone(buzzer, 658, 100);
      KE = 1;
      break;

    case '#':
      tone(buzzer, 783, 100);
      KF = 1;
      break;

    default:
      break;
    }
  }
}
