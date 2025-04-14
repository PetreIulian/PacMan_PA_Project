#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmake-build-debug/_deps/raylib-src/src/raylib.h"


#define MAZE_WIDTH 28
#define MAZE_HEIGHT 31

int velocity_x = 6;
int velocity_y = 6;

void drawPacMan(Texture2D player, int x, int y);
void updatePosition(int *x, int *y);

typedef struct {
    int row;
    int col;
} Vector2Int;



const char initialmaze[MAZE_HEIGHT][MAZE_WIDTH] = {
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

char maze[MAZE_HEIGHT][MAZE_WIDTH];





Vector2Int  ConvertToVector2Int(Vector2 pos) {
    Vector2Int vec2Int;
    vec2Int.row = (int)(pos.y / 20);
    vec2Int.col = (int)(pos.x / 20);
    return vec2Int;
}

Vector2Int* findShortestPath(Vector2Int start, Vector2Int target, int* pathLength) {
    Vector2Int* queue = (Vector2Int*)malloc(sizeof(Vector2Int) * MAZE_HEIGHT * MAZE_WIDTH);
    bool visited[MAZE_HEIGHT][MAZE_WIDTH];
    memset(visited, false, sizeof(visited));
    int front = 0, rear = 0;

    Vector2Int parent[MAZE_HEIGHT][MAZE_WIDTH];
    memset(parent, -1, sizeof(parent));

    queue[rear++] = start;
    visited[start.row][start.col] = true;

    while (front < rear) {
        Vector2Int current = queue[front++];
        if (current.row == target.row && current.col == target.col) {
            Vector2Int* path = (Vector2Int*)malloc(sizeof(Vector2Int) * MAZE_HEIGHT * MAZE_WIDTH);
            int length = 0;
            while (!(current.row == start.row && current.col == start.col)) {
                path[length++] = current;
                current = parent[current.row][current.col];
            }
            path[length++] = start;

            *pathLength = length;
            Vector2Int* reversedPath = (Vector2Int*)malloc(sizeof(Vector2Int) * length);
            for (int i = 0; i < length; i++) {
                reversedPath[i] = path[length - 1 - i];
            }
            free(path);
            free(queue);
            return reversedPath;
        }

        Vector2Int directions[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            Vector2Int next = {current.row + directions[i].row, current.col + directions[i].col};
            if (!visited[next.row][next.col]) {
                queue[rear++] = next;
                visited[next.row][next.col] = true;
                parent[next.row][next.col] = current;
            }
        }
    }

    free(queue);
    return NULL;
}

void moveGhost(Texture2D ghost, Vector2Int* ghostPos, Vector2Int targetPosghost, int* ghostMoveCounter, Vector2Int** path, int* pathIndex, int* pathLength, int speed) {
    if (*path == NULL || *pathIndex >= *pathLength) {
        if (*path != NULL) {
            free(*path);
        }
        *path = findShortestPath(*ghostPos, targetPosghost, pathLength);
        *pathIndex = 0;
    }

    (*ghostMoveCounter)++;
    if (*ghostMoveCounter >= speed) {
        if (*path != NULL && *pathIndex < *pathLength) {
            *ghostPos = (*path)[*pathIndex];
            (*pathIndex)++;
        }
        *ghostMoveCounter = 0;
    }
    DrawTexture(ghost, ghostPos->col * 20, ghostPos->row * 20, WHITE);
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 800;

    Vector2 pos = {(float)screenWidth / 2 + 10, (float)(screenHeight / 2 + 40)};


    int x = 400;
    int y = 400;

    InitWindow(screenWidth, screenHeight, "PacMan");
    SetTargetFPS(60);

    //GHOST INIT
    Vector2Int blinkyPos = {14, 11};

    Vector2Int scatterblinkyPos = {1, 2};

    int pathLengthblinky = 0; //how long the path to pacman is

    int pathIndexblinky = 0;

    Vector2Int *pathblinky = NULL; // the nodes of the ghost path

    int blinkyMoveCounter = 0; // how many steps the ghost has made





    Texture2D playerNarrow = LoadTexture("pacman.png");
    Texture2D playerWide = LoadTexture("pacman2.png");
    Texture2D blinky = LoadTexture("blinky1.png");

    Vector2Int targetPosblinky = ConvertToVector2Int(pos);
    int ghostblink = 0;



    while (!WindowShouldClose() && !IsKeyDown(KEY_ESCAPE)) {

        updatePosition(&x,&y);

        moveGhost(blinky, &blinkyPos, targetPosblinky, &blinkyMoveCounter, &pathblinky, &pathIndexblinky, &pathLengthblinky, 5);





        BeginDrawing();
        ClearBackground(BLACK);
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

