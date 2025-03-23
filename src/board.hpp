#pragma once

#include <bits/stdc++.h>
#include <raylib.h>

enum PieceType {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
};

enum PieceColor {
    White,
    Black,
};

struct Piece
{
    uint64_t bitboard = 0;
    PieceType type;
    PieceColor color;

    Texture2D texture;
};

std::array<Piece, 12> InitBoard();

uint64_t LegalMoves(uint64_t bitboard, PieceType type, PieceColor color);

std::array<uint8_t, 2> PositionFromBitboard(uint64_t bitboard);
