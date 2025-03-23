#include <bits/stdc++.h>
#include <raylib.h>
#include "board.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_MAXIMIZED | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Chess");
    MaximizeWindow();

    Texture2D boardTexture = LoadTexture("../assets/board.jpg");
    std::array<Piece, 12> pieces = InitBoard();
    uint64_t legalMoves = 0;

    uint64_t selectedBitboard = 0;
    Piece *selectedPiece = nullptr;

    while (!WindowShouldClose())
    {
        int width = GetRenderWidth();
        int height = GetRenderHeight();
        float scale = (float)height / (float)(boardTexture.height);
        float squareSize = (float)height / 8;
        Vector2 boardOffset = {(float)width / 2 - boardTexture.width * scale / 2, (float)height / 2 - boardTexture.height * scale / 2};

        // Input handling
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();
            int file = (mousePosition.x - boardOffset.x) / squareSize;
            int rank = (mousePosition.y - boardOffset.y) / squareSize;

            if (!selectedPiece)
            {
                selectedBitboard = (uint64_t)1 << (rank * 8 + file);

                for (Piece &piece : pieces)
                {
                    if (selectedBitboard & piece.bitboard)
                    {
                        selectedPiece = &piece;
                        legalMoves = LegalMoves(selectedBitboard, piece.type, piece.color);
                    }
                }
            }
            else
            {
                // Check if click is within board bounds
                if (file >= 0 && file < 8 && rank >= 0 && rank < 8)
                {
                    // Calculate new position's bitboard
                    uint64_t newPosition = (uint64_t)1 << (rank * 8 + file);

                    // Update piece position
                    selectedPiece->bitboard = (selectedPiece->bitboard & ~selectedBitboard) | newPosition;
                }

                selectedPiece = nullptr;
                selectedBitboard = 0;
                legalMoves = 0;
            }
        }

        // Rendering
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureEx(boardTexture, boardOffset, 0, scale, WHITE);

        for (uint8_t i = 0; i < 64; i++)
        {
            for (const Piece &piece : pieces)
            {
                if ((uint64_t)1 << i & piece.bitboard)
                {
                    Vector2 piecePosition = {squareSize * (i % 8) + boardOffset.x, squareSize * (i / 8) + boardOffset.y};
                    DrawTextureEx(piece.texture, piecePosition, 0, 2, WHITE);
                }
            }
            if ((uint64_t)1 << i & legalMoves)
            {
                Vector2 highlightPosition = {squareSize * (i % 8) + boardOffset.x + squareSize / 2, squareSize * (i / 8) + boardOffset.y + squareSize / 2};
                DrawCircleV(highlightPosition, 10.0f, GREEN);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
