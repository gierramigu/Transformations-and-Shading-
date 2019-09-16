
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include <vector>

#include "bounding_box.hpp"
#include "cgra/cgra_mesh.hpp"
#include "cgra/cgra_wavefront.hpp"



// Basic model that holds the shader, mesh and transform for drawing.
// Can be copied and/or modified for adding in extra information for drawing
// including colors for diffuse/specular, and textures for texture mapping etc.
struct basic_model {

	GLuint shader = 0;
	cgra::gl_mesh mesh;
	cgra::gl_mesh boundingBox;
	//cgra::mesh_builder mb;
	glm::vec3 color{ 0, 1, 0 }; //at this case would be object colour
	glm::vec3 lightColor{ 1, 1, 1 };
	//glm::vec3 lightDirection { 1, 0.821, 0.241 };
	//glm::vec3 lightDirection{ 0.25, 0.25, -1 };
	glm::mat4 modelTransform{ 1.0 };
	glm::vec3 specularStrength{ 0.5 };
	//float specularStrength = 0.5 ;
	glm::vec3 specularColor{ 1, 0, 0 };
	glm::vec3 ambientStrength{ 0.0 };
	float diffStrength = 0.0; 

	//instancing
	glm::vec3 translations[100];
	int index = 0;
	float offset = 0.1f;

	//glm::vec3 maxv { 10,10,20 };
	//glm::vec3 minv { 10,20,20 };


	void draw(const glm::mat4 &view, const glm::mat4 &pitch, const glm::mat4 &yaw, const glm::mat4 proj) {
		using namespace glm;

		// calcuate the modelview transfsor 
		mat4 modelview = view * yaw * pitch * modelTransform;

		// load shader and variables
		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false,	 value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
		glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));


		glUniform3fv(glGetUniformLocation(shader, "testLightColor"), 1, value_ptr(lightColor));
		//glUniform3fv(glGetUniformLocation(shader, "lightDir"), 1, value_ptr(lightDirection));
		//glUniform1f(glGetUniformLocation(shader, "shininess"),  specularStrength);
		glUniform3fv(glGetUniformLocation(shader, "specColor"), 1, value_ptr(specularColor));
		glUniform3fv(glGetUniformLocation(shader, "shininess"), 1, value_ptr(specularStrength));
		glUniform3fv(glGetUniformLocation(shader, "ambience"), 1, value_ptr(ambientStrength));
		glUniform1f(glGetUniformLocation(shader, "diffuseLevel"), diffStrength);


		//Instancing 
		for (int z = -1000; z < 1000; z += 200){
			for (int x = -1000; x < 1000; x += 200)  {
				if (index ==100) break;
				glm::vec3 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (rand() % 200 - rand() % 200);  //randomized
				//translation.y = 0; //fixed 
				translation.z = (float)z / 10.0f + offset;
				translations[index++] = translation; 
			}
			if (index == 100) break;
		}

		glUniform3fv(glGetUniformLocation(shader, "offsets"), 100, value_ptr(translations[0]));



		//Bounding box
		/*cgra::gl_mesh createBoundingBoxMesh();
		cgra::mesh_builder mb = cgra::mesh_builder(GL_LINES);
		cgra::mesh_vertex mv;
		mv.pos = vec3(5.f);  
		mb.push_vertex(mv);
		mv.pos = vec3(10.f);
		mb.push_vertex(mv);
		mv.pos = vec3(15.f);
		mb.push_vertex(mv);
		mv.pos = vec3(-15.f);
		mb.push_vertex(mv);
		
		mb.push_index(0);
		mb.push_index(1);
		mb.push_index(2);
		mb.push_index(3);
		boundingBox = mb.build(); //!!!!!!!!!
		//mb.print();
		boundingBox.draw(); */


		// draw the mesh
		mesh.draw(); 
	}
};