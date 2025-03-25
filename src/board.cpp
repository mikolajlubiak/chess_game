#include "board.hpp"

std::array<Piece, 12> InitBoard()
{
    Piece blackPawn;
    blackPawn.bitboard = (uint64_t)1 << 8 | (uint64_t)1 << 9 | (uint64_t)1 << 10 | (uint64_t)1 << 11 | (uint64_t)1 << 12 | (uint64_t)1 << 13 | (uint64_t)1 << 14 | (uint64_t)1 << 15;
    blackPawn.firstMoveBitboard = blackPawn.bitboard;
    blackPawn.type = PieceType::Pawn;
    blackPawn.color = PieceColor::Black;
    blackPawn.texture = LoadTexture("../assets/pieces/bP.svg.png");

    Piece blackRook;
    blackRook.bitboard = (uint64_t)1 << 0 | (uint64_t)1 << 7;
    blackRook.type = PieceType::Rook;
    blackRook.color = PieceColor::Black;
    blackRook.texture = LoadTexture("../assets/pieces/bR.svg.png");

    Piece blackKnight;
    blackKnight.bitboard = (uint64_t)1 << 1 | (uint64_t)1 << 6;
    blackKnight.type = PieceType::Knight;
    blackKnight.color = PieceColor::Black;
    blackKnight.texture = LoadTexture("../assets/pieces/bN.svg.png");

    Piece blackBishop;
    blackBishop.bitboard = (uint64_t)1 << 2 | (uint64_t)1 << 5;
    blackBishop.type = PieceType::Bishop;
    blackBishop.color = PieceColor::Black;
    blackBishop.texture = LoadTexture("../assets/pieces/bB.svg.png");

    Piece blackQueen;
    blackQueen.bitboard = (uint64_t)1 << 3;
    blackQueen.type = PieceType::Queen;
    blackQueen.color = PieceColor::Black;
    blackQueen.texture = LoadTexture("../assets/pieces/bQ.svg.png");

    Piece blackKing;
    blackKing.bitboard = (uint64_t)1 << 4;
    blackKing.type = PieceType::King;
    blackKing.color = PieceColor::Black;
    blackKing.texture = LoadTexture("../assets/pieces/bK.svg.png");

    Piece whitePawn;
    whitePawn.bitboard = (uint64_t)1 << 48 | (uint64_t)1 << 49 | (uint64_t)1 << 50 | (uint64_t)1 << 51 | (uint64_t)1 << 52 | (uint64_t)1 << 53 | (uint64_t)1 << 54 | (uint64_t)1 << 55;
    whitePawn.firstMoveBitboard = whitePawn.bitboard;
    whitePawn.type = PieceType::Pawn;
    whitePawn.color = PieceColor::White;
    whitePawn.texture = LoadTexture("../assets/pieces/wP.svg.png");

    Piece whiteRook;
    whiteRook.bitboard = (uint64_t)1 << 56 | (uint64_t)1 << 63;
    whiteRook.type = PieceType::Rook;
    whiteRook.color = PieceColor::White;
    whiteRook.texture = LoadTexture("../assets/pieces/wR.svg.png");

    Piece whiteKnight;
    whiteKnight.bitboard = (uint64_t)1 << 57 | (uint64_t)1 << 62;
    whiteKnight.type = PieceType::Knight;
    whiteKnight.color = PieceColor::White;
    whiteKnight.texture = LoadTexture("../assets/pieces/wN.svg.png");

    Piece whiteBishop;
    whiteBishop.bitboard = (uint64_t)1 << 58 | (uint64_t)1 << 61;
    whiteBishop.type = PieceType::Bishop;
    whiteBishop.color = PieceColor::White;
    whiteBishop.texture = LoadTexture("../assets/pieces/wB.svg.png");

    Piece whiteQueen;
    whiteQueen.bitboard = (uint64_t)1 << 59;
    whiteQueen.type = PieceType::Queen;
    whiteQueen.color = PieceColor::White;
    whiteQueen.texture = LoadTexture("../assets/pieces/wQ.svg.png");

    Piece whiteKing;
    whiteKing.bitboard = (uint64_t)1 << 60;
    whiteKing.type = PieceType::King;
    whiteKing.color = PieceColor::White;
    whiteKing.texture = LoadTexture("../assets/pieces/wK.svg.png");

    return {blackPawn, blackRook, blackKnight, blackBishop, blackQueen, blackKing, whitePawn, whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing};
}

uint64_t PossibleMoves(uint64_t bitboard, const Piece &piece)
{
    uint64_t legalMoves = 0;
    std::array<uint8_t, 2> position = PositionFromBitboard(bitboard);

    switch (piece.type)
    {
    case PieceType::Pawn:
        if (bitboard & piece.firstMoveBitboard)
        {
            if (piece.color == PieceColor::White)
            {
                position[1]--;
            }
            else
            {
                position[1]++;
            }

            legalMoves = (uint64_t)1 << (position[1] * 8 + position[0]);
        }

        if (piece.color == PieceColor::White)
        {
            position[1]--;
        }
        else
        {
            position[1]++;
        }

        legalMoves |= (uint64_t)1 << (position[1] * 8 + position[0]);
        break;
    case PieceType::Rook:
        for (uint8_t rank = 0; rank < 8; rank++)
        {
            legalMoves |= (uint64_t)1 << (position[1] * 8 + rank);
        }
        for (uint8_t file = 0; file < 8; file++)
        {
            legalMoves |= (uint64_t)1 << (file * 8 + position[0]);
        }

    default:
        break;
    }

    return legalMoves;
}

std::array<uint8_t, 2> PositionFromBitboard(uint64_t bitboard)
{
    for (uint8_t rank = 0; rank < 8; rank++)
    {
        for (uint8_t file = 0; file < 8; file++)
        {
            if ((uint64_t)1 << (rank * 8 + file) & bitboard)
            {
                return {file, rank};
            }
        }
    }

    return {0, 0};
}

uint64_t AllPiecesBitboard(const std::array<Piece, 12>& pieces) {
    uint64_t bitboard = 0;

    for (const Piece &piece : pieces)
    {
        bitboard |= piece.bitboard;
    }

    return bitboard;
}

uint64_t LegalMoves(uint64_t possibleMovesBitboard, uint64_t allPiecesBitboard) {
    return possibleMovesBitboard & ~allPiecesBitboard;
}
