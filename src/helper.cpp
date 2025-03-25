#include "helper.hpp"

void PrintBitboard(uint64_t bitboard) {
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
}