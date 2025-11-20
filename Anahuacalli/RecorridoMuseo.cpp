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

// SFML audio
#include <SFML/Audio.hpp>

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
// Animacion de gato
void AnimarGato(Shader& shader, GLint modelLoc, Model& body, Model& head,
	Model& legFL, Model& legFR, Model& legBL, Model& legBR,
	glm::vec3 posicion, float escala, float tiempo);
// Animacion de helicoptero
void AnimarHelicoptero(Shader& shader, GLint modelLoc, Model& cuerpo, Model& helice, glm::vec3 pos, float escala, float tiempo);
// Animacion de avion volando en forma infinito
void AnimarAvionInfinito(Shader& shader, GLint modelLoc, Model& modeloAvion, glm::vec3 centro, float radio, float escala, float tiempo);
// Dibuja un objeto compuesto por dos partes (ej. tronco y hojas)
void DibujarArboles(Shader& shader, GLint modelLoc, Model& tronco, Model& hojas, glm::vec3 posicion, float escala);
// Dibuja un solo arbol
void DibujarArbol(Shader& shader, GLint modelLoc, Model& arbol, Model& hoja, glm::vec3 posicion, float angulo);
// Dibuja el modelo de flores con su rotación específica
void DibujarFlor(Shader& shader, GLint modelLoc, Model& modeloFlor, glm::vec3 posicion, float escala, float angulo);
// Animacion de camara por el museo
void Animacion_Camara(GLfloat deltaTime);



// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera Inicia en la entrada del museo
Camera  camera(glm::vec3(48.0f, 2.0f, 5.0f));
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

glm::vec3 pos_cuadros[] = {
	glm::vec3(-17.0f, 16.0f, -26.25f),
	glm::vec3(-25.5f, 14.0f, -27.75f),
	glm::vec3(-25.0f, 11.0f, -29.5f),
	glm::vec3(-20.0f, 11.0f, -40.0f)
};

float rotaciones[] = {
	0,
	90.0f,
	180.0f,
	270.0f
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

typedef struct {
	float posX_camera;
	float posY_camera;
	float posZ_camera;
	float rot_horizontal;
	float rot_vertical;
	float time;
} FRAME_CAMERA;

FRAME_CAMERA frames[] = {
	{ 48.0f, 2.0f, 5.0f, 270.0f, 0.0f, 0.0f},	//Entrada Principal
	{ 48.0f, 2.0f, -8.0f, 180.0f, 0.0f, 2.0f},	//Rotacion hacia la compra del boleto
	{ 20.0f, 2.0f, -8.0f, 180.0f, 0.0f, 3.0f},	//Avanza hacia la compra del boleto
	{ 20.0f, 2.0f, -12.0f, 270.0f, 0.0f, 2.0f}, //Avanza por el costado derecho
	{ 25.0f, 2.0f, -12.0f, 360.0f, 0.0f, 3.0f},	//Avanza por la orilla y gira a la derecha
	{ 30.0f, 2.0f, -12.0f, 270.0f, 0.0f, 3.0f},	//Avanza por la orilla y gira a la izquierda
	{ 30.0f, 2.0f, -46.5f, 270.0f, 0.0f, 3.0f}, //Avanzo por enfrente del museo
	{ 33.0f, 2.0f, -48.5f, 290.0f, 0.0f, 1.0f}, //Avanza por el primer edifico
	{ 33.0f, 2.0f, -50.5f, 270.0f, 0.0f, 2.0f}, //Avanza por el costado primer edifico
	{ 33.0f, 2.0f, -70.5f, 270.0f, 0.0f, 3.0f}, //Avanza por el costado del primer edifico
	{ 33.0f, 2.0f, -80.0f, 360.0f, 0.0f, 5.0f}, //Avanza mirando el centro del primer edifico
	{ 33.0f, 2.0f, -85.0f, 270.0f, 0.0f, 1.0f}, //Avanza retornando la mirada al pasillo principal
	{ 33.0f, 2.0f, -98.0f, 180.0f, 0.0f, 2.0f}, //Avanza hasta el final del pasillo y voltea al segundo edificio
	{ 15.0f, 2.0f, -98.0f, 180.0f, 0.0f, 3.0f}, //Avanza para el segundo edificio
	{ 7.0f, 2.0f, -98.0f, 270.0f, 0.0f, 5.0f},  //Avanza mirando al centro del segundo edificio
	{ 1.0f, 2.0f, -98.0f, 180.0f, 0.0f, 1.0f},  //Avanza retornando la vista sobre el pasillo
	{ -7.0f, 2.0f, -98.0f, 180.0f, 0.0f, 1.0f},  //Avanza hasta el final del segundo pasillo
	{ -7.0f, 2.0f, -110.0f, 225.0f, 0.0f, 2.0f},  //Observa en el mirador
	{ -7.0f, 2.0f, -98.0f, 135.0f, 0.0f, 2.0f},  //Observa en el mirador
	{ 1.0f, 2.0f, -98.0f, 0.0f, 0.0f, 2.0f},  //Retorna hacia atras
	{ 33.0f, 2.0f, -98.0f, 90.0f, 0.0f, 3.0f }, // Regresa por el pasillo volteando hacia la entrada
	{33.0f, 2.0f, -50.0f, 180.0f, 0.0f, 3.0f},  // Se acerca a la entrada
	{12.0f, 2.0f, -24.0f, 180.0f, 0.0f, 3.0f},	// Se posiciona en la sala izqueirda
	{-16.0f, 2.0f, -24.0f, 45.0f, 0.0f, 3.0f},  // Se fija por la sala de la izquierda
	{-16.0f, 2.0f, -24.0f, 90.0f, 0.0f, 3.0f},  // Inspecciona lo restante de la sala izquierda
	{ -9.0f, 2.0f, -18.0f, -87.0f, 0.0f, 3.0f },
	{ -9.0f, 2.0f, -18.0f, -87.0f, 0.0f, 1.0f },
	{-15.0f, 2.0f, -35.0, 270.0f, 0.0f, 3.0f},	// Avanza a la entrada de la sala derecha
	{-15.0f, 2.0f, -35.0, 180.0f, 0.0f, 3.0f},	// Inspecciona la sala empezando por la izqueirda
	{-15.0f, 2.0f, -35.0, 320.0f, 0.0f, 3.0f},	// Inspecciona la sala terminando por la derecha
	{-28.0f, 5.0f, -31.0, 180.0f, 15.0f, 3.0f}, // Sube las escaleras
	{-29.0, 8.0f, -17.0f, 45.0f, 15.0f, 3.0f},  // Sube las escaleras de la izquierda
	{-29.0, 8.0f, -17.0f, 0.0f, 0.0f, 3.0f},	// Coloca bien la vista
	{-17.0f, 9.0f, -22.0f, 45.0f, 0.0f, 3.0f},	// Inspecciona la sala donde esta
	{-17.0f, 9.0f, -22.0f, 90.0f, 0.0f, 3.0f},  // termina de inspeccionar la sala
	{ -12.0f, 9.0f, -29.6482f, 54.0f, 0.0f, 3.0f},
	{ -12.0f, 9.0f, -30.0f, 117.25f, 22.5f, 3.0f},
	{ -12.0f, 9.0f, -29.8716f, -90.0f, 20.0f, 3.0f },
	{ -12.0f, 9.0f, -29.8716f, -90.0f, 20.0f, 1.0f },
	{ -18.0f, 9.0f, -23.0f, -143.25f, 6.25f, 3.0f}, //Mural grande
	{ -18.0f, 9.0f, -23.0f, -143.25f, 6.25f, 1.0f},
	{-20.0f, 9.0f, -34.0f, -88.0f, 0.0f, 3.0f}, // Insecciona la segunda sala
	{-8.6f, 9.0f, -42.0f, 92.0f, 0.0f, 3.0f},
	{-8.6f, 9.0f, -42.0f, 92.0f, 0.0f, 1.0f}, // Instrumentos
	{-35.0f, 9.0f, -40.0f, 177.0f, 0.0f, 3.0f },// vasijas
	{-35.0f, 9.0f, -40.0f, 177.0f, 0.0f, 1.0f },
	{-35.0f, 9.0f, -40.0f, 177.0f, 78.0f, 3.0f },//mural en el techo
	{-35.0f, 9.0f, -40.0f, 177.0f, 78.0f, 1.0f },
	{-42.0f, 9.0f, -42.0f, 89.0f, -0.25f, 3.0f }, // escultura
	{-42.f, 9.0f, -29.0f, 88.0f, 0.0f, 3.0f },
	{-42.0f, 9.0f, -28.0f, 23.75f, -7.25f, 3.0f },
	{-42.0f, 9.0f, -28.0f, 23.75f, -7.25f, 1.0f },
	{-41.0f, 10.0f, -28.0f, 0.0f, 5.0f, 3.0f},   // Se posicina en las escaleras para el tercer piso
	{-33.0f, 12.0f, -28.0f, 0.0f, 5.0f, 3.0f},   // Avanza en las escaleras
	{-33.0f, 12.0f, -28.0f, -90.0f, 5.0f, 3.0f},  // Gira a la izquierda
	{-32.0f, 13.0f, -31.0f, -90.0f, 5.0f, 3.0f}, // Avanza ligeramente
	{-41.0f, 20.0f, -31.0f, -180.f, 30.0f, 3.0f}, // sube al tercer piso
	{-41.0f, 20.0f, -31.0f, -90.0f, 0.0f, 3.0f},  // Avanza para inspeccionar la primera sala del tercer piso
	{-37.0f, 20.0f, -38.0f, -49.75f, 0.0f, 3.0f },
	{ -37.0f, 20.0f, -38.9257f, -21.75f, 0.0f, 3.0f },
	{ -37.0f, 20.0f, -38.9257f, -127.25f, 0.0f, 3.0f },
	{ -40.0f, 20.0f, -36.0f, 103.0f, 0.0f, 3.0f },
	{ -41.0f, 20.0f, -29.0f, 85.0f, 0.0f, 3.0f },
	{ -37.0f, 20.0f, -21.0f, 61.0f, 0.0f, 3.0f },
	{ -37.0f, 20.0f, -21.0f, 13.0f, 0.0f, 3.0f },// ispecciona la segunda sala
	{ -37.0f, 20.0f, -21.0f, 133.0f, 0.0f, 3.0f },
	{-34.0f, 23.0f, -28.0f, -90.0f, 10.0f, 3.0f},  // Escaleras al tercer piso
	{-32.0f, 23.0f, -31.0f, -90.f, 0.0f, 3.0f},	 // avanza ligeramente	
	{-40.0f, 31.0f, -32.0f, -180.f, 30.f, 3.0f}, //LLega al ultimo puso
	{-42.0f, 31.0f, -17.0f, 0.0f, 0.0f, 3.0f}, // Se posiciona en una esquina
	{-0.3f, 31.0f, -17.0f, 0.0f, 0.0f, 3.0f},
	{-0.3f, 31.0f, -17.0f, 0.0f, -45.0f, 3.0f},
	{-0.3f, 31.0f, -17.0f, 0.0f, 0.0f, 3.0f},
	{-0.3f, 31.0f, -17.0f, -180.0f, 0.0f, 3.0f},
	{-42.0f, 31.0f, -17.0f, -90.0f, 0.0f, 3.0f},
	{-42.0f, 32.0f, -42.0f, -90.0f, 0.0f, 3.0f},
	{-42.0f, 32.0f, -42.0f, 0.0f, 0.0f, 3.0f},
	{-1.5f, 32.0f, -41.0f, 0.0f, 0.0f, 3.0f},
	{-1.5f, 32.0f, -41.0f, -45.0f, -45.0f, 3.0f},
	{-1.5f, 32.0f, -41.0f, 0.0f, -45.0f, 3.0f},
	{-1.5f, 32.0f, -41.0f, 45.0f, -45.0f, 3.0f}


};

glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
// Variables para la animación de la cámara
float posX_camera = 50.0f;
float posY_camera = 3.0f;
float posZ_camera = 5.0f;
float rot_horizontal = 0.0f;
float rot_vertical = 0.0f;
bool recorrido_automatico = false;

int indice_frame = 0;
int total_keyframes = sizeof(frames) / sizeof(frames[0]);

void Reset_Camera_Animation(void) {
	posX_camera = frames[0].posX_camera;
	posY_camera = frames[0].posY_camera;
	posZ_camera = frames[0].posZ_camera;
	rot_horizontal = frames[0].rot_horizontal;
	rot_vertical = frames[0].rot_vertical;
	indice_frame = 0;
}

// Nuevas variables (deberían ser globales o pasadas como parámetros)
float tiempo_transcurrido = 0.0f;

void interpolation(GLfloat deltaTime)
{
	// Verificar si el recorrido ha terminado ANTES de acceder a frames
	if (indice_frame >= total_keyframes - 1) {
		// Asegura que la cámara está en la posición del último keyframe (que ya está hecha por LERP)
		recorrido_automatico = false;
		// Reiniciar el contador de tiempo y el índice para el próximo inicio
		tiempo_transcurrido = 0.0f;
		indice_frame = 0;
		// Reset_Camera_Animation(); // Resetea la cámara al primer keyframe
		recorrido_automatico = false; // Detiene el recorrido automático
		return; // Termina la función
	}

	// 1. Obtener los keyframes de inicio y fin
	FRAME_CAMERA start = frames[indice_frame];
	FRAME_CAMERA end = frames[indice_frame + 1];

	// 2. Actualizar el tiempo transcurrido para este segmento
	tiempo_transcurrido += deltaTime;

	// 3. Calcular el factor de progreso (t) [0.0 a 1.0]
	float t = tiempo_transcurrido / end.time;

	// 4. Asegurarse de que t no exceda 1.0
	if (t > 1.0f) {
		t = 1.0f; // Evita el overshoot
	}

	// 5. Aplicar la Interpolación Lineal (LERP)
	// LERP(A, B, t) = A + (B - A) * t

	// Interpolación de Posición
	posX_camera = start.posX_camera + (end.posX_camera - start.posX_camera) * t;
	posY_camera = start.posY_camera + (end.posY_camera - start.posY_camera) * t;
	posZ_camera = start.posZ_camera + (end.posZ_camera - start.posZ_camera) * t;

	camera.SetPosition(glm::vec3(posX_camera, posY_camera, posZ_camera));

	// Interpolación de Orientación
	rot_horizontal = start.rot_horizontal + (end.rot_horizontal - start.rot_horizontal) * t;
	rot_vertical = start.rot_vertical + (end.rot_vertical - start.rot_vertical) * t;

	camera.SetRotation(rot_horizontal, rot_vertical);

	// 6. Verificar si el movimiento ha terminado
	if (t >= 1.0f) {
		// Mueve al siguiente keyframe
		indice_frame++;
		tiempo_transcurrido = 0.0f; // Resetear el contador de tiempo
	}
}




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
	Model diane((char*)"Models/DIANE1.obj");
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
	// Caballo
	Model horseB((char*)"Models/horseB.obj");
	Model horseF((char*)"Models/horseF.obj");
	//Gato
	Model catB((char*)"Models/catBody.obj");
	Model catH((char*)"Models/catHead.obj");
	Model catFR((char*)"Models/catFR.obj");
	Model catFL((char*)"Models/catFL.obj");
	Model catBR((char*)"Models/catBR.obj");
	Model catBL((char*)"Models/catBL.obj");
	//Helicoptero
	Model heliCabin((char*)"Models/heliCabin.obj");
	Model heliProp((char*)"Models/heliProp.obj");
	//Avion
	Model plane((char*)"Models/floatplane.obj");

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

	// Biblioteca de audio 
	sf::Music music;
	if (!music.openFromFile("ringtones-pink-panther.mp3")) {
		std::cout << "No se pudo cargar la musica" << std::endl;
		return -1; // Salir si no se puede cargar la música
	}
	music.play();
	music.setVolume(20.0f);

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

		Animacion_Camara(deltaTime);

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

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

		// Luz
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

		// Variables necesarias para las animaciones
		glm::vec3 centroPajaro = glm::vec3(20.0f, 2.0f, -10.0f);
		float radio = 2.0f;
		glm::vec3 posicionQ = glm::vec3(17.0f, -3.0f, -62.0f);
		glm::vec3 girlPos = glm::vec3(-4.0f, 7.5f, -30.0f);
		glm::vec3 posCaballo = glm::vec3(19.0f, 0.0f, -80.0f);
		glm::vec3 posGato = glm::vec3(15.0f, 0.0f, -15.0f);
		glm::vec3 posHeli = glm::vec3(53.0f, 10.0f, -27.0f);
		glm::vec3 centroVuelo = glm::vec3(20.0f, 15.0f, -82.0f);
		float amplitudVuelo = 30.0f;
		// Llamadas a las funciones de animación
		AnimarPajaro(lightingShader, modelLoc, birdB, birdWR, birdWL, currentFrame, centroPajaro, radio);
		AnimarQuetzal(lightingShader, modelLoc, quetzalB, quetzalH, quetzalT, posicionQ, true,currentFrame);
		AnimarPersonaje(lightingShader, modelLoc, girlL, girlB, girlPos, 0.6f, currentFrame);
		AnimarCaballo(lightingShader, modelLoc, horseB, horseF, posCaballo, 2.0f, currentFrame);
		AnimarGato(lightingShader, modelLoc, catB, catH, catFL, catFR, catBL, catBR, posGato, 4.0f, currentFrame);
		AnimarHelicoptero(lightingShader, modelLoc, heliCabin, heliProp, posHeli, 1.0f, currentFrame);
		AnimarAvionInfinito(lightingShader, modelLoc, plane, centroVuelo, amplitudVuelo, 0.08f, currentFrame);

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
		
		//Se dibuja el Museo completo
		model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		MuseoAnahuacalli.Draw(lightingShader);
		Dibujar_Vidrios(lightingShader, vidrios, modelLoc);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		// Se dibuja la estatua en el primer piso del museo
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-42.0f, 7.4f, -18.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		diane.Draw(lightingShader);
		
		
		glBindVertexArray(0);
		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
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
	// Se posiciona la camara en la entrada del museo al presionar la tecla 1
	if (keys[GLFW_KEY_1]) {
		camera.TeleportAndReset(glm::vec3(-13.9923f, 30.1483f, -46.064f));
		camera.SetRotation(-54.75f, -19.75f);
	}
	if (keys[GLFW_KEY_2]) {
		if (recorrido_automatico == false) {
			recorrido_automatico = true;
			printf("Recorrido automatico activado\n");
		}
	}
	if (keys[GLFW_KEY_3]) {
		if (recorrido_automatico == true) {
			recorrido_automatico = false;
			printf("Recorrido automatico desactivado\n");
		}
	}
	if (keys[GLFW_KEY_4])
	{
		camera.TeleportAndReset(glm::vec3(-16.7981f, 15.0926f, -27.1761f));
		camera.SetRotation(91.0f, -0.75f);
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
		glm::vec3 pos = camera.GetPosition();

		// Obtener el vector frontal (hacia donde mira)
		// Asumiendo que tienes un método GetFront() o acceso a camera.Front
		glm::vec3 front = camera.GetFront();

		// Ingeniería inversa de los ángulos (Matemáticas de Euler)
		float pitch = glm::degrees(asin(front.y));
		float yaw = glm::degrees(atan2(front.z, front.x));

		std::cout << "{ " << pos.x << "f, " << pos.y << "f, " << pos.z << "f, "
			<< yaw << "f, " << pitch << "f, 3.0f }," << std::endl;
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

void AnimarGato(Shader& shader, GLint modelLoc, Model& body, Model& head,
	Model& legFL, Model& legFR, Model& legBL, Model& legBR,
	glm::vec3 posicionOriginal, float escala, float tiempo)
{
	// --- VARIABLES DE CONFIGURACIÓN ---
	float velocidadCaminata = 4.0f;
	float distanciaRecorrido = 10.0f;

	float ajusteRotacion = 270.0f;

	// --- 1. LÓGICA DE TIEMPOS (CICLO EXTENDIDO) ---
	// Aumentamos el ciclo a 14 segundos para dar tiempo a "Buscar"
	float tiempoCiclo = fmod(tiempo, 14.0f);

	glm::vec3 posActual = posicionOriginal;
	float rotacionY = 0.0f;      // Rotación del cuerpo
	float rotacionCabeza = 0.0f; // Rotación INDEPENDIENTE de la cabeza (buscar)
	bool moviendoPatas = false;

	// --- MÁQUINA DE ESTADOS ---

	// FASE 1: CAMINAR (IDA) [0s - 4s]
	if (tiempoCiclo < 4.0f)
	{
		float factor = tiempoCiclo / 4.0f;
		posActual.x += factor * distanciaRecorrido; // Avanza en X
		rotacionY = 0.0f;
		moviendoPatas = true;
		// Cabeza firme con ligero bamboleo natural
		rotacionCabeza = sin(tiempo * 2.0f) * 5.0f;
	}
	// FASE 2: BUSCAR [4s - 6s] -> DETIENE Y MIRA
	else if (tiempoCiclo < 6.0f)
	{
		posActual.x += distanciaRecorrido; // Se mantiene al final
		rotacionY = 0.0f; // Sigue mirando al frente
		moviendoPatas = false; // Patas quietas

		// ANIMACIÓN DE BÚSQUEDA: La cabeza gira de izquierda a derecha
		float tiempoLocal = tiempoCiclo - 4.0f;
		rotacionCabeza = sin(tiempoLocal * 3.0f) * 45.0f; // Gira 45 grados a cada lado
	}
	// FASE 3: GIRO DE CUERPO [6s - 7s]
	else if (tiempoCiclo < 7.0f)
	{
		posActual.x += distanciaRecorrido;
		float factor = (tiempoCiclo - 6.0f); // 0 a 1
		rotacionY = factor * 180.0f; // Gira 180 grados
		rotacionCabeza = 0.0f; // Centra la cabeza mientras gira el cuerpo
		moviendoPatas = true; // Marcha en su lugar para girar
	}
	// FASE 4: CAMINAR (VUELTA) [7s - 11s]
	else if (tiempoCiclo < 11.0f)
	{
		float factor = (tiempoCiclo - 7.0f) / 4.0f;
		float regreso = distanciaRecorrido * (1.0f - factor);
		posActual.x += regreso;
		rotacionY = 180.0f; // Mira hacia atrás
		moviendoPatas = true;
		rotacionCabeza = sin(tiempo * 2.0f) * 5.0f;
	}
	// FASE 5: BUSCAR (ORIGEN) [11s - 13s] -> MIRA DE NUEVO AL LLEGAR
	else if (tiempoCiclo < 13.0f)
	{
		posActual.x += 0.0f; // En el origen
		rotacionY = 180.0f;
		moviendoPatas = false;

		float tiempoLocal = tiempoCiclo - 11.0f;
		rotacionCabeza = sin(tiempoLocal * 3.0f) * 45.0f; // Busca de nuevo
	}
	// FASE 6: GIRO FINAL [13s - 14s]
	else
	{
		posActual.x += 0.0f;
		float factor = (tiempoCiclo - 13.0f);
		rotacionY = 180.0f + (factor * 180.0f); // Completa la vuelta a 360
		moviendoPatas = true;
		rotacionCabeza = 0.0f;
	}

	// --- 2. CÁLCULO DE ARTICULACIONES ---
	float rotPata1 = 0.0f;
	float rotPata2 = 0.0f;
	float bobbing = 0.0f;

	if (moviendoPatas) {
		rotPata1 = sin(tiempo * velocidadCaminata) * 5.0f; // Aumenté a 30 para que se note el paso
		rotPata2 = sin(tiempo * velocidadCaminata + 3.14159f) * 5.0f;
		bobbing = abs(sin(tiempo * velocidadCaminata)) * 0.05f;
	}
	else {
		// Respiración cuando está parado buscando
		bobbing = sin(tiempo * 2.0f) * 0.02f;
	}

	// --- 3. DIBUJADO ---

	// Matriz Maestra (Cuerpo)
	glm::mat4 modelBody = glm::mat4(1.0f);
	modelBody = glm::translate(modelBody, posActual);
	// Rotación combinada: La lógica de ida/vuelta + el ajuste para que no camine de lado
	modelBody = glm::rotate(modelBody, glm::radians(rotacionY + ajusteRotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	modelBody = glm::translate(modelBody, glm::vec3(0.0f, bobbing, 0.0f));
	modelBody = glm::scale(modelBody, glm::vec3(escala));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBody));
	body.Draw(shader);

	// --- JOINTS (Pivotes) ---
	// Asegúrate que estos valores coincidan con tu modelo
	float offX = 0.15f;
	float offY = 0.4f;
	float offZ_F = 0.4f;
	float offZ_B = -0.4f;

	glm::vec3 pFL = glm::vec3(offX, offY, offZ_F);
	glm::vec3 pFR = glm::vec3(-offX, offY, offZ_F);
	glm::vec3 pBL = glm::vec3(offX, offY, offZ_B);
	glm::vec3 pBR = glm::vec3(-offX, offY, offZ_B);

	// Pivote del cuello (IMPORTANTE: Ajustar altura y profundidad para que la cabeza no flote)
	glm::vec3 pHead = glm::vec3(0.0f, 0.6f, 0.6f);

	// --- DIBUJAR CABEZA (CORREGIDO) ---
	glm::mat4 mHead = modelBody;
	// 2. Aplicar la rotación de "Búsqueda" (Izquierda/Derecha)
	mHead = glm::rotate(mHead, glm::radians(rotacionCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
	// 3. Regresar del pivote (opcional, depende del origen del modelo de la cabeza)
	// Si la cabeza rota sobre su base, no necesitas el translate negativo. 
	

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mHead));
	head.Draw(shader);

	// --- DIBUJAR PATAS ---
	// FL
	glm::mat4 mFL = modelBody;
	mFL = glm::translate(mFL, pFL);
	mFL = glm::rotate(mFL, glm::radians(rotPata1), glm::vec3(1.0f, 0.0f, 0.0f));
	mFL = glm::translate(mFL, -pFL);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mFL));
	legFL.Draw(shader);

	// FR
	glm::mat4 mFR = modelBody;
	mFR = glm::translate(mFR, pFR);
	mFR = glm::rotate(mFR, glm::radians(rotPata2), glm::vec3(1.0f, 0.0f, 0.0f));
	mFR = glm::translate(mFR, -pFR);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mFR));
	legFR.Draw(shader);

	// BL
	glm::mat4 mBL = modelBody;
	mBL = glm::translate(mBL, pBL);
	mBL = glm::rotate(mBL, glm::radians(rotPata2), glm::vec3(1.0f, 0.0f, 0.0f));
	mBL = glm::translate(mBL, -pBL);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBL));
	legBL.Draw(shader);

	// BR
	glm::mat4 mBR = modelBody;
	mBR = glm::translate(mBR, pBR);
	mBR = glm::rotate(mBR, glm::radians(rotPata1), glm::vec3(1.0f, 0.0f, 0.0f));
	mBR = glm::translate(mBR, -pBR);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBR));
	legBR.Draw(shader);
}

void AnimarHelicoptero(Shader& shader, GLint modelLoc, Model& cuerpo, Model& helice, glm::vec3 pos, float escala, float tiempo)
{
	// Variables
	float velocidadHélice = 20.0f; // Muy rápido
	float velocidadLevitacion = 2.0f;

	// Levitación suave (Sube y baja)
	float posY = pos.y + sin(tiempo * velocidadLevitacion) * 0.5f;

	// 1. DIBUJAR CUERPO
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos.x, posY, pos.z));

	// Inclinación ligera hacia adelante (como si avanzara un poco)
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(escala));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	cuerpo.Draw(shader);

	// 2. DIBUJAR HÉLICE
	// Asumiendo que el origen de la hélice está en su centro
	glm::mat4 modelH = model; // Copiamos la matriz del cuerpo (ya tiene la posición y escala)
	modelH = glm::rotate(modelH, tiempo * velocidadHélice, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelH));
	helice.Draw(shader);
}

void AnimarAvionInfinito(Shader& shader, GLint modelLoc, Model& modeloAvion, glm::vec3 centro, float radio, float escala, float tiempo)
{
	// --- CONFIGURACIÓN ---
	float velocidad = 1.0f; // Velocidad de recorrido

	// 1. CÁLCULO DE LA POSICIÓN (TRAYECTORIA EN 8)
	// Ecuación paramétrica de la Lemniscata (Figura 8)
	// X se mueve con cos(t), Z se mueve con sin(2*t) para hacer el cruce
	float t = tiempo * velocidad;

	float x = centro.x + (radio * cos(t));
	float z = centro.z + (radio * sin(2.0f * t) / 2.0f); // Dividimos entre 2 para que el 8 sea proporcional

	// Agregamos una leve variación en Y para que no vuele plano
	float y = centro.y + (sin(t * 2.0f) * 1.0f);

	// 2. CÁLCULO DE LA ORIENTACIÓN (YAW - Rumbo)
	// Para que el avión mire al frente, necesitamos la derivada (velocidad) de la posición
	// Derivada de cos(t) es -sin(t)
	// Derivada de sin(2t) es 2*cos(2t)
	float dx = -sin(t);
	float dz = cos(2.0f * t);

	// atan2 nos da el ángulo en radianes desde el vector (0,0) al vector (dx,dz)
	float anguloY = atan2(dx, dz);
	// Convertimos a grados
	float gradosY = glm::degrees(anguloY);

	// 3. CÁLCULO DEL ALABEO (ROLL - Inclinación)
	// El avión debe inclinarse hacia adentro de la curva.
	// Usamos la curvatura o simplemente sincronizamos con el giro.
	// En un 8, el giro cambia de izquierda a derecha, así que cos(t) funciona bien para simular esto.
	float anguloRoll = cos(t) * 45.0f; // 45 grados de inclinación máxima

	// --- CONSTRUCCIÓN DE LA MATRIZ ---
	glm::mat4 model = glm::mat4(1.0f);

	// A. Posición
	model = glm::translate(model, glm::vec3(x, y, z));

	// B. Orientación (Rumbo/Yaw)
	// Sumamos 90, 180 o 270 si el modelo viene rotado por defecto.
	// Prueba sumando +180.0f o +90.0f si vuela de lado o de reversa.
	model = glm::rotate(model, glm::radians(gradosY), glm::vec3(0.0f, 1.0f, 0.0f));

	// C. Inclinación (Roll)
	// Importante: Rotamos en el eje Z (o X) local para el alabeo
	model = glm::rotate(model, glm::radians(anguloRoll), glm::vec3(0.0f, 0.0f, 1.0f)); // Eje X si el avión apunta en Z, o Z si apunta en X
	// NOTA: Si el avión gira raro (como un taladro), cambia este eje a (0,0,1)

	model = glm::scale(model, glm::vec3(escala));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	modeloAvion.Draw(shader);
}

void Animacion_Camara(GLfloat deltaTime) {

	if (recorrido_automatico == true) {
		//Implementar aqui la animacion de la camara
		interpolation(deltaTime);
	}
}
