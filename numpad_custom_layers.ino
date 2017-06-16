//
// Based on
// https://github.com/theimmc/numeric-keypad
// - but customized

#include <Keyboard.h>


const int LAYERS  = 3;
const int COLUMNS = 4;
const int ROWS    = 5;
const int HB_LED  = 17;

#define HEARTBEAT_ENABLED 0

// These are the column pins. They're configured for input with
// internal pullup

int input_pins[COLUMNS] = { 3, 16, 4, 15 } ;

// These are the row strobes. They're configured for output
// and initially all set to high. Individual pins are set to
// low to read that row. Only one row is low at any time.

int strobe_pins[ROWS] = { 9, 8, 7, 6, 5 };

unsigned long key_state[ROWS][COLUMNS];


int strobe_row = 0;
int q = 0;
int layer = 0;

// Codes for numeric keypad
// NUM  /   *   -
//  7   8   9   x
//  4   5   6   +
//  1   2   3   x
//  x   0  . ENTER

/*
  int keycode[ROWS][COLUMNS] = { { 0xDB, 0xDC, 0xDD, 0xDE },
                               { 0xE7, 0xE8, 0xE9, 0    },
                               { 0xE4, 0xE5, 0xE6, 0xDF },
                               { 0xE1, 0xE2, 0xE3, 0 },
                               { 0,    0xEA, 0xEB, 0XE0 } };
*/



// Custom layered setup

// Key mappings/functions
#define K_CHANGE_LAYER 1
#define K_SLASH        2
#define K_ASTERISK     3
#define K_MINUS        4
#define K_PLUS         5
#define K_1            6
#define K_2            7
#define K_3            8
#define K_4            9
#define K_5            10
#define K_6            11
#define K_7            12
#define K_8            13
#define K_9            14
#define K_0            15
#define K_PERIOD       16
#define K_HOME         17
#define K_END          18
#define K_PAGEUP       19
#define K_PAGEDOWN     20
#define K_UP           21
#define K_DOWN         22
#define K_LEFT         23
#define K_RIGHT        24
#define K_INSERT       25
#define K_DELETE       26
#define K_LSHIFT       27
#define K_ENTER        28

#define K_WS0          30
#define K_WS1          31
#define K_WS2          32
#define K_WS3          33
#define K_WS4          34
#define K_WS5          35
#define K_WS6          36
#define K_WS7          37
#define K_WS8          38
#define K_WS9          39
#define K_RWS0         40
#define K_RWS1         41
#define K_RWS2         42
#define K_RWS3         43
#define K_RWS4         44
#define K_RWS5         45
#define K_RWS6         46
#define K_RWS7         47
#define K_RWS8         48
#define K_RWS9         49

#define K_LCTRL        70
#define K_DOUBLEO      71

#define K_HELLO        577
#define K_TEST         666
#define K_NOTHING      999

int keymap[LAYERS][ROWS][COLUMNS] =
  // layer 0
{ {
    { K_CHANGE_LAYER, K_SLASH, K_ASTERISK, K_MINUS },
    { K_7,            K_8,     K_9,        0       },
    { K_4,            K_5,     K_6,        K_PLUS  },
    { K_1,            K_2,     K_3,        0       },
    { 0,              K_0,     K_DOUBLEO,  K_ENTER }
  },
  // layer 1
  {
    { K_CHANGE_LAYER, K_SLASH, K_ASTERISK, K_MINUS },
    { K_HOME,         K_UP,    K_PAGEUP,   0       },
    { K_LEFT,         K_5,     K_RIGHT,    K_PLUS  },
    { K_END,          K_DOWN,  K_PAGEDOWN, 0       },
    { 0,              K_INSERT, K_DELETE,   K_ENTER }
  },
  // layer 2
  {
    { K_CHANGE_LAYER, K_SLASH, K_NOTHING,  K_MINUS },
    { K_WS7,          K_WS8,   K_WS9,      0       },
    { K_WS4,          K_WS5,   K_WS6,      K_LSHIFT},
    { K_WS1,          K_WS2,   K_WS3,      0       },
    { 0,              K_WS0,   K_NOTHING,  K_LCTRL }
  }/*,
  // layer 3
  {
    { K_CHANGE_LAYER, K_SLASH, K_NOTHING,  K_MINUS },
    { K_RWS7,         K_RWS8,  K_RWS9,     0       },
    { K_RWS4,         K_RWS5,  K_RWS6,     K_LSHIFT},
    { K_RWS1,         K_RWS2,  K_RWS3,     0       },
    { 0,              K_RWS0,  K_NOTHING,  K_ENTER }
  }*/
};



void keymap_to_keypress(int k)
{
  switch (k) {
    case K_CHANGE_LAYER:
      layer++;
      if (layer >= LAYERS)
        layer = 0;
      break;
    case K_SLASH:
      Keyboard.press('/');
      break;
    case K_ASTERISK:
      Keyboard.press('*');
      break;
    case K_MINUS:
      Keyboard.press(0xDE);
      break;
    case K_PLUS:
      Keyboard.press(0xDF);
      break;
    case K_ENTER:
      Keyboard.press(0xE0);
      break;
    case K_0:
      Keyboard.press('0');
      break;
    case K_1:
      Keyboard.press('1');
      break;
    case K_2:
      Keyboard.press('2');
      break;
    case K_3:
      Keyboard.press('3');
      break;
    case K_4:
      Keyboard.press('4');
      break;
    case K_5:
      Keyboard.press('5');
      break;
    case K_6:
      Keyboard.press('6');
      break;
    case K_7:
      Keyboard.press('7');
      break;
    case K_8:
      Keyboard.press('8');
      break;
    case K_9:
      Keyboard.press('9');
      break;
    case K_DOUBLEO:
      Keyboard.write('0');
      Keyboard.write('0');
      break;
    case K_PERIOD:
      Keyboard.press('.');
      break;
    case K_LEFT:
      Keyboard.press(KEY_LEFT_ARROW);
      break;
    case K_RIGHT:
      Keyboard.press(KEY_RIGHT_ARROW);
      break;
    case K_UP:
      Keyboard.press(KEY_UP_ARROW);
      break;
    case K_DOWN:
      Keyboard.press(KEY_DOWN_ARROW);
      break;
    case K_HOME:
      Keyboard.press(KEY_HOME);
      break;
    case K_END:
      Keyboard.press(KEY_END);
      break;
    case K_PAGEUP:
      Keyboard.press(KEY_PAGE_UP);
      break;
    case K_PAGEDOWN:
      Keyboard.press(KEY_PAGE_DOWN);
      break;
    case K_INSERT:
      Keyboard.press(KEY_INSERT);
      break;
    case K_DELETE:
      Keyboard.press(KEY_DELETE);
      break;
    case K_LSHIFT:
      Keyboard.press(KEY_LEFT_SHIFT);
      break;
    case K_LCTRL:
      Keyboard.press(KEY_LEFT_CTRL);
      break;
    case K_WS1:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('1');
      break;
    case K_WS2:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('2');
      break;
    case K_WS3:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('3');
      break;
    case K_WS4:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('4');
      break;
    case K_WS5:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('5');
      break;
    case K_WS6:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('6');
      break;
    case K_WS7:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('7');
      break;
    case K_WS8:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('8');
      break;
    case K_WS9:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('9');
      break;
    case K_WS0:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('0');
      break;
    case K_RWS0:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('0');
      break;
    case K_RWS1:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('1');
      break;
    case K_RWS2:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('2');
      break;
    case K_RWS3:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('3');
      break;
    case K_RWS4:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('4');
      break;
    case K_RWS5:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('5');
      break;
    case K_RWS6:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('6');
      break;
    case K_RWS7:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('7');
      break;
    case K_RWS8:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('8');
      break;
    case K_RWS9:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('9');
      break;



    case K_HELLO:
      Keyboard.write('H');
      Keyboard.write('e');
      Keyboard.write('l');
      Keyboard.write('l');
      Keyboard.write('o');
      break;

    default:
      break;
  }
}

void keymap_release(int k)
{
  switch (k) {
    case K_SLASH:
      Keyboard.release('/');
      break;
    case K_ASTERISK:
      Keyboard.release('*');
      break;
    case K_MINUS:
      Keyboard.release(0xDE);
      break;
    case K_PLUS:
      Keyboard.release(0xDF);
      break;
    case K_ENTER:
      Keyboard.release(0xE0);
      break;
    case K_0:
      Keyboard.release('0');
      break;
    case K_1:
      Keyboard.release('1');
      break;
    case K_2:
      Keyboard.release('2');
      break;
    case K_3:
      Keyboard.release('3');
      break;
    case K_4:
      Keyboard.release('4');
      break;
    case K_5:
      Keyboard.release('5');
      break;
    case K_6:
      Keyboard.release('6');
      break;
    case K_7:
      Keyboard.release('7');
      break;
    case K_8:
      Keyboard.release('8');
      break;
    case K_9:
      Keyboard.release('9');
      break;
    case K_PERIOD:
      Keyboard.release('.');
      break;
    case K_LEFT:
      Keyboard.release(KEY_LEFT_ARROW);
      break;
    case K_RIGHT:
      Keyboard.release(KEY_RIGHT_ARROW);
      break;
    case K_UP:
      Keyboard.release(KEY_UP_ARROW);
      break;
    case K_DOWN:
      Keyboard.release(KEY_DOWN_ARROW);
      break;
    case K_HOME:
      Keyboard.release(KEY_HOME);
      break;
    case K_END:
      Keyboard.release(KEY_END);
      break;
    case K_PAGEUP:
      Keyboard.release(KEY_PAGE_UP);
      break;
    case K_PAGEDOWN:
      Keyboard.release(KEY_PAGE_DOWN);
      break;
    case K_INSERT:
      Keyboard.release(KEY_INSERT);
      break;
    case K_DELETE:
      Keyboard.release(KEY_DELETE);
      break;
    case K_LSHIFT:
      Keyboard.release(KEY_LEFT_SHIFT);
      break;
    case K_LCTRL:
      Keyboard.release(KEY_LEFT_CTRL);
      break;
    case K_WS1:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('1');
      break;
    case K_WS2:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('2');
      break;
    case K_WS3:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('3');
      break;
    case K_WS4:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('4');
      break;
    case K_WS5:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('5');
      break;
    case K_WS6:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('6');
      break;
    case K_WS7:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('7');
      break;
    case K_WS8:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('8');
      break;
    case K_WS9:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('9');
      break;
    case K_WS0:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release('0');
      break;
    case K_RWS0:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('0');
      break;
    case K_RWS1:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('1');
      break;
    case K_RWS2:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('2');
      break;
    case K_RWS3:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('3');
      break;
    case K_RWS4:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('4');
      break;
    case K_RWS5:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('5');
      break;
    case K_RWS6:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('6');
      break;
    case K_RWS7:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('7');
      break;
    case K_RWS8:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('8');
      break;
    case K_RWS9:
      Keyboard.release(KEY_LEFT_GUI);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release('9');
      break;



    case K_TEST:
      break;

    default:
      break;
  }
}


void setup() {
  int cnt;
  int cnt2;

#ifdef HEARTBEAT_ENABLED
  pinMode(HB_LED, OUTPUT);
#endif

  for (cnt = 0; cnt < ROWS; cnt++) {
    pinMode(strobe_pins[cnt], OUTPUT);
    digitalWrite(strobe_pins[cnt], HIGH);

    for (cnt2 = 0; cnt2 < COLUMNS; cnt2++) key_state[cnt][cnt2] = 0;
  }

  for (cnt = 0; cnt < COLUMNS; cnt++) {
    pinMode(input_pins[cnt], INPUT_PULLUP);
  }
  
  pinMode(17, OUTPUT);

}


const int DEBOUNCE_MS = 20;

bool debounce(unsigned long t_now, unsigned long t_prev) {
  unsigned long diff;

  diff = t_now - t_prev; // need to check for underflow?

  if (diff <= DEBOUNCE_MS) return true;
  else return false;
}

void loop() {
  unsigned long tick_now = millis();
  int cnt;

#ifdef HEARTBEAT_ENABLED
  if (q == 0) digitalWrite(HB_LED, LOW);
  else if (q == 128) digitalWrite(HB_LED, HIGH);
  q++;
  q &= 0xff;
  // should just make this into an unsigned char, but keeping it as int
  // in case we want to adjust heartbeat freq.
#endif

  // Turn off at least one LED
  digitalWrite(17, LOW);


  // since we use non zero to indicate pressed state, we need
  // to handle the edge case where millis() returns 0

  if (tick_now == 0) tick_now = 1;

  // every time we enter this loop, we're reading only the switches
  // on strobe_row

  if (strobe_row >= ROWS) strobe_row = 0;

  digitalWrite(strobe_pins[strobe_row], LOW);
  delay(2); // give it some time to stabilize just in case

  // We check all the switches in the row

  for (cnt = 0; cnt < COLUMNS; cnt++) {
    // ignore state change for pin if in debounce period
    if (key_state[strobe_row][cnt] != 0)
      if (debounce(tick_now, key_state[strobe_row][cnt]) == true)
        continue;

    if (digitalRead(input_pins[cnt]) == HIGH) {
      if (key_state[strobe_row][cnt] != 0) {
        keymap_release(keymap[layer][strobe_row][cnt]);
        //Keyboard.release(keymap[strobe_row][cnt]);
        key_state[strobe_row][cnt] = 0;
      }
    } else {
      if (key_state[strobe_row][cnt] == 0) {
        keymap_to_keypress(keymap[layer][strobe_row][cnt]);
        //Keyboard.press(keymap[strobe_row][cnt]);
        key_state[strobe_row][cnt] = tick_now;
      }
    }
  }

  digitalWrite(strobe_pins[strobe_row], HIGH);
  strobe_row++;
  delay(5);
}
