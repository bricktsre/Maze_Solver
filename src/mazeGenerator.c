#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mazeGenerator.h"

// Generates a random correct maze
// 0 is an empty walkable space, 1 is a wall, 2 is a post
void makeMaze(int maze[][17]) {
	mazeInit(maze);
	srand(time(0));
	mazeRecursive(0,17-1,0,17-1,maze);
	maze[1][0] = 0;
	maze[17-2][17-1] = 0;
	addPosts(maze);
	printMaze(maze);
}

void mazeInit(int maze[][17]) {
	int i, j;
	for(i = 0; i < 17; i+=17-1){
		for(j = 0; j < 17; j++)
			maze[i][j] = 1;
	}
	for(i = 0; i < 17; i+=17-1){
		for(j = 0; j < 17; j++)
			maze[j][i] = 1;
	}	
}

void addPosts(int maze[][17]) {
	int i,j;
	for(i = 0; i < 17; i++){
		for(j = 0; j < 17; j++)
			if(j%2 == 0 && i%2 == 0)
				maze[i][j] = 2;
	}
}	

void mazeRecursive(int rowStart, int rowEnd, int colStart, int colEnd, int maze[][17]) {
	if( !((rowEnd - rowStart) > 2) || !((colEnd - colStart) > 2)) return;	
	int pointRow = pickPoint(rowStart, rowEnd);
	int pointCol = pickPoint(colStart, colEnd);
	maze[pointRow][pointCol] = 1;
	//printf("Point: %i, %i\n", pointRow, pointCol);
	int up = extendWall(0, pointRow, pointCol, maze);
	int right = extendWall(1, pointRow, pointCol, maze);
	int down = extendWall(2, pointRow, pointCol, maze);
	int left = extendWall(3, pointRow, pointCol, maze);
	removeWallSegments(pointRow, pointCol, up, right, down, left, maze);
	mazeRecursive(rowStart, pointRow, colStart, pointCol, maze);
	mazeRecursive(rowStart, pointRow, pointCol, colEnd, maze);
	mazeRecursive(pointRow, rowEnd, pointCol, colEnd, maze);
	mazeRecursive(pointRow, rowEnd, colStart, pointCol, maze);
}

void removeWallSegments(int pointRow, int pointCol, int up, int right, int down, int left, int maze[][17]) {
	int skip_val = rand() % 4;
	while(1){
		if(skip_val != 0)
			removeWallSegment(up, pointRow, 1, pointCol, maze);
		if(skip_val != 1)
			removeWallSegment(pointCol, right, 0, pointRow, maze);
		if(skip_val != 2)
			removeWallSegment(pointRow, down, 1, pointCol, maze);
		if(skip_val != 3)
			removeWallSegment(left, pointCol, 0, pointRow, maze);
		break;
	}
}

// rowOrCol is a boolean, 0 = row, 1 = col
void removeWallSegment(int start, int end, int rowOrCol, int other, int maze[][17]) {
	int val = (rand() % ((end-1) - (start+1) + 1 )) + (start + 1);
	if( val % 2 == 0)
		val--;
	if(rowOrCol) {
	//printf("Removing: %i, %i\n",val,other);
		maze[val][other] = 0;
	}else{ 
	//printf("Removing: %i, %i\n",other,val);
		maze[other][val] = 0;
	}
}

// 0 = Up, 1 = Right, 2 = Down, 3 = Left
int extendWall(int direction, int row, int col, int maze[][17]) {
	//printf("Point: %i, %i\n", row, col);
	if( direction == 0){
		row--;
		while( row >= 0){
			if( maze[row][col] == 1) return row;
			maze[row][col] = 1;
			row--;
		}
	} else if( direction == 1) {
		col++;
		while( col < 17){
			if( maze[row][col] == 1) return col;
			maze[row][col] = 1;
			col++;
		}
	} else if( direction == 2) {
		row++;
		while( row < 17){
			if( maze[row][col] == 1) return row;
			maze[row][col] = 1;
			row++;
		}
	} else {
		col--;
		while( col >= 0){
			if( maze[row][col] == 1) return col;
			maze[row][col] = 1;
			col--;
		}
	}
	return 0;	
}

int pickPoint(int low, int high) {
	int val = (rand() % ((high-1) - (low+1))) + (low+1);
	if( val%2 != 0)
		val++;
	return val;
}

void printMaze(int maze[][17]) {
	int i,j;
	for(i = 0; i < 17; i++){
		for(j = 0; j < 17; j++){
			int val = maze[i][j];
			if( val == 0)
				printf(" ");
			else
				printf("%i", maze[i][j]);
		}
		printf("\n");
	}
}
/*
int main(int argc, char ** argv) {
	int maze[17][17];
	int i, j;
	for(i = 0; i < 17; i++){
		for(j = 0; j < 17; j++)
			maze[i][j] = 0;
	}
	makeMaze(maze);
}*/
