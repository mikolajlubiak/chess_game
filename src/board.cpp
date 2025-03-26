#include "board.hpp"
#include "helper.hpp"

std::array<Piece, 12> InitBoard() {
  Piece blackPawn;
  blackPawn.bitboard = 1ULL << 8 | 1ULL << 9 | 1ULL << 10 | 1ULL << 11 |
                       1ULL << 12 | 1ULL << 13 | 1ULL << 14 | 1ULL << 15;
  blackPawn.firstMoveBitboard = blackPawn.bitboard;
  blackPawn.type = PieceType::Pawn;
  blackPawn.color = PieceColor::Black;
  blackPawn.texture = LoadTexture("../assets/pieces/bP.svg.png");

  Piece blackRook;
  blackRook.bitboard = 1ULL << 0 | 1ULL << 7;
  blackRook.type = PieceType::Rook;
  blackRook.color = PieceColor::Black;
  blackRook.texture = LoadTexture("../assets/pieces/bR.svg.png");

  Piece blackKnight;
  blackKnight.bitboard = 1ULL << 1 | 1ULL << 6;
  blackKnight.type = PieceType::Knight;
  blackKnight.color = PieceColor::Black;
  blackKnight.texture = LoadTexture("../assets/pieces/bN.svg.png");

  Piece blackBishop;
  blackBishop.bitboard = 1ULL << 2 | 1ULL << 5;
  blackBishop.type = PieceType::Bishop;
  blackBishop.color = PieceColor::Black;
  blackBishop.texture = LoadTexture("../assets/pieces/bB.svg.png");

  Piece blackQueen;
  blackQueen.bitboard = 1ULL << 3;
  blackQueen.type = PieceType::Queen;
  blackQueen.color = PieceColor::Black;
  blackQueen.texture = LoadTexture("../assets/pieces/bQ.svg.png");

  Piece blackKing;
  blackKing.bitboard = 1ULL << 4;
  blackKing.type = PieceType::King;
  blackKing.color = PieceColor::Black;
  blackKing.texture = LoadTexture("../assets/pieces/bK.svg.png");

  Piece whitePawn;
  whitePawn.bitboard = 1ULL << 48 | 1ULL << 49 | 1ULL << 50 | 1ULL << 51 |
                       1ULL << 52 | 1ULL << 53 | 1ULL << 54 | 1ULL << 55;
  whitePawn.firstMoveBitboard = whitePawn.bitboard;
  whitePawn.type = PieceType::Pawn;
  whitePawn.color = PieceColor::White;
  whitePawn.texture = LoadTexture("../assets/pieces/wP.svg.png");

  Piece whiteRook;
  whiteRook.bitboard = 1ULL << 56 | 1ULL << 63;
  whiteRook.type = PieceType::Rook;
  whiteRook.color = PieceColor::White;
  whiteRook.texture = LoadTexture("../assets/pieces/wR.svg.png");

  Piece whiteKnight;
  whiteKnight.bitboard = 1ULL << 57 | 1ULL << 62;
  whiteKnight.type = PieceType::Knight;
  whiteKnight.color = PieceColor::White;
  whiteKnight.texture = LoadTexture("../assets/pieces/wN.svg.png");

  Piece whiteBishop;
  whiteBishop.bitboard = 1ULL << 58 | 1ULL << 61;
  whiteBishop.type = PieceType::Bishop;
  whiteBishop.color = PieceColor::White;
  whiteBishop.texture = LoadTexture("../assets/pieces/wB.svg.png");

  Piece whiteQueen;
  whiteQueen.bitboard = 1ULL << 59;
  whiteQueen.type = PieceType::Queen;
  whiteQueen.color = PieceColor::White;
  whiteQueen.texture = LoadTexture("../assets/pieces/wQ.svg.png");

  Piece whiteKing;
  whiteKing.bitboard = 1ULL << 60;
  whiteKing.type = PieceType::King;
  whiteKing.color = PieceColor::White;
  whiteKing.texture = LoadTexture("../assets/pieces/wK.svg.png");

  return {blackPawn,   blackRook,   blackKnight, blackBishop,
          blackQueen,  blackKing,   whitePawn,   whiteRook,
          whiteKnight, whiteBishop, whiteQueen,  whiteKing};
}

uint64_t PossibleMoves(uint64_t bitboard, const Piece &piece,
                       uint64_t allPiecesBitboard) {
  uint64_t possibleMoves = 0;
  std::array<int8_t, 2> position = PositionFromBitboard(bitboard);

  int8_t rank = position[1];
  int8_t file = position[0];

  if (piece.type == PieceType::Pawn) {
    if (piece.color == PieceColor::White) {
      rank--;
    } else {
      rank++;
    }

    if (1ULL << (rank * 8 + file) & allPiecesBitboard) {
      return possibleMoves;
    }

    possibleMoves |= 1ULL << (rank * 8 + file);

    if (bitboard & piece.firstMoveBitboard) {
      if (piece.color == PieceColor::White) {
        rank--;
      } else {
        rank++;
      }

      if (1ULL << (rank * 8 + file) & allPiecesBitboard) {
        return possibleMoves;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);
    }
  } else if (piece.type == PieceType::Rook) {
    for (file = position[0] + 1; file < 8; file++) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (position[1] * 8 + file);
    }

    for (file = position[0] - 1; file >= 0; file--) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (position[1] * 8 + file);
    }

    for (rank = position[1] - 1; rank >= 0; rank--) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + position[0]);
    }

    for (rank = position[1] + 1; rank < 8; rank++) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + position[0]);
    }

  } else if (piece.type == PieceType::Knight) {
    if (file + 2 < 8 && rank + 1 < 8) {
      possibleMoves |= 1ULL << ((rank + 1) * 8 + file + 2);
    }

    if (file - 2 >= 0 && rank + 1 < 8) {
      possibleMoves |= 1ULL << ((rank + 1) * 8 + file - 2);
    }

    if (file + 2 < 8 && rank - 1 >= 0) {
      possibleMoves |= 1ULL << ((rank - 1) * 8 + file + 2);
    }

    if (file - 2 >= 0 && rank - 1 >= 0) {
      possibleMoves |= 1ULL << ((rank - 1) * 8 + file - 2);
    }

    if (file + 1 < 8 && rank + 2 < 8) {
      possibleMoves |= 1ULL << ((rank + 2) * 8 + file + 1);
    }

    if (file + 1 < 8 && rank - 2 >= 0) {
      possibleMoves |= 1ULL << ((rank - 2) * 8 + file + 1);
    }

    if (file - 1 >= 0 && rank + 2 < 8) {
      possibleMoves |= 1ULL << ((rank + 2) * 8 + file - 1);
    }

    if (file - 1 >= 0 && rank - 2 >= 0) {
      possibleMoves |= 1ULL << ((rank - 2) * 8 + file - 1);
    }
  } else if (piece.type == PieceType::Bishop) {
    rank = position[1] + 1;
    file = position[0] + 1;

    while (rank < 8 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank++;
      file++;
    }

    rank = position[1] + 1;
    file = position[0] - 1;

    while (rank < 8 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank++;
      file--;
    }

    rank = position[1] - 1;
    file = position[0] + 1;

    while (rank >= 0 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank--;
      file++;
    }

    rank = position[1] - 1;
    file = position[0] - 1;

    while (rank >= 0 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank--;
      file--;
    }
  }

  else if (piece.type == PieceType::Queen) {
    rank = position[1] + 1;
    file = position[0] + 1;

    while (rank < 8 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank++;
      file++;
    }

    rank = position[1] + 1;
    file = position[0] - 1;

    while (rank < 8 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank++;
      file--;
    }

    rank = position[1] - 1;
    file = position[0] + 1;

    while (rank >= 0 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank--;
      file++;
    }

    rank = position[1] - 1;
    file = position[0] - 1;

    while (rank >= 0 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + file);

      rank--;
      file--;
    }

    for (file = position[0] + 1; file < 8; file++) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (position[1] * 8 + file);
    }

    for (file = position[0] - 1; file >= 0; file--) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (position[1] * 8 + file);
    }

    for (rank = position[1] - 1; rank >= 0; rank--) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + position[0]);
    }

    for (rank = position[1] + 1; rank < 8; rank++) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleMoves |= 1ULL << (rank * 8 + position[0]);
    }
  } else if (piece.type == PieceType::King) {
    if (file + 1 < 8) {
      possibleMoves |= 1ULL << (rank * 8 + file + 1);
    }

    if (rank + 1 < 8) {
      possibleMoves |= 1ULL << ((rank + 1) * 8 + file);
    }

    if (file + 1 < 8 && rank + 1 < 8) {
      possibleMoves |= 1ULL << ((rank + 1) * 8 + file + 1);
    }

    if (file - 1 >= 0) {
      possibleMoves |= 1ULL << (rank * 8 + file - 1);
    }

    if (rank - 1 >= 0) {
      possibleMoves |= 1ULL << ((rank - 1) * 8 + file);
    }

    if (rank - 1 >= 0 && file - 1 >= 0) {
      possibleMoves |= 1ULL << ((rank - 1) * 8 + file - 1);
    }

    if (rank + 1 < 8 && file - 1 >= 0) {
      possibleMoves |= 1ULL << ((rank + 1) * 8 + file - 1);
    }

    if (rank - 1 >= 0 && file + 1 < 8) {
      possibleMoves |= 1ULL << ((rank - 1) * 8 + file + 1);
    }
  }

  return possibleMoves;
}

std::array<int8_t, 2> PositionFromBitboard(uint64_t bitboard) {
  for (int8_t rank = 0; rank < 8; rank++) {
    for (int8_t file = 0; file < 8; file++) {
      if (1ULL << (rank * 8 + file) & bitboard) {
        return {file, rank};
      }
    }
  }

  return {0, 0};
}

uint64_t AllPiecesBitboard(const std::array<Piece, 12> &pieces) {
  uint64_t bitboard = 0;

  for (const Piece &piece : pieces) {
    bitboard |= piece.bitboard;
  }

  return bitboard;
}

uint64_t LegalMoves(uint64_t possibleMovesBitboard,
                    uint64_t allPiecesBitboard) {
  return possibleMovesBitboard & ~allPiecesBitboard;
}

uint64_t PossibleCaptures(uint64_t bitboard, const Piece &piece,
                          const std::array<Piece, 12> &pieces) {
  uint64_t possibleCaptures = 0;
  std::array<int8_t, 2> position = PositionFromBitboard(bitboard);
  uint64_t allPiecesBitboard = AllPiecesBitboard(pieces);

  int8_t rank = position[1];
  int8_t file = position[0];

  if (piece.type == PieceType::Pawn) {
    if (piece.color == PieceColor::White) {
      if (rank - 1 >= 0) {
        rank--;
      }
    } else {
      if (rank + 1 < 8) {
        rank++;
      }
    }

    if (rank == position[1]) {
      return possibleCaptures;
    }

    if (file - 1 >= 0) {
      file--;
    }

    if (1ULL << (rank * 8 + file) & allPiecesBitboard) {
      for (const Piece &otherPiece : pieces) {
        if (1ULL << (rank * 8 + file) & otherPiece.bitboard &&
            piece.color != otherPiece.color) {
          possibleCaptures |= 1ULL << (rank * 8 + file);
        }
      }
    }

    if (file + 2 < 8) {
      file += 2;
    }

    if (1ULL << (rank * 8 + file) & allPiecesBitboard) {
      for (const Piece &otherPiece : pieces) {
        if (1ULL << (rank * 8 + file) & otherPiece.bitboard &&
            piece.color != otherPiece.color) {
          possibleCaptures |= 1ULL << (rank * 8 + file);
        }
      }
    }
  } else if (piece.type == PieceType::Rook) {
    for (file = position[0] + 1; file < 8; file++) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (position[1] * 8 + file);
    }

    for (file = position[0] - 1; file >= 0; file--) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (position[1] * 8 + file);
    }

    for (rank = position[1] - 1; rank >= 0; rank--) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + position[0]);
    }

    for (rank = position[1] + 1; rank < 8; rank++) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + position[0]);
    }

  } else if (piece.type == PieceType::Knight) {
    if (file + 2 < 8 && rank + 1 < 8) {
      possibleCaptures |= 1ULL << ((rank + 1) * 8 + file + 2);
    }

    if (file - 2 >= 0 && rank + 1 < 8) {
      possibleCaptures |= 1ULL << ((rank + 1) * 8 + file - 2);
    }

    if (file + 2 < 8 && rank - 1 >= 0) {
      possibleCaptures |= 1ULL << ((rank - 1) * 8 + file + 2);
    }

    if (file - 2 >= 0 && rank - 1 >= 0) {
      possibleCaptures |= 1ULL << ((rank - 1) * 8 + file - 2);
    }

    if (file + 1 < 8 && rank + 2 < 8) {
      possibleCaptures |= 1ULL << ((rank + 2) * 8 + file + 1);
    }

    if (file + 1 < 8 && rank - 2 >= 0) {
      possibleCaptures |= 1ULL << ((rank - 2) * 8 + file + 1);
    }

    if (file - 1 >= 0 && rank + 2 < 8) {
      possibleCaptures |= 1ULL << ((rank + 2) * 8 + file - 1);
    }

    if (file - 1 >= 0 && rank - 2 >= 0) {
      possibleCaptures |= 1ULL << ((rank - 2) * 8 + file - 1);
    }
  } else if (piece.type == PieceType::Bishop) {
    rank = position[1] + 1;
    file = position[0] + 1;

    while (rank < 8 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank++;
      file++;
    }

    rank = position[1] + 1;
    file = position[0] - 1;

    while (rank < 8 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank++;
      file--;
    }

    rank = position[1] - 1;
    file = position[0] + 1;

    while (rank >= 0 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank--;
      file++;
    }

    rank = position[1] - 1;
    file = position[0] - 1;

    while (rank >= 0 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank--;
      file--;
    }
  }

  else if (piece.type == PieceType::Queen) {
    rank = position[1] + 1;
    file = position[0] + 1;

    while (rank < 8 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank++;
      file++;
    }

    rank = position[1] + 1;
    file = position[0] - 1;

    while (rank < 8 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank++;
      file--;
    }

    rank = position[1] - 1;
    file = position[0] + 1;

    while (rank >= 0 && file < 8) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank--;
      file++;
    }

    rank = position[1] - 1;
    file = position[0] - 1;

    while (rank >= 0 && file >= 0) {
      uint64_t squareBitboard = 1ULL << (rank * 8 + file) & allPiecesBitboard;

      if (squareBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + file);

      rank--;
      file--;
    }

    for (file = position[0] + 1; file < 8; file++) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (position[1] * 8 + file);
    }

    for (file = position[0] - 1; file >= 0; file--) {

      uint64_t fileBitboard =
          1ULL << (position[1] * 8 + file) & allPiecesBitboard;

      if (fileBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (position[1] * 8 + file);
    }

    for (rank = position[1] - 1; rank >= 0; rank--) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + position[0]);
    }

    for (rank = position[1] + 1; rank < 8; rank++) {

      uint64_t rankBitboard =
          1ULL << (rank * 8 + position[0]) & allPiecesBitboard;

      if (rankBitboard) {
        break;
      }

      possibleCaptures |= 1ULL << (rank * 8 + position[0]);
    }
  } else if (piece.type == PieceType::King) {
    if (file + 1 < 8) {
      possibleCaptures |= 1ULL << (rank * 8 + file + 1);
    }

    if (rank + 1 < 8) {
      possibleCaptures |= 1ULL << ((rank + 1) * 8 + file);
    }

    if (file + 1 < 8 && rank + 1 < 8) {
      possibleCaptures |= 1ULL << ((rank + 1) * 8 + file + 1);
    }

    if (file - 1 >= 0) {
      possibleCaptures |= 1ULL << (rank * 8 + file - 1);
    }

    if (rank - 1 >= 0) {
      possibleCaptures |= 1ULL << ((rank - 1) * 8 + file);
    }

    if (rank - 1 >= 0 && file - 1 >= 0) {
      possibleCaptures |= 1ULL << ((rank - 1) * 8 + file - 1);
    }

    if (rank + 1 < 8 && file - 1 >= 0) {
      possibleCaptures |= 1ULL << ((rank + 1) * 8 + file - 1);
    }

    if (rank - 1 >= 0 && file + 1 < 8) {
      possibleCaptures |= 1ULL << ((rank - 1) * 8 + file + 1);
    }
  }

  return possibleCaptures;
}
