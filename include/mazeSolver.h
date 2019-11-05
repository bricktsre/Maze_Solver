#ifndef MAZESOLVER_H
#define MAZESOLVER_H
void solveMaze(int maze[][17], char solution[]);
int solveRecursive(int maze[][17], int direction, char solution[], int *at, int row, int col);
void loadDown(int direction, char solution[], int *at, int *row, int *col);
void loadRight(int *direction, char solution[], int *at, int *row, int *col);
void loadLeft(int *direction, char solution[], int *at, int *row, int *col);
#endif
