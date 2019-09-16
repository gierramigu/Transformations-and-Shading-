
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "application.hpp"
#include "bounding_box.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"
#include "cgra/cgra_mesh.hpp"


using namespace std;
using namespace cgra;
using namespace glm;


Application::Application(GLFWwindow *window) : m_window(window) {
	
	// build the shader for the model
	shader_builder color_sb;
	color_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_vert.glsl"));
	color_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_frag.glsl"));
	GLuint color_shader = color_sb.build();

	// build the mesh for the model
	mesh_builder teapot_mb = load_wavefront_data(CGRA_SRCDIR + std::string("//res//assets//teapot.obj"));
	gl_mesh teapot_mesh = teapot_mb.build();

	// put together an object
	m_model.shader = color_shader;
	m_model.mesh = teapot_mesh;
	m_model.color = glm::vec3(1, 0, 1);
	m_model.modelTransform = glm::mat4(1);
}


void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);
	//yaw - rotating around the y axis
	//pitch - rotating around the x axis
	//mat4 view = rotate(view, (glm::mediump_float)90, glm::vec3(m_yaw, 90, 0.0f)); //new code 
	//view = translate(mat4(1), vec3(0, -5, -m_distance)); // TODO replace view matrix with the camera transform

	// calculate the projection and view matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f); //perspective projection, there are other types
	mat4 view = translate(mat4(1), vec3(0, -5, -m_distance)); // TODO replace view matrix with the camera transform

	//calculating the pitch and yaw 
	//Note: mat4(1) is the identity matrix and is being passed into the rotation matrix
	mat4 yaw = glm::rotate(mat4(1), (glm::radians(m_yaw)), glm::vec3(-m_yaw, 0.0f, 0.0f));
	mat4 pitch = glm::rotate(mat4(1), (glm::radians(m_pitch)), glm::vec3(0, -m_pitch, 0.0f));
	//view = (pitch * yaw * view); //VR simulation purposes only
	//view = (view * pitch * yaw);

	// draw options
	//grid and axis plane are also rotated according to pitch and yaw when multiplied: 
	if (m_show_grid) cgra::drawGrid(view * yaw * pitch, proj);
	if (m_show_axis) cgra::drawAxis(view * yaw * pitch, proj);

	//if (m_show_grid) cgra::drawGrid(view, proj); //play around for VR simulation 
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);

	// draw the model
	m_model.draw(view, pitch, yaw, proj);
}


void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(340, 250), ImGuiSetCond_Once);
	ImGui::Begin("Camera", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	//gui rotation sliders 
	ImGui::SliderFloat("Pitch", &m_pitch, 0.1, 360, "%.1f");
	ImGui::SliderFloat("Yaw", &m_yaw, 0.1, 360, "%.1f");


	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.1f");
	ImGui::SliderFloat3("Model Color", value_ptr(m_model.color), 0, 1, "%.2f");

	//ImGui::SliderFloat3("Light Direction", value_ptr(m_model.lightDirection), 0, 1.0f);
	ImGui::SliderFloat("Shininess", value_ptr(m_model.specularStrength), 0, 100);
	ImGui::SliderFloat3("Specular Color", value_ptr(m_model.specularColor), 0, 1, "%.2f");
	ImGui::SliderFloat("Ambience", value_ptr(m_model.ambientStrength), 0, 1);
	ImGui::SliderFloat("Diffuse", &m_model.diffStrength, 0, 1);



	// extra drawing parameters
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);

	// finish creating window
	ImGui::End();
} 


void Application::cursorPosCallback(double xpos, double ypos) {
	 
	//this allows for the user to stop the camera from moving when the mouse is released. (right click)
	(void)xpos, ypos;
	if (ImGui::IsMouseDragging(1)) {
		m_yaw = ypos;
		m_pitch = xpos;
	}

}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)button, action, mods; // currently un-used





}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset, yoffset; 

	m_distance += yoffset; 

	//Capping of when scrolling the distance so it doesn't go endless when zooming in and out of the object
	if (m_distance < 0) {
		m_distance = 0;
	}
	else if (m_distance > 100) {	
		m_distance = 100;
	}
     
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	//(void)key, (void)scancode, (void)action, (void)mods; // currently un-used

	if (key == 'W') {


	}
	else {
	
	
	}
	if (key == 'S') {


	}
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used

}