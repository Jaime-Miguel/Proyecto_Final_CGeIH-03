#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
GLuint LoadTextureFromFile(const char* path);
// Funciones creadas
//Dibuja las ventanas de diferentes edificios.
void Dibujar_Vidrios(Shader shader, Model* vidrios, GLint modelLoc); 
//Animación por Keyframes de un pajaro volando
void AnimarPajaro(Shader& shader, GLint modelLoc, Model& body, Model& wingR, Model& wingL, float tiempo, glm::vec3 centro, float radio);
// Animacion Keyframes de un quetzal
void AnimarQuetzal(Shader& shader, GLint modelLoc, Model& body, Model& head, Model& tail, glm::vec3 posicion, bool animar, float time);
// Animacion basica de chica perdida
void AnimarPersonaje(Shader& shader, GLint modelLoc, Model& piernas, Model& torso, glm::vec3 posicion, float escala, float tiempo);
// Animacion de fotogrago
//void AnimarFotografo(Shader& shader, GLint modelLoc, Model& base, Model& cabeza, glm::vec3 posicion, float escala, float tiempo);
void AnimarCaballo(Shader& shader, GLint modelLoc, Model& parteTrasera, Model& parteDelantera, glm::vec3 posicion, float escala, float tiempo);

// Dibuja un objeto compuesto por dos partes (ej. tronco y hojas)
void DibujarArboles(Shader& shader, GLint modelLoc, Model& tronco, Model& hojas, glm::vec3 posicion, float escala);
// Dibuja un solo arbol
void DibujarArbol(Shader& shader, GLint modelLoc, Model& arbol, Model& hoja, glm::vec3 posicion, float angulo);
// Dibuja el modelo de flores con su rotación específica
void DibujarFlor(Shader& shader, GLint modelLoc, Model& modeloFlor, glm::vec3 posicion, float escala, float angulo);



// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera Inicia en la entrada del museo
Camera  camera(glm::vec3(50.0f, 3.0f, 45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
int cant_pisos = 20;
float repetir = 0.0f;

glm::vec3 centro = glm::vec3(-16.0f, 1.0f, -16.0f);		//Centro del museo
glm::vec3 eje_rotacion_cuadro = glm::vec3(1.0f, 0.0f, 0.0f);
glm::mat4 modelTemp(1.0f);
// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-15.0f,4.0f, 8.5f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

glm::vec3 posicionesCuadros[] = {
	glm::vec3(0.0f, 2.0f, 1.0f),
	glm::vec3(5.0f, 1.0f, 1.0f),
	glm::vec3(8.0f, 1.0f, 1.0f),
	glm::vec3(11.0f, 1.0f, 1.0f),
	glm::vec3(15.0f, 1.5f, 1.0f),
	glm::vec3(19.0f, 1.5f, 1.5f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	Model vidrios[] = {
		Model((char*)"Models/Cristal1.obj"),
		Model((char*)"Models/Cristal2.obj"),
		Model((char*)"Models/Cristal3.obj")
	};

	Model Ball((char*)"Models/ball.obj");
	Model MuseoAnahuacalli((char*)"Models/Museo_Anahuacalli.obj");
	Model Cesped((char*)"Models/Cesped.obj");
	Model Cuadro01((char*)"Models/Cuadro1.obj");
	Model Cuadro02((char*)"Models/Cuadro2.obj");
	Model Cuadro03((char*)"Models/Cuadro3.obj");
	Model Cuadro04((char*)"Models/Cuadro4.obj");
	Model Mural01((char*)"Models/Mural1.obj");
	Model Mural02((char*)"Models/Mural2.obj");
	Model pinoL((char*)"Models/PinosHojas.obj");
	Model pinoB((char*)"Models/PinosRama.obj");
	Model flores((char*)"Models/Flores.obj");
	Model tree((char*)"Models/aviarioarbol.obj");
	Model treeL((char*)"Models/hojasaviario.obj");
	//Modelos animados
	// Pajaro
	Model birdB((char*)"Models/pajaro1body.obj");
	Model birdWR((char*)"Models/pajaro1der.obj");
	Model birdWL((char*)"Models/pajaro1izq.obj");
	// Quetzal
	Model quetzalB((char*)"Models/pajaro2body.obj");
	Model quetzalH((char*)"Models/pajaro2head.obj");
	Model quetzalT((char*)"Models/pajaro2tail.obj");
	// Chica perdida
	Model girlB((char*)"Models/lostGirlBody.obj");
	Model girlL((char*)"Models/lostGirlLegs.obj");
	// Fotografo
	//Model photoB((char*)"Models/photoBody.obj");
	//Model photoL((char*)"Models/photoLegs.obj");
	//GLuint texturaPhoto = LoadTextureFromFile("Models/rqnger-tex.jpg");
	Model horseB((char*)"Models/horseB.obj");
	Model horseF((char*)"Models/horseF.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float rotFlores = 45.0f;


		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse1"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);


		// Point light 1
		glm::vec3 lightColor = glm::vec3(0);
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		// Luz Cafeteria
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.5f, 0.3f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.1f, 0.05f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 0.1f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model = glm::mat4(1);


		//Se dibuja esfera para el centro del patio principal
		model = glm::translate(model, centro);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		glm::vec3 centroPajaro = glm::vec3(20.0f, 2.0f, -10.0f);
		float radio = 2.0f;
		glm::vec3 posicionQ = glm::vec3(17.0f, -3.0f, -62.0f);
		glm::vec3 girlPos = glm::vec3(-4.0f, 7.5f, -30.0f);
		glm::vec3 photoPos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 posCaballo = glm::vec3(19.0f, 0.0f, -80.0f);

		AnimarPajaro(lightingShader, modelLoc, birdB, birdWR, birdWL, currentFrame, centroPajaro, radio);
		AnimarQuetzal(lightingShader, modelLoc, quetzalB, quetzalH, quetzalT, posicionQ, true,currentFrame);
		AnimarPersonaje(lightingShader, modelLoc, girlL, girlB, girlPos, 0.6f, currentFrame);
		AnimarCaballo(lightingShader, modelLoc, horseB, horseF, posCaballo, 2.0f, currentFrame);



		
		

		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(40.0f, 0.0f, -30.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(40.0f, 0.0f, -40.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(40.0f, 0.0f, -50.0f), 0.5);
		//Cubriendo areas verdes costadod derecho entrada
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(78.0f, 0.0f, -5.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(88.0f, 0.0f, -5.0f), 0.5);
		
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(78.0f, 0.0f, -15.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(88.0f, 0.0f, -15.0f), 0.5);
		
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(78.0f, 0.0f, -25.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(88.0f, 0.0f, -25.0f), 0.5);
		
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(78.0f, 0.0f, -35.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(88.0f, 0.0f, -35.0f), 0.5);
		
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(78.0f, 0.0f, -45.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(88.0f, 0.0f, -45.0f), 0.5);
		
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(78.0f, 0.0f, -55.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(88.0f, 0.0f, -55.0f), 0.5);
		
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(78.0f, 0.0f, -65.0f), 0.5);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(88.0f, 0.0f, -65.0f), 0.5);

		//Cubriendo areas verdes costado izquierdo museo
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(10.0f, 0, -17.0f), 1.0);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(0.0f, 0, -17.0f), 1.0);
		
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(10.0f, 0, -27.0f), 1.0);
		DibujarArboles(lightingShader, modelLoc, pinoB, pinoL, glm::vec3(0.0f, 0, -27.0f), 1.0);

		DibujarArbol(lightingShader, modelLoc, tree, treeL, glm::vec3(30.0f, -5.0f, -70.0f),0.0f);
		DibujarArbol(lightingShader, modelLoc, tree, treeL, glm::vec3(20.0f, -5.0f, -95.0f), 90.0f);

		
		


		DibujarFlor(lightingShader, modelLoc, flores, glm::vec3(30.0f, 0.5f, -16.0f), 0.5f, 90.0f);
		DibujarFlor(lightingShader, modelLoc, flores, glm::vec3(15.0f, 0.5f, -4.0f), 0.5f, 90.0f);
		DibujarFlor(lightingShader, modelLoc, flores, glm::vec3(-3.0f, 0.5f, -4.0f), 0.5f, 90.0f);
		
		//Se dibuja el patio central
		model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		MuseoAnahuacalli.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Se dibuja el primer cuadro del museo
		model = glm::mat4(1);
		model = glm::translate(model, posicionesCuadros[0]);
		model = glm::rotate(model, glm::radians(90.0f), eje_rotacion_cuadro);
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Cuadro01.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		// Se dibuja el segundo cuadro del museo
		model = glm::mat4(1);
		model = glm::translate(model, posicionesCuadros[1]);
		model = glm::rotate(model, glm::radians(90.0f), eje_rotacion_cuadro);
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Cuadro02.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		// Se dibuja el tercer cuadro del museo
		model = glm::mat4(1);
		model = glm::translate(model, posicionesCuadros[2]);
		model = glm::rotate(model, glm::radians(90.0f), eje_rotacion_cuadro);
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Cuadro03.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		// Se dibuja el cuarto cuadro del museo
		model = glm::mat4(1);
		model = glm::translate(model, posicionesCuadros[3]);
		model = glm::rotate(model, glm::radians(90.0f), eje_rotacion_cuadro);
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Cuadro04.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		Dibujar_Vidrios(lightingShader, vidrios, modelLoc);

		// Se dibuja el cuadro mas grande del museo
		model = glm::mat4(1);
		model = glm::translate(model, posicionesCuadros[4]);
		model = glm::rotate(model, glm::radians(90.0f), eje_rotacion_cuadro);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Mural01.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);




		// Se dibuja el segundo cuadro del mural del museo
		model = glm::mat4(1);
		model = glm::translate(model, posicionesCuadros[5]);
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f,1.0f,0.0f));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Mural02.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);






		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Se dibuja la primera luz puntual
		// model = glm::mat4(1);
		// model = glm::translate(model, pointLightPositions[0]);
		// model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		// glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		// Obtiene la posición actual de la cámara
		glm::vec3 pos = camera.GetPosition();

		// Imprime en formato listo para copiar y pegar (ej: 10.5f, 2.0f, -5.0f)
		std::cout << "\nCOORDENADAS CAPTURADAS: " << std::endl;
		std::cout << "glm::vec3(" << pos.x << "f, " << pos.y << "f, " << pos.z << "f)" << std::endl;

		// Opcional: Imprimir también la dirección hacia donde miras (Yaw/Pitch)
		// por si necesitas rotar el objeto.
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void Dibujar_Vidrios(Shader shader, Model* vidrios, GLint modelLoc) {
	// Dibuja el Vidrio para las ventanas
	glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa

	glm::mat4 model = glm::mat4(1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shader.Program, "transparency"), 1);
	vidrios[0].Draw(shader);
	glBindVertexArray(0);

	// Dibuja el Vidrio para las ventanas
	model = glm::mat4(1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shader.Program, "transparency"), 1);
	vidrios[1].Draw(shader);
	glBindVertexArray(0);

	// Dibuja el Vidrio para las ventanas
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(-16.0f, 0.0f, 0.0f));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shader.Program, "transparency"), 1);
	vidrios[1].Draw(shader);
	glBindVertexArray(0);

	// Dibuja el Vidrio para las ventanas
	model = glm::mat4(1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shader.Program, "transparency"), 1);
	vidrios[2].Draw(shader);
	glBindVertexArray(0);

	glDisable(GL_BLEND);  //Desactiva el canal alfa 
}

void AnimarPajaro(Shader& shader, GLint modelLoc, Model& body, Model& wingR, Model& wingL, float tiempo, glm::vec3 centro, float radio)
{
	float speed = 1.0f; // Velocidad de órbita
	float speedWings = 3.0f;
	float amplWings = 2.0f;
	// Cálculo de la posición (Trayectoria Circular)
	float posX = cos(tiempo * speed) * radio;
	float posZ = sin(tiempo * speed) * radio;

	// Calculamos el ángulo de rotación para que el pájaro mire al frente mientras gira
	float rotacionY = -tiempo * speed;
	//Calcula el ángulo del aleteo de las alas
	float angleWings = sin(tiempo * speedWings) * amplWings;
	// Matriz base (Padre)
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, centro + glm::vec3(posX, 0.0f, posZ));
	model = glm::rotate(model, rotacionY, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	body.Draw(shader);
	// Ala derecha
	glm::mat4 modelWingR = model;
	modelWingR = glm::rotate(modelWingR, glm::radians(angleWings), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelWingR));
	wingR.Draw(shader);

	// Ala Izquierda
	glm::mat4 modelWingL = model;
	modelWingL = glm::rotate(modelWingL, glm::radians(-angleWings), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelWingL));
	wingL.Draw(shader);
}

void AnimarQuetzal(Shader& shader, GLint modelLoc, Model& body, Model& head, Model& tail, glm::vec3 posicion, bool animar, float time)
{
	// tiempos para cada rotación
	//				  [0.0s  0.5s    1.0s  1.5s   2.0s]
	float keyBody[] = { 0.0f, 2.0f, 0.0f, -2.0f, 0.0f };
	float keyTail[] = { 0.0f, 1.5f, 0.0f, -1.5f, 0.0f };

	// Inicalización de angulos
	float angleBody = 0.0f;
	float angleTail = 0.0f;
	//Interpolacion
	if (animar)
	{
		//duracion del ciclo
		float cycle = 4.0f;
		// Tiempo del ciclo entre 0 y cycle
		float actualTime = fmod(time, cycle);

		int indexActual = (int)actualTime;
		int indexNext = (indexActual + 1) % 5;
		//Calculo del avance entre el siguiente frame y el actual
		float factor = actualTime - (float)indexActual;
		// Interpolación con glm::mix
		angleBody = glm::mix(keyBody[indexActual], keyBody[indexNext], factor);
		angleTail = glm::mix(keyTail[indexActual], keyTail[indexNext], factor);
	}
	else
	{
		// En caso de no estar activa la animacion
		angleBody = 0.0f;
		angleTail = 0.0f;
	}

	// Matriz padre: cuerpo
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, posicion);
	model = glm::rotate(model, glm::radians(angleBody), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	body.Draw(shader);
	// Cabeza
	glm::mat4 modelHead = model;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHead));
	head.Draw(shader);
	// Cola
	glm::mat4 modelTail = model;
	modelTail = glm::rotate(modelTail, glm::radians(angleTail), glm::vec3(0.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTail));
	tail.Draw(shader);
}


void DibujarArboles(Shader& shader, GLint modelLoc, Model& tronco, Model& hojas, glm::vec3 posicion, float escala)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posicion));
	model = glm::scale(model, glm::vec3(escala));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	tronco.Draw(shader);
	hojas.Draw(shader);
}
void DibujarArbol(Shader& shader, GLint modelLoc, Model& arbol, Model& hoja, glm::vec3 posicion, float angulo)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posicion));
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	arbol.Draw(shader);
	hoja.Draw(shader);
}

void DibujarFlor(Shader& shader, GLint modelLoc, Model& modeloFlor, glm::vec3 posicion, float escala, float angulo) 
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posicion));
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(escala));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	modeloFlor.Draw(shader);
}

void AnimarPersonaje(Shader& shader, GLint modelLoc, Model& piernas, Model& torso, glm::vec3 posicion, float escala, float tiempo)
{
	// 1. Configuración de la Animación
	float velocidadBusqueda = 2.0f; // Qué tan rápido mueve la cabeza/torso
	float amplitudGiro = 45.0f;     // Cuántos grados gira hacia cada lado

	// Calculamos el ángulo usando SENO para que vaya de derecha a izquierda suavemente
	float anguloGiro = sin(tiempo * velocidadBusqueda) * amplitudGiro;

	// 2. Dibujar PIERNAS (Base estática)
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, posicion);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(escala));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	piernas.Draw(shader);

	// 3. Dibujar TORSO (Animado)
	// Usamos la misma matriz 'model' para que el torso esté en la misma posición que las piernas
	glm::mat4 modelTorso = model;

	// Si el origen del torso en Blender estaba bien puesto en la cintura, esto rotará perfecto.
	// Si el torso se ve muy abajo o arriba, puedes agregar un translate aquí:
	// modelTorso = glm::translate(modelTorso, glm::vec3(0.0f, 0.0f, 0.0f)); 

	modelTorso = glm::rotate(modelTorso, glm::radians(anguloGiro), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar en Y

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTorso));
	torso.Draw(shader);
}

void AnimarFotografo(Shader& shader, GLint modelLoc, Model& base, Model& cabeza, glm::vec3 posicion, float escala, float tiempo)
{
	// 1. Configuración del movimiento
	float velocidad = 2.0f;     // Qué tan rápido sube y baja
	float alturaLevitacion = 0.5f; // Qué tanto se separa de la base (amplitud)

	// Ajuste de altura inicial:
	// Si la cabeza aparece enterrada en el trípode, aumenta este valor.
	float alturaBase = 0.0f;

	// Cálculo del desplazamiento en Y
	// Usamos 'abs(sin)' si queremos que solo suba y vuelva a 0, 
	// o solo 'sin' si queremos que suba y baje pasando por el centro.
	// Probemos con 'sin' simple primero:
	float desplazamientoX = sin(tiempo * velocidad) * alturaLevitacion;

	// 2. Dibujar la BASE (Estática)
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, posicion);
	model = glm::scale(model, glm::vec3(escala));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	base.Draw(shader);

	// 3. Dibujar la CABEZA (Animada)
	glm::mat4 modelCabeza = model; // Copiamos la posición de la base

	// APLICAR ANIMACIÓN: Traslación en el eje Y (Arriba/Abajo)
	// Sumamos 'alturaBase' por si el modelo necesita estar más arriba por defecto
	modelCabeza = glm::translate(modelCabeza, glm::vec3(0.0f, alturaBase + desplazamientoX, 0.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCabeza));
	cabeza.Draw(shader);
}

void AnimarCaballo(Shader& shader, GLint modelLoc, Model& parteTrasera, Model& parteDelantera, glm::vec3 posicion, float escala, float tiempo)
{
	// --- CONFIGURACIÓN ---
	float velocidad = 2.0f;
	float anguloLevantamientoMax = 30.0f;
	float anguloGiroMax = 90.0f;

	// Lógica de movimiento (Seno y Coseno)
	float encabritado = abs(sin(tiempo * velocidad)) * anguloLevantamientoMax;
	float giro = abs(sin(tiempo * velocidad * 0.5f)) * anguloGiroMax;

	// Pivote (Patas traseras)
	glm::vec3 puntoPivote = glm::vec3(0.0f, -1.0f, 1.5f);

	// --- MATRIZ ---
	glm::mat4 model = glm::mat4(1.0f);

	// 1. Posición en el mundo
	model = glm::translate(model, posicion);

	// 2. Giro Global (Y)
	model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. ESCALA (Aplicada antes de la lógica del pivote)
	model = glm::scale(model, glm::vec3(escala)); // <--- NUEVO: Aquí cambiamos el tamaño

	// 4. Lógica del Pivote y Encabritamiento (X)
	// Al escalar antes, la distancia al pivote también se escala automáticamente.
	model = glm::translate(model, puntoPivote);
	model = glm::rotate(model, glm::radians(encabritado), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, -puntoPivote);

	// --- DIBUJAR ---
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	parteTrasera.Draw(shader);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	parteDelantera.Draw(shader);
}