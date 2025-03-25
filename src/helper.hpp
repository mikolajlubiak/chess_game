#pragma once
#include <bits/stdc++.h>

#define SHOW(T,V) do { T x = V; PrintBytes(#T, #V, (unsigned char*) &x, sizeof(x)); } while(0)

void PrintBitboard(uint64_t bitboard);

void PrintBytes(char * ty, char * val, unsigned char * bytes, size_t num_bytes);