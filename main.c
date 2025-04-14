#include <stdio.h>
#include "raylib.h"

#define MAZE_LAT 28
#define MAZE_INALT 31

int velocity_x = 6;
int velocity_y = 6;

char mazeInit[MAZE_INALT][MAZE_LAT] = {
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#O####.#####.##.#####.####O#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.##### ## #####.######",
    "     #.##### ## #####.#     ",
    "     #.##          ##.#     ",
    "     #.## ###__### ##.#     ",
    "######.## #      # ##.######",
    "      .   #      #   .      ",
    "######.## #      # ##.######",
    "     #.## ######## ##.#     ",
    "     #.##          ##.#     ",
    "     #.## ######## ##.#     ",
    "######.## ######## ##.######",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#.####.#####.##.#####.####.#",
    "#...##................##...#",
    "###.##.##.########.##.##.###",
    "###.##.##.########.##.##.###",
    "#......##....##....##......#",
    "#.##########.##.##########.#",
    "#.##########.##.##########.#",
    "#O.......................O.#",
    "############################"
};

void drawPacMan(Texture2D player, int x, int y);
void updatePosition(int *x, int *y);
void drawMaze(char maze[MAZE_INALT][MAZE_LAT]);

int main(void) {
    const int screenWidth = MAZE_LAT * 20;
    const int screenHeight = MAZE_INALT * 23;

    int x = 400;
    int y = 400;
 
    InitWindow(screenWidth, screenHeight, "PacMan");
    SetTargetFPS(60);

    Texture2D playerNarrow = LoadTexture("pacman.png");
    Texture2D playerWide = LoadTexture("pacman2.png");

    while (!WindowShouldClose() && !IsKeyDown(KEY_ESCAPE)) {

        updatePosition(&x,&y);

        BeginDrawing();
        ClearBackground(BLACK);
        drawMaze(mazeInit);
        drawPacMan(playerNarrow, x, y);
        drawPacMan(playerWide, x, y);
        EndDrawing();
    }

    UnloadTexture(playerNarrow);
    UnloadTexture(playerWide);
    CloseWindow();

    return 0;
}

void drawPacMan(Texture2D player, int x, int y) {
    DrawTextureV(player, (Vector2){x, y}, WHITE);
}

void updatePosition(int *x, int *y) {
    if (IsKeyDown(KEY_UP)) {
        *y -= velocity_y;
    }
    if (IsKeyDown(KEY_DOWN)) {
        *y += velocity_y;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        *x += velocity_x;
    }
    if (IsKeyDown(KEY_LEFT)) {
        *x -= velocity_x;
    }
}

// Nare collision logic si poate mod culori
void drawMaze(char maze[MAZE_INALT][MAZE_LAT]){
    for (int y = 0; y < MAZE_INALT; y++)
    {
        for (int x = 0; x < MAZE_LAT; x++)
        {
            if (maze[y][x] == '#')
            {   
                Rectangle rec = {x*20, y*20, 20, 20};
                DrawRectangleRec(rec, BLACK);
                DrawRectangleLinesEx(rec, 2, DARKBLUE);
            }
            else if (maze[y][x] == '.')
            {
                DrawCircle(x*20+10, y*20+10, 2, YELLOW);
            }
            else if (maze[y][x] == 'O')
            {
                DrawCircle(x*20+10, y*20+10, 6, ORANGE);
            }
            if (maze[y][x] == '_')
            {   
                Rectangle rec = {x*20, y*20, 20, 20};
                DrawRectangleRec(rec, BLACK);
                DrawLineEx((Vector2){x*20,y*20+10}, (Vector2){x*20+20,y*20+10}, 5, WHITE);
            }
        }
    }
}
