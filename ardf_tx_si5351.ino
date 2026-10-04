// from https://sites.google.com/site/ik1lbw/home/sketch-arduino-beacon?pli=1&authuser=0
// CW Beacon using Si5351, LZ2WSG, KN34PC
//---------------------------------------------------------------------------------------------------------
#include "si5351.h"
#define PIN_SP 10// posizione out monitor audio= pin 10 Arduino
#define PIN_TX 13// posizione out monitor ledtx= pin 13 Arduino
Si5351 si5351(0x60);
uint16_t duration = 100;//valore velocita' cw (80-100)(maggiore e' il valore...minore WPM)
uint16_t hz = 750;//valore nota monitor (750hz)
String cw_message = " VVV DE HA5UDS/B   JN97 PSE QSL "; // inserire il msg. da trasmettere (maiuscolo)
uint32_t tx = 3579000; // inserire freq. tx (es.)--> 50.426 mhz= 50426000   144.444 mhz= 144444000 (28224=28222000)  (50444=50443000)  (144444=144433500)
//---------------------------------------------------------------------------------------------------------
void setup() {
  pinMode(PIN_TX, OUTPUT);
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 25000000, 0);
  si5351.set_freq(tx * SI5351_FREQ_MULT, SI5351_CLK1);
  si5351.output_enable(SI5351_CLK1, 0);
  si5351.drive_strength(SI5351_CLK1,SI5351_DRIVE_2MA);// settaggio PWT out...2MA=2Mw...4MA=4Mw...8MA=8Mw...MAX !!
}
//---------------------------------------------------------------------------------------------------------
void loop() {
  cw_string_proc(cw_message); // durata spazio tra msg. e portante fissa bcn (>900)
  delay(1000);
  cw(true);
  delay(10000); // durata portante fissa bcn (>15000)
  cw(false);
  delay(10000); // durata spazio tra portante fissa bcn e inizio msg. (>5000)
}
//---------------------------------------------------------------------------------------------------------
void cw_string_proc(String str) {                      // processing string to characters
  for (uint8_t j = 0; j < str.length(); j++)
    cw_char_proc(str[j]);
}
//---------------------------------------------------------------------------------------------------------
void cw_char_proc(char m) {                            // processing characters to Morse code symbols
  String s;
  if (m == ' ') {                                      // пауза между думите
    word_space();
    return;
  }
  if (m > 96)                                          // ACSII, case change a-z to A-Z
    if (m < 123)
      m -= 32;
  switch (m) {                                         // Morse code
    case 'A': s = ".-#";     break;
    case 'B': s = "-...#";   break;
    case 'C': s = "-.-.#";   break;
    case 'D': s = "-..#";    break;
    case 'E': s = ".#";      break;
    case 'F': s = "..-.#";   break;
    case 'G': s = "--.#";    break;
    case 'H': s = "....#";   break;
    case 'I': s = "..#";     break;
    case 'J': s = ".---#";   break;
    case 'K': s = "-.-#";    break;
    case 'L': s = ".-..#";   break;
    case 'M': s = "--#";     break;
    case 'N': s = "-.#";     break;
    case 'O': s = "---#";    break;
    case 'P': s = ".--.#";   break;
    case 'Q': s = "--.-#";   break;
    case 'R': s = ".-.#";    break;
    case 'S': s = "...#";    break;
    case 'T': s = "-#";      break;
    case 'U': s = "..-#";    break;
    case 'V': s = "...-#";   break;
    case 'W': s = ".--#";    break;
    case 'X': s = "-..-#";   break;
    case 'Y': s = "-.--#";   break;
    case 'Z': s = "--..#";   break;
    case '1': s = ".----#";  break;
    case '2': s = "..---#";  break;
    case '3': s = "...--#";  break;
    case '4': s = "....-#";  break;
    case '5': s = ".....#";  break;
    case '6': s = "-....#";  break;
    case '7': s = "--...#";  break;
    case '8': s = "---..#";  break;
    case '9': s = "----.#";  break;
    case '0': s = "-----#";  break;
    case '?': s = "..--..#"; break;
    case ',': s = "--..--#"; break;
    case '/': s = "-..-.#";  break;
  }
  for (uint8_t i = 0; i < 7; i++) {
    switch (s[i]) {
      case '.': ti();  break;                          // TI (punto)
      case '-': ta();  break;                          // TA (linea)
      case '#': char_space(); return;                  // end of Morse code symbol
    }
  }
}
//---------------------------------------------------------------------------------------------------------
void ti() {
  cw(true);                                            // TX TI (punto)
  delay(duration);
  cw(false);                                           // stop TX TI (punto)
  delay(duration);
}
//---------------------------------------------------------------------------------------------------------
void ta() {
  cw(true);                                            // TX TA (linea)
  delay(3 * duration);
  cw(false);                                           // stop TX TA (linea)
  delay(duration);
}
//---------------------------------------------------------------------------------------------------------
void char_space() {                                    // 3x, пауза между буквите
  delay(2 * duration);                                 // 1 from element-end + 2 new
}
//---------------------------------------------------------------------------------------------------------
void word_space() {                                    // 7x, пауза между думите
  delay(6 * duration);                                 // 1 from element-end + 6 new
}
//---------------------------------------------------------------------------------------------------------
void cw(bool state) {                                  // TX-CW, monitor>TX-LED, monitor audio>750 Hz sound
  if (state) {
    si5351.output_enable(SI5351_CLK1, 1);
    digitalWrite(PIN_TX, HIGH);
    tone(PIN_SP, hz);
  } else {
    si5351.output_enable(SI5351_CLK1, 0);
    digitalWrite(PIN_TX, LOW);
    noTone(PIN_SP);
  }
}