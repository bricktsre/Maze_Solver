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
vec2 tex_coords[36] = {
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
	{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.5,0.5},{0.5,0.0},
};	

void fill(vec4 *vertices) {
	int i, v_index = 0;
	mat4 trans, rotate, copy;
	matrixTranslation(-0.5,-0.5,0.0, trans);
	matrixRotateX(M_PI/4, rotate);
	matrixMultiplication(trans, rotate, copy);
	vec4 temp;
	for(i = 0; i<36; i++){
                matrixVectorMultiplication(copy,cube_vertices[i],temp);
		vectorCopy(temp, vertices[v_index]);
                v_index++;
        }
}

void init(void)
{
	num_vertices = 36;
	vec4 vertices[num_vertices];
	fill(vertices);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+ sizeof(tex_coords)/* + sizeof(colors)*/, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(tex_coords), tex_coords); 
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
	identityMatrix(projection);
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

	//glutPostRedisplay();
}

void idle(void) {
	mat4 temp, copy;
	matrixRotateY(theta, temp);
	matrixMultiplication( temp, ctm, copy);
	matrixCopy(copy, ctm);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Triangle");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
