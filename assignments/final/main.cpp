#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>

#include <ilgl/framebuffer.h>
#include <ilgl/ilgl_scene.h>
#include <ilgl/hierarchy.h>

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

//Number of Particle Systems
const int MAX_PARTICLES = 5;

unsigned int shadowWidth = 1024;
unsigned int shadowHeight = 1024;

float shadowMinBias = 0.005;
float shadowMaxBias = 0.015;

float prevFrameTime;
float deltaTime;

bool isDirty = false;

#pragma region imgui values
glm::vec4 startColor = glm::vec4(1,1,1,1);
glm::vec4 endColor = glm::vec4(1,1,1,1);

float opacity = 2.0f;
float fadeInPower = 0.5f;
float fadeOutPower = 0.8f;
float alphaClipThreshhold = 0.06f;

//Vert Shader

glm::vec3 gravity = glm::vec3(0,-980,0);
glm::vec3 wind = glm::vec3(0,0,0);

glm::vec3 emmiterDimensions = glm::vec3(30,3,3);
float rotSpeed = 1.0f;
float rotation = 1.0f;
int rotRandomOffset = 1;
int rotRandomDirection = 1;

float particleSpeed = 0.3f;
glm::vec3 particleDirMin = glm::vec3(0,0,1);
glm::vec3 particleDirMax = glm::vec3(0,0,1);
float particleSpread = 0;
float particleVelStart = 300;
float particleVelEnd = 60;

float particleStartSize = 0.15;
float particleEndSize = 1.0;
#pragma endregion


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
ilgl::FrameBuffer framebuffer;

glm::vec3 lightDir = glm::vec3(0, -1, 0);

ew::Camera lightCam;
float lightCamDist = 5.0f;
glm::vec3 dirLightColor = glm::vec3(1, 1, 1);
float pointLightIntensity = 0.2f;

ew::CameraController cameraController;
ew::Camera camera;

bool drawPointLightSpheres = false;

struct PointLight {
	float radius;
	glm::vec4 color;
	ew::Transform transform;
};

const int MAX_POINT_LIGHTS = 0;

glm::vec4 randomColor()
{
	float r = rand() / ((float)RAND_MAX + 1);
	float g = rand() / ((float)RAND_MAX + 1);
	float b = rand() / ((float)RAND_MAX + 1);

	return glm::vec4(r, g, b, 1.0f);
}

int main() {

	camera.farPlane = 10000.0f;

	GLFWwindow* window = initWindow("Assignment 5", screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	lightCam.orthographic = true;
	lightCam.farPlane = 15;
	lightCam.aspectRatio = 1.0f;
	lightCam.orthoHeight = 16.0f;

	ilgl::ILGL_Scene scene;

	ew::Shader shader = ew::Shader("assets/lit.vert", "assets/lit.frag");
	ew::Shader particlesShader = ew::Shader("assets/ParticleSystem.vert", "assets/ParticleSystem.frag");
	//ew::Shader gBufferShader = ew::Shader("assets/lit.vert", "assets/geometryPass.frag");

	//ew::Shader postProcessShader = ew::Shader("assets/fullquad.vert", "assets/postProcess.frag");
	ew::Shader depthOnlyShader = ew::Shader("assets/depthOnly.vert", "assets/depthOnly.frag");
	//ew::Shader deferredLitShader = ew::Shader("assets/fullquad.vert", "assets/deferredLit.frag");
	ew::Shader fullTex = ew::Shader("assets/fullquad.vert", "assets/fullTex.frag");
	ew::Shader skyboxShader = ew::Shader("assets/skybox.vert", "assets/skybox.frag");

	//ew::Shader billboardShader = ew::Shader("assets/billboard.vert", "assets/lit.frag");

	//std::vector<ew::Model> particleMeshes;
	//std::vector<ew::Transform> particleTransforms;
	ew::Model waterfallMesh = ew::Model("assets/ParticleStack.fbx", true);
	ew::Transform waterfallTransform;
	waterfallTransform.position = glm::vec3(-4, 3, -5);
	waterfallTransform.scale = glm::vec3(0.01, 0.01, 0.01);

	ew::Model smokeMesh = ew::Model("assets/ParticleStack.fbx", true);
	ew::Transform smokeTransform;
	smokeTransform.position = glm::vec3(4, 0, -5);
	smokeTransform.scale = glm::vec3(0.01, 0.01, 0.01);

	ew::Model mistMesh = ew::Model("assets/ParticleStack.fbx", true);
	ew::Transform mistTransform;
	mistTransform.position = glm::vec3(0, 1.5, -5);
	mistTransform.scale = glm::vec3(0.01, 0.01, 0.01);

	ew::Model guiMesh = ew::Model("assets/ParticleStack.fbx", true);
	ew::Transform guiTransform;
	guiTransform.position = glm::vec3(0, 1.5, 0);
	guiTransform.scale = glm::vec3(0.01, 0.01, 0.01);


	//for (int i = 0; i < MAX_PARTICLES; i++)
	//{
	//	particleMeshes.push_back(ew::Model("assets/ParticleStack.fbx", true));
	//	ew::Transform particleTransform;
	//	particleTransform.position = glm::vec3(-4 + i * 2, -1, 0);
	//	particleTransform.scale = glm::vec3(0.01, 0.01, 0.01);
	//	particleTransforms.push_back(particleTransform);
	//}


	ew::Model groundPlane = ew::Model(ew::createPlane(15, 15, 2));
	ew::Transform groundTransform;
	groundTransform.position = glm::vec3(0, -1, 0);

	ew::Transform billboardTransform;
	billboardTransform.position = glm::vec3(0, 5, 0);
	billboardTransform.scale = glm::vec3(1, 1, 1);

	//Skybox
	ew::Model skyboxMesh = ew::Model(ew::createCube(1.0));
	ew::Transform skyboxTransform;
	//skyboxTransform.position = glm::vec3(0.0,100.0,0.0);
	skyboxTransform.scale = glm::vec3(-1000.0);
	std::vector<std::string> skyboxFaces
	{
			"assets/textures/skyboxLeft.jpg",
			"assets/textures/skyboxRight.jpg",
			"assets/textures/skyboxBottom.jpg",
			"assets/textures/skyboxTop.jpg",
			"assets/textures/skyboxFront.jpg" ,
			"assets/textures/skyboxBack.jpg"

	};
	ilgl::Material skyboxMat;
	skyboxMat.colorTexture = ew::createCubeMap(skyboxFaces);

	ilgl::Material monkeyMat;
	monkeyMat.Ka = 0.4;
	monkeyMat.Kd = 0.8;
	monkeyMat.Ks = 0.5;
	monkeyMat.Shininess = 50;
	monkeyMat.colorTexture = ew::loadTexture("assets/color.jpg");
	monkeyMat.normalTexture = ew::loadTexture("assets/normal.jpg");
	
	ilgl::Material waterFlipBook;
	waterFlipBook.colorTexture = ew::loadTexture("assets/textures/waterFlipbook.png", STBI_rgb_alpha);
	ilgl::Material cloudFlipBook;
	cloudFlipBook.colorTexture = ew::loadTexture("assets/textures/Cloud04_8x8.png", STBI_rgb_alpha);
	ilgl::Material singleCloudFlipBook;
	singleCloudFlipBook.colorTexture = ew::loadTexture("assets/textures/cloud.png", STBI_rgb_alpha);



	ilgl::Material mistMat(glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1), singleCloudFlipBook.colorTexture /*flipText*/, 1, 1, 2, 0.004f, 1, 1, 1/*flipSpeed*/, 0/*match phase*/, glm::vec3(0, -40, 0), glm::vec3(0, 0, 0), glm::vec3(40, 40, 40), 1/*rotationSpeed*/, 1/*rotation*/, 1/*randomOffset*/, 1/*Randomdirection*/, 0.1, glm::vec3(0, -.1, 0.0), glm::vec3(1, -.1, 1) /*parDirMax*/, 360, 40, 30, 0.5, 1.25);
	ilgl::Material smokeMat(glm::vec4(0.421f, 0.421f, 0.421f, 1.0f), glm::vec4(1, 1, 1, 1), cloudFlipBook.colorTexture/*flip tex*/, 4, 0.2, 1, 0.01, 8, 8, 1/*flipspeed*/, 0/*match phase*/, glm::vec3(0, 0, 0), glm::vec3(0, 100, 0), glm::vec3(10, 10, 10), 1/*rotspeed*/, 1/*rot*/, 1/*rotRandomOffset*/, 1/*rotDir*/, 0.1, glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)/*maxDir*/, 30, 100, 100, 0.2, 0.8);
	ilgl::Material waterfallMat(glm::vec4(1, 1, 1, 1), glm::vec4(0.702, 1, 1, 1), waterFlipBook.colorTexture/*fliptex*/, 2, 0.5, 0.8, 0.06, 8, 4, 1/*flipspeed*/, 1/*match phase*/, glm::vec3(0, -980, 0), glm::vec3(0, 0, 0)/*wind*/, glm::vec3(30, 3, 3), 1/*rotspeed*/, 1/*rot*/, 1/*rotRandomOffset*/, 1/*rotDir*/, 0.3, glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)/*maxDir*/, 0.3, 300, 60, 0.15, 1);
	
	ilgl::Material imParticles(startColor,endColor,waterFlipBook.colorTexture,opacity,fadeInPower,fadeOutPower,alphaClipThreshhold,8,4,1,1,gravity,wind,emmiterDimensions,rotSpeed,rotation,rotRandomOffset,rotRandomDirection,particleSpeed,particleDirMin,particleDirMax,particleSpread,particleVelStart,particleVelEnd,particleStartSize,particleEndSize);

	int skyboxID = scene.addElement(&skyboxShader, &skyboxMesh, skyboxTransform, skyboxMat);
	int groundID = scene.addElement(&shader, &groundPlane, groundTransform, monkeyMat);
	//int billboardID = scene.addElement(&billboardShader, &groundPlane, billboardTransform, monkeyMat);

	int waterfallID = scene.addElement(&particlesShader, &waterfallMesh, waterfallTransform, waterfallMat);
	int mistID = scene.addElement(&particlesShader, &mistMesh, mistTransform, mistMat);
	int smokeID = scene.addElement(&particlesShader, &smokeMesh, smokeTransform, smokeMat);

	int imParticleID = scene.addElement(&particlesShader, &guiMesh, guiTransform, imParticles);

	

	//Setup for post process Buffer & Fullscreen Quad
	ilgl::FrameBuffer postProcessBuffer = ilgl::FrameBuffer(screenWidth, screenHeight);
	unsigned int dummyVAO;
	glCreateVertexArrays(1, &dummyVAO);

	//Setup for ShadowMap
	shadowMapBuffer = ilgl::FrameBuffer(shadowWidth, shadowHeight, true);
	shadowMapBuffer.checkValidity();

	framebuffer = ilgl::FrameBuffer(screenWidth, screenHeight);
	framebuffer.addAttachment(0, GL_RGBA32F);
	framebuffer.addDepthAttachment();
	framebuffer.checkValidity();
	framebuffer.finalize();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Setup for Camera
	camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
	camera.target = glm::vec3(0.0f, 0.0f, 0.0f); //Look at the center of the scene
	camera.aspectRatio = (float)screenWidth / screenHeight;
	camera.fov = 60.0f; //Vertical field of view, in degrees

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //Back face culling
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); //Depth testing


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		deltaTime = time - prevFrameTime;
		prevFrameTime = time;

		cameraController.move(window, &camera, deltaTime);

		//RENDER
		lightCam.position = glm::vec3(0,0,0) - lightDir * lightCamDist;
		scene.setLightDir(lightDir);
		shadowMapBuffer.use();
		scene.drawSceneDepth(lightCam, depthOnlyShader);

		framebuffer.use();
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.getFBO());
		glClearColor(0.6f, 0.8f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (isDirty)
		{
			ilgl::Material temp(startColor, endColor, waterFlipBook.colorTexture, opacity, fadeInPower, fadeOutPower, alphaClipThreshhold, 8, 4, 1, 1, gravity, wind, emmiterDimensions, rotSpeed, rotation, rotRandomOffset, rotRandomDirection, particleSpeed, particleDirMin, particleDirMax, particleSpread, particleVelStart, particleVelEnd, particleStartSize, particleEndSize);
			scene.setMaterial(imParticleID, temp);
			isDirty = false;
		}
		scene.setShadowBiasMinMax(shadowMinBias, shadowMaxBias);
		scene.setShadowBuffer(shadowMapBuffer.getDepthBuffer());
		scene.drawScene(camera, lightCam);

		//Post Process
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		fullTex.use();
		glBindTextureUnit(0, framebuffer.getColorTexture(0));
		fullTex.setInt("_ColorBuffer", 0);

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

	//if(ImGui::CollapsingHeader("Shadowmap"))
	//{
	//	ImGui::DragFloat("Height", &lightCam.orthoHeight);
	//	ImGui::DragFloat("Far Plane", &lightCam.farPlane);
	//	ImGui::DragFloat("Near Plane", &lightCam.nearPlane);
	//	ImGui::DragFloat("Light Distance", &lightCamDist);
	//	ImGui::DragFloat("Max Bias", &shadowMaxBias, .001f, 0.0f, 0.2);
	//	ImGui::DragFloat("Min Bias", &shadowMinBias, .001f, 0.0f, 0.2);
	//}

	//if (ImGui::DragFloat3("Light Direction", &lightDir.x, 0.1))
	//{
	//	if (glm::length(lightDir) != 0)
	//	{
	//		lightDir = glm::normalize(lightDir);
	//	}
	//}
	//ImGui::ColorPicker4("Directional Light Color", &dirLightColor.x);
	//ImGui::DragFloat("Point Light Intensity", &pointLightIntensity, 0.01f, 0.0f, 1.0f);

	//ImGui::Checkbox("Render Point Light Spheres", &drawPointLightSpheres);

	//if (ImGui::CollapsingHeader("Post Process")) {
	//	if (ImGui::CollapsingHeader("Vignette"))
	//	{
	//		ImGui::Checkbox("Vignette On/Off", &vignetteEffect);
	//		ImGui::SliderFloat("Vignette Distance", &vignette_Intensity, 0.0f, 25.0f);
	//		ImGui::SliderFloat("Vignette Intensity", &vignette_Distance, 0.0f, 1);
	//	}
	//	if (ImGui::CollapsingHeader("Aberration"))
	//	{
	//		ImGui::Checkbox("Aberration On/Off", &aberrationEffect);
	//		ImGui::SliderFloat("Aberration R", &rOffset, -.01f, .01f);
	//		ImGui::SliderFloat("Aberration G", &gOffset, -.01f, .01f);
	//		ImGui::SliderFloat("Aberration B", &bOffset, -.01f, .01f);
	//	}
	//}
	
	if (ImGui::Button("Reset Camera")) {
		resetCamera(camera, cameraController);
	}
	if (ImGui::CollapsingHeader("Particle System"))
	{
		if (ImGui::ColorPicker4("Start Color", &startColor.x))
			isDirty = true;
		if (ImGui::ColorPicker4("End Color", &endColor.x))
			isDirty = true;

		if(ImGui::DragFloat("Opacity", &opacity, 0.01f, 0.0f, 5.0f))
			isDirty = true;
		if(ImGui::DragFloat("Fade In Power", &fadeInPower, 0.01f, 0.0f, 1.0f))
			isDirty = true;
		if(ImGui::DragFloat("Fade Out Power", &fadeOutPower, 0.01f, 0.0f, 1.0f))
			isDirty = true;
		if(ImGui::DragFloat("Alpha Clip Threshhold", &alphaClipThreshhold, 0.001f, 0.0f, 0.5f))
			isDirty = true;

		if(ImGui::DragFloat3("Gravity", &gravity.x, 1.0f, -1000.0f, 1000.0f))
			isDirty = true;
		if(ImGui::DragFloat3("Wind", &wind.x, 0.1f, -100.0f, 100.0f))
			isDirty = true;

		if(ImGui::DragFloat("Rotation Speed", &rotation, 0.01f, 0.0f, 10.0f))
			isDirty = true;

		if(ImGui::DragFloat("Particle Speed", &particleSpeed, 0.01f, 0.0f, 5.0f))
			isDirty = true;
		if(ImGui::DragFloat3("Particle Direction Max", &particleDirMax.x, 0.1f, 0.0f, 10.0f))
			isDirty = true;
		if(ImGui::DragFloat3("Particle Direction Min", &particleDirMin.x, 0.1f, 0.0f, 10.0f))
			isDirty = true;
		if(ImGui::DragFloat("Particle Spread", &particleSpread, 1,0,1))
			isDirty = true;
		if(ImGui::DragFloat("Particle Start Velocity", &particleVelStart, 0.1f, 0.0f, 500.0f))
			isDirty = true;
		if(ImGui::DragFloat("Particle End Velocity", &particleVelEnd, 0.1f, 0.0f, 100.0f))
			isDirty = true;

		if(ImGui::DragFloat("Particle Start Size", &particleStartSize, 0.001f, 0.0f, 1.0f))
			isDirty = true;
		if(ImGui::DragFloat("Particle End Size", &particleEndSize, 0.001f, 0.0f, 1.0f))
			isDirty = true;

		
	}

	ImGui::Text("Add Controls Here!");
	ImGui::End();

	ImGui::Begin("Shadow Map");
	ImGui::BeginChild("Shadow Map");
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::Image((ImTextureID)shadowMapBuffer.getDepthBuffer(), ImVec2(shadowWidth / 4, shadowHeight / 4), ImVec2(0, 1), ImVec2(1, 0));

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

