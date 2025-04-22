#include "board.hpp"
#include "helper.hpp"
#include <raylib.h>
#include <stdexcept>

Piece CreatePiece(uint64_t bitboard, uint64_t firstMoveBitboard, PieceType type, PieceColor color, const std::string &texturePath)
{
  Piece piece;
  piece.bitboard = bitboard;
  piece.firstMoveBitboard = (type == PieceType::King || type == PieceType::Rook) ? firstMoveBitboard : 0;
  piece.type = type;
  piece.color = color;
  piece.texture = LoadTexture(texturePath.c_str());
  return piece;
}

std::array<Piece, PIECES_COUNT> InitBoard()
{
  return {
      CreatePiece(1ULL << 8 | 1ULL << 9 | 1ULL << 10 | 1ULL << 11 | 1ULL << 12 | 1ULL << 13 | 1ULL << 14 | 1ULL << 15, 0, PieceType::Pawn, PieceColor::Black, "assets/pieces/bP.svg.png"),
      CreatePiece(1ULL << 0 | 1ULL << 7, 1ULL << 0 | 1ULL << 7, PieceType::Rook, PieceColor::Black, "assets/pieces/bR.svg.png"),
      CreatePiece(1ULL << 1 | 1ULL << 6, 0, PieceType::Knight, PieceColor::Black, "assets/pieces/bN.svg.png"),
      CreatePiece(1ULL << 2 | 1ULL << 5, 0, PieceType::Bishop, PieceColor::Black, "assets/pieces/bB.svg.png"),
      CreatePiece(1ULL << 3, 0, PieceType::Queen, PieceColor::Black, "assets/pieces/bQ.svg.png"),
      CreatePiece(1ULL << 4, 1ULL << 4, PieceType::King, PieceColor::Black, "assets/pieces/bK.svg.png"),
      CreatePiece(1ULL << 48 | 1ULL << 49 | 1ULL << 50 | 1ULL << 51 | 1ULL << 52 | 1ULL << 53 | 1ULL << 54 | 1ULL << 55, 0, PieceType::Pawn, PieceColor::White, "assets/pieces/wP.svg.png"),
      CreatePiece(1ULL << 56 | 1ULL << 63, 1ULL << 56 | 1ULL << 63, PieceType::Rook, PieceColor::White, "assets/pieces/wR.svg.png"),
      CreatePiece(1ULL << 57 | 1ULL << 62, 0, PieceType::Knight, PieceColor::White, "assets/pieces/wN.svg.png"),
      CreatePiece(1ULL << 58 | 1ULL << 61, 0, PieceType::Bishop, PieceColor::White, "assets/pieces/wB.svg.png"),
      CreatePiece(1ULL << 59, 0, PieceType::Queen, PieceColor::White, "assets/pieces/wQ.svg.png"),
      CreatePiece(1ULL << 60, 1ULL << 60, PieceType::King, PieceColor::White, "assets/pieces/wK.svg.png")};
}

uint64_t DiagonalMoves(int8_t rank, int8_t file, uint64_t allPiecesBitboard, const std::array<Piece, PIECES_COUNT> &pieces, PieceColor color)
{
  uint64_t moves = 0;
  int8_t directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

  for (auto &dir : directions)
  {
    int8_t r = rank + dir[0];
    int8_t f = file + dir[1];
    while (r >= 0 && r < 8 && f >= 0 && f < 8)
    {
      uint64_t square = 1ULL << (r * 8 + f);
      if (square & allPiecesBitboard)
      {
        for (const auto &piece : pieces)
        {
          if (piece.bitboard & square && piece.color != color)
          {
            moves |= square;
          }
        }
        break;
      }
      moves |= square;
      r += dir[0];
      f += dir[1];
    }
  }
  return moves;
}

uint64_t StraightLineMoves(int8_t rank, int8_t file, uint64_t allPiecesBitboard, const std::array<Piece, PIECES_COUNT> &pieces, PieceColor color)
{
  uint64_t moves = 0;
  int8_t directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (auto &dir : directions)
  {
    int8_t r = rank + dir[0];
    int8_t f = file + dir[1];
    while (r >= 0 && r < 8 && f >= 0 && f < 8)
    {
      uint64_t square = 1ULL << (r * 8 + f);
      if (square & allPiecesBitboard)
      {
        for (const auto &piece : pieces)
        {
          if (piece.bitboard & square && piece.color != color)
          {
            moves |= square;
          }
        }
        break;
      }
      moves |= square;
      r += dir[0];
      f += dir[1];
    }
  }
  return moves;
}

uint64_t KnightMoves(int8_t rank, int8_t file, uint64_t allPiecesBitboard, const std::array<Piece, PIECES_COUNT> &pieces, PieceColor color)
{
  uint64_t moves = 0;
  int8_t offsets[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

  for (auto &offset : offsets)
  {
    int8_t r = rank + offset[0];
    int8_t f = file + offset[1];
    if (r >= 0 && r < 8 && f >= 0 && f < 8)
    {
      uint64_t square = 1ULL << (r * 8 + f);
      if (!(square & allPiecesBitboard))
      {
        moves |= square;
      }
      else
      {
        for (const auto &piece : pieces)
        {
          if (piece.bitboard & square && piece.color != color)
          {
            moves |= square;
          }
        }
      }
    }
  }
  return moves;
}

uint64_t PawnMoves(int8_t rank, int8_t file, const Piece &piece, uint64_t allPiecesBitboard)
{
  uint64_t moves = 0;
  int8_t direction = (piece.color == PieceColor::White) ? -1 : 1;

  // Single step forward
  if (!(1ULL << ((rank + direction) * 8 + file) & allPiecesBitboard))
  {
    moves |= 1ULL << ((rank + direction) * 8 + file);

    // Double step forward only if on initial rank
    if ((piece.color == PieceColor::White && rank == 6) || (piece.color == PieceColor::Black && rank == 1))
    {
      if (!(1ULL << ((rank + 2 * direction) * 8 + file) & allPiecesBitboard))
      {
        moves |= 1ULL << ((rank + 2 * direction) * 8 + file);
      }
    }
  }

  return moves;
}

uint64_t KingMoves(int8_t rank, int8_t file, uint64_t allPiecesBitboard, uint64_t captures, PieceColor color, const std::array<Piece, PIECES_COUNT> &pieces)
{
  uint64_t moves = 0;
  int8_t offsets[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

  for (auto &offset : offsets)
  {
    int8_t r = rank + offset[0];
    int8_t f = file + offset[1];
    if (r >= 0 && r < 8 && f >= 0 && f < 8)
    {
      uint64_t square = 1ULL << (r * 8 + f);
      if (!(square & allPiecesBitboard))
      {
        moves |= square;
      }
      else
      {
        for (const auto &piece : pieces)
        {
          if (piece.bitboard & square && piece.color != color)
          {
            moves |= square;
          }
        }
      }
    }
  }

  return moves;
}

uint64_t PossibleMoves(uint64_t bitboard, const Piece &piece, uint64_t allPiecesBitboard, const std::array<Piece, PIECES_COUNT> &pieces)
{
  uint64_t possibleMoves = 0;
  std::array<int8_t, 2> position = PositionFromBitboard(bitboard);
  int8_t rank = position[1];
  int8_t file = position[0];

  if (piece.type == PieceType::Pawn)
  {
    possibleMoves |= PawnMoves(rank, file, piece, allPiecesBitboard);
  }
  else if (piece.type == PieceType::Rook)
  {
    possibleMoves |= StraightLineMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::Knight)
  {
    possibleMoves |= KnightMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::Bishop)
  {
    possibleMoves |= DiagonalMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::Queen)
  {
    possibleMoves |= StraightLineMoves(rank, file, allPiecesBitboard, pieces, piece.color);
    possibleMoves |= DiagonalMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::King)
  {
    uint64_t captures = 0;
    for (const auto &otherPiece : pieces)
    {
      if (otherPiece.color != piece.color)
      {
        captures |= PossibleCaptures(otherPiece.bitboard, otherPiece, pieces, 0);
      }
    }
    possibleMoves |= KingMoves(rank, file, allPiecesBitboard, captures, piece.color, pieces);

    // Castling logic
    if (piece.bitboard & piece.firstMoveBitboard)
    {
      uint64_t opponentAttacks = 0;
      for (const auto &opponentPiece : pieces)
      {
        if (opponentPiece.color != piece.color)
        {
          opponentAttacks |= PossibleCaptures(opponentPiece.bitboard, opponentPiece, pieces, 0);
        }
      }

      // Check for kingside castling
      if ((file == 4 && rank == 0 && !(allPiecesBitboard & (1ULL << 5 | 1ULL << 6))) ||
          (file == 4 && rank == 7 && !(allPiecesBitboard & (1ULL << 61 | 1ULL << 62))))
      {
        if (!(opponentAttacks & (1ULL << 4 | 1ULL << 5 | 1ULL << 6)))
        {
          for (const auto &rook : pieces)
          {
            if (rook.type == PieceType::Rook && rook.color == piece.color && (rook.bitboard & rook.firstMoveBitboard))
            {
              if ((rook.bitboard & (1ULL << 7)) || (rook.bitboard & (1ULL << 63)))
              {
                possibleMoves |= (piece.color == PieceColor::White) ? (1ULL << 62) : (1ULL << 6);
              }
            }
          }
        }
      }

      // Check for queenside castling
      if ((file == 4 && rank == 0 && !(allPiecesBitboard & (1ULL << 1 | 1ULL << 2 | 1ULL << 3))) ||
          (file == 4 && rank == 7 && !(allPiecesBitboard & (1ULL << 57 | 1ULL << 58 | 1ULL << 59))))
      {
        if (!(opponentAttacks & (1ULL << 4 | 1ULL << 3 | 1ULL << 2)))
        {
          for (const auto &rook : pieces)
          {
            if (rook.type == PieceType::Rook && rook.color == piece.color && (rook.bitboard & rook.firstMoveBitboard))
            {
              if ((rook.bitboard & (1ULL << 0)) || (rook.bitboard & (1ULL << 56)))
              {
                possibleMoves |= (piece.color == PieceColor::White) ? (1ULL << 58) : (1ULL << 2);
              }
            }
          }
        }
      }
    }
  }

  return possibleMoves;
}

std::array<int8_t, 2> PositionFromBitboard(uint64_t bitboard)
{
  for (int8_t rank = 0; rank < 8; rank++)
  {
    for (int8_t file = 0; file < 8; file++)
    {
      if (1ULL << (rank * 8 + file) & bitboard)
      {
        return {file, rank};
      }
    }
  }

  return {0, 0};
}

uint64_t AllPiecesBitboard(const std::array<Piece, PIECES_COUNT> &pieces)
{
  uint64_t bitboard = 0;

  for (const Piece &piece : pieces)
  {
    bitboard |= piece.bitboard;
  }

  return bitboard;
}

uint64_t LegalMoves(uint64_t possibleMovesBitboard,
                    uint64_t allPiecesBitboard)
{
  return possibleMovesBitboard & ~allPiecesBitboard;
}

uint64_t PawnCaptures(int8_t rank, int8_t file, const Piece &piece, uint64_t allPiecesBitboard, uint64_t enPassantBitboard, const std::array<Piece, PIECES_COUNT> &pieces)
{
  uint64_t captures = 0;
  int8_t direction = (piece.color == PieceColor::White) ? -1 : 1;

  for (int8_t offset : {-1, 1})
  {
    int8_t targetFile = file + offset;
    if (targetFile >= 0 && targetFile < 8)
    {
      uint64_t square = 1ULL << ((rank + direction) * 8 + targetFile);
      if (square & enPassantBitboard)
      {
        captures |= square;
      }
      else if (square & allPiecesBitboard)
      {
        for (const auto &otherPiece : pieces)
        {
          if (otherPiece.bitboard & square && otherPiece.color != piece.color)
          {
            captures |= square;
          }
        }
      }
    }
  }

  return captures;
}

uint64_t PossibleCaptures(uint64_t bitboard, const Piece &piece, const std::array<Piece, PIECES_COUNT> &pieces, uint64_t enPassantBitboard)
{
  uint64_t possibleCaptures = 0;
  std::array<int8_t, 2> position = PositionFromBitboard(bitboard);
  int8_t rank = position[1];
  int8_t file = position[0];
  uint64_t allPiecesBitboard = AllPiecesBitboard(pieces);

  if (piece.type == PieceType::Pawn)
  {
    possibleCaptures |= PawnCaptures(rank, file, piece, allPiecesBitboard, enPassantBitboard, pieces);
  }
  else if (piece.type == PieceType::Rook)
  {
    possibleCaptures |= StraightLineMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::Knight)
  {
    possibleCaptures |= KnightMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::Bishop)
  {
    possibleCaptures |= DiagonalMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::Queen)
  {
    possibleCaptures |= StraightLineMoves(rank, file, allPiecesBitboard, pieces, piece.color);
    possibleCaptures |= DiagonalMoves(rank, file, allPiecesBitboard, pieces, piece.color);
  }
  else if (piece.type == PieceType::King)
  {
    possibleCaptures |= KingMoves(rank, file, allPiecesBitboard, 0, piece.color, pieces);
  }

  return possibleCaptures;
}

Piece &GetPieceAt(uint64_t bitboard, std::array<Piece, PIECES_COUNT> &pieces)
{
  for (auto &piece : pieces)
  {
    if (piece.bitboard & bitboard)
    {
      return piece;
    }
  }

  throw std::runtime_error("[GetPieceAt]: Invalid bitboard");
}

uint8_t GameOver(const std::array<Piece, PIECES_COUNT> &pieces)
{
  for (auto &piece : pieces)
  {
    if (piece.type == PieceType::King)
    {
      if (piece.bitboard == 0)
      {
        if (piece.color == PieceColor::White)
        {
          return 1;
        }
        else
        {
          return 2;
        }
      }

      uint64_t allPiecesBitboard = AllPiecesBitboard(pieces);
      if ((PossibleMoves(piece.bitboard, piece, allPiecesBitboard, pieces) &
           ~allPiecesBitboard) == 0)
      {

        uint64_t captures = 0;
        for (const auto &otherPiece : pieces)
        {
          if (otherPiece.color != piece.color &&
              otherPiece.type != PieceType::King)
          {
            for (uint8_t i = 0; i < 64; i++)
            {
              if (1ULL << i & otherPiece.bitboard)
              {
                captures |= PossibleCaptures(1ULL << i, otherPiece, pieces, 0);
              }
            }
          }
        }

        if (captures & piece.bitboard)
        {
          if (piece.color == PieceColor::White)
          {
            return 1;
          }
          else
          {
            return 2;
          }
        }
      }
    }
  }

  return 0;
}

void HandleCastling(Piece &king, uint64_t selectedBitboard, uint64_t newPositionBitboard, std::array<Piece, PIECES_COUNT> &pieces)
{
  std::array<int8_t, 2> selectedPosition = PositionFromBitboard(selectedBitboard);
  std::array<int8_t, 2> newPosition = PositionFromBitboard(newPositionBitboard);

  if (king.type == PieceType::King)
  {
    if (newPosition[0] == selectedPosition[0] - 2)
    {
      uint64_t rookBitboard = (king.color == PieceColor::White) ? 1ULL << 56 : 1ULL << 0;
      Piece &leftRook = GetPieceAt(rookBitboard, pieces);
      leftRook.bitboard = (leftRook.bitboard & ~rookBitboard) | (1ULL << (selectedPosition[1] * 8 + 3));
    }
    else if (newPosition[0] == selectedPosition[0] + 2)
    {
      uint64_t rookBitboard = (king.color == PieceColor::White) ? 1ULL << 63 : 1ULL << 7;
      Piece &rightRook = GetPieceAt(rookBitboard, pieces);
      rightRook.bitboard = (rightRook.bitboard & ~rookBitboard) | (1ULL << (selectedPosition[1] * 8 + 5));
    }
  }
}

void HandleEnPassant(Piece &pawn, uint64_t selectedBitboard, uint64_t newPositionBitboard, uint64_t &enPassantBitboard, uint64_t &enPassantedBitboard, std::array<Piece, PIECES_COUNT> &pieces)
{
  if (newPositionBitboard & enPassantBitboard)
  {
    for (Piece &piece : pieces)
    {
      if (piece.bitboard & enPassantedBitboard)
      {
        piece.bitboard = piece.bitboard & ~enPassantedBitboard;
      }
    }
  }

  enPassantBitboard = 0;

  if (pawn.type == PieceType::Pawn)
  {
    int8_t direction = (pawn.color == PieceColor::White) ? -1 : 1;
    std::array<int8_t, 2> selectedPosition = PositionFromBitboard(selectedBitboard);
    std::array<int8_t, 2> newPosition = PositionFromBitboard(newPositionBitboard);

    if (newPosition[1] == selectedPosition[1] + direction * 2)
    {
      enPassantBitboard = 1ULL << ((selectedPosition[1] + direction) * 8 + selectedPosition[0]);
      enPassantedBitboard = newPositionBitboard;
    }
  }
}
