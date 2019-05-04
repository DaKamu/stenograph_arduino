
#include <Arduino.h>

// I use the underscore to indicate which side of the board each key is on.
// These values are currently incomplete.
//
// Note: The main file should not reference these since they conflict with macros that the main file is using
//Number of dictionary entries
#define numEntries 11
namespace value {
  /*
   *    [T][P][H][*][F][P][L][T][D]
      [S][K][W][R][*][R][B][G][S][Z]
              [A][O][E][U]
   */
   //Left to right starting from top - *_L/R/M indicates Left,Right or Middle of keyboard
  uint32_t T_L   = 0b01000000000000000000000;
  uint32_t P_L   = 0b10000000000000000000;
  uint32_t H_L    = 0b00000010000000000000000;
  
  uint32_t F_R    = 0b00000000000100000000000;
  uint32_t P_R   = 0b100000000;
  uint32_t L_R    = 0b1000000;
  uint32_t T_R   = 0b10000;
  uint32_t D_R   = 0b100;

  uint32_t S_L   = 0b10000000000000000000000;
  uint32_t K_L    = 0b100000000000000000000;
  uint32_t W_L    = 0b00001000000000000000000;
  uint32_t R_L   = 0b1000000000000000;
  
  uint32_t THESTAR = 0b00000000010000000000000;
  
  uint32_t R_R   = 0b10000000000;
  uint32_t G_R   = 0b00000000000000000100000;
  uint32_t S_R   = 0b1000;
  uint32_t Z_R   = 0b10;

  uint32_t A_M    = 0b100000000000000000;
  uint32_t O_M    = 0b00000000100000000000000;
  uint32_t E_M    = 0b1000000000000;
  uint32_t U_M    = 0b1000000000;

}

// This struct acts poorly as a dictionary, but is a quick makeshift.
// If time allows, an actual dictionary may be used.
using namespace value;
struct DictionaryEntry  {
    uint32_t key;
    const char* value;
} ;
DictionaryEntry dict[] = {
  {THESTAR, "*"},
  {S_L | O_M, "so"},
  {S_L, "is"},
  {T_L | H_L, "this"},
  {W_L | O_M | G_R, "working"},
  {H_L | F_R, "?"},
  {H_L | E_M | L_R, "hello"},
  {W_L | R_L | L_R | D_R, "world"},
  {H_L | E_M | R_R, "here"},
  {A_M, "a"},
  {K_L | P_L | L_R, "example"}
};

const char* dictionary_lookup(uint32_t result) {
    for (int i = 0; i < numEntries; i++) {
        if (dict[i].key == result) 
            return dict[i].value;   
    }
    return NULL;
}
