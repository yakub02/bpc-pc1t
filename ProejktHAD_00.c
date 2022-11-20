#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <time.h>

#define N 20 //radky
#define M 80 //sloupce

int i, j, field[N][M]; //field - herní pole kde se pohybuje had
int x, y, Gy, head, tail, game, points; // x,y = cords of snake // game = game loop aby bezela hra dokola //points = body
int a, b; // nahodne pozice pointu
void snakeProperties() { //inicializace hada

    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++) {
            field[i][j] = 0;
        }
    }

    x = N / 2; y = M / 2; Gy = y; head = 5; tail = 1, game = 0; points = 0; //pozice hada uprostred pole, //kdyz game = 0 

    for (i = 0; i < head; i++) {

        Gy++;
        field[x][Gy - head] = i + 1;
    }
}

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
}
void ResetScreenPosition() {

    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

Random() { //nahodna pozice pointu v poli
    srand(time(0)); //nahodna pozice pro point 
    a = 1 + rand() % 18; //18 kvuli 0 a 20 pozici, kde by byl point v borderu herniho pole
    b = 1 + rand() % 78;

    if (points == 0 && field[a][b] == 0) { //pokud uz je jeden point nevytiskne se, to same plati kdyz je na stejne pozici jako snake
        field[a][b] = -1;
        points = 1;
    }
 }
void main() {

    snakeProperties();

    while (game == 0) {
        printBorder();
        ResetScreenPosition();
        Random();
    }
}