#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FOR_WINDOWS 1

#if FOR_WINDOWS == 1
#define scanf scanf_s
#define gets gets_s
#endif

int directions[4] = {
    72,  // UP
    75,  // LEFT
    77,  // RIGHT
    80   // DOWN
};
int score = 0;

void clearScreen() {
    if (FOR_WINDOWS)
        system("cls");
    else
        printf("\e[1;1H\e[2J");
}

int randomMinMax(int min, int max) { return rand() % (max + 1 - min) + min; }

int randomNum() { return rand() % 4 == 0 ? 4 : 2; }

int isTableFree(int table[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (!table[i][j]) return 1;
    return 0;
}

int canMove(int table[4][4]) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (table[i][j] == table[i][j + 1] ||
                table[i][j] == table[i + 1][j])
                return 1;
    return 0;
}

void randomCoord(int coord[2]) {
    int i;
    for (i = 0; i < 2; i++) coord[i] = randomMinMax(0, 3);
}

void newNum(int table[4][4]) {
    int coord[2], done = 0;
    do {
        randomCoord(coord);
        if (table[coord[0]][coord[1]] == 0) {
            table[coord[0]][coord[1]] = randomNum();
            done++;
        }
    } while (!done && isTableFree(table));
}

void moveNumbers(int table[4][4], int dir) {
    int i, j, k;
    switch (dir) {
        case 72:
            // UP
            for (k = 0; k < 3; k++)
                for (i = 3; i > 0; i--)
                    for (j = 0; j < 4; j++)
                        if (table[i - 1][j] == 0) {
                            table[i - 1][j] = table[i][j];
                            table[i][j] = 0;
                        }
            break;
        case 75:
            // LEFT
            for (k = 0; k < 3; k++)
                for (i = 0; i < 4; i++)
                    for (j = 3; j > 0; j--)
                        if (table[i][j - 1] == 0) {
                            table[i][j - 1] = table[i][j];
                            table[i][j] = 0;
                        }
            break;
        case 77:
            // RIGHT
            for (k = 0; k < 3; k++)
                for (i = 0; i < 4; i++)
                    for (j = 0; j < 3; j++)
                        if (table[i][j + 1] == 0) {
                            table[i][j + 1] = table[i][j];
                            table[i][j] = 0;
                        }
            break;
        case 80:
            // DOWN
            for (k = 0; k < 3; k++)
                for (i = 0; i < 3; i++)
                    for (j = 0; j < 4; j++)
                        if (table[i + 1][j] == 0) {
                            table[i + 1][j] = table[i][j];
                            table[i][j] = 0;
                        }
            break;
    }
}

int elaborateTable(int table[4][4], int dir) {
    int i, j, result[4][4];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) result[i][j] = table[i][j];
    moveNumbers(table, dir);
    switch (dir) {
        case 72:
            // UP
            for (i = 0; i < 3; i++)
                for (j = 0; j < 4; j++)
                    if (table[i][j] == table[i + 1][j]) {
                        table[i + 1][j] = 0;
                        table[i][j] *= 2;
                        score += table[i][j];
                    }
            break;
        case 75:
            // LEFT
            for (i = 0; i < 4; i++)
                for (j = 0; j < 3; j++)
                    if (table[i][j] == table[i][j + 1]) {
                        table[i][j + 1] = 0;
                        table[i][j] *= 2;
                        score += table[i][j];
                    }
            break;
        case 77:
            // RIGHT
            for (i = 0; i < 4; i++)
                for (j = 3; j > 0; j--)
                    if (table[i][j] == table[i][j - 1]) {
                        table[i][j - 1] = 0;
                        table[i][j] *= 2;
                        score += table[i][j];
                    }
            break;
        case 80:
            // DOWN
            for (i = 3; i > 0; i--)
                for (j = 0; j < 4; j++)
                    if (table[i][j] == table[i - 1][j]) {
                        table[i - 1][j] = 0;
                        table[i][j] *= 2;
                        score += table[i][j];
                    }
            break;
    }
    moveNumbers(table, dir);
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (result[i][j] != table[i][j]) return 1;
    return 0;
}

void initTable(int table[4][4]) {
    int i, j, coord[2], initNumbers[2];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) table[i][j] = 0;
    initNumbers[0] = randomNum();
    if (initNumbers[0] == 4)
        initNumbers[1] = 2;
    else
        initNumbers[1] = randomNum();
    for (i = 0; i < 2; i++) {
        randomCoord(coord);
        if (table[coord[0]][coord[1]] == 0)
            table[coord[0]][coord[1]] = initNumbers[i];
        else
            i--;
    }
}

void printTable(int table[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        printf("|");
        for (j = 0; j < 4; j++) {
            if (table[i][j] != 0) {
                printf("%4d", table[i][j]);
                system("COLOR 00");
            } else
                printf("    ");
            printf("|");
        }
        printf("\n\n");
    }
    printf("Actual score: %d\n", score);
}

int nextMove(int table[4][4], int dir) {
    if (elaborateTable(table, dir))
        newNum(table);
    else if (!isTableFree(table) && !canMove(table))
        return 1;
    printTable(table);
    return 0;
}

int main() {
    int table[4][4], dir, gameOver = 0, requestNewGame = 0, cont;
    do {
        requestNewGame = 0;
        score = 0;
        srand((unsigned int)time(0));
        clearScreen();
        initTable(table);
        printTable(table);
        do {
            do {
                dir = _getch();
                if (dir == 'n') requestNewGame = 1;
            } while (dir != directions[0] && dir != directions[1] &&
                     dir != directions[2] && dir != directions[3] &&
                     !requestNewGame);
            if (!requestNewGame) {
                clearScreen();
                gameOver = nextMove(table, dir);
            }
        } while (!gameOver && !requestNewGame);
        if (!requestNewGame) {
            printf("GAME OVER!\n\nYour score is: %d\n", score);
            system("PAUSE");
            printf("Continue? (Y/N)\n");
            cont = _getch();
        }
    } while (cont == 'y' || requestNewGame);
    if (FOR_WINDOWS) system("PAUSE");
    return 0;
}
