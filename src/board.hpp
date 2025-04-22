#pragma once

#include <array>
#include <cstdint>

#include <raylib.h>

constexpr uint8_t PIECES_COUNT = 12;

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

struct Piece {
  uint64_t bitboard = 0;
  uint64_t firstMoveBitboard = 0;
  PieceType type;
  PieceColor color;

  Texture2D texture;
};

std::array<Piece, PIECES_COUNT> InitBoard();

uint64_t PossibleMoves(uint64_t bitboard, const Piece &piece,
                       uint64_t allPiecesBitboard,
                       const std::array<Piece, PIECES_COUNT> &pieces);

std::array<int8_t, 2> PositionFromBitboard(uint64_t bitboard);

uint64_t AllPiecesBitboard(const std::array<Piece, PIECES_COUNT> &pieces);

uint64_t LegalMoves(uint64_t possibleMovesBitboard, uint64_t allPiecesBitboard);

uint64_t PossibleCaptures(uint64_t bitboard, const Piece &piece,
                          const std::array<Piece, PIECES_COUNT> &pieces,
                          uint64_t enPassantBitboard);

Piece &GetPieceAt(uint64_t bitboard, std::array<Piece, PIECES_COUNT> &pieces);

uint8_t GameOver(const std::array<Piece, PIECES_COUNT> &pieces);

void HandleCastling(Piece &king, uint64_t selectedBitboard,
                    uint64_t newPositionBitboard,
                    std::array<Piece, PIECES_COUNT> &pieces);
void HandleEnPassant(Piece &pawn, uint64_t selectedBitboard,
                     uint64_t newPositionBitboard, uint64_t &enPassantBitboard,
                     uint64_t &enPassantedBitboard,
                     std::array<Piece, PIECES_COUNT> &pieces);
