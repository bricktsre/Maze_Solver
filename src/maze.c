/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrixOperations.h"
#include "mazeGenerator.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef GLfloat vec4[4];
typedef GLfloat mat4[16];
typedef GLfloat vec2[2];

int num_vertices = 0;
GLfloat angle = 0.0;
GLfloat theta = 0.01;
GLuint model_view_location, projection_location, ctm_location;
mat4 model_view, projection, ctm;
vec4 cube_vertices[36] = {
	{0.0,1.0,0.0,1.0},{0.0,0.0,0.0,1.0},{1.0,0.0,0.0,1.0},
	{0.0,1.0,0.0,1.0},{1.0,0.0,0.0,1.0},{1.0,1.0,0.0,1.0},
	{1.0,1.0,0.0,1.0},{1.0,0.0,0.0,1.0},{1.0,0.0,-1.0,1.0},
	{1.0,1.0,0.0,1.0},{1.0,0.0,-1.0,1.0},{1.0,1.0,-1.0,1.0},
	{1.0,1.0,-1.0,1.0},{1.0,0.0,-1.0,1.0},{0.0,0.0,-1.0,1.0},
	{1.0,1.0,-1.0,1.0},{0.0,0.0,-1.0,1.0},{0.0,1.0,-1.0,1.0},
	{0.0,1.0,-1.0,1.0},{0.0,0.0,-1.0,1.0},{0.0,0.0,0.0,1.0},
	{0.0,1.0,-1.0,1.0},{0.0,0.0,0.0,1.0},{0.0,1.0,0.0,1.0},
	{0.0,1.0,0.0,1.0},{1.0,1.0,0.0,1.0},{1.0,1.0,-1.0,1.0},
	{0.0,1.0,0.0,1.0},{1.0,1.0,-1.0,1.0},{0.0,1.0,-1.0,1.0},
	{0.0,0.0,0.0,1.0},{0.0,0.0,-1.0,1.0},{1.0,0.0,-1.0,1.0},
	{0.0,0.0,0.0,1.0},{1.0,0.0,-1.0,1.0},{1.0,0.0,0.0,1.0},
};
vec2 wall_tex_coords[36] = {
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
};	
vec2 post_tex_coords[36] = {
	{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.5},{1.0,0.0},
	{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.5},{1.0,0.0},
	{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.5},{1.0,0.0},
	{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.5},{1.0,0.0},
	{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.5},{1.0,0.0},
	{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.5},{1.0,0.0},
};	
vec2 floor_tex_coords[36] = {
	{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,1.0},{0.5,0.5},
	{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,1.0},{0.5,0.5},
	{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,1.0},{0.5,0.5},
	{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,1.0},{0.5,0.5},
	{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,1.0},{0.5,0.5},
	{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,1.0},{0.5,0.5},
};	

void makeWall(vec4 *vertices, vec2 *textures, int *v_index, int *t_index, int row, int col){
	int i;
	mat4 trans, scale, copy;
	matrixTranslation((col*1)-4.0,0,row*-1,trans);
	matrixScale(1.0,1.0,0.2,scale);
	matrixMultiplication(trans,scale,copy);
	vec4 temp;
	for(i = 0; i<36; i++){
		matrixVectorMultiplication(copy,cube_vertices[i],temp);
		vectorCopy(temp, vertices[*v_index]);
		textures[*t_index][0] = wall_tex_coords[i][0];
		textures[*t_index][1] = wall_tex_coords[i][1];
		(*v_index)++;
		(*t_index)++;
	}	
}

void makeWallRotated(vec4 *vertices, vec2 *textures, int *v_index, int *t_index, int row, int col){
	int i;
	mat4 trans, scale, rotate, copy, temp1;
	matrixTranslation((col*1)-3.8,0.0,row*-1,trans);
	matrixRotateY(M_PI/2,rotate);
	matrixMultiplication(trans,rotate,temp1);
	matrixScale(1.0,1.0,0.2,scale);
	matrixMultiplication(temp1,scale,copy);
	vec4 temp;
	for(i = 0; i<36; i++){
		matrixVectorMultiplication(copy,cube_vertices[i],temp);
		vectorCopy(temp, vertices[*v_index]);
		textures[*t_index][0] = wall_tex_coords[i][0];
		textures[*t_index][1] = wall_tex_coords[i][1];
		(*v_index)++;
		(*t_index)++;
	}	
}

void makePost(vec4 *vertices, vec2 *textures, int *v_index, int *t_index, int row, int col){
	int i;
	mat4 trans, scale, copy;
	matrixTranslation((col*1)-4.05,0.0,(row*-1)+0.05,trans);
	matrixScale(0.3,1.1,0.3,scale);
	matrixMultiplication(trans,scale,copy);
	vec4 temp;
	for(i = 0; i<36; i++){
		matrixVectorMultiplication(copy,cube_vertices[i],temp);
		vectorCopy(temp, vertices[*v_index]);
		textures[*t_index][0] = post_tex_coords[i][0];
		textures[*t_index][1] = post_tex_coords[i][1];
		(*v_index)++;
		(*t_index)++;
	}	

}
void makeFloor(vec4 *vertices, vec2 *textures, int *v_index, int *t_index){
	int i;
	mat4 trans, scale, copy;
	matrixTranslation(-6,-0.25,2.0,trans);
	matrixScale(12.0,0.25,12.0,scale);
	matrixMultiplication(trans,scale,copy);
	vec4 temp;
	for(i = 0; i<36; i++){
		matrixVectorMultiplication(copy,cube_vertices[i],temp);
		vectorCopy(temp, vertices[*v_index]);
		textures[*t_index][0] = floor_tex_coords[i][0];
		textures[*t_index][1] = floor_tex_coords[i][1];
		(*v_index)++;
		(*t_index)++;
	}	

}

void fill(int maze[][17], vec4 *vertices, vec2 *textures) {
	int v_index = 0, t_index = 0, row = 0, col=0, i, j;
	for(i=0;i<17;i++){
		for(j=0;j<17;j++){
			int square = maze[i][j];
			//fprintf(stderr,"val: %d row: %d col: %d\n", square, i/2, j/2);
			if(square == 0) continue;
			if(square == 2) {
				makePost(vertices,textures,&v_index,&t_index,i/2,j/2);
			} if(square == 1){
				makeWall(vertices,textures,&v_index,&t_index,i/2,j/2);
				col++;
			} if(square == 3){
				makeWallRotated(vertices,textures,&v_index,&t_index,i/2,j/2);
				col++;
			} if(col == 8){
				row++;
				col = 0;
			}
		}
	}
	makeFloor(vertices,textures,&v_index,&t_index);
}

void init(void)
{
	int maze[17][17];
	makeMaze(maze);
	num_vertices = 5796;
	vec4 vertices[num_vertices];
	vec2 textures[num_vertices];
	fill(maze,vertices,textures);
	//vec4 colors[num_vertices];

	int width = 800;
	int height = 800;
	GLubyte my_texels[width][height][3];

	FILE *fp = fopen("p2texture04.raw", "r");
	fread(my_texels, width * height * 3, 1, fp);
	fclose(fp);

	GLuint program = initShader("vshader_ctm.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint mytex[1];
	glGenTextures(1, mytex);
	glBindTexture(GL_TEXTURE_2D, mytex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	int param;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+ sizeof(textures)/* + sizeof(colors)*/, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textures), textures); 
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	/*GLuint vColor = glGetAttribLocation(program, "vColor");
	  glEnableVertexAttribArray(vColor);
	  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
	 */
	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices)/* + sizeof(colors)*/));

	GLuint texture_location = glGetUniformLocation(program, "texture");
	glUniform1i(texture_location, 0);

	identityMatrix(model_view);
	vec4 e = {0,5.0,3.0,0.0};
	vec4 a = {0.0,0.0,-4,0.0};
	vec4 vup = {0.0,1.0,0.0,0.0};
	lookAt(e,a,vup,model_view);
	//identityMatrix(projection);
	vec4 lrb = {-0.7,0.7,-0.7,0.0};
	vec4 tnf = {0.7,-1.0,-10,0.0};
	frustum(lrb,tnf,projection);
	identityMatrix(ctm);
	model_view_location = glGetUniformLocation(program, "model_view_matrix");
	projection_location = glGetUniformLocation(program, "projection_matrix");
	ctm_location = glGetUniformLocation(program, "ctm");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDepthRange(1,0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, ctm);
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, model_view);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection);	

	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
	if(key == 'q')
		exit(0);
	if(key == 'c'){
		GLfloat i,j,k;
		scanf("%f %f %f", &i,&j,&k);
		vec4 lrb = {i,j,k,0.0};
		scanf("%f %f %f", &i,&j,&k);
		vec4 tnf = {i,j,k,0.0};
		identityMatrix(projection);
		frustum(lrb,tnf,projection);
		glutPostRedisplay();
	}

	//glutPostRedisplay();
}

void idle(void) {
	GLfloat x = cos(angle) * 7;
	GLfloat z = -4 + sin(angle) * 7;
	angle+=theta;
	vec4 e = {x,5.0,z,0.0};
	vec4 a = {0.0,0.0,-4,0.0};
	vec4 vup = {0.0,1.0,0.0,0.0};
	lookAt(e,a,vup,model_view);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1024, 1024);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Maze");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
