// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "func.h"
#include <iostream>

using namespace std;

GLdouble line[4];
int mouseCount = 0;

void findIntersection(int sideNum, double *vertices, double *line, bool *inFlag, double *inLine) {
	// set the line & polygon's data
	Line l(line[0], line[1], line[2], line[3]);
	Polygon p(sideNum, vertices);

	cout << endl;
	cout << "Line: " << endl;
	cout << l.x0 << " " << l.y0 << " " << l.x1 << " " << l.y1 << endl;
	cout << l.vx << " " << l.vy << endl;
	cout << "Polygon: " << endl;
	for (int i = 0; i < sideNum; i++) {
		cout << p.vx[i] << " " << p.vy[i] << " " << p.a[i] << " " << p.b[i] << " " << p.c[i] << endl;
	}
	cout << endl;

	/*l.normalize();
	p.normalize();

	cout << "Line: (normalize) " << endl;
	cout << l.x0 << " " << l.y0 << " " << l.x1 << " " << l.y1 << endl;
	cout << l.vx << " " << l.vy << endl;
	cout << "Polygon: (normalize)" << endl;
	for (int i = 0; i < sideNum; i++) {
		cout << p.nvx[i] << " " << p.nvy[i] << " " << p.a[i] << " " << p.b[i] << " " << p.c[i] << endl;
	}
	cout << endl;*/

	// compute the dot and distance
	double *cross = new double[sideNum];
	double *dis = new double[sideNum];
	for (int i = 0; i < sideNum; i++) {
		cross[i] = p.cross(i, l.vx, l.vy);
		dis[i] = p.distance(i, l.x0, l.y0);
	}
	cout << "Dot: (p)" << endl;
	for (int i = 0; i < sideNum; i++) {
		cout << i << ": " << cross[i] << endl;
	}
	cout << "Distance: (q)" << endl;
	for (int i = 0; i < sideNum; i++) {
		cout << i << ": " << dis[i] << endl;
	}
	cout << endl;

	// find the intersection u1, u2
	double *r = new double[sideNum];
	for (int i = 0; i < sideNum; i++) {
		if (cross[i] != 0) {
			r[i] = dis[i] / cross[i];
		}
	}
	int u1i = -1;
	double u1 = 0;
	for (int i = 0; i < sideNum; i++) {
		if (cross[i] < 0 && u1 < r[i]) {
			u1i = i;
			u1 = r[i];
		}
	}
	int u2i = -1;
	double u2 = 1;
	for (int i = 0; i < sideNum; i++) {
		if (cross[i] > 0 && u2 > r[i]) {
			u2i = i;
			u2 = r[i];
		}
	}

	double isx, isy;
	cout << "Result:" << endl;
	cout << u1i << " " << u1 << endl;
	cout << u2i << " " << u2 << endl;
	if (u1 > u2) {
		cout << "completely outside!" << endl;
	}
	else {
		*inFlag = true;
		cout << "out -> in: ";
		if (u1i == -1) {
			cout << "no intersection!" << endl;

			inLine[0] = l.x0;
			inLine[1] = l.y0;
		}
		else {
			isx = l.x0 + u1 * l.vx;
			isy = l.y0 + u1 * l.vy;
			cout << "(" << isx << ", " << isy << "), with side " << u1i << "!" << endl;

			inLine[0] = isx;
			inLine[1] = isy;
		}
		cout << "in -> out: ";
		if (u2i == -1) {
			cout << "no intersection!" << endl;

			inLine[2] = l.x1;
			inLine[3] = l.y1;
		}
		else {
			isx = l.x0 + u2 * l.vx;
			isy = l.y0 + u2 * l.vy;
			cout << "(" << isx << ", " << isy << "), with side " << u2i << "!" << endl;

			inLine[2] = isx;
			inLine[3] = isy;
		}
	}
	cout << endl;

	delete[] cross;
	delete[] dis;
	delete[] r;
	return;
}

void getLine(GLFWwindow* window) {
	double xpos, ypos;
	int width, height;

	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &width, &height);
	cout << "getCursoPos: (" << xpos << ", " << ypos << ")" << endl;
	if (mouseCount == 1) {
		line[0] = (xpos / width) * 2 - 1;
		line[1] = -(ypos / height) * 2 + 1;
	}
	else if (mouseCount == 2) {
		line[2] = (xpos / width) * 2 - 1;
		line[3] = -(ypos / height) * 2 + 1;
	}
	cout << "getCursoPos(NDC): (" << (xpos / width) * 2 - 1 << ", " << -(ypos / height) * 2 + 1 << ")" << endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouseCount++;
		getLine(window);
	}
}

int main()
{
	/************** initialize **************/ 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "LineClipping", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	/****************************************/

	/**************** shader ****************/
	// shader code
	const GLchar *vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 in_color;\n"
		"out vec3 ex_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"	ex_color = in_color;\n"
		"}\n";
	const GLchar *fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec3 ex_color;\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(ex_color, 1.0);\n"
		"}\n";

	// vertexShader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success; // debug
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// fragmentShader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // debug
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// shaderProgram
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // debug
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/****************************************/

	// input
	int sideNum;
	GLdouble *vertices = nullptr;
	cout << "Please input the number of polygon's sides:" << endl;
	cin >> sideNum;
	vertices = new GLdouble[sideNum*2];
	cout << "Please input the vertices of polygon in clockwise: (x, y)" << endl;
	for (int i = 0; i < sideNum*2; i += 2)
		cin >> vertices[i] >> vertices[i + 1];
	/*for (int i = 0; i < sideNum*2; i += 2)
		cout << vertices[i] << " " << vertices[i + 1] << endl;*/
	cout << "Please input the 2 vertices (start & end) of line: (x, y)" << endl;
	/*for (int i = 0; i < 4; i += 2)
		cin >> line[i] >> line[i + 1];*/
	line[0] = -0.8;
	line[1] = 0;
	line[2] = 0.8;
	line[3] = 0;
	mouseCount = 2;

	// intersection
	GLdouble inLine[4];
	bool inFlag = false;

	// VAO, VBO
	GLuint polygonVBO, lineVBO, inLineVBO;
	glGenBuffers(1, &polygonVBO);
	glGenBuffers(1, &lineVBO);
	glGenBuffers(1, &inLineVBO);
	GLuint polygonVAO, lineVAO, inLineVAO;
	glGenVertexArrays(1, &polygonVAO);
	glGenVertexArrays(1, &lineVAO);
	glGenVertexArrays(1, &inLineVAO);

	glBindVertexArray(polygonVAO);
	glBindBuffer(GL_ARRAY_BUFFER, polygonVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * sideNum * 2, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_TRUE, 2 * sizeof(GLdouble), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 4, line, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_TRUE, 2 * sizeof(GLdouble), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(inLineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, inLineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 4, inLine, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_TRUE, 2 * sizeof(GLdouble), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);	

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (mouseCount >= 2) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);

			// draw polygon
			glBindVertexArray(polygonVAO);
			glVertexAttrib3f(1, 1.0, 1.0, 1.0);
			glDrawArrays(GL_LINE_LOOP, 0, sideNum);

			// draw line
			glBindVertexArray(lineVAO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 4, line, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 2, GL_DOUBLE, GL_TRUE, 2 * sizeof(GLdouble), (GLvoid*)0);
			glVertexAttrib3f(1, 0.0, 0.8, 0.0);
			glDrawArrays(GL_LINE_LOOP, 0, 2);

			// find intersection
			inFlag = false;
			findIntersection(sideNum, vertices, line, &inFlag, inLine);

			// draw inner line
			if (inFlag) {
				glBindVertexArray(inLineVAO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 4, inLine, GL_DYNAMIC_DRAW);
				glVertexAttribPointer(0, 2, GL_DOUBLE, GL_TRUE, 2 * sizeof(GLdouble), (GLvoid*)0);
				glVertexAttrib3f(1, 0.8, 0.0, 0.0);
				glDrawArrays(GL_LINE_LOOP, 0, 2);
			}

			glBindVertexArray(0);

			glfwSwapBuffers(window);

			mouseCount = 0;
		}
	}

	glfwTerminate();
	delete[] vertices;
	return 0;
}