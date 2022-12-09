#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSL.h"
#include "Program.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "ShapeSkin.h"
#include "Texture.h"
#include "TextureMatrix.h"

#include "BlendedAnim.h"

#include "Shape.h"
#include "Component.h"


using namespace std;
using glm::vec4;
using glm::vec3;

// Stores information in data/input.txt
class DataInput
{
public:
	vector<string> textureData;
	vector< vector<string> > meshData;
	string skeletonData;
};

DataInput dataInput;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the shaders are loaded from
string DATA_DIR = ""; // where the data are loaded from
string DATA_DIR2 = ""; // where the data are loaded from
bool keyToggles[256] = {false};

shared_ptr<Camera> camera = NULL;
vector< shared_ptr<ShapeSkin> > shapes;
map< string, shared_ptr<Texture> > textureMap;
shared_ptr<Program> progSimple = NULL;
shared_ptr<Program> progSkin = NULL;
shared_ptr<Program> prog = NULL;

shared_ptr<Texture> texture;


vector<shared_ptr<Frame>> frames;
double t, t0;

bool drawFrenetFrames = false;


int frameCount = 1; // default to 1, should be replaced by boneParser value
int fileCount = 0;

int frameTick = 0;

bool useBlend = true;

bool temppp = false;


bool paused = true;

bool moveForward = false;


bool currentlyBlending = false;

// NEW ###########


BlendedAnim blendAnim;


glm::mat3 movementMat;
glm::vec3 movementVec = { 0,0,0 };
glm::vec3 rotationVec = { 0,0,0 };

int handIdx = 0;
pair<int, int> handIdxs;

bool useleftHand = true;

shared_ptr<Component> testShape = NULL;


static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
		frameTick++;
		frameTick = frameTick % (frameCount + 1);
		cout << "frame: " << frameTick << " / " << frameCount << "\n";
	}
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
		frameTick--;
		frameTick = frameTick % (frameCount + 1);
		cout << "frame: " << frameTick << " / " << frameCount << "\n";
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
	
		useBlend = !useBlend;
		if (useBlend) { // switch to blended framecount and frames
			frameCount = blendAnim.frameCountBLENDED;
			frames = blendAnim.blendedFrames;
		}
		else {
			frameCount = blendAnim.anims[0]->frameCount;
			frames = blendAnim.anims[0]->frames;
		}
		frameTick = frameTick % (frameCount + 1);

		cout << "frame: " << frameTick << " / " << frameCount << "\n";
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)  { // forward
		//camera->moveForward();
		//movementVec.z += 10;
		moveForward = true;
		//if (temppp) {
		//	movementVec.x -= 10 * sin(rotationVec.y);
		//	movementVec.z -= 10 * cos(rotationVec.y);
		//}
		//else {
		//	movementVec.x += 10 * sin(rotationVec.y);
		//	movementVec.z += 10 * cos(rotationVec.y);
		//}
		//camera->updatePos(movementVec);
		//cout << "movementVec" << to_string(movementVec) << "\n";
		
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		moveForward = false;
	}


	else if (key == GLFW_KEY_S) { // back
		//camera->moveBackward();
		//movementVec.z -= 10;
		//cout << "movementVec" << to_string(movementVec) << "\n";
		//camera->updatePos(movementVec);
	}
	else if (key == GLFW_KEY_A) {
		//camera->moveLeft();
		//movementVec.x += 10;
		//cout << "movementVec" << to_string(movementVec) << "\n";
		//camera->updatePos(movementVec);
	}
	else if (key == GLFW_KEY_D) {
		//camera->moveRight();
		//movementVec.x -= 10;
		//cout << "movementVec" << to_string(movementVec) << "\n";
		//camera->updatePos(movementVec);
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		//handIdx--;
		//handIdx = glm::clamp(handIdx, 0, 80);
		//cout << "idx: " << handIdx << "\n";
	}
	else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		//handIdx++;
		//handIdx = glm::clamp(handIdx, 0, 80);
		//cout << "idx: " << handIdx << "\n";
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) { // PAUSE
		paused = !paused;
	}
	if (key == GLFW_KEY_SPACE) { // JUMP
		if (!currentlyBlending) {
			currentlyBlending = true;
			t = 0;
			blendAnim.test2(frameTick + 1);
			frameTick = 0;
			frames = blendAnim.blendedFrames2;
			frameCount = blendAnim.frameCountBLENDED2;

			//cout << "THIS FAR!\n";
		}
	}
	else if (key == GLFW_KEY_H && action == GLFW_PRESS) { // toggle hand mat indxs
		if (handIdx == handIdxs.first) {
			handIdx = handIdxs.second;
		}
		else {
			handIdx = handIdxs.first;
		}
		cout << "new handIdx: " << handIdx << "\n";
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		cout << "F\n";
		//camera->sf = -camera->sf;
		//camera->rotations.x = -camera->rotations.x;
		temppp = !temppp;
		//camera->rotations.x += 3.14;
	}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	keyToggles[key] = !keyToggles[key];
	
	for(const auto &shape : shapes) {
		shape->getTextureMatrix()->update(key);
	}
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) {
		//
		camera->mouseMoved((float)xmouse, (float)ymouse);
		if(!paused) {
			rotationVec.y = camera->rotations.x;
		}
	}

	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state == GLFW_PRESS) {
		
		camera->mouseClicked2((float)xmouse, (float)ymouse);
		//rotationVec.y = camera->charRotation;
		//rotationVec.y = camera->rotations.x;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) {
		bool shift = mods & GLFW_MOD_SHIFT;
		bool ctrl  = mods & GLFW_MOD_CONTROL;
		bool alt   = mods & GLFW_MOD_ALT;
		camera->mouseClicked((float)xmouse, (float)ymouse, shift, ctrl, alt);
		//camera->mouseClicked2((float)xmouse, (float)ymouse);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {

	}
}

void init()
{
	keyToggles[(unsigned)'c'] = true;
	
	camera = make_shared<Camera>();
	camera->updatePos(movementVec);
	// Create shapes
	for(const auto &mesh : dataInput.meshData) {
		auto shape = make_shared<ShapeSkin>();
		shapes.push_back(shape);
		shape->setTextureMatrixType(mesh[0]);
		shape->loadMesh(DATA_DIR + mesh[0]);
		shape->loadAttachment(DATA_DIR + mesh[1]);
		shape->setTextureFilename(mesh[2]);
		
	}
	
	// For drawing the grid, etc.
	progSimple = make_shared<Program>();
	progSimple->setShaderNames(RESOURCE_DIR + "simple_vert.glsl", RESOURCE_DIR + "simple_frag.glsl");
	progSimple->setVerbose(true);
	
	// For skinned shape, CPU/GPU
	progSkin = make_shared<Program>();
	progSkin->setShaderNames(RESOURCE_DIR + "skin_vert.glsl", RESOURCE_DIR + "skin_frag.glsl");
	progSkin->setVerbose(true);
	


	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	for(auto shape : shapes) { // send in the bind frame to generate inverse bind mats
		shape->init();
	}
	
	progSimple->init();
	progSimple->addUniform("P");
	progSimple->addUniform("MV");
	
	progSkin->init();
	progSkin->addAttribute("aPos");
	progSkin->addAttribute("aNor");
	progSkin->addAttribute("aTex");
	progSkin->addUniform("P");
	progSkin->addUniform("MV");
	progSkin->addUniform("ka");
	progSkin->addUniform("ks");
	progSkin->addUniform("s");
	progSkin->addUniform("kdTex");
	progSkin->addUniform("T");
	
	//prog = make_shared<Program>();
	//prog->setShaderNames(RESOURCE_DIR + "phong_vert.glsl", RESOURCE_DIR + "phong_frag.glsl");
	//prog->init();
	//prog->addUniform("P");
	//prog->addUniform("MV");
	//prog->addUniform("lightPos");
	//prog->addUniform("ka");
	//prog->addUniform("kd");
	//prog->addUniform("ks");
	//prog->addUniform("s");
	//prog->addAttribute("aPos");
	//prog->addAttribute("aNor");
	//prog->setVerbose(false);

	prog = make_shared<Program>();
	prog->setShaderNames(RESOURCE_DIR + "tex_vert.glsl", RESOURCE_DIR + "tex_frag.glsl");
	prog->init();
	prog->setVerbose(true);
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("aPos");
	prog->addAttribute("aTex");
	prog->addUniform("texture0");
	prog->setVerbose(false);

	texture = make_shared<Texture>();
	texture->setFilename(RESOURCE_DIR + "amogus.png");
	//cout << RESOURCE_DIR + "metal_texture_15_by_wojtar_stock.jpg" << "\n";
	texture->init();
	texture->setUnit(0);
	texture->setWrapModes(GL_REPEAT, GL_REPEAT);
	//exit(0);

	testShape = make_shared<Component>();
	testShape->shape = make_shared<Shape>();
	testShape->shape->loadMesh(RESOURCE_DIR + "amogus.obj");
	testShape->shape->init();

	//testShape->meshTrans = { -20,0,0};
	testShape->kd = { 1,0,0 };
	testShape->scale = { 15,15,15 };
	testShape->angles = {0, 0, 90};
	//testShape->scale = { 1,1,1 };
	// Bind the texture to unit 1.
	int unit = 1;
	progSkin->bind();
	glUniform1i(progSkin->getUniform("kdTex"), unit);
	progSkin->unbind();
	
	for(const auto &filename : dataInput.textureData) {
		auto textureKd = make_shared<Texture>();
		textureMap[filename] = textureKd;
		textureKd->setFilename(DATA_DIR + filename);
		textureKd->init();
		textureKd->setUnit(unit); // Bind to unit 1
		textureKd->setWrapModes(GL_REPEAT, GL_REPEAT);
	}
	
	// Initialize time.
	glfwSetTime(0.0);
	
	GLSL::checkError(GET_FILE_LINE);
}

void drawFrenetFrame() {
	float LINE_THICKNESS = 2.5f;
	float LINE_DISTANCE = 5.0f;
	
	glLineWidth(LINE_THICKNESS);
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0, 0, 0);
	glVertex3f(LINE_DISTANCE, 0, 0);

	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(0, 0, 0);
	glVertex3f(0, LINE_DISTANCE, 0);

	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, LINE_DISTANCE);

	glEnd();
}

void drawBoneFrames(shared_ptr<MatrixStack>& P, shared_ptr<MatrixStack>& MV,int frame = 0) { // default to bind pose (frame 0)
	progSimple->bind();
	for (int i = 0; i < frames[frame]->bones.size(); i++) {
		MV->pushMatrix();
		MV->multMatrix(frames[frame]->bones[i].mat);
		glUniformMatrix4fv(progSimple->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(progSimple->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		drawFrenetFrame();
		MV->popMatrix();
	}
	progSimple->unbind();
}

void render()
{
	// Update time.
	double t1 = glfwGetTime();
	float dt = (float)(t1 - t0);
	if(keyToggles[(unsigned)'p']) { // THIS IS TOGGLING WITH SPACE
		t += dt;
	}
	t0 = t1;
	if (moveForward) {
		//cout << rotationVec.y;
		if (temppp) {
			movementVec.x -= 10 * sin(rotationVec.y);
			movementVec.z -= 10 * cos(rotationVec.y);
		}
		else {
			movementVec.x += 10 * sin(rotationVec.y);
			movementVec.z += 10 * cos(rotationVec.y);
		}
		camera->updatePos(movementVec);
	}

	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Use the window size for camera.
	glfwGetWindowSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyToggles[(unsigned)'c']) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyToggles[(unsigned)'z']) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawFrenetFrames = true;
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		drawFrenetFrames = false;
	}
	
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	
	// Draw grid
	progSimple->bind();
	glUniformMatrix4fv(progSimple->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(progSimple->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	float gridSizeHalf = 1600.0f;
	int gridNx = 41;
	int gridNz = 41;
	glLineWidth(1);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_LINES);
	for(int i = 0; i < gridNx; ++i) {
		float alpha = i / (gridNx - 1.0f);
		float x = (1.0f - alpha) * (-gridSizeHalf) + alpha * gridSizeHalf;
		glVertex3f(x, 0, -gridSizeHalf);
		glVertex3f(x, 0,  gridSizeHalf);
	}
	for(int i = 0; i < gridNz; ++i) {
		float alpha = i / (gridNz - 1.0f);
		float z = (1.0f - alpha) * (-gridSizeHalf) + alpha * gridSizeHalf;
		glVertex3f(-gridSizeHalf, 0, z);
		glVertex3f( gridSizeHalf, 0, z);
	}
	glEnd();
	progSimple->unbind();
	
	// draw bone matrix frenet frames

	// Draw character
	double fps = 30;
	if (!paused) {
		frameTick = ((int)floor(t * fps)) % (frameCount + 1);
	}
	
	if (frameTick == 0) {
		frameTick = 1;
	}
	//cout << frameTick << "\n";
	
	if (drawFrenetFrames) {
		drawBoneFrames(P, MV, frameTick);
	}
	prog->bind();
	texture->bind(prog->getUniform("texture0"));
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	
	//MV->translate(movementVec);
	//cout << to_string(MV->topMatrix()) << "\n\n";

	//cout << to_string(MV->topMatrix()[2]) << "\n";

	//MV->translate({ 0,0,0 });
	MV->translate(movementVec);
	//MV->rotate(rotationVec.y,vec3(0,1,0));
	
	MV->rotate(rotationVec.y,vec3(0,1,0));

	if (temppp) {
		MV->rotate(3.14, vec3(0, 1, 0));
	}


	//MV->rotate(camera->rotations.x, vec3(0, 1, 0));
	
	//MV->rotate(forward.x,vec3(1,0,0));
	//MV->rotate(forward.y,vec3(0,1,0));
	//MV->rotate(forward.z,vec3(0,0,1));

	if (handIdx != -1) {
		MV->pushMatrix();
		//vec4 temp = frames[frameTick]->bones[handIdx].mat[3];
		//MV->translate(vec3(temp));
		MV->multMatrix(frames[frameTick]->bones[handIdx].mat);
		testShape->draw(*MV, prog, t);

		MV->popMatrix();
	}

	prog->unbind();

	
	progSkin->bind();
	for (const auto& shape : shapes) {
		MV->pushMatrix();

		// Draw bone
		// TODO: implement

		// Draw skin
		
		textureMap[shape->getTextureFilename()]->bind(progSkin->getUniform("kdTex"));
		glLineWidth(1.0f); // for wireframe
		glUniformMatrix4fv(progSkin->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(progSkin->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniform3f(progSkin->getUniform("ka"), 0.1f, 0.1f, 0.1f);
		glUniform3f(progSkin->getUniform("ks"), 0.1f, 0.1f, 0.1f);
		glUniform1f(progSkin->getUniform("s"), 200.0f);
		shape->setProgram(progSkin);
		shape->update(frameTick, frames); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//shape->update(frame, blendAnim.anims[0]->frames);
		shape->draw(frameTick);
		

		MV->popMatrix();
	}
	progSkin->unbind();
	// Pop matrix stacks.
	MV->popMatrix();
	P->popMatrix();

	//cout << frameTick << "/" << frameCount << "\n";

	if (currentlyBlending && frameTick == frameCount) {
		//cout << "return to normal!\n";
		currentlyBlending = false;
		t = 0;
		
		frames = blendAnim.anims[0]->frames;
		frameCount = blendAnim.anims[0]->frameCount;
	}




	GLSL::checkError(GET_FILE_LINE);
}

void loadDataInputFile(string DATA_DIR)
{
	shared_ptr<Anim> anim = make_shared<Anim>();
	string filename = DATA_DIR + "input.txt";
	ifstream in;
	in.open(filename);
	if(!in.good()) {
		cout << "Cannot read " << filename << endl;
		return;
	}
	cout << "Loading " << filename << endl;
	
	string line;
	while (getline(in, line)) {
		if (line.empty()) {
			continue;
		}
		// Skip comments
		if (line.at(0) == '#') {
			continue;
		}
		//cout << line << "\n";
		// Parse lines`
		string key, value;
		stringstream ss(line);
		// key
		ss >> key;

		if (key.compare("TEXTURE") == 0) {
			ss >> value;
			if (fileCount == 0) {
				dataInput.textureData.push_back(value);
			}
		}
		else if (key.compare("MESH") == 0) { // HERE IS THE PROBLEM?
			if (fileCount == 0) {
				vector<string> mesh;
				ss >> value;
				mesh.push_back(value); // obj
				ss >> value;
				mesh.push_back(value); // skin
				ss >> value;
				mesh.push_back(value); // texture
				dataInput.meshData.push_back(mesh);
			}
		}
		else if (key.compare("SKELETON") == 0) {
			cout << "hi\n";
			ss >> value;
			cout << value << "\n";
			dataInput.skeletonData = value;
			anim->genBoneFrames(DATA_DIR, value);
			
			
			if (fileCount == 0) {
				//frames = anim->frames;
			}
		}
		else if (key.compare("STATICTRANS") == 0) {
			cout << "c\n\n";
			ss >> value;
			//stat_trans.Parse(DATA_DIR, value);
		}
		else if (key.compare("LOCALSKELE") == 0) {
			cout << "c\n\n";
			ss >> value;
			//anim.genStaticTransforms(DATA_DIR, value);
		}
		else if (key.compare("PCHIERARCHY") == 0) {
			ss >> value;
			anim->genHierarchy(DATA_DIR, value);
			blendAnim.boneCount = anim->boneCount;
		}
		else if (key.compare("LOCALBIND") == 0) {
			cout << "c\n\n";
			ss >> value;
			//local_bind.Parse(DATA_DIR, value);
		}
		else {
			cout << "Unkown key word: " << key << endl;
			exit(1);
		}
	}

	fileCount++;
	blendAnim.anims.push_back(anim);
	in.close();
}

int main(int argc, char **argv)
{
	if(argc < 4) {
		cout << "Usage: A2 <SHADER DIR> <DATA DIR1> <DATA DIR2>" << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");
	DATA_DIR = argv[2] + string("/");
	DATA_DIR2 = argv[3] + string("/");

	blendAnim = BlendedAnim(); // !!!

	loadDataInputFile(DATA_DIR);
	loadDataInputFile(DATA_DIR2);
	
	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "Miles Versaw", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Initialize scene.
	init();


	

	//blendAnim.test();
	handIdxs = blendAnim.getHandIdx();
	handIdx = handIdxs.first;

	//blendAnim.test2(5);

	//frameCount = blendAnim.anims[0]->frameCount;
	
	//frames = blendAnim.blendedFrames; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//frameCount = blendAnim.frameCountBLENDED;

	frames = blendAnim.anims[0]->frames;
	frameCount = blendAnim.anims[0]->frameCount;

	//cout << "#BFrames " << frames.size() << "\n";
	//cout << "FC: " << frameCount << "\n";

	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		if(!glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
			// Render scene.
			render();
			// Swap front and back buffers.
			glfwSwapBuffers(window);
		}
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
