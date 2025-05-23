#include "helper.hpp"
#include <string>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include <cstdlib>
#include <raylib.h>

#include "board.hpp"

Texture2D boardTexture;
std::array<Piece, 12> pieces;

uint64_t moves = 0;
uint64_t captures = 0;
uint64_t allPiecesBitboard;
uint64_t selectedBitboard = 0;
uint64_t enPassantBitboard = 0;
uint64_t enPassantedBitboard = 0;
uint64_t checkmateBitboard = 0;
uint8_t gameOver = 0;
std::string status = "";
Vector2 textSize;
Vector2 textPosition;

Piece *selectedPiece = nullptr;

PieceColor colorsTurn = PieceColor::White;

void loop();

int main() {
  InitWindow(700, 700, "Chess");
  boardTexture = LoadTexture("assets/board.jpg");
  pieces = InitBoard();
  allPiecesBitboard = AllPiecesBitboard(pieces);

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(loop, 0, 1);
#else
  while (!WindowShouldClose()) {
    loop();
  }
#endif

  CloseWindow();
  return EXIT_SUCCESS;
}

void loop() {
  int width = GetRenderWidth();
  int height = GetRenderHeight();
  float scale = (float)height / (float)(boardTexture.height);
  float squareSize = (float)height / 8;
  Vector2 boardOffset = {(float)width / 2 - boardTexture.width * scale / 2,
                         (float)height / 2 - boardTexture.height * scale / 2};

  // Input handling
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !gameOver) {
    Vector2 mousePosition = GetMousePosition();
    int file = (mousePosition.x - boardOffset.x) / squareSize;
    int rank = (mousePosition.y - boardOffset.y) / squareSize;

    if (!selectedPiece) {
      selectedBitboard = 1ULL << (rank * 8 + file);

      for (Piece &piece : pieces) {
        if (selectedBitboard & piece.bitboard) {
          if (piece.color == colorsTurn) {
            selectedPiece = &piece;
            uint64_t possibleMoves = PossibleMoves(selectedBitboard, piece,
                                                   allPiecesBitboard, pieces);
            moves = LegalMoves(possibleMoves, allPiecesBitboard);
            captures = PossibleCaptures(selectedBitboard, piece, pieces,
                                        enPassantBitboard);
          }
        }
      }
    } else {
      uint64_t newPositionBitboard = 1ULL << (rank * 8 + file);

      if (file >= 0 && file < 8 && rank >= 0 && rank < 8 &&
          newPositionBitboard & (moves | captures)) {

        HandleCastling(*selectedPiece, selectedBitboard, newPositionBitboard,
                       pieces);
        HandleEnPassant(*selectedPiece, selectedBitboard, newPositionBitboard,
                        enPassantBitboard, enPassantedBitboard, pieces);

        for (Piece &piece : pieces) {
          if (piece.bitboard & newPositionBitboard) {
            piece.bitboard = piece.bitboard & ~newPositionBitboard;
          }
        }

        selectedPiece->bitboard =
            (selectedPiece->bitboard & ~selectedBitboard) | newPositionBitboard;
        selectedPiece->firstMoveBitboard =
            (selectedPiece->firstMoveBitboard & ~selectedBitboard);
        allPiecesBitboard = AllPiecesBitboard(pieces);

        if (colorsTurn == PieceColor::White) {
          colorsTurn = PieceColor::Black;
        } else {
          colorsTurn = PieceColor::White;
        }

        gameOver = GameOver(pieces);
      }

      selectedPiece = nullptr;
      selectedBitboard = 0;
      moves = 0;
      captures = 0;
    }
  }

  // Rendering
  if (gameOver) {
    textSize = MeasureTextEx(GetFontDefault(), status.c_str(), 100.0f, 1.0f);
    textPosition = {(width - textSize.x) / 2, (height - textSize.y) / 2};

    if (gameOver == 1) {
      status = "Black Wins";
    } else {
      status = "White Wins";
    }

    for (auto &piece : pieces) {
      if (piece.type == PieceType::King) {
        PieceColor checkmatedColor;
        if (gameOver == 1) {
          checkmatedColor = PieceColor::White;
        } else {
          checkmatedColor = PieceColor::Black;
        }
        if (piece.color == checkmatedColor) {
          checkmateBitboard = piece.bitboard;
        }
      }
    }
  }

  BeginDrawing();

  ClearBackground(RAYWHITE);
  DrawTextureEx(boardTexture, boardOffset, 0, scale, WHITE);

  for (uint8_t i = 0; i < 64; i++) {
    for (const Piece &piece : pieces) {
      if (1ULL << i & piece.bitboard) {
        Vector2 piecePosition = {squareSize * (i % 8) + boardOffset.x,
                                 squareSize * (i / 8) + boardOffset.y};
        DrawTextureEx(piece.texture, piecePosition, 0, 1.4f, WHITE);
      }
    }

    if (1ULL << i & captures) {
      Vector2 highlightPosition = {
          squareSize * (i % 8) + boardOffset.x + squareSize / 2,
          squareSize * (i / 8) + boardOffset.y + squareSize / 2};
      DrawCircleV(highlightPosition, 10.0f, ORANGE);
    }

    if (1ULL << i & moves) {
      Vector2 highlightPosition = {
          squareSize * (i % 8) + boardOffset.x + squareSize / 2,
          squareSize * (i / 8) + boardOffset.y + squareSize / 2};
      DrawCircleV(highlightPosition, 10.0f, GREEN);
    }

    if (1ULL << i & checkmateBitboard) {
      Vector2 highlightPosition = {
          squareSize * (i % 8) + boardOffset.x + squareSize / 2,
          squareSize * (i / 8) + boardOffset.y + squareSize / 2};
      DrawCircleV(highlightPosition, 10.0f, RED);
    }

    if (status != "") {
      DrawTextEx(GetFontDefault(), status.c_str(), textPosition, 100.0f, 1.0f,
                 BLACK);
    }
  }

  EndDrawing();
}
