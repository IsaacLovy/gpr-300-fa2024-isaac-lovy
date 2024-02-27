#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>

#include <ilgl/framebuffer.h>
#include <ilgl/ilgl_scene.h>

#include <ew/procGen.h>
#include <ew/shader.h>
#include <ew/model.h>
#include <ew/camera.h>
#include <ew/transform.h>
#include <ew/procGen.h>
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

unsigned int shadowWidth = 1024;
unsigned int shadowHeight = 1024;

float shadowMinBias = 0.005;
float shadowMaxBias = 0.015;

float prevFrameTime;
float deltaTime;

void resetCamera(ew::Camera* camera, ew::CameraController* controller)
{
	camera->position = glm::vec3(0, 0, 5.0f);
	camera->target = glm::vec3(0);
	controller->yaw = controller->pitch = 0;
}

//Post Process States
bool vignetteEffect = false;
bool aberrationEffect = false;
float vignette_Intensity = 15;
float vignette_Distance = 0.25;

float rOffset = 0;
float gOffset = 0;
float bOffset = 0;

GLuint colorTexture;
GLuint normalTexture;

ilgl::FrameBuffer shadowMapBuffer;

glm::vec3 lightDir = glm::vec3(0, -1, 0);

ew::Camera lightCam;
float lightCamDist = 5.0f;

ew::CameraController cameraController;
ew::Camera camera;

int main() {
	GLFWwindow* window = initWindow("Assignment 1", screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	lightCam.orthographic = true;
	lightCam.farPlane = 15;
	lightCam.aspectRatio = 1.0f;
	lightCam.orthoHeight = 16.0f;

	ilgl::ILGL_Scene scene;

	//ew::Shader shader = ew::Shader("assets/lit.vert", "assets/lit.frag");
	ew::Shader gBufferShader = ew::Shader("assets/lit.vert", "assets/geometryPass.frag");
	//ew::Shader postProcessShader = ew::Shader("assets/fullquad.vert", "assets/postProcess.frag");
	ew::Shader depthOnlyShader = ew::Shader("assets/depthOnly.vert", "assets/depthOnly.frag");
	ew::Shader deferredLitShader = ew::Shader("assets/fullquad.vert", "assets/deferredLit.frag");

	ew::Model monkeyModel = ew::Model("assets/suzanne.fbx");
	ew::Transform monkeyTransform;

	ew::Model groundPlane = ew::Model(ew::createPlane(15, 15, 2));
	ew::Transform groundTransform;
	groundTransform.position = glm::vec3(0, -1, 0);

	ilgl::Material monketMat;
	monketMat.Ka = 0.4;
	monketMat.Kd = 0.8;
	monketMat.Ks = 0.5;
	monketMat.Shininess = 50;
	monketMat.colorTexture = ew::loadTexture("assets/color.jpg");
	monketMat.normalTexture = ew::loadTexture("assets/normal.jpg");

	int monkeyID = scene.addElement(&gBufferShader, &monkeyModel, monkeyTransform, monketMat);
	int groundID = scene.addElement(&gBufferShader, &groundPlane, groundTransform, monketMat);

	//Setup for post process Buffer & Fullscreen Quad
	ilgl::FrameBuffer postProcessBuffer = ilgl::FrameBuffer(screenWidth, screenHeight);
	unsigned int dummyVAO;
	glCreateVertexArrays(1, &dummyVAO);

	//Setup for ShadowMap
	shadowMapBuffer = ilgl::FrameBuffer(shadowWidth, shadowHeight, true);

	ilgl::FrameBuffer gBufffer = ilgl::FrameBuffer();
	gBufffer.setResolution(screenWidth, screenHeight);
	gBufffer.addAttachment(0, GL_RGB32F);
	gBufffer.addAttachment(1, GL_RGB16F);
	gBufffer.addAttachment(2, GL_RGB8);
	gBufffer.addDepthAttachment();

	//Setup for Camera
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
		//monkeyTransform.rotation = glm::rotate(monkeyTransform.rotation, deltaTime, glm::vec3(0.0, 1.0, 0.0));

		//RENDER

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		//Shadow Pass
		lightCam.position = monkeyTransform.position - lightDir * lightCamDist;
		scene.setLightDir(lightDir);
		shadowMapBuffer.use();
		scene.drawSceneDepth(lightCam, depthOnlyShader);

		//Geometry Pass
		glBindFramebuffer(GL_FRAMEBUFFER, gBufffer.getFBO());
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.drawScene(camera, lightCam);

		//Lighting Pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.6f, 0.8f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		deferredLitShader.use();
		glBindTextureUnit(0, gBufffer.getColorTexture(0));
		glBindTextureUnit(1, gBufffer.getColorTexture(1));
		glBindTextureUnit(2, gBufffer.getColorTexture(2));
		glBindTextureUnit(3, gBufffer.getDepthBuffer());
		deferredLitShader.setInt("_gWorldPos", 0);
		deferredLitShader.setInt("_gWorldNormal", 1);
		deferredLitShader.setInt("_gAlbedo", 2);
		deferredLitShader.setInt("_ShadowMap", 3);
		scene.setShadowBiasMinMax(shadowMinBias, shadowMaxBias);
		scene.setShadowBuffer(shadowMapBuffer.getDepthBuffer());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.6f, 0.8f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	/*if (ImGui::CollapsingHeader("Material")) {
		ImGui::SliderFloat("AmbientK", &material.Ka, 0.0f, 1.0f);
		ImGui::SliderFloat("DiffuseK", &material.Kd, 0.0f, 1.0f);
		ImGui::SliderFloat("SpecularK", &material.Ks, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &material.Shininess, 2.0f, 1024.0f);
	}*/

	if(ImGui::CollapsingHeader("Shadowmap"))
	{
		ImGui::DragFloat("Height", &lightCam.orthoHeight);
		ImGui::DragFloat("Far Plane", &lightCam.farPlane);
		ImGui::DragFloat("Near Plane", &lightCam.nearPlane);
		ImGui::DragFloat("Light Distance", &lightCamDist);
		ImGui::DragFloat("Max Bias", &shadowMaxBias, .001f, 0.0f, 0.2);
		ImGui::DragFloat("Min Bias", &shadowMinBias, .001f, 0.0f, 0.2);
	}

	if (ImGui::DragFloat3("Light Direction", &lightDir.x, 0.1))
	{
		if (glm::length(lightDir) != 0)
		{
			lightDir = glm::normalize(lightDir);
		}
	}

	if (ImGui::CollapsingHeader("Post Process")) {
		if (ImGui::CollapsingHeader("Vignette"))
		{
			ImGui::Checkbox("Vignette On/Off", &vignetteEffect);
			ImGui::SliderFloat("Vignette Distance", &vignette_Intensity, 0.0f, 25.0f);
			ImGui::SliderFloat("Vignette Intensity", &vignette_Distance, 0.0f, 1);
		}
		if (ImGui::CollapsingHeader("Aberration"))
		{
			ImGui::Checkbox("Aberration On/Off", &aberrationEffect);
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

	ImGui::Begin("Shadow Map");
	ImGui::BeginChild("Shadow Map");
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::Image((ImTextureID)shadowMapBuffer.getDepthBuffer(), windowSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
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

