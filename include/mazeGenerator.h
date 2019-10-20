#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
int rows = 17;
int cols = 17;

void makeMaze(int maze[][cols]);
void mazeInit(int maze[][cols]);
void addPosts(int maze[][cols]);
void mazeRecursive(int rowStart, int rowEnd, int colStart, int colEnd, int maze[][cols]);
void removeWallSegments(int pointRow, int pointCol, int up, int right, int down, int left, int maze[][cols]);
void removeWallSegment(int start, int end, int rowOrCol, int other, int maze[][cols]);
int extendWall(int direction, int row, int col, int maze[][cols]);
int pickPoint(int low, int high);
void printMaze(int maze[][cols]);

#endif
