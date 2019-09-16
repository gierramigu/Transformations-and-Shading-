
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"
#include "basic_model.hpp"


// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// oribital camera
	float m_distance = 20.0; 

	//rotational camera 
	float m_pitch = 0.1;
	float m_yaw = 0.1;

	//Phong Shading
	float m_diffuse = 20;
	float m_ambient = 20;
	float m_specular = 20;

	// drawing flags
	bool m_show_axis = false;
	bool m_show_grid = false;
	bool m_showWireframe = false;

	bool mouseClick = false;

	// basic model
	// contains a shader, a model transform
	// a mesh, and other model information (color etc.)
	basic_model m_model;

public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};