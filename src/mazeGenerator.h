#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
//int rows = 17;
//int 17 = 17;

void makeMaze(int maze[][17]);
void mazeInit(int maze[][17]);
void addPosts(int maze[][17]);
void mazeRecursive(int rowStart, int rowEnd, int colStart, int colEnd, int maze[][17]);
void removeWallSegments(int pointRow, int pointCol, int up, int right, int down, int left, int maze[][17]);
void removeWallSegment(int start, int end, int rowOrCol, int other, int maze[][17]);
int extendWall(int direction, int row, int col, int maze[][17]);
int pickPoint(int low, int high);
void printMaze(int maze[][17]);

#endif
