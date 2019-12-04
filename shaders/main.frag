#version 430 core

out vec4 colour;
in vec3 C;
void main(void) {    	
//	colour = vec4(0.0, 0.0, 0.0, 1.0);
	colour = vec4(C, 1.0);
}
