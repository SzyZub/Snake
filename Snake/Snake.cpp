#include "raylib.h"
#include <vector>
#include <time.h>
#include <iostream>

#define SCREENW 1280
#define SCREENH 960
#define FBIGSIZE 48
#define CELLSIZE 40

typedef enum enScene {
    EMenu = 0,
    EPlay,
    EOver
} enScene;

typedef enum enObj {
    ENone = 0,
    EWall,
    EPlayerHead,
    EPlayer,
    EFruit
} enObj;

class SnakeCell {
public:
    int posY, posX, movY, movX, wait;
    SnakeCell(int y, int x, int w) : posY(y), posX(x), wait(w) {
        movY = 0;
        movX = 0;
    }
    void _setMov(int y, int x) {
        movY = y;
        movX = x;
    }
    void _changePos() {
        if (!wait) {
            posY += movY;
            posX += movX;
        }
    }
};

class Snake {
public:
    std::vector <SnakeCell> pl;
    Snake() {
        SnakeCell temp(11, 15, pl.size());
        pl.push_back(temp);
    }
    void _setHeadMov() {
        for (int i = pl.size() - 1; i > 0; i--) {
            pl[i].movX = pl[i - 1].movX;
            pl[i].movY = pl[i - 1].movY;
        }
        switch (GetKeyPressed()) {
        case KEY_LEFT:
            if (pl[0].movX != 1) pl[0]._setMov(0, -1);
            break;
        case KEY_RIGHT:
            if (pl[0].movX != -1) pl[0]._setMov(0, 1);
            break;
        case KEY_UP:
            if (pl[0].movY != 1) pl[0]._setMov(-1, 0);
            break;
        case KEY_DOWN:
            if (pl[0].movY != -1) pl[0]._setMov(1, 0);
            break;
        }
    }
    void _movSnake() {
        for (int i = 0; i < pl.size(); i++) {
            pl[i]._changePos();
            if (pl[i].wait != 0) {
                    pl[i].wait--;
            }
        }
    }
    void _addCell(int y, int x) {
        SnakeCell temp(y, x, pl.size());
        pl.push_back(temp);
    }
};

class Fruit {
public:
    int posY, posX;
    Fruit() {
        posY = rand() % 10 + 1;
        posX = rand() % 14 + 1;
    }
    void _randPos(enObj playSpace[24][32]) {
        do {
            posY = rand() % 22 + 1;
            posX = rand() % 30 + 1;
        } while (playSpace[posY][posX] != ENone);
    }
};

class GameHandler {
    enScene flag;
    enObj playSpace[24][32];
    Snake snake;
    Fruit fruit;
public:
    GameHandler() : flag(EMenu) {
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 32; j++) {
                playSpace[i][j] = ENone;
            }
        }
        for (int i = 0; i < 32; i++) {
            playSpace[0][i] = EWall;
            playSpace[23][i] = EWall;
        }
        for (int i = 0; i < 24; i++) {
            playSpace[i][0] = EWall;
            playSpace[i][31] = EWall;
        }
    }
    void _start(){
        InitWindow(SCREENW, SCREENH, "Snake");
        SetTargetFPS(8);
        srand(time(0));
    }
    void _mainLoop() {
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(BLACK);
            switch (flag) {
            case EMenu:
                _menu();
                break;
            case EPlay:
                _play();
                break;
            case EOver:
                _over();
                break;
            }
            EndDrawing();
        }
    }
    void _menu() {
        DrawText("Press space to start playing", (SCREENW - MeasureText("Press space to start playing", FBIGSIZE)) / 2, (SCREENH - FBIGSIZE) / 2, FBIGSIZE, WHITE);
        if (IsKeyPressed(KEY_SPACE)) {
            Snake temp;
            snake = temp;
            Fruit fruitTemp;
            fruit = fruitTemp;
            flag = EPlay;
        }
    }
    void _play() {
        snake._setHeadMov();
        if (_checkCol()) {
            flag = EOver;
            return;
        }
        _playSpaceUpdt();
        snake._movSnake();
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 32; j++) {
                switch (playSpace[i][j]) {
                    case ENone:
                        break;
                    case EWall:
                        DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE, WHITE);
                        break;
                    case EPlayerHead:
                        DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE, RED);
                        break;
                    case EPlayer:
                        DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE, BLUE);
                        break;
                    case EFruit:
                        DrawRectangle(j * CELLSIZE, i * CELLSIZE, CELLSIZE, CELLSIZE, ORANGE);
                        break;
                }
            }
        }
    }
    void _playSpaceUpdt() {
        for (int i = 1; i < 23; i++) {
            for (int j = 1; j < 31; j++) {
                playSpace[i][j] = ENone;
            }
        }
        playSpace[snake.pl[0].posY][snake.pl[0].posX] = EPlayerHead;
        for (int i = 1; i < snake.pl.size(); i++) {
            playSpace[snake.pl[i].posY][snake.pl[i].posX] = EPlayer;
        }
        playSpace[fruit.posY][fruit.posX] = EFruit;
    }
    bool _checkCol() {
        int x = snake.pl[0].posX;
        int y = snake.pl[0].posY;
        if (playSpace[y][x] == EFruit) {
            snake._addCell(fruit.posY, fruit.posX);
            fruit._randPos(playSpace);
        }
        if (playSpace[y][x] == EWall || playSpace[y][x] == EPlayer) return true;
        return false;
    }
    void _over() {
        DrawText("Your score is:", (SCREENW - MeasureText("Your score is:", FBIGSIZE)) / 2, SCREENH / 2 - FBIGSIZE, FBIGSIZE, WHITE);
        DrawText(TextFormat("%d", snake.pl.size() - 1), (SCREENW - MeasureText(TextFormat("%d", snake.pl.size() - 1), FBIGSIZE)) / 2, SCREENH / 2, FBIGSIZE, WHITE);
        if (GetKeyPressed() == KEY_SPACE) {
            flag = EMenu;
        }
    }
};

int main(void)
{
    GameHandler gameHandler;
    gameHandler._start();
    gameHandler._mainLoop();
    CloseWindow();
    return 0;
}