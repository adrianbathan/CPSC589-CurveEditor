#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>

//#include "InputHandler.h"


class Geometry {

public:
	Geometry();

	GLuint drawMode;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint colourBuffer;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> colours;
	glm::mat4 modelMatrix;
	
	void makeCircle(float radius, float uInc, float x, float y);
	void makePoint(glm::vec3 v, glm::vec3 c);
	
	void addPoints(glm::vec3 v, glm::vec3 c);
	void makeLine(glm::vec3 p0,glm::vec3 p1);
	
	void clearGeometry();
};

