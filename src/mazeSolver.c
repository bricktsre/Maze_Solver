#include "mazeGenerator.h"
#include <stdio.h>

void solveMaze(int maze[][17]);
int solveRecursive(int maze[][17], int direction, char solution[], int *at, int row, int col);
void loadDown(int *direction, char solution[], int *at, int row, int col);
void loadRight(int *direction, char solution[], int *at, int row, int col);
void loadLeft(int *direction, char solution[], int *at, int row, int col);


void solveMaze(int maze[][17]){
	char solution[60];
	solution[0] = 'F';
	int at = 1;
	solveRecursive(maze,1,solution,&at,1,0);
	printf("At: %d\n",at);
	solution[at++] = 'L';
	solution[at++] = 'L';
	solution[at++] = '\0';
	printf("%s\n",solution);
}

// Direction Right = 1, Down = 2, Left = 3, Up = 0;
int solveRecursive(int maze[][17], int direction, char solution[], int *at, int row, int col){
	if(row < 0 || row >= 17 || col < 0 || col >= 17) return 0;
	if(maze[row][col] == 1 || maze[row][col] == 2 || maze[row][col] == 3) return 0;
	if(row == 15 && col == 16){
		solution[*at] = 'F';
		printf("At: %d\n",*at);
		printf("At: %d\n",*at);
		return 1;
	}
	int b =0;
	printf("Row: %d Col:%d At: %d\n", row, col, *at);
	printf("%s\n",solution);
	loadRight(&direction,solution,at);
	b = solveRecursive(maze,direction,solution,at,row,col++);
	if( b == 1) return 1;
	solution[(*at)--] = '\0';
	printf("Row: %d Col:%d At: %d\n", row, col,*at);
	printf("%s\n",solution);
	loadDown(&direction,solution,at);
	b = solveRecursive(maze,direction,solution,at,row++,col);
	if( b == 1) return 1;
	solution[(*at)--] = '\0';
	loadLeft(&direction,solution);
	b = solveRecursive(maze,direction,solution,at,row,col--);
	if( b == 1) return 1;
	solution[(*at)--] = '\0';
	return 0;
}

void loadDown(int *direction, char solution[], int *at, int row, int col){
	if(direction == 1) {
		(*col)++;
	}else if(direction == 2){
		(*row)++;
	}else if(direction == 3){
		(*col)--;
	}else{
		(*row)--;
	}
	solution[*at] = 'F';
	(*at)++;
}

void loadRight(int *direction, char solution[], int *at){
	if(*direction == 1){
		solution[*at] = 'F';
	}else if(*direction == 2){
		solution[*at] = 'L';
		*direction = 1;
	}else if(*direction == 0){
		solution[*at] = 'R';
		*direction = 1;
	}else{
		solution[*at] = 'R';
		*direction = 1;
	}
	(*at)++;
}

void loadLeft(int *direction, char solution[], int *at, int row, int col){
	if(*direction == 1){
		(*row)--;
		*direction = 0;
	}else if(*direction == 2){
		(*col)++;
		*direction = 1;
	}else if(*direction == 3){
		(*row)++;
		*direction = 2;
	}else{
		(*col)--;
		*direction = 3;
	}
	solution[*at] = 'L';
	(*at)++;
}

int main(int argc, char ** argv) {
        int maze[17][17];
        int i, j;
        for(i = 0; i < 17; i++){
                for(j = 0; j < 17; j++)
                        maze[i][j] = 0;
        }
        makeMaze(maze);
	solveMaze(maze);
}
