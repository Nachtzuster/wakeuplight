#include <inttypes.h>
#include <Arduino.h>

/*
      A
     ---
  F |   | B
     -G-
  E |   | C
     ---
      D
*/

const uint8_t bitsToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001,    // F
 // XGFEDCBA
  0b01011000,    // c
  0b01110110,    // H
  0b01110100,    // h
  0b00000100,    // i
  0b00001110,    // J
  0b00001100,    // j
  0b00111000,    // L
  0b01010100,    // n
  0b01011100,    // o
  0b01110011,    // P
  0b01010000,    // r
  0b01110000,    // t
  0b00111110,    // U
  0b00011100,    // u
  0b01000000,    // -
  0b00001000,    // _
  };

const char bitsToChar[] = {
  '0',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  'A',
  'b',
  'C',
  'd',
  'E',
  'F',

  'c',
  'H',
  'h',
  'i',
  'J',
  'j',
  'L',
  'n',
  'o',
  'P',
  'r',
  't',
  'U',
  'u',
  '-',
  '_',
  };

uint8_t encodeBitsToSegment(uint8_t bits);
char encodeBitsToChar(uint8_t bits);

void encodeBitsToCStr(uint8_t bits[], char * chars, int n);
void getRandomBits(uint8_t bits[], int n);

uint8_t encodeBitsToSegment(uint8_t bits)
{
   return bitsToSegment[bits & 0x1f];
}

char encodeBitsToChar(uint8_t bits) {
   return bitsToChar[bits & 0x1f];
}

void encodeBitsToCStr(uint8_t bits[], char * chars, int n)
{
   for (int i=0;  i<n; i++) {
      chars[i] = encodeBitsToChar(bits[i]);
   }
   chars[n] = 0;
}

// each item holds 5 bits of random data
void getRandomBits(uint8_t bits[], int n) {
   for (int i=0;  i<n; i++) {
      bits[i] = random(32);
   }
}
