#include "Program.h"

Program::Program() {
	window = nullptr;
	renderEngine = nullptr;
}

// Error callback for glfw errors
void Program::error(int error, const char* description) {
	std::cerr << description << std::endl;
}

// Called to start the program
void Program::start() {
	setupWindow();
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK) {
		std::cerr << glewGetErrorString(err) << std::endl;
	}

	renderEngine = new RenderEngine(window);
	InputHandler::setUp(renderEngine);
	mainLoop();
}

// Creates GLFW window for the program and sets callbacks for input
void Program::setupWindow() {
	glfwSetErrorCallback(Program::error);
	if (glfwInit() == 0) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(512, 512, "589 Boilerplate", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // V-sync on

	glfwSetKeyCallback(window, InputHandler::key);
	glfwSetMouseButtonCallback(window, InputHandler::mouse);
	glfwSetCursorPosCallback(window, InputHandler::motion);
	glfwSetScrollCallback(window, InputHandler::scroll);
	glfwSetWindowSizeCallback(window, InputHandler::reshape);
}

double InputHandler::xclick = 0;
double InputHandler::yclick = 0;
bool InputHandler::lefthold = false;
Geometry InputHandler::temp;
Geometry InputHandler::glines;
std::vector<Geometry*> InputHandler::points;
std::vector<Geometry*> InputHandler::stuff;
//std::vector<Geometry*> InputHandler::curvePoints;
std::vector<glm::vec3> InputHandler::coords;
std::vector<glm::vec3> InputHandler::splinePoints;
std::vector<float> InputHandler::knots = {};
std::vector<float> InputHandler::weights;
int InputHandler::selected = -1;
int InputHandler::k = 3;
int InputHandler::m = 0;
float InputHandler::uPos = 0.5;
bool InputHandler::showU = false;
bool InputHandler::showPoints = true;
bool InputHandler::showLines = false;
bool InputHandler::showConvexHaul = false;
bool InputHandler::showEntireCH = false;

Geometry InputHandler::rotAxis;
bool InputHandler::showRotAxis = false;
glm::vec3 InputHandler::r0 = glm::vec3(0.0f,-1000.0f,0.0f);
glm::vec3 InputHandler::r1 = glm::vec3(0.0f,1000.0f,0.0f);
glm::vec3 InputHandler::rdx = glm::vec3(0.0f,0.0f,0.0f);
float InputHandler::theta = 0.0f;
bool InputHandler::showSurfaceOfRotation = false;
bool InputHandler::drawonce = false;

// Main loop
void Program::mainLoop() {
	
	Geometry lineSegment;
	Geometry spline;
	InputHandler::setUp(renderEngine);

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	delete renderEngine;
	glfwTerminate();
}
