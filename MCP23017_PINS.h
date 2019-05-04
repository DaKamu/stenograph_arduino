
/*
 * There are two parts to this file.
 *
 * In part 1, there is a guide for the MCP23017 part that we use, along
 * with links to a datasheet and the adafruit library for it. We also
 * define GPIO pins for our use. These are GPA0-GPA7 and GPB0-GPB7.
 *
 * In part 2, because we use two MCP23017 parts, we define the stenography
 * switches and LEDs as a combination of the MCP2307 address and the pin
 * being used. At the moment, duplicates are resolved by indexing the
 * key name like `KEY_N`.
 * */

// Part 1: Overview of MCP23017
// 
// The MCP23017 communicates using the I2C protocol. The SDA and SCL are buses
// that connect all slaves to the master controller. In order for the master to
// differentiate between slaves, each slave has its own address.
// 
// The I2C scanner finds the addresses 0x20-0x27.
// The address inputs are usually wired into GND or 5.5V to set HIGH/LOW value.
// The adafruit library uses 0x20 as a common base automatically.
// 
// A2 A1 A0  Full Addr  Adafruit Addr
//  0  0  0     0x20        0
//  0  0  1     0x21        1
//  0  1  0     0x22        2
//  0  1  1     0x23        3
//  1  0  0     0x24        4
//  1  0  1     0x25        5
//  1  1  0     0x26        6
//  1  1  1     0x27        7
// 
//      --------------
//   A0 | 15      14 | 
//   A1 | 16      13 | SDA (I2C Serial Data)
//   A2 | 17      12 | SCL (I2C Serial Clock Input)
//      | 18      11 | 
//      | 19      10 | GND
//      | 20       9 | 5.5v
// GPA0 | 21       8 | GPB7
// GPA1 | 22       7 | GPB6
// GPA2 | 23       6 | GPB5
// GPA3 | 24       5 | GPB4
// GPA4 | 25       4 | GPB3
// GPA5 | 26       3 | GPB2
// GPA6 | 27       2 | GPB1
// GPA7 | 28       1 | GPB0
//      --------------
//
//  11 and 14 are NC, which stands for "No Connection"
//  18 is RESET, which I probably don't need
//  19 is INTB, which I might be interested in later
//  20 is INTA, which I might be interested in later
//
// Datasheet: http://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf
// Library: https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library

#define GPA0 0
#define GPA1 1
#define GPA2 2
#define GPA3 3
#define GPA4 4
#define GPA5 5
#define GPA6 6
#define GPA7 7
#define GPB0 8
#define GPB1 9
#define GPB2 10
#define GPB3 11
#define GPB4 12
#define GPB5 13
#define GPB6 14
#define GPB7 15

// Part 2: Steno Keyboard:
//
// [S][T][P][H][*][F][P][L][T][D]
// [S][K][W][R][*][R][B][G][S][Z]
//        [A][O][E][U]
//
// Keyboard that shows duplicates:
// (T: 1), (P: 2), (S: 3), (R: 4)
//
// [ ][1][2][ ][ ][ ][2][ ][1][ ]
// [3][ ][ ][4][ ][4][ ][ ][3][ ]
//        [ ][ ][ ][ ]
//
// Note: the duplicates are split between the left and the right.
// We will have the left duplicates be indexed by 0 and the right
// duplicates will be indexed by 1.
//

// Switch Pins for MCP23017 GPIO_0

#define R_0   0, GPA7
#define H     0, GPA6
// Yellow LED:   GPA5
#define O     0, GPA4
#define STAR  0, GPA3
#define W     0, GPA2
#define P_0   0, GPA1
#define A     0, GPA0
// No Connection GPB7
// No Connection GPB6
// No Connection GPB5
// Green LED:    GPB4
// Blue LED:     GPB3
#define T_0   0, GPB2
#define K     0, GPB1
#define S_0   0, GPB0

// Switch Pins for MCP23017 GPIO_1

#undef F /* previously defined */

#define E   1, GPA7
// Red LED:    GPA6
#define R_1 1, GPA5
#define F   1, GPA4
#define U   1, GPA3
#define B   1, GPA2
#define P_1 1, GPA1
// Yellow LED: GPA0
#define Z   1, GPB7
#define D   1, GPB6
// Blue LED:   GPB5
#define T_1 1, GPB4
#define S_1 1, GPB3
// Green LED:  GPB2
#define L   1, GPB1
#define G   1, GPB0

// LED pins
#define LED_BLUE_0    0, GPB3
#define LED_BLUE_1    1, GPB5
#define LED_GREEN_0   0, GPB4
#define LED_GREEN_1   1, GPB2
#define LED_YELLOW_0  0, GPA5
#define LED_YELLOW_1  1, GPA0
#define LED_RED       1, GPA6
