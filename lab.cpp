// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>
#include <common/light.h> 


#include <pokemon_cpp/Box.h> 
#include <pokemon_cpp/Cube.h> 
#include <pokemon_cpp/Collision.h> 
#include <pokemon_cpp/Sphere.h> 
#include <pokemon_cpp/RigidBody.h> 




using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Pokemon Pearl"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024
#define gravity 9.80665f

/*allages p exw kanei :
plane -> ground
model1 -> grass_model
*/
// Creating a structure to store the material parameters of an object
struct Material {
	vec4 Ka;
	vec4 Kd;
	vec4 Ks;
	float Ns;
};


// Global Variables
GLFWwindow* window;
Camera* camera;
Light* light;
GLuint shaderProgram, depthProgram, miniMapProgram;
Drawable* grass_model;
Drawable* ground_thingies;
Drawable* signs_model;


Drawable* trees_model;

Box* box;
Box* box2;
Box* box3;
Box* box4;
Box* box5;
//Box* box6;
Drawable* sky;
Sphere* pokeball;
Drawable* mudkip;
Drawable* sun;




Drawable* ground;
GLuint modelPokeballTexture, modelSpecularTexture, skyTexture, modelMudkipTexture;
GLuint depthFrameBuffer, depthTexture;

Drawable* quad;

// locations for shaderProgram
GLuint viewMatrixLocation;
GLuint projectionMatrixLocation;
GLuint modelMatrixLocation;
GLuint KaLocation, KdLocation, KsLocation, NsLocation;
GLuint LaLocation, LdLocation, LsLocation;
GLuint lightPositionLocation;
GLuint lightPowerLocation;
GLuint diffuseColorSampler; 
GLuint specularColorSampler;
GLuint useTextureLocation;
GLuint depthMapSampler;
GLuint lightVPLocation;
GLuint lightDirectionLocation;
GLuint lightFargroundLocation;
GLuint lightNeargroundLocation;


// locations for depthProgram
GLuint shadowViewProjectionLocation; 
GLuint shadowModelLocation;

// locations for miniMapProgram
GLuint quadTextureSamplerLocation;

// All Materials
const Material ground_thingies_material{
	vec4{0.23125, 0.23125, 0.23125, 1},
	vec4{0.745221, 0.595035, 0.287183, 1},
	vec4{ 0.5, 0.5, 0.5, 1.0 },
	89.6f
};

const Material signs_material{
	vec4{ 0.3, 0.3, 0.3, 1.0},
	vec4{ 0.271558, 0.271558, 0.271558, 1.0 },
	vec4{ 0.5, 0.5, 0.5, 1.0 },
	12.8f
};

const Material ground_material{
	vec4{0.23125, 0.23125, 0.23125, 1.0},
	vec4{0.109323, 0.595806, 0.108349, 1.0 },
	vec4{ 0.1, 0.1, 0.1, 0.1 },
	225.0f
};

const Material grass_material{
	vec4{0.3, 0.3, 0.3, 1},
	vec4{ 0.022540, 0.465506 ,0.040524, 0.8 },
	vec4{ 0.1, 0.1, 0.1, 0.1 },
	225.0f
};

// NOTE: Since the Light and Material struct are used in the shader programs as well 
//		 it is recommended to create a function that will update all the parameters 
//       of an object.
// 
// Creating a function to upload (make uniform) the light parameters to the shader program
void uploadLight(const Light& light) {
	glUniform4f(LaLocation, light.La.r, light.La.g, light.La.b, light.La.a);
	glUniform4f(LdLocation, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
	glUniform4f(LsLocation, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
	glUniform3f(lightPositionLocation, light.lightPosition_worldspace.x,
		light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
	glUniform1f(lightPowerLocation, light.power);
}


// Creating a function to upload the material parameters of a model to the shader program
void uploadMaterial(const Material& mtl) {
	glUniform4f(KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
	glUniform4f(KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
	glUniform4f(KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
	glUniform1f(NsLocation, mtl.Ns);
}





void createContext() {

	// Create and complile our GLSL program from the shader
	shaderProgram = loadShaders("ShadowMapping.vertexshader", "ShadowMapping.fragmentshader");
	
	// Task 3.1 
	// Create and load the shader program for the depth buffer construction
	// You need to load and use the Depth.vertexshader, Depth.fragmentshader
	// NOTE: These files are not visible in the visual studio, as they are not a part of
	//       the main project. To add a new file to our project, we need to add it to the
	//       CMAKELists.txt and build the project again. 
	// NOTE: We alse need to create a global variable to store new shader program
	depthProgram = loadShaders("Depth.vertexshader", "Depth.fragmentshader");
	

	// Task 2.1
	// Use the SimpleTexture.vertexshader, "SimpleTexture.fragmentshader"
	miniMapProgram = loadShaders("SimpleTexture.vertexshader", "SimpleTexture.fragmentshader");;
	

	// NOTE: Don't forget to delete the shader programs on the free() function


	// Get pointers to uniforms
	// --- shaderProgram ---
	projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
	viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
	modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
	// for phong lighting
	KaLocation = glGetUniformLocation(shaderProgram, "mtl.Ka");
	KdLocation = glGetUniformLocation(shaderProgram, "mtl.Kd");
	KsLocation = glGetUniformLocation(shaderProgram, "mtl.Ks");
	NsLocation = glGetUniformLocation(shaderProgram, "mtl.Ns");
	LaLocation = glGetUniformLocation(shaderProgram, "light.La");
	LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
	LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
	lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
	lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");
	diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
	specularColorSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");
	
	// Task 1.4
	useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture"); 

	// locations for shadow rendering
	depthMapSampler = glGetUniformLocation(shaderProgram, "shadowMapSampler");
	lightVPLocation = glGetUniformLocation(shaderProgram, "lightVP");


	// --- depthProgram ---
	shadowViewProjectionLocation = glGetUniformLocation(depthProgram, "VP");
	shadowModelLocation = glGetUniformLocation(depthProgram, "M");

	// --- miniMapProgram ---
	quadTextureSamplerLocation = glGetUniformLocation(miniMapProgram, "textureSampler");
	

	

	// Loading my models
	//METAKINHSE TO OBJ STO FAKELO POKEMON_CPP//
	
	// 1. Using Drawable to load grass
	grass_model = new Drawable("grass.obj");
	ground_thingies = new Drawable("ground_thingies.obj");
	signs_model = new Drawable("signs.obj"); 
	
	
	// load tree
	trees_model = new Drawable("trees.obj");

	//load sun
	sun = new Drawable("sphere.obj");

	for (int i = 0; i < sun->normals.size(); i++) {
		sun->normals[i] = -sun->normals[i];
	}
	sun = new Drawable(sun->vertices, sun->uvs, sun->normals);


	// 1. Using Drawable to load ground
	ground = new Drawable("ground.obj");

	//using Drawable to load mudkip
	//mudkip = new Drawable("Mudkip.obj");

	//box//
	
	box = new Box(vec3(-0.5, 11, -1.5), vec3(0, 0, 0), 9, 17, 0.1);
	box2 = new Box(vec3(-0.5, 10.7, 5), vec3(0, 0, 0), 9, 17, 0.1);
	box3 = new Box(vec3(-0.5, 10.3, 12), vec3(0, 0, 0), 9, 17, 0.1);
	box4 = new Box(vec3(-2.5, 4.25, -6), vec3(0, 0, 0), 9, 3, 0.1);
	box5 = new Box(vec3(-2.5, 6.0, -14), vec3(0, 0, 0), 6, 6, 0.1);
	//box6 = new Box(vec3(-2.1, 5.0, -6), vec3(0, 0, 0), 9, 1.5, 0.1);
	//sky//
	sky = new Drawable("sky.obj");

	//pokeball//
	
	//pokeball = new Sphere(vec3(0,0,0), vec3(0,0,0), 0.0001, 0.1);
	modelPokeballTexture = loadSOIL("pokeball.bmp");
	skyTexture = loadSOIL("Skybox.bmp");
	//mudkipTexture
	//modelMudkipTexture = loadSOIL("Mudkip.bmp");

	// loading a diffuse and a specular texture
	
	//modelSpecularTexture = loadSOIL("suzanne_specular.bmp");
	

	
	// Task 1.2 Load earth.obj using drawable 
	
	// Call the Drawable constructor
	// Notice, that this way we dont have to generate VAO and VBO for the matrices
	// Task 2.2: Creating a 2D quad to visualize the depthmap
	// create geometry and vao for screen-space quad 
	//tetragwno sthn panw deksia gwnia
	
	vector<vec3> quadVertices = {
	  vec3(0.5, 0.5, 0.0),
	  vec3(1.0, 0.5, 0.0),
	  vec3(1.0, 1.0, 0.0),
	  vec3(1.0, 1.0, 0.0),
	  vec3(0.5, 1.0, 0.0),
	  vec3(0.5, 0.5, 0.0)
	};

	vector<vec2> quadUVs = {
	  vec2(0.0, 0.0),
	  vec2(1.0, 0.0),
	  vec2(1.0, 1.0),
	  vec2(1.0, 1.0),
	  vec2(0.0, 1.0),
	  vec2(0.0, 0.0)
	};

	quad = new Drawable(quadVertices, quadUVs);
	

	


	// ---------------------------------------------------------------------------- //
	// -  Task 3.2 Create a depth framebuffer and a texture to store the depthmap - //
	// ---------------------------------------------------------------------------- //
	
	// Tell opengl to generate a framebuffer
	glGenFramebuffers(1, &depthFrameBuffer);
	// Binding the framebuffer, all changes bellow will affect the binded framebuffer
	// **Don't forget to bind the default framebuffer at the end of initialization
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);


	// We need a texture to store the depth image
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	// Telling opengl the required information about the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// Task 4.5 Texture wrapping methods
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);							// GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	

	// Task 4.5 Don't shadow area out of light's viewport
	
	// Step 1 : (Don't forget to comment out the respective lines above
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Set color to set out of border 
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Next go to fragment shader and add an iff statement, so if the distance in the z-buffer is equal to 1, 
	// meaning that the fragment is out of the texture border (or further than the far clip ground) 
	// then the shadow value is 0.
	

	// Task 3.2 Continue
	// Attaching the texture to the framebuffer, so that it will monitor the depth component
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	

	// Since the depth buffer is only for the generation of the depth texture, 
	// there is no need to have a color output
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);


	// Finally, we have to always check that our frame buffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glfwTerminate();
		throw runtime_error("Frame buffer not initialized correctly");
	}

	// Binding the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

}


void free() {
	// Delete Shader Programs
	glDeleteProgram(shaderProgram);
	glDeleteProgram(depthProgram);
	glDeleteProgram(miniMapProgram);
	//ground//
	if (ground != NULL) {
		delete ground;
	}
	//grass//
	if (grass_model != NULL) {
		delete grass_model;
	}
	if (ground_thingies != NULL) {
		delete ground_thingies;
	}
	if (signs_model != NULL) {
		delete signs_model;
	}
	
	
	//tree//
	if (trees_model != NULL) {
		delete trees_model;
	}
	
	
	//hit boxes//
	if (box != NULL) {
		delete box;
	}
	if (box2 != NULL) {
		delete box2;
	}
	if (box3 != NULL) {
		delete box3;
	}
	if (box4 != NULL) {
		delete box4;
	}
	if (box5 != NULL) {
		delete box5;
	}
	/*
	if (box6 != NULL) {
		delete box6;
	}
	*/

	//pokeball//
	if (pokeball != NULL) {
		delete pokeball;
	}

	//skybox//
	if (sky != NULL) {
		delete sky;
	}

	//Mudkip//
	/*
	if (mudkip != NULL) {
		delete mudkip;
	}
	*/
	

	glfwTerminate();
}


void depth_pass(mat4 viewMatrix, mat4 projectionMatrix) {

	// Task 4.3



	// Setting viewport to shadow map size
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Binding the depth framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

	// Cleaning the framebuffer depth information (stored from the last render)
	glClear(GL_DEPTH_BUFFER_BIT);

	// Selecting the new shader program that will output the depth component
	glUseProgram(depthProgram);

	// sending the view and projection matrix to the shader
	mat4 view_projection = projectionMatrix * viewMatrix;
	glUniformMatrix4fv(shadowViewProjectionLocation, 1, GL_FALSE, &view_projection[0][0]);












	// ---- rendering the scene ---- //


	// creating grass model matrix and sending to GPU
	//grass //
	mat4 modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	grass_model->bind();
	grass_model->draw();

	//ground_thingies//
	modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	ground_thingies->bind();
	ground_thingies->draw();

	//signs//
	modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	signs_model->bind();
	signs_model->draw();



	//trees//
	modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	trees_model->bind();
	trees_model->draw();
	
	// same for ground

	modelMatrix = translate(mat4(), vec3(0.0f, -3.0f, 0.0f));
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	ground->bind();
	ground->draw();


	if (pokeball != nullptr) {
		glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &pokeball->modelMatrix[0][0]);

		pokeball->draw();
	}
	//sky//
	//mat4 modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	sky->bind();
	sky->draw();



	/*
	if (mudkip != nullptr) {
	mat4 modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	mudkip->bind();
	mudkip->draw();
	}
	*/

	// binding the default framebuffer again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glUniform1i(useTextureLocation, 0);
	
}

void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix) {
	
	// Step 1: Binding a frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, W_WIDTH, W_HEIGHT);

	// Step 2: Clearing color and depth info
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Step 3: Selecting shader program
	glUseProgram(shaderProgram);

	// Making view and projection matrices uniform to the shader program
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	// uploading the light parameters to the shader program
	uploadLight(*light);

	// Task 4.1 Display shadows on the 
	
	// Sending the shadow texture to the shaderProgram
	glActiveTexture(GL_TEXTURE0);								// Activate texture position
	glBindTexture(GL_TEXTURE_2D, depthTexture);			// Assign texture to position 
	glUniform1i(depthMapSampler, 0);						// Assign sampler to that position

	// Sending the light View-Projection matrix to the shader program
	mat4 lightVP = light->projectionMatrix * light->viewMatrix;
	glUniformMatrix4fv(lightVPLocation, 1,GL_FALSE, &lightVP[0][0]);
	glUniform1i(useTextureLocation, 0);
	










	// ----------------------------------------------------------------- //
	// --------------------- Drawing scene objects --------------------- //	
	// ----------------------------------------------------------------- //
	
	// Task 1.2 - Draw the sphere on the scene
	// Use a scaling of 0.5 across all dimensions and translate it to (-3, 1, -3)
	/*
	glUniform1i(useTextureLocation, 0);
	uploadMaterial(polishedSilver);
	mat4 sphere_model = translate(mat4(), vec3(-3.0, 1.0,-3.0));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &sphere_model[0][0]);
	model2->bind();
	model2->draw();*/



	// creating a model matrix
	mat4 modelMatrix = translate(mat4(), vec3(0.0, 0.0, 0.0));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	/*
	// Setting up texture to display on shader program          //  --- Texture Pipeline ---
	glActiveTexture(GL_TEXTURE1);								// Activate texture position
	glBindTexture(GL_TEXTURE_2D, modelPokeballTexture);			// Assign texture to position 
	glUniform1i(diffuseColorSampler, 1);						// Assign sampler to that position
	
	glActiveTexture(GL_TEXTURE2);								//
	glBindTexture(GL_TEXTURE_2D, modelSpecularTexture);			// Same process for specular texture
	glUniform1i(specularColorSampler, 2);						//
	*/
	// Inside the fragment shader, there is an if statement whether to use  
	// the material of an object or sample a texture
	//glUniform1i(useTextureLocation, 1); //suzanne exei texture:D
	//grass//
	glUniform1i(useTextureLocation, 0);
	uploadMaterial(grass_material);
	modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// Draw grass_model
	grass_model->bind();
	grass_model->draw();

	//ground thingies//
	uploadMaterial(ground_thingies_material);
	modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	ground_thingies->bind();
	ground_thingies->draw();

	//signs//
	uploadMaterial(signs_material);
	modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	signs_model->bind();
	signs_model->draw();

	//sun//
	
	mat4 light1_translation = translate(mat4(), light->lightPosition_worldspace);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &light1_translation[0][0]);
	
	sun->bind();
	sun->draw();
	

	//trees//
	uploadMaterial(grass_material);
	modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	trees_model->bind();
	trees_model->draw();

	
	// Task 1.4 Use different material for the ground
	// NOTE: when we make a variable uniform to the shader program the value of 
	//       that variable remains the same until we upload a new value. 
	//       This means that the same model matrix and material defined for
	//       grass_model are used for the ground as well. 

	// upload the material

	// upload the model matrix
	// Create an identity model matrix
	
	
	// Task 1.4 Remove the texture from the ground and use material instead
	//			** Use bool variable to tell the shader not to use a texture
	//			** Look if statement in the fragment shader
	 glUniform1i(useTextureLocation, 0);

	// Task 1.2 Draw a ground
	mat4 ground_model = mat4(1.0);
	uploadMaterial(ground_material);
	
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &ground_model[0][0]);
	ground->bind();
	ground->draw();

	
	//hit boxes//
	/*
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box->modelMatrix[0][0]);
	box->draw();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box2->modelMatrix[0][0]);
	box2->draw();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box3->modelMatrix[0][0]);
	box3->draw();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box4->modelMatrix[0][0]);
	box4->draw();
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box5->modelMatrix[0][0]);
	box5->draw();
	
	//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box6->modelMatrix[0][0]);
	//box6->draw();
	*/

	//sky//
	/*
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box->modelMatrix[0][0]);
	glBindTexture(GL_TEXTURE_2D, skyTexture);
	glUniform1i(useTextureLocation, 1);
	sky->draw();
	glUniform1i(useTextureLocation, 0);
	*/
	//pokeball
	glUniform1i(useTextureLocation, 0);
	if (pokeball != nullptr) {
		//pokeball//
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &pokeball->modelMatrix[0][0]);
		//pokeball texture//
		glBindTexture(GL_TEXTURE_2D, modelPokeballTexture);
		glUniform1i(useTextureLocation, 1);
		pokeball->draw();
		glUniform1i(useTextureLocation, 0);
	}



	//draw mudkip
	/*
	if (mudkip != nullptr) {

	mudkip->bind();
	mudkip->draw();
	}
	*/

}

// Task 2.3: visualize the depth_map on a sub-window at the top of the screen
void renderDepthMap() {
	// using the correct shaders to visualize the depth texture on the quad
	glUseProgram(miniMapProgram);

	//enabling the texture - follow the aforementioned pipeline
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(quadTextureSamplerLocation, 0);

	/*
	// Drawing the quad
	mat4 quadmodelMatrix = scale(mat4(1.0), vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &quadmodelMatrix[0][0]);

	quad->bind();
	quad->draw();
	*/
}



void mainLoop() {
	float t = glfwGetTime();
	float maxEnergy = 0;
	light->update();
	mat4 light_proj = light->projectionMatrix;
	mat4 light_view = light->viewMatrix;

	// Task 3.3
	// Create the depth buffer
	depth_pass(light_view, light_proj);	
	
	
	
	do {
		float dt = 0.001f;
		box->update(t, dt);
		box2->update(t, dt);
		box3->update(t, dt);
		box4->update(t, dt);
		box5->update(t, dt);
		//box6->update(t, dt);
		
		//rixnw thn pokeball//
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			camera->update();

			pokeball = new Sphere(camera->position + normalize(camera->direction) * vec3(2, 2, 2), camera->direction * vec3(5, 5, 5), 0.1, 0.1);

		}
		

		

		if (pokeball != nullptr) {
		pokeball->update(t, dt);


		handleBoxSphereCollision1(*box, *pokeball);
		handleBoxSphereCollision2(*box2, *pokeball);
		handleBoxSphereCollision3(*box3, *pokeball);
		handleBoxSphereCollision4(*box4, *pokeball);
		handleBoxSphereCollision5(*box5, *pokeball);
		//handleBoxSphereCollision6(*box6, *pokeball);
		

		pokeball->forcing = [&](float t, const vector<float>& y)->vector<float> {
			vector<float> f(6, 0.0f);
			f[1] = -pokeball->m * gravity;
			return f;
		};

		pokeball->update(t, dt);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &pokeball->modelMatrix[0][0]);


		pokeball->draw();


		//dunamh ths baruthtas sthn pokeball/

		pokeball->forcing = [&](float t, const vector<float>& y)->vector<float> {
			vector<float> f(6, 0.0f);
			f[1] = -pokeball->m * gravity;
			return f;
		};

		pokeball->update(t, dt);



		pokeball->draw();
	}

	glUniform1i(useTextureLocation, 0);

		light->update();
		mat4 light_proj = light->projectionMatrix;
		mat4 light_view = light->viewMatrix;


		// Task 3.5
		// Create the depth buffer
		depth_pass(light_view, light_proj);

		// Getting camera information
		camera->update();
		mat4 projectionMatrix = camera->projectionMatrix;
		mat4 viewMatrix = camera->viewMatrix;
		
		//get camera position//


		
		
		// Task 1.5
		// Rendering the scene from light's perspective when F1 is pressed
		
		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
			
			lighting_pass(light_view,light_proj);
		}
		else {
			// Render the scene from camera's perspective
			lighting_pass(viewMatrix,projectionMatrix );
		}
		

		// Task 2.3:
		 renderDepthMap();
		 t += dt;

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

}


void initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		throw runtime_error("Failed to initialize GLFW\n");
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		throw runtime_error(string(string("Failed to open GLFW window.") +
			" If you have an Intel GPU, they are not 3.3 compatible." +
			"Try the 2.1 version.\n"));
	}
	glfwMakeContextCurrent(window);

	// Start GLEW extension handler
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLEW\n");
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

	// Gray background color
	glClearColor(0.686f, 0.933f, 0.933f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	// glEnable(GL_CULL_FACE);

	// enable texturing and bind the depth texture
	glEnable(GL_TEXTURE_2D);

	// Log
	logGLParameters();

	// Create camera
	camera = new Camera(window);

	// Task 1.1 Creating a light source
	// Creating a custom light 
	light = new Light(window,
		vec4{ 1, 1, 1, 1 },
		vec4{ 1, 1, 1, 1 },
		vec4{ 1, 1, 1, 1 },
		vec3{ 0, 17, 5 },
		200.0f
	);

}

int main(void) {
	try {
		initialize();
		createContext();
		mainLoop();
		free();
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
		getchar();
		free();
		return -1;
	}

	return 0;
}