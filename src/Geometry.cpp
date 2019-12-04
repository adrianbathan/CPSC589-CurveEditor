#include "Geometry.h"
#include <cmath>

Geometry::Geometry() {
	drawMode = GL_TRIANGLES;
	vao = 0;
	vertexBuffer = 0;
	colourBuffer = 0;
	modelMatrix = glm::mat4(1.f);
}

void Geometry::makeCircle(float radius, float uInc, float x, float y) {
//	Geometry circle;
	for (double u=0.0; u<=2*M_PI; u+=uInc) {
			verts.push_back(radius * glm::vec3(cos(u), sin(u), 0) + glm::vec3(x, y, 0));
			colours.push_back(glm::vec3(0.f, 0.f, 0.f));
	}

	drawMode = GL_LINE_STRIP;
//	return circle;
}

void Geometry::makePoint(glm::vec3 v, glm::vec3 c) {
	glPointSize(7.5);
	verts.push_back(v);
	colours.push_back(c);
	drawMode = GL_POINTS;
}

void Geometry::addPoints(glm::vec3 v, glm::vec3 c) {
	verts.push_back(v);
	colours.push_back(c);
	drawMode = GL_LINE_STRIP;
}

void Geometry::makeLine(glm::vec3 p0,glm::vec3 p1) {
	verts.push_back(p0);
	verts.push_back(p1);
	colours.push_back(glm::vec3(0.f, 0.f, 1.f));
	colours.push_back(glm::vec3(0.f, 0.f, 1.f));
	drawMode = GL_LINES;
}

void Geometry::clearGeometry() {
	verts.clear();
	colours.clear();
	modelMatrix = glm::mat4(1.f);
}
