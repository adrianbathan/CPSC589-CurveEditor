#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "RenderEngine.h"
#include "Geometry.h"

class InputHandler {

public:
	static void setUp(RenderEngine* renderEngine);

	static void key(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse(GLFWwindow* window, int button, int action, int mods);
	static void motion(GLFWwindow* window, double x, double y);
	static void scroll(GLFWwindow* window, double x, double y);
	static void reshape(GLFWwindow* window, int width, int height);
	
	static double xclick;
	static double yclick;
	static bool lefthold;
	static std::vector<Geometry*> points;
//	static std::vector<std::vector<Geometry*>> curvePoints;
//	static std::vector<Geometry*> curvePoints;
	static std::vector<glm::vec3> coords;
	static std::vector<glm::vec3> splinePoints;
	static std::vector<float> knots;
	static std::vector<float> weights;
	static int onPoint();
	static void makeKnotSequence();
	static glm::vec3 pointAtU(float u);
	static void geometryAtU(float u);
	static Geometry glines;
	static Geometry temp;
	static int selected;
	static int k;
	static int m;	
	static float uPos;
	
	static bool showPoints;
	static bool showLines;
	static bool showConvexHaul;
	static bool showEntireCH;
	static bool showU;
	static bool showSurfaceOfRotation;
	
	static bool showRotAxis;
	static bool drawonce;
	static Geometry rotAxis;
	
	static glm::vec3 r0;
	static glm::vec3 r1;
	static glm::vec3 rdx;
	static float theta;
	
	static void renderGeometries();
	static std::vector<Geometry*> stuff;
		
private:
	static RenderEngine* renderEngine;

	static int mouseOldX;
	static int mouseOldY;
	
};
