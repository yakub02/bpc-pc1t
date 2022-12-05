#include "snake_final.h"
#include <iostream>
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


int field[N][M]; //pole kde se pohybuje had
int x, y, head, tail, game, points, score , highscore, dir; // x,y - pozice hada // game = game loop aby bezela hra dokola //points = body
char plname[20], nplname[20], cha, c; //maximalni pocet znaku

FILE* f;

void snakeVlastnosti() { //inicializace hada
    int z, i, j, px; //pomocna promenna
    FILE* hraci;
    printf("\nVitejte ve hre Had \n\n(Pokracujte stisknutim jakekoliv klavesy...)");
    hraci = fopen("tabulka.txt", "a+"); //otevreni souboru "tabulka.txt"
    getch();
    system("cls");

    printf("Zadej sve jmeno (Hra se po zadani jmena automaticky...)\n");
    scanf("%[^\n]", plname); //zadani jmena hrace
    for (j = 0; plname[j] != '\0'; j++) {
        nplname[0] = toupper(plname[0]);
        if (plname[j - 1] == ' ') {
            nplname[j] = toupper(plname[j]);
            nplname[j - 1] = plname[j - 1];
        }
        else nplname[j] = plname[j];
    }
    nplname[j] = '\0';
    fprintf(hraci, "\n");
    fclose(hraci);
    system("cls");

    f = fopen("tabulka.txt", "r");
    fscanf(f, "%d", &highscore);
    fclose(f);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            field[i][j] = 0;
        }
    }

    x = N / 2; y = M / 2; z = y; head = 5; tail = 1, game = 0; points = 0;score = 0, dir = RIGHT; //pozice hada uprostred pole, //kdyz game = 0, //smer kterzm had zacina

    for (int i = 0; i < head; i++) {

        z++;
        field[x][z - head] = i + 1;
    }
}

void print() {

    for (int i = 0; i <= M + 1; i++) {  //horni radek
        printf("%c", 43);
    }
    printf("\n");

    for (int i = 0; i < N - 1; i++) {     // stred herniho pole

        printf("%c", 43);
        for (int j = 0; j < M; j++) {
            if (field[i][j] == 0) printf(" ");
            printf("\033[0;32m"); //zmena na zelenou barvu
            if (field[i][j] > 0 && field[i][j] != head) printf("%c", 254); //vzkresleni hadova tela
            if (field[i][j] == head) printf("0"); //vzkresleni hadovy hlavy
            printf("\033[0;31m"); //zmena na cervenou barvu
            if (field[i][j] == -1) printf("%c", 248);
            printf("\033[0m"); //zmena na puvodni barvu
        }
        printf("%c\n", 43);
    }

    for (int i = 0; i <= M + 1; i++) {    // spodni radek
        printf("%c", 43);
    }
    printf("\n  Aktualni skore : %d", score);
    printf("\n  Nejvyssi skore : ");printf("\033[0;31m");printf("%d", highscore);
    printf("\033[0m"); //zmena na puvodni barvu
    printf("\n");
}

void resetovaniPozice() {

    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void Random() { //nahodna pozice pointu v poli
    int a, b;
    srand(time(0)); //nahodna pozice pro point 
    a = 1 + rand() % 18; //18 kvuli 0 a 20 pozici, kde by byl point v borderu herniho pole
    b = 1 + rand() % 78;

    if (points == 0 && field[a][b] == 0) { //pokud uz je jeden point nevytiskne se, to same plati kdyz je na stejne pozici jako snake
        field[a][b] = -1;
        points = 1;
    }
}

void konecHry() {
    printf("\a");
    Sleep(1500);
    system("cls"); //vymaze command prompt

    f = fopen("tabulka.txt", "a"); //otevre txt ve write mode
    fprintf(f, "%s %d", nplname, score);
    fclose(f);

    system("cls");
    printf("\033[0;31m"); //zmena na cervenou barvu
    printf("\n\n    GAME OVER\n\n");
    printf("    SKORE :  %d \n\n", score);
    printf("Tabulka hracu je v textovem souboru.\n");
    printf("\033[0m"); //zmena na puvodni barvu
    system("pause");
    game = 1;
}

int Hit() { //registrace stisku klavesy
    if (_kbhit())
        return _getch();
    else
        return -1;
}

//nefunknci pohyb pres sipky
void movement() {
    int key;
    key = Hit();
    //kontrola, aby had nemohl otocit zpet do sveho tela
    if ((key == LEFT && dir != RIGHT) ||
        (key == RIGHT && dir != LEFT) ||
        (key == UP && dir != DOWN) ||
        (key == DOWN && dir != UP))
        dir = key;

    if (dir == UP) { //pohyb nahoru
        x--;
        if (field[x][y] != 0 && field[x][y] != -1) konecHry();
        head++;
        if (field[x][y] == -1) points = 0, score += 5, tail -= 1;
        field[x][y] = head;
        if (x == -1) konecHry(); // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale dole
    }
    if (dir == DOWN) { //pohyb dolu
        x++;
        if (field[x][y] != 0 && field[x][y] != -1) konecHry();
        head++;
        if (field[x][y] == -1) points = 0, score += 5, tail -= 1;
        field[x][y] = head;
        if (x == N) konecHry(); // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale nahore
    }
    if (dir == RIGHT) { //pohyb doprava
        y++;
        if (field[x][y] != 0 && field[x][y] != -1) konecHry();
        head++;
        if (field[x][y] == -1) points = 0, score += 5, tail -= 1;
        field[x][y] = head;
        if (y == M - 1) konecHry(); // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale vlevo
    }
    if (dir == LEFT) { //pohyb doleva
        y--;
        if (field[x][y] != 0 && field[x][y] != -1) konecHry();
        head++;
        if (field[x][y] == -1) points = 0, score += 5, tail -= 1;
        field[x][y] = head;
        if (y == 0) konecHry(); // had nemuze prekrocit hranici hraciho pole ale objevi se na stejnem miste ale vpravo
    }

}

void tailremover() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (field[i][j] == tail) { //konstantni odstranovani konce hada 
                field[i][j] = 0;
            }
        }
    }
    tail++; //opetovne pridavani konce hada, diky tomu zustava delka konstantni
}

void main() {

    snakeVlastnosti();

    while (game == 0) {
        print();
        resetovaniPozice();
        Random();
        movement();
        tailremover();
        //sleep(99);
    }
}
