#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <time.h>

#define N 20 //radky
#define M 80 //sloupce

#define UP 72 //nahoru
#define DOWN 80 //dolu
#define LEFT 75 //doleva
#define RIGHT 77 //doprava

int i, j, field[N][M]; //field - herní pole kde se pohybuje had
int x, y, Gy, head, tail, game, points; // x,y = cords of snake // game = game loop aby bezela hra dokola //points = body
int a, b; // nahodne pozice pointu
int key, dir; //pohyb
int gameover;

/////////////////////////
// inicializace hada
////////////////////////

void snakeProperties() { //inicializace hada

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            field[i][j] = 0;
        }
    }

    x = N / 2; y = M / 2; Gy = y; head = 5; tail = 1, game = 0; points = 0; dir = RIGHT; //pozice hada uprostred pole, //kdyz game = 0, //smer kterzm had zacina

    for (i = 0; i < head; i++) {

        Gy++;
        field[x][Gy - head] = i + 1;
    }
    gameover = 0;
}

//////////////////////////////////
// herni pole, kde se had pohybuje
//////////////////////////////////

void printBorder() {

    for (int i = 0; i <= M + 1; i++) {  //horni radek
        printf("%c", 43);
    }
    printf("\n");


    for (int i = 0; i <= N + 1; i++) {     // stred herniho pole

        printf("%c", 43);
        for (int j = 0; j < M + 1; j++) {
            if (field[i][j] == 0) printf(" ");
            if (field[i][j] > 0 && field[i][j] != head) printf("%c", 254);
            if (field[i][j] == head) printf("%c", 233);
            if (field[i][j] == -1) printf("%c", 248);
            if (j == M) printf("%c\n", 43);
        }
    }


    for (int i = 0; i <= M + 1; i++) {    // spodni radek
        printf("%c", 43);
    }
    printf("\n");

    printf("score =");
    printf("\n");
    printf("press X to quit the game");
}


void ResetScreenPosition() {

    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void Random() { //nahodna pozice pointu v poli
    srand(time(0)); //nahodna pozice pro point 
    a = 1 + rand() % 18; //18 kvuli 0 a 20 pozici, kde by byl point v borderu herniho pole
    b = 1 + rand() % 78;

    if (points == 0 && field[a][b] == 0) { //pokud uz je jeden point nevytiskne se, to same plati kdyz je na stejne pozici jako snake
        field[a][b] = -1;
        points = 1;
    }
}

//////////////////////////////////////
// pohyb hada
//////////////////////////////////////
int Hit() { //registrace stisku klavesy
    if (_kbhit())
        return _getch();
    else
        return -1;
}


void movement() {
    key = Hit();
    //kontrola, aby had nemohl otocit zpet do sveho tela
    if ((key == LEFT && dir != RIGHT) ||
        (key == RIGHT && dir != LEFT) ||
        (key == UP && dir != DOWN) ||
        (key == DOWN && dir != UP))
        dir = key;

    if (dir == UP) { //pohyb nahoru
        x--;
        head++;
        field[x][y] = head;
        if (field[x][y] == -1) points = 0;
        if (x == 0) x = N - 1; // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale dole
    }
    if (dir == DOWN) { //pohyb dolu
        x++;
        head++;
        field[x][y] = head;
        if (field[x][y] == -1) points = 0;
        if (x == N - 1) x = 0; // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale nahore
    }
    if (dir == RIGHT) { //pohyb doprava
        y++;
        head++;
        field[x][y] = head;
        if (field[x][y] == -1) points = 0;
        if (y == M - 1) y = 0; // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale vlevo
    }
    if (dir == LEFT) { //pohyb doleva
        y--;
        head++;
        field[x][y] = head;
        if (field[x][y] == -1) points = 0;
        if (y == 0) y = M - 1; // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale vpravo
    }
}

// pohyb sipkama pres switch
/*
int flag;
void input()
{
    if (kbhit()) {
        switch (getch()) {
        case LEFT:
            flag = 1;
            break;
        case DOWN:
            flag = 2;
            break;
        case RIGHT:
            flag = 3;
            break;
        case UP:
            flag = 4;
            break;
        }
    }
}

void movement_arrows()
{
    if ((((flag == RIGHT || flag == LEFT) && (abs(dir - flag) > 2)) || ((flag == UP || flag == DOWN)) && (abs(dir - flag) > 8))) dir = flag;
    //sleep(0.01);
    switch (flag) {
    case 1: //doleva
        y--;
        head++;
        field[x][y] = head;
        break;
    case 2: //dolu
        x++;
        head++;
        field[x][y] = head;
        if (x == N - 1) x = 0;
        break;
    case 3: //doprava
        y++;
        head++;
        field[x][y] = head;
        if (y == M - 1) y = 0;
        break;
    case 4: //nahoru
        x--;
        head++;
        field[x][y] = head;
        if (x == 0) x = N - 1;
        break;
    default:
        break;
    }
}*/

void tailremover() {
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            if (field[i][j] == tail) { //konstantni odstranovani konce hada 
                field[i][j] = 0;
            }
        }
    }
    tail++; //opetovne pridavani konce hada, diky tomu zustava delka konstantni
}

void main() {

    snakeProperties();

    while (game == 0) {
        printBorder();
        ResetScreenPosition();
        Random();
        movement();
        //input();
        //movement_arrows();
        tailremover();
        //sleep(99);
    }
}
