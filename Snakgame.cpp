#include <bits/stdc++.h>
#include <conio.h>
using namespace std;
bool gameOver;
const int width = 40;
const int height = 20;
int x, y, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
class Food {
public:
    int x, y;
    virtual void Generate() = 0;
    virtual int GetScoreValue() = 0;
};
class NormalFood : public Food {
public:
    NormalFood() { Generate(); }
    void Generate() override {
        x = rand() % width;
        y = rand() % height;
    }
    int GetScoreValue() override {
        return 10;
    }
};
class SpecialFood : public Food {
public:
    SpecialFood() { Generate(); }
    void Generate() override {
        x = rand() % width;
        y = rand() % height;
    }
    int GetScoreValue() override {
        return 30;
    }
};
class PowerUp {
public:
    int x, y;
    bool Active;
    clock_t spawnTime;
    PowerUp() : Active(false) { 
            Generate(); 
        }
    void Generate() {
        x = rand() % width;
        y = rand() % height;
        Active = true;
        spawnTime = clock();
    }
};
Food* food;
PowerUp powerUp;
bool isSpeedReduced = false;
clock_t speedReductionStartTime;
void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    score = 0;
    nTail = 0;
    if (rand() % 5 == 0)
        food = new SpecialFood();
    else
        food = new NormalFood();
    if (rand() % 10 == 0)  
        powerUp.Generate();   
    else 
        powerUp.Active = false; 
}
void gotoxy(int x, int y) {
    cout << "\033[H";  
}
void hideCursor() {
    cout << "\033[?25l";
}
void showCursor() {
    cout << "\033[?25h"; 
}
void Draw() {
    gotoxy(0, 0);
    cout << "\033[1;37m+";
    for (int i = 0; i < width; i++) cout << "-";
    cout << "+\n";
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width + 2; j++) {
            if (j == 0 || j == width + 1)
                cout << "\033[1;37m|";
            else if (i == y && j - 1 == x)
                cout << "\033[1;32mO";
            else if (i == food->y && j - 1 == food->x) {
                if (dynamic_cast<SpecialFood*>(food))
                    cout << "\033[1;31mS";
                else
                    cout << "\033[1;33mF";
            }
            else if (i == powerUp.y && j - 1 == powerUp.x && powerUp.Active) {
                cout << "\033[1;35mP"; 
            }
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j - 1 && tailY[k] == i) {
                        cout << "\033[1;32mo";
                        print = true;
                        break;
                    }
                }
                if (!print)
                    cout << " ";
            }
        }
        cout << "\n";
    }
    cout << "\033[1;37m+";
    for (int i = 0; i < width; i++) cout << "-";
    cout << "+\n";

    cout << "\033[1;37mScore: " << score << endl;
}
void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'x': gameOver = true; break;
        }
    }
}
void Logic() {
    int prevX = tailX[0], prevY = tailY[0], prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }
    if (x == food->x && y == food->y) {
        score += food->GetScoreValue();
        
        if (nTail > 0) {
            tailX[nTail] = tailX[nTail - 1];
            tailY[nTail] = tailY[nTail - 1];
        
        }
        nTail++;
        delete food;
        if (rand() % 5 == 0)
            food = new SpecialFood();
        else
            food = new NormalFood();
    }
        if (x == powerUp.x && y == powerUp.y && powerUp.Active) {
        isSpeedReduced = true;
        speedReductionStartTime = clock();
        powerUp.Active = false;  
    }
    if (isSpeedReduced && (clock() - speedReductionStartTime) / CLOCKS_PER_SEC >= 10) {
        isSpeedReduced = false;    
    }
    if (powerUp.Active && (clock() - powerUp.spawnTime) / CLOCKS_PER_SEC >= 10) {
        powerUp.Active = false;
    }
    if (!powerUp.Active && rand() % 300 == 0) {  
        powerUp.Generate();
    }
}
void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds * CLOCKS_PER_SEC / 1000);
}
int main() {
    cout << "\033[2J\033[H";
    Setup();
    hideCursor();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        if (isSpeedReduced) {
            delay(200);  
        } 
        else {
            delay(100);  
        }       
    }
    showCursor();
    cout << "Game Over!" << endl;
    delete food;
    return 0;
}
