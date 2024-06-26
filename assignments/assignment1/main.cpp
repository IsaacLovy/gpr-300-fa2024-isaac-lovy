#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>

#include <ilgl/framebuffer.h>

#include <ew/procGen.h>
#include <ew/shader.h>
#include <ew/model.h>
#include <ew/camera.h>
#include <ew/transform.h>
#include <ew/cameraController.h>
#include <ew/texture.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* initWindow(const char* title, int width, int height);
void drawUI(ew::Camera* camera, ew::CameraController* cameraController);

//Global state
int screenWidth = 1080;
int screenHeight = 720;
float prevFrameTime;
float deltaTime;

void resetCamera(ew::Camera* camera, ew::CameraController* controller)
{
	camera->position = glm::vec3(0, 0, 5.0f);
	camera->target = glm::vec3(0);
	controller->yaw = controller->pitch = 0;
}

struct Material {
	float Ka = 0.25;
	float Kd = 0.8;
	float Ks = 0.5;
	float Shininess = 50;
}material;

//Post Process State
float vignette_Intensity = 15;
float vignette_Distance = 0.25;

float rOffset = 0;
float gOffset = 0;
float bOffset = 0;

int main() {
	GLFWwindow* window = initWindow("Assignment 1", screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	ew::Shader shader = ew::Shader("assets/lit.vert", "assets/lit.frag");

	ew::Shader postProcessShader = ew::Shader("assets/fullquad.vert", "assets/postProcess.frag");

	ew::Model monkeyModel = ew::Model("assets/suzanne.fbx");
	ew::Transform monkeyTransform;
	
	GLuint colorTexture = ew::loadTexture("assets/color.jpg");
	GLuint normalTexture = ew::loadTexture("assets/normal.jpg");

	//Make "_MainTex" sampler2D sample from the 2D texture bound to unit 0
	shader.use();

	ilgl::FrameBuffer postProcessBuffer = ilgl::FrameBuffer(screenWidth, screenHeight);

	unsigned int dummyVAO;
	glCreateVertexArrays(1, &dummyVAO);

	ew::CameraController cameraController;
	ew::Camera camera;

	camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
	camera.target = glm::vec3(0.0f, 0.0f, 0.0f); //Look at the center of the scene
	camera.aspectRatio = (float)screenWidth / screenHeight;
	camera.fov = 60.0f; //Vertical field of view, in degrees

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //Back face culling
	glEnable(GL_DEPTH_TEST); //Depth testing

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		deltaTime = time - prevFrameTime;
		prevFrameTime = time;

		cameraController.move(window, &camera, deltaTime);

		//Rotate model around Y axis
		monkeyTransform.rotation = glm::rotate(monkeyTransform.rotation, deltaTime, glm::vec3(0.0, 1.0, 0.0));


		//RENDER
		postProcessBuffer.use();
		shader.use();
		//Bind brick texture to texture unit 0 
		glBindTextureUnit(0, colorTexture);
		shader.setInt("_MainTex", 0);
		glBindTextureUnit(1, normalTexture);
		shader.setInt("_NormalTex", 1);
		shader.setMat4("_Model", monkeyTransform.modelMatrix());
		shader.setMat4("_ViewProjection", camera.projectionMatrix() * camera.viewMatrix());
		shader.setVec3("_EyePos", camera.position);
		shader.setFloat("_Material.Ka", material.Ka);
		shader.setFloat("_Material.Kd", material.Kd);
		shader.setFloat("_Material.Ks", material.Ks);
		shader.setFloat("_Material.Shininess", material.Shininess);
		monkeyModel.draw(); //Draws monkey model using current shader

		//Render post process to backbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.6f, 0.8f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		postProcessShader.use();
		glBindTextureUnit(0, postProcessBuffer.getColorTexture());
		//postProcessShader.setInt("_ColorBuffer", 0);
		postProcessShader.setFloat("_Vignette_I", vignette_Intensity);
		postProcessShader.setFloat("_Vignette_D", vignette_Distance);
		postProcessShader.setFloat("_RedOffset", rOffset);
		postProcessShader.setFloat("_GreenOffset", gOffset);
		postProcessShader.setFloat("_BlueOffset", bOffset);
		glBindVertexArray(dummyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		drawUI(&camera, &cameraController);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void drawUI(ew::Camera* camera, ew::CameraController* cameraController) {
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Settings");

	if (ImGui::CollapsingHeader("Material")) {
		ImGui::SliderFloat("AmbientK", &material.Ka, 0.0f, 1.0f);
		ImGui::SliderFloat("DiffuseK", &material.Kd, 0.0f, 1.0f);
		ImGui::SliderFloat("SpecularK", &material.Ks, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &material.Shininess, 2.0f, 1024.0f);
	}

	if (ImGui::CollapsingHeader("Post Process")) {
		if (ImGui::CollapsingHeader("Vignette"))
		{
			ImGui::SliderFloat("Vignette Distance", &vignette_Intensity, 0.0f, 25.0f);
			ImGui::SliderFloat("Vignette Intensity", &vignette_Distance, 0.0f, 1);
		}
		if (ImGui::CollapsingHeader("Aberration"))
		{
			ImGui::SliderFloat("Aberration R", &rOffset, -.01f, .01f);
			ImGui::SliderFloat("Aberration G", &gOffset, -.01f, .01f);
			ImGui::SliderFloat("Aberration B", &bOffset, -.01f, .01f);
		}
	}
	
	if (ImGui::Button("Reset Camera")) {
		resetCamera(camera, cameraController);
	}

	ImGui::Text("Add Controls Here!");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	screenWidth = width;
	screenHeight = height;
}

/// <summary>
/// Initializes GLFW, GLAD, and IMGUI
/// </summary>
/// <param name="title">Window title</param>
/// <param name="width">Window width</param>
/// <param name="height">Window height</param>
/// <returns>Returns window handle on success or null on fail</returns>
GLFWwindow* initWindow(const char* title, int width, int height) {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return nullptr;
	}

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return nullptr;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	return window;
}

