#include "InputHandler.h"

RenderEngine* InputHandler::renderEngine;
int InputHandler::mouseOldX;
int InputHandler::mouseOldY;

// Must be called before processing any GLFW events
void InputHandler::setUp(RenderEngine* renderEngine) {
	InputHandler::renderEngine = renderEngine;
	rotAxis.makeLine(r0, r1);
	renderGeometries();
}

// Callback for key presses
void InputHandler::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(0);
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		showSurfaceOfRotation = !showSurfaceOfRotation;
		drawonce = true;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		for (Geometry* g : points) 
			g->clearGeometry();
		points.clear();
		coords.clear();
		weights.clear();
		k=3;
		uPos = 0.5;
		showConvexHaul = false;
		showEntireCH = false;
		showLines = false;
		showU = false;
		showPoints = true;
		showRotAxis = false;
		r0 = glm::vec3(0.0f,-1000.0f,0.0f);
		r1 = glm::vec3(0.0f,1000.0f,0.0f);
		rdx = glm::vec3(0.0f, 0.0f, 0.0f);
		theta = 0;
		showSurfaceOfRotation = false;
		drawonce = false;
	}
	if (!showSurfaceOfRotation) {
		if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if (k < (int)coords.size()) {
				k++;
			}
		}
		if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if (k > 2) {
				k--;
			}
		}
		if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if (uPos<1)
				uPos += 0.05/m;
			if (uPos>=1)
				uPos = 0.9999;
		}
		if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if (uPos>0)
				uPos -= 0.05/m;
			if (uPos<=0)
				uPos = 0;
		}
		if (key == GLFW_KEY_U && action == GLFW_PRESS) 
			showU = !showU;
		if (key == GLFW_KEY_G && action == GLFW_PRESS) 
			showConvexHaul = !showConvexHaul;
		if (key == GLFW_KEY_P && action == GLFW_PRESS) 
			showPoints = !showPoints;
		if (key == GLFW_KEY_L && action == GLFW_PRESS) 
			showLines = !showLines;
//		if (key == GLFW_KEY_J && action == GLFW_PRESS) 
//			showEntireCH = !showEntireCH;
		if (key == GLFW_KEY_R && action == GLFW_PRESS) 
			showRotAxis = !showRotAxis;
		
		
/*		if (key == GLFW_KEY_KP_4 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			rdx-=glm::vec3(1.f,0.f,0.f);
	//		r1-=glm::vec3(1.f,0.f,0.f);
		}
		if (key == GLFW_KEY_KP_6 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			rdx+=glm::vec3(1.f,0.f,0.f);
	//		r1+=glm::vec3(1.f,0.f,0.f);
		}
		if (key == GLFW_KEY_KP_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			rdx-=glm::vec3(0.f,1.f,0.f);
	//		r1-=glm::vec3(0.f,1.f,0.f);
		}
		if (key == GLFW_KEY_KP_8 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			rdx+=glm::vec3(0.f,1.f,0.f);
	//		r1+=glm::vec3(0.f,1.f,0.f);
		}*/
		if (key == GLFW_KEY_COMMA && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if (theta<180)
				theta++;
		}
		if (key == GLFW_KEY_PERIOD && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			if (theta>-180)
				theta--;
		}
		
		if (key == GLFW_KEY_EQUAL && (action == GLFW_PRESS || action == GLFW_REPEAT) && selected != -1) {
			weights[selected] += 0.1f;
		}
		if (key == GLFW_KEY_MINUS && (action == GLFW_PRESS || action == GLFW_REPEAT) && selected != -1) {
			if (weights[selected] > 0.1f)
				weights[selected] -= 0.1f;
		}
		
		m = coords.size();
		if (m>0) {
			makeKnotSequence();
			splinePoints.clear();
			for (float u=0; u<1.0f; u+=0.001f) {
				splinePoints.push_back(pointAtU(u));
			}
			temp.clearGeometry();
			temp.makePoint(pointAtU(uPos), glm::vec3(0.0f, 1.0f, 1.0f));
			geometryAtU(uPos);
		}
		rotAxis.clearGeometry();
		rotAxis.makeLine(r0, r1);
//		rotAxis.modelMatrix = glm::translate(rotAxis.modelMatrix, rdx);
		rotAxis.modelMatrix = glm::rotate(rotAxis.modelMatrix, glm::radians(theta), glm::vec3(0.f,0.f,1.f));
//		std::cout << (rotAxis.modelMatrix*glm::vec4(r0,1.0f)).x <<","<< (rotAxis.modelMatrix*glm::vec4(r0,1.0f)).y<< "," << (rotAxis.modelMatrix*glm::vec4(r0,1.0f)).z<< std::endl;
	}
	if ((drawonce && showSurfaceOfRotation) || (!showSurfaceOfRotation)) 
	renderGeometries();
	drawonce = false;

}

// Callback for mouse button presses
void InputHandler::mouse(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	if (!showSurfaceOfRotation) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			glfwGetCursorPos(window, &x, &y);
			lefthold = true;
			xclick = (-256+x);
			yclick = (256-y);
			if (onPoint() == -1) {
				Geometry* tempObj = new Geometry();
				tempObj->makePoint(glm::vec3(xclick, yclick, 0.f), glm::vec3(0.f, 0.f, 0.f));
				coords.push_back(glm::vec3(xclick, yclick, 0));
				points.push_back(tempObj);
				weights.push_back(1.0f);
			}
			else
				selected = onPoint();
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			lefthold = false;
			selected = -1;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			glfwGetCursorPos(window, &x, &y);
			xclick = (-256+x);
			yclick = (256-y);
			selected = onPoint();
			if (selected != -1 && coords.size() > 0) {
				points[selected]->clearGeometry();
				points.erase(points.begin()+selected);
				coords.erase(coords.begin()+selected);
				weights.erase(weights.begin()+selected);
			}
			selected = -1;
		}
		m = coords.size();
		makeKnotSequence();
		splinePoints.clear();
		for (float u=0; u<1.0f; u+=0.001f) {
			splinePoints.push_back(pointAtU(u));
		}
		temp.clearGeometry();
		temp.makePoint(pointAtU(uPos), glm::vec3(0.0f, 1.0f, 1.0f));
		geometryAtU(uPos);
		renderGeometries();
	}
}

// Callback for mouse motion
void InputHandler::motion(GLFWwindow* window, double x, double y) {
	if (lefthold && selected != -1 && !showSurfaceOfRotation) {
		mouseOldX = x;
		mouseOldY = y;
		xclick = (-256+x);
		yclick = (256-y);
		coords[selected] = glm::vec3(xclick, yclick, 0);
		points[selected]->clearGeometry();
		points[selected]->makePoint(glm::vec3(xclick, yclick, 0.f), glm::vec3(0.f, 0.f, 0.f));
		m = coords.size();
		makeKnotSequence();
		splinePoints.clear();
		for (float u=0; u<1.0f; u+=0.001f) {
			splinePoints.push_back(pointAtU(u));
		}
		temp.clearGeometry();
		temp.makePoint(pointAtU(uPos), glm::vec3(0.0f, 1.0f, 1.0f));
		glines.clearGeometry();
		geometryAtU(uPos);
		geometryAtU(uPos);
		renderGeometries();
	}
}

// Callback for mouse scroll
void InputHandler::scroll(GLFWwindow* window, double x, double y) {
}

// Callback for window reshape/resize
void InputHandler::reshape(GLFWwindow* window, int width, int height) {
	renderEngine->setWindowSize(width, height);
	geometryAtU(uPos);
	renderGeometries();

}

int InputHandler::onPoint() {
	for (int i=0; i<(int)coords.size(); i++) {
		if (abs(glm::distance(coords[i], glm::vec3(xclick, yclick, 0))) <= 5.0f)
			return i;
	}
	return -1;
}

void InputHandler::makeKnotSequence() {
	knots.clear();
	std::vector<float> tempKnots;
	int remainingKnots = k + m - 2.0*k;
	for (int i=0; i<k; i++)
		tempKnots.push_back(0.0f);
	for (int i=0; i<remainingKnots; i++)
		tempKnots.push_back(i+1.0f);
	for (int i=0; i<k; i++) {
		if (remainingKnots +1 < 1)
			tempKnots.push_back(1.0f);
		else 
			tempKnots.push_back(remainingKnots+1.0f);
	}
	float stepSize = m - k + 1;
	for (int i=0; i<(int)tempKnots.size(); i++)
		knots.push_back(tempKnots[i]/stepSize);
}

glm::vec3 InputHandler::pointAtU(float u) {
			std::vector<glm::vec3> ctemp;
			std::vector<float> wtemp;
			int delta = -1;
			for (int i=0; i<m+k-2; i++) {
				if (u>=knots[i] && u<knots[i+1])
					delta = i;
			}
			int d = delta;
			for (int i=0; i<=k-1; i++) {
				ctemp.push_back(coords[d-i] * weights[d-i]);
				wtemp.push_back(weights[d-i]);
			}
			for (int i=k; i >=2; i--) {
				int index = d;
				for (int j=0; j<=i-2; j++) {
					float o = (u - knots[index]) / (knots[index+i-1] - knots[index]);
					ctemp[j] = (o * ctemp[j] + (1-o)*ctemp[j+1]);
					wtemp[j] = (o * wtemp[j] + (1-o)*wtemp[j+1]);
					index--;
				}
			}
	return (ctemp[0]/wtemp[0]);
}
void InputHandler::geometryAtU(float u) {
			std::vector<glm::vec3> ctemp, ctemp1;
			std::vector<float> wtemp, wtemp1;
			int delta = -1;
			for (int i=0; i<m+k-2; i++) {
				if (u>=knots[i] && u<knots[i+1])
					delta = i;
			}
			int d = delta;
			for (int i=0; i<=k-1; i++) {
				ctemp.push_back(coords[d-i] * weights[d-i]);
				wtemp.push_back(weights[d-i]);
				ctemp1.push_back(coords[d-i+1] * weights[d-i+1]);
				wtemp1.push_back(weights[d-i+1]);
			}
			for (int i=k; i >=2; i--) {
				int index = d;
				for (int j=0; j<=i-2; j++) {
					float o = (u - knots[index]) / (knots[index+i-1] - knots[index]);
					float o1 = (u - knots[index+1]) / (knots[index+i] - knots[index+1]);
					
					ctemp[j] = (o * ctemp[j] + (1-o)*ctemp[j+1]);
					wtemp[j] = (o * wtemp[j] + (1-o)*wtemp[j+1]);
					ctemp1[j] = (o1 * ctemp1[j] + (1-o1)*ctemp1[j+1]);
					wtemp1[j] = (o1 * wtemp1[j] + (1-o1)*wtemp1[j+1]);
					index--;
				}
			}
			glines.clearGeometry();
			glines.makeLine(ctemp[0]/wtemp[0], ctemp[(ctemp.size()-1)/2]/wtemp[(wtemp.size()-1)/2]);
			glines.makeLine(ctemp[0]/wtemp[0], ctemp1[(ctemp1.size()-1)/2]/wtemp1[(wtemp1.size()-1)/2]);
			glines.makeLine(coords[d],coords[d-1]);
			glines.makeLine(coords[d-1],coords[d-2]);
}

void InputHandler::renderGeometries() {
	Geometry lineSegment;
	Geometry spline;
	lineSegment.clearGeometry();
	spline.clearGeometry();
	stuff.clear();
	if (showRotAxis) {
		renderEngine->assignBuffers(rotAxis);
		renderEngine->updateBuffers(rotAxis);
		stuff.push_back(&rotAxis);
	}
	
	if (m >= k) {
		for (glm::vec3 v : splinePoints) {
			spline.addPoints(v, glm::vec3(0.0f, 1.0f, 0.0f));
			if (showSurfaceOfRotation) {
				Geometry* surface = new Geometry();
				for (float r=0; r<180; r+=0.01) {
					glm::vec3 rvector = glm::vec3((rotAxis.modelMatrix*glm::vec4(r0,1.0f)).x,(rotAxis.modelMatrix*glm::vec4(r0,1.0f)).y,(rotAxis.modelMatrix*glm::vec4(r0,1.0f)).z)-glm::vec3((rotAxis.modelMatrix*glm::vec4(r1,1.0f)).x,(rotAxis.modelMatrix*glm::vec4(r1,1.0f)).y,(rotAxis.modelMatrix*glm::vec4(r1,1.0f)).z);
					glm::mat4 tempm = glm::rotate(surface->modelMatrix, glm::radians(r), rvector);
					surface->addPoints(glm::vec3((tempm*glm::vec4(v,1.0f)).x,(tempm*glm::vec4(v,1.0f)).y,(tempm*glm::vec4(v,1.0f)).z), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				renderEngine->assignBuffers(*surface);
				renderEngine->updateBuffers(*surface);
				stuff.push_back(surface);
			}
		}
		renderEngine->assignBuffers(spline);
		renderEngine->updateBuffers(spline);
		stuff.push_back(&spline);
	}
	if (showU && m>=k) {
		renderEngine->assignBuffers(temp);
		renderEngine->updateBuffers(temp);
		stuff.push_back(&temp);
		if (showConvexHaul) {
			renderEngine->assignBuffers(glines);
			renderEngine->updateBuffers(glines);
			stuff.push_back(&glines);
		}
	}
	if (showLines) {
		for (glm::vec3 v : coords) {
			lineSegment.addPoints(v,glm::vec3(1.0f, 0.0f, 0.0f));
		}
		renderEngine->assignBuffers(lineSegment);
		renderEngine->updateBuffers(lineSegment);
		stuff.push_back(&lineSegment);
	}
	
	if (showPoints) {
		for (Geometry* g : points) {
			renderEngine->assignBuffers(*g);
			renderEngine->updateBuffers(*g);
			stuff.push_back(g);	
		}
	}
//	for (float v=0; v<360; v++)
//		renderEngine->render(stuff, glm::rotate(glm::mat4(1.f),glm::radians(v), glm::vec3((rotAxis.modelMatrix*glm::vec4(r0,1.0f)).x,(rotAxis.modelMatrix*glm::vec4(r0,1.0f)).y,(rotAxis.modelMatrix*glm::vec4(r0,1.0f)).z)-glm::vec3((rotAxis.modelMatrix*glm::vec4(r1,1.0f)).x,(rotAxis.modelMatrix*glm::vec4(r1,1.0f)).y,(rotAxis.modelMatrix*glm::vec4(r1,1.0f)).z)));
	if (showSurfaceOfRotation)
		renderEngine->render(stuff, glm::mat4(1.f), 1);
	else
		renderEngine->render(stuff, glm::mat4(1.f), 0);

}
