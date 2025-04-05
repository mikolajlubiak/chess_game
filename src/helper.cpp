#include "helper.hpp"

#include <iostream>

void PrintBitboard(uint64_t bitboard) {
  std::cout << "\n\n\n";
  for (uint8_t rank = 0; rank < 8; rank++) {
    for (uint8_t file = 0; file < 8; file++) {
      if (bitboard & (1ULL << (rank * 8 + file))) {
        std::cout << "1 ";
      } else {
        std::cout << ". ";
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n\n\n";
}

void PrintBytes(char *ty, char *val, unsigned char *bytes, size_t num_bytes) {
  printf("(%*s) %*s = [ ", 15, ty, 16, val);
  for (size_t i = 0; i < num_bytes; i++) {
    printf("%*u ", 3, bytes[i]);
  }
  printf("]\n");
}
