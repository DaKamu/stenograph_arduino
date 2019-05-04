/*
  HELO
   TODO Title

   Description..

   Matthew Moltzau, Julia Vrooman, and Mikey

   Project status:
   The WPM calculation should maybe move to its own file.
   The Dictionary is in progress moving to its own file.
   The WPM calculation is not yet done.
   The dictionary lookup is not yet done.
   Changing the leds in response to the WPM is not yet done.
   Outputting a buzzer sound on a negative lookup is not yet done.

   We mention using the EEPROM. After the dictionary is made, we should
   save it to the EEPROM, load it, then comment out the dictionary definition
   in the files we have.

   We currently do not have an input interrupt. See this for details
   on something we can do:
   https://www.best-microcontroller-projects.com/mcp23017.html
*/

#include "Adafruit_MCP23017.h"
#include "MCP23017_PINS.h"
#include "timer.h"

// Change 'true' to 'false' to have less debug statements.
#define DEBUG if (true)

Adafruit_MCP23017 GPIO_0;
Adafruit_MCP23017 GPIO_1;

#define PIN_MODE(N, PIN, TYPE) GPIO_##N.pinMode(PIN, TYPE)
#define SET(KEY, TYPE) PIN_MODE(KEY, TYPE)

#define DIGITAL_WRITE(N, PIN, STATE) GPIO_##N.digitalWrite(PIN, STATE)
#define WRITE(KEY, STATE) DIGITAL_WRITE(KEY, STATE)

struct key {
  uint32_t pin;
  char symbol;
};

static struct key LR_Sweep[22] = {
    {S_0, 'S'},
    {T_0, 'T'}, {K, 'K'},
    {P_0, 'P'}, {W, 'W'}, {A, 'A'},
    {H, 'H'}, {R_0, 'R'}, {O, 'O'},
    {STAR, '*'}, {E, 'E'},
    {F, 'F'}, {R_1, 'R'}, {U, 'U'},
    {P_1, 'P'}, {B, 'B'},
    {L, 'L'}, {G, 'G'},
    {T_1, 'T'}, {S_1, 'S'},
    {D, 'D'}, {Z, 'Z'},
  };
void setup() {

  Serial.begin(9600);
  GPIO_0.begin(0b000);
  GPIO_1.begin(0b001);

  setup_timer_interrupts();

  int SWITCH = INPUT_PULLUP;
  int LED = OUTPUT;

  // Make all GPIO pins SWITCHes by default
  // NC pins should be set to avoid floating pins
  // LED and buzzer pins are reset below
  //
  for (int pin = GPA0; pin <= GPB7; pin++)
    GPIO_0.pinMode(pin, SWITCH);
  for (int pin = GPA0; pin <= GPB7; pin++)
    GPIO_1.pinMode(pin, SWITCH);

  SET(LED_BLUE_0, LED);
  SET(LED_BLUE_1, LED);
  SET(LED_GREEN_0, LED);
  SET(LED_GREEN_1, LED);
  SET(LED_YELLOW_0, LED);
  SET(LED_YELLOW_1, LED);
  SET(LED_RED, LED);

  // Buzzer pins
  PIN_MODE(0, GPB7, OUTPUT);
  PIN_MODE(0, GPB5, OUTPUT);

  uint16_t initState;
  DEBUG initState = GPIO_0.readGPIOAB();
  DEBUG Serial.print("DEBUG: Initial state for GPIO_0: ");
  DEBUG Serial.println(initState, BIN);
  DEBUG initState = GPIO_1.readGPIOAB();
  DEBUG Serial.print("DEBUG: Initial state for GPIO_1: ");
  DEBUG Serial.println(initState, BIN);
  //Initial state of pins when no keys are pressed
  
  DEBUG Serial.println("Setup Complete");
  WRITE(LED_BLUE_0, HIGH);
  WRITE(LED_BLUE_1, HIGH);
  WRITE(LED_GREEN_0, HIGH);
  WRITE(LED_GREEN_1, HIGH);
  WRITE(LED_YELLOW_0, HIGH);
  WRITE(LED_YELLOW_1, HIGH);
  WRITE(LED_RED, HIGH);
  for(int i; i < 22;i++){
    Serial.println(LR_Sweep[i].pin, BIN);
    Serial.println(LR_Sweep[i].symbol);
  }
}

// ARRAY HELPER FUNCTIONS

bool all_false(bool *arr, int n) {
  for (int i = 0; i < n; i++) {
    if (arr[i]) {
      return false;
    }
  }
  return true;
}

bool one_true(bool *arr, int n) {
  for (int i = 0; i < n; i++) {
    if (arr[i]) {
      return true;
    }
  }
  return false;
}

uint32_t to_int(bool *arr, int n) {
  uint32_t j = 0;
  for (int i = 0; i < n; i++)
    j = (j | arr[i]) << 1;
  return j;
}

// This is a cyclic array last 200 chord timestamps.
// It is used to calculate the WPM.
namespace stroke {
typedef long Timestamp;
Timestamp timestamps [200];
int index;
}

bool pressed = false;
void loop() {

  // Turn LEDs ON or OFF
  /*
    WRITE(LED_BLUE_0, LOW);
    WRITE(LED_BLUE_1, LOW);
    WRITE(LED_GREEN_0, LOW);
    WRITE(LED_GREEN_1, LOW);
    WRITE(LED_YELLOW_0, LOW);
    WRITE(LED_YELLOW_1, LOW);
    WRITE(LED_RED, LOW);
    //*/

  // discoverChordValues();
  
  do{ discoverKeys();} while(!pressed);
  delay(10);
  do{ discoverKeys();} while(pressed);
  delay(50); 


  //DIGITAL_WRITE(0, GPB5, LOW);

  //uint32_t result = get_chord(true);

  // Modify array used to calculate WPM
  // if result == star set last item in array to -1
  // else set current item in array to 1
  // increment index (using modulo)

  // now = milliseconds()
  // elapsed = now - 30000 milliseconds
  // clamp the result to 0
  //
  // Iterate over the WPM array
  // If array item is in range from `elapsed` to `now` we increment a counter by 1
  // We stop after we are out of range, but the -1 does not count
  // We need to be able to cycle using modulo
}

// Returns an integer representation of the chord
// Has the option to print the chord as well.
/*
uint32_t get_chord(bool doPrint) {

  // SO S- TH- WOG H-F
  //
  // [S][T][P][H][*][F][P][L][T][D]
  // [S][K][W][R][*][R][B][G][S][Z]
  //        [A][O][E][U]
  //
  //  S         O             | so
  //  S                       | is
  //     T     H              | this
  //        W   O          G  | working
  //           H     F        | ?
  //
  // so is this working?
  //
  // If there is nothing on the line star will be printed
  // If done by itself, star will delete the last word.
  // If played before a character, it will print `{>} char`
  // for some reason.
  //

  struct key {
    int device;
    int pin;
    char symbol;
  };
  //read_pins will always be true since GPIO_0/1.readGPIOAB will return a non-zero value when no keys are being pressed
  //so just_down variable below 
  bool read_pins = (GPIO_0.readGPIOAB() || GPIO_1.readGPIOAB());
  static bool pressed[22];
  static struct key LR_Sweep[22] = {
    {S_0, 'S'},
    {T_0, 'T'}, {K, 'K'},
    {P_0, 'P'}, {W, 'W'}, {A, 'A'},
    {H, 'H'}, {R_0, 'R'}, {O, 'O'},
    {STAR, '*'}, {E, 'E'},
    {F, 'F'}, {R_1, 'R'}, {U, 'U'},
    {P_1, 'P'}, {B, 'B'},
    {L, 'L'}, {G, 'G'},
    {T_1, 'T'}, {S_1, 'S'},
    {D, 'D'}, {Z, 'Z'},
  };
  Serial.println("Still Running");
  for(int i; i < 22; i++){
    Serial.println(LR_Sweep[i].symbol);
  }
  int BUTTON_PRESSED = HIGH;
  static bool prev_keys_down = true;
  bool keys_down = !read_pins;
  bool keys_just_down = ((!prev_keys_down && keys_down) ? true : false);
  prev_keys_down = keys_down;

  uint32_t result = (uint32_t) - 1;
  if (keys_just_down)
    result = to_int(pressed, 22);

  for (int i = 0; i < 22; i++) {

    Adafruit_MCP23017 *GPIO = (LR_Sweep[i].device == 0b001 ? &GPIO_1 : &GPIO_0);

    if (GPIO->digitalRead(LR_Sweep[i].pin) == BUTTON_PRESSED) {
      pressed[i] = true;
    }

    if (read_pins) continue;
    if (!keys_just_down) continue;

    if (doPrint) {
      if (pressed[i])
        Serial.print(LR_Sweep[i].symbol);
      else Serial.print(' ');
    }
    pressed[i] = 0;
  }

  if (doPrint && !read_pins && keys_just_down)
    Serial.println();

  return result;
}

// Discover Functions
// These functions are temporary to help me find information about the keys.
// I call one of the functions from the loop then press down the key to find
// its value.

void discoverChordValues() {
  uint32_t result = get_chord(true);
  if (result != (uint32_t) - 1) {
    Serial.println(result, BIN);
  }

  // The values end up like this:
  //0000000000000000100000
  //                L

}
*/
void discoverKeys() {
  pressed = false;
  bool always = false;

  uint16_t state0;
  uint16_t state1;
  static uint16_t s0 = 0;
  static uint16_t s1 = 0;
  bool a0, a1;
  state0 = GPIO_0.readGPIOAB();
  a0 = (state0 != s0);
  DEBUG if ( state0 != 0b1100000100000) {
    Serial.print("DEBUG: Changed state for GPIO_0: ");
    Serial.println(state0, BIN);

    //Print the letter
    for(int i; i < 22; i++){
      if(state0 == LR_Sweep[i].pin) Serial.println(LR_Sweep[i].symbol);
    }
    pressed = true;
    s0 = state0;
  }

  state1 = GPIO_1.readGPIOAB();
  a1 = (state1 != s1);
  DEBUG if ( state1 != 0b10010001000001) {
    Serial.print("DEBUG: Changed state for GPIO_1: ");
    Serial.println(state1, BIN);

    //Print the letter
    for(int i; i < 22;i++){
      if(state1 == LR_Sweep[i].pin) Serial.println(LR_Sweep[i].symbol);
    }    
    s1 = state1;
    pressed = true;

  }



  // To enumerate the pins for the following, I am going from left to right, starting at the top of the keyboard.
  //
  // Results:
  //
  // S 0.GPB0, T 0.GPB2, P 0.GPA1, H 0.GPA6, * 0.GPA3, F 1.GPA4, P 1.GPA1, L 1.GPB1, T 1.GPB4, D 1.GPB6
  // S (dup 0.GPB0), K 0.GPB1, W 0.GPA2, R 0.GPA7, * (dup 0.GPA3), R 1.GPA5, B 1.GPA2, G 1.GPB0, S 1.GPB3, Z 1.GPB7
  // A 0.GPA0, O 0.GPA4, E 1.GPA7, U 1.GPA3
/*
  
    int BUTTON_PRESSED = HIGH;
    if (a0 && (GPIO_0.digitalRead(GPA7) == BUTTON_PRESSED)) Serial.println("0.GPA7");
    if (a0 && (GPIO_0.digitalRead(GPA6) == BUTTON_PRESSED)) Serial.println("0.GPA6");
    if (a0 && (GPIO_0.digitalRead(GPA5) == BUTTON_PRESSED)) Serial.println("0.GPA5");
    if (a0 && (GPIO_0.digitalRead(GPA4) == BUTTON_PRESSED)) Serial.println("0.GPA4");
    if (a0 && (GPIO_0.digitalRead(GPA3) == BUTTON_PRESSED)) Serial.println("0.GPA3");
    if (a0 && (GPIO_0.digitalRead(GPA2) == BUTTON_PRESSED)) Serial.println("0.GPA2");
    if (a0 && (GPIO_0.digitalRead(GPA1) == BUTTON_PRESSED)) Serial.println("0.GPA1");
    if (a0 && (GPIO_0.digitalRead(GPA0) == BUTTON_PRESSED)) Serial.println("0.GPA0");

    if (a0 && (GPIO_0.digitalRead(GPB7) == BUTTON_PRESSED)) Serial.println("0.GPB7");
    if (a0 && (GPIO_0.digitalRead(GPB6) == BUTTON_PRESSED)) Serial.println("0.GPB6");
    if (a0 && (GPIO_0.digitalRead(GPB5) == BUTTON_PRESSED)) Serial.println("0.GPB5");
    if (a0 && (GPIO_0.digitalRead(GPB4) == BUTTON_PRESSED)) Serial.println("0.GPB4");
    if (a0 && (GPIO_0.digitalRead(GPB3) == BUTTON_PRESSED)) Serial.println("0.GPB3");
    if (a0 && (GPIO_0.digitalRead(GPB2) == BUTTON_PRESSED)) Serial.println("0.GPB2");
    if (a0 && (GPIO_0.digitalRead(GPB1) == BUTTON_PRESSED)) Serial.println("0.GPB1");
    if (a0 && (GPIO_0.digitalRead(GPB0) == BUTTON_PRESSED)) Serial.println("0.GPB0");

    if (a1 && (GPIO_1.digitalRead(GPA7) == BUTTON_PRESSED)) Serial.println("1.GPA7");
    if (a1 && (GPIO_1.digitalRead(GPA6) == BUTTON_PRESSED)) Serial.println("1.GPA6");
    if (a1 && (GPIO_1.digitalRead(GPA5) == BUTTON_PRESSED)) Serial.println("1.GPA5");
    if (a1 && (GPIO_1.digitalRead(GPA4) == BUTTON_PRESSED)) Serial.println("1.GPA4");
    if (a1 && (GPIO_1.digitalRead(GPA3) == BUTTON_PRESSED)) Serial.println("1.GPA3");
    if (a1 && (GPIO_1.digitalRead(GPA2) == BUTTON_PRESSED)) Serial.println("1.GPA2");
    if (a1 && (GPIO_1.digitalRead(GPA1) == BUTTON_PRESSED)) Serial.println("1.GPA1");
    if (a1 && (GPIO_1.digitalRead(GPA0) == BUTTON_PRESSED)) Serial.println("1.GPA0");

    if (a1 && (GPIO_1.digitalRead(GPB7) == BUTTON_PRESSED)) Serial.println("1.GPB7");
    if (a1 && (GPIO_1.digitalRead(GPB6) == BUTTON_PRESSED)) Serial.println("1.GPB6");
    if (a1 && (GPIO_1.digitalRead(GPB5) == BUTTON_PRESSED)) Serial.println("1.GPB5");
    if (a1 && (GPIO_1.digitalRead(GPB4) == BUTTON_PRESSED)) Serial.println("1.GPB4");
    if (a1 && (GPIO_1.digitalRead(GPB3) == BUTTON_PRESSED)) Serial.println("1.GPB3");
    if (a1 && (GPIO_1.digitalRead(GPB2) == BUTTON_PRESSED)) Serial.println("1.GPB2");
    if (a1 && (GPIO_1.digitalRead(GPB1) == BUTTON_PRESSED)) Serial.println("1.GPB1");
    if (a1 && (GPIO_1.digitalRead(GPB0) == BUTTON_PRESSED)) Serial.println("1.GPB0");
*/ 

}
