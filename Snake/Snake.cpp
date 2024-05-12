#include "raylib.h"
#include <vector>
#include <time.h>

#define SCREENW 1280
#define SCREENH 960
#define FBIGSIZE 48
#define CELLSIZE 40

typedef enum enScene {
    EMenu = 0,
    EPlay
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
    int posY, posX, movY, movX;
    SnakeCell(int y, int x) : posY(y), posX(x) {
        movY = 0;
        movX = 0;
    }
    void _setMov(int y, int x) {
        movY = y;
        movX = x;
    }
    void _changePos() {
        posY += movY;
        posX += movX;
    }
};

class Snake {
public:
    std::vector <SnakeCell> pl;
    Snake() {
        SnakeCell temp(11, 15);
        pl.push_back(temp);
    }
    void _setHeadMov() {
        switch (GetKeyPressed()) {
        case KEY_LEFT:
            pl[0]._setMov(0, -1);
            break;
        case KEY_RIGHT:
            pl[0]._setMov(0, 1);
            break;
        case KEY_UP:
            pl[0]._setMov(-1, 0);
            break;
        case KEY_DOWN:
            pl[0]._setMov(1, 0);
            break;
        }
    }
    void _movSnake() {
        for (int i = 0; i < pl.size(); i++) {
            pl[i]._changePos();
        }
    }
};

class Fruit {
public:
    int posY, posX;
    Fruit() {
        _randPos();
    }
    void _randPos() {
        posY = rand() % 22 + 1;
        posX = rand() % 31 + 1;
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
        if (_checkCol()) {
            flag = EMenu;
            return;
        }
        _playSpaceUpdt();
        snake._setHeadMov();
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
        playSpace[fruit.posY][fruit.posX] = EFruit;
        playSpace[snake.pl[0].posY][snake.pl[0].posX] = EPlayerHead;
        for (int i = 1; i < snake.pl.size(); i++) {
            playSpace[snake.pl[i].posY][snake.pl[i].posX] = EPlayer;
        }
    }
    bool _checkCol() {
        int x = snake.pl[0].posX;
        int y = snake.pl[0].posY;
        if (playSpace[y][x] == EWall) return true;
        return false;
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