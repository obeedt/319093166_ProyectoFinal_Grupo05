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
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation(float deltaTime);
void Animation_garage(float deltaTime);
void AnimationCarrito(float deltaTime);
void updateNoiseTexture(); // Función que definiremos después


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 135.0f));
//Camera  camera(glm::vec3(0.0f, 0.0f, 0.0f));

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
bool isNight = false; // Para determinar si es de noche o d�a
float transitionSpeed = 1.0f; // Velocidad de transici�n de d�a a noche
bool keyPressed = false; // Para almacenar el estado previo de la tecla N
bool lightsOff = false;
bool keyPressed2 = false; // Para almacenar el estado previo de la tecla L

glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
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

glm::vec3 Light1 = glm::vec3(1.0f, 1.0f, 0.0f);

//Animacion puerta
bool animPuerta = false;
bool abriendo = true;
float rotPuerta= 0.0f;
float pivotOffset = 0.5f; // Ajusta según tu modelo
float doorSpeed = 90.0f; // Grados por segundo

//Animacion garage
bool animGarage = false;
float rotPuerta_Garage = 0.0f;
bool abriendoGarage = true;
float pivotOffsetGarage = 0.5f; // Ajusta según tu modelo

//Animacion carrito
bool animCarrito = false;       // Activa/desactiva la animación
float posCarrito = 0.0f;        // Posición actual (0 a distanciaTotal)
const float distanciaTotal = 45.0f; // Distancia máxima de recorrido
const float velocidadCarrito = 11.0f; // Unidades por segundo
bool enPosicionFinal = false;   // Estado del carrito
bool girando = false;               // Controla si está en modo "giro"
float anguloGiro = 0.0f;           // Ángulo acumulado de rotación
const float velocidadGiro = 200.0f; // Grados/segundo de rotación (ajusta según necesidad)
const int vueltasRequeridas = 2;    // Número de vueltas completas antes de regresar

//animacion tele
// Global variables
GLuint tvScreenTexture;      // Texture for static effect
GLuint tvOffTexture;         // Texture for black screen (off state)
GLuint tvNoiseTexture;       // Procedural noise texture
bool tvOn = false;
float tvNoiseIntensity = 0.5f;
float animationTime = 0.0f;  // Renamed to avoid ambiguity with std::time

// Luces puntuales
glm::vec3 pointLightPositions[] = {
	glm::vec3(-14.0f, 12.5f, -11.0f),
	glm::vec3(-17.0f, 17.0f, 80.0f),
};

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	glfwInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final - Computacion Grafica", nullptr, nullptr);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	// Cargar shader especial para TV
	Shader tvShader("shaders/tv.vs", "shaders/tv.frag");
	// Load textures
	tvScreenTexture = TextureLoading::LoadTexture("Models/ruido.jpg"); // Static noise image
	tvOffTexture = TextureLoading::LoadTexture("Models/negro.jpg");     // Black screen image

	// Create procedural noise texture
	glGenTextures(1, &tvNoiseTexture);
	updateNoiseTexture(); // Initialize noise texture


	//Modelos exterior
	Model Casa((char*)"Models/casa.obj");
	Model Arbusto((char*)"Models/arbusto.fbx");
	Model Arbol((char*)"Models/arbol.obj");
	Model Farol((char*)"Models/faro.obj");
	Model Cesped((char*)"Models/cesped.obj");
	Model Cielo((char*)"Models/cieloo.obj");
	Model Garage((char*)"Models/garage.obj");

	// Modelos interior cocina
	Model Puerta_Cocina((char*)"Models/puerta_cocina.obj");
	Model Ventana_Cocina((char*)"Models/ventana_cocina.obj");
	Model Pared_Cocina((char*)"Models/pared_cocina.obj");
	Model Pared_Cocina_Anterior((char*)"Models/pared_cocina_anterior.obj");
	Model Pared_Madera((char*)"Models/pared_madera.obj");
	Model Piso_Cocina((char*)"Models/piso_cocina.obj");
	Model Alacena((char*)"Models/alacena.obj");
	Model Alacena_Superior((char*)"Models/alacena_superior.obj");
	Model Alacena_Grande((char*)"Models/alacena_grande.obj");
	Model Campana((char*)"Models/campana.obj");
	Model Estufa((char*)"Models/estufa.obj");
	Model Mesa((char*)"Models/mesa.obj");
	Model Refrigerador((char*)"Models/refrigerador.obj");
	Model Silla((char*)"Models/silla.obj");
	Model Tostadora((char*)"Models/tostadora.obj");

	Model Puerta((char*)"Models/puertaAnim.obj");
	Model Puerta_Garage((char*)"Models/puerta_garage.obj");
	Model Carrito((char*)"Models/carrito.obj");

	// Modelo de la sala
	Model Sala((char*)"Models/sala.obj");
	Model Tele((char*)"Models/tele.obj");
	Model PantallaTele((char*)"Models/pantalla.obj"); // Modelo plano para la pantalla

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
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 350.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		animationTime += deltaTime;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation(deltaTime);
		Animation_garage(deltaTime);
		AnimationCarrito(deltaTime);
		updateNoiseTexture();
		
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Interpolaci�n entre d�a y noche
		float timeOfDay = isNight ? 1.0f : 0.0f; // 1.0f = noche, 0.0f = d�a
		float lerpFactor = timeOfDay * transitionSpeed; // Factores de interpolaci�n

		// Lerp para la direcci�n de la luz
		glm::vec3 dayLightDir(-0.2f, -1.0f, -0.3f); // Direcci�n de luz de d�a
		glm::vec3 nightLightDir(-0.2f, -1.0f, -0.3f); // Direcci�n de luz de noche (podr�as cambiarla si quieres)

		// Lerp para la luz ambiental
		glm::vec3 dayAmbient(0.5f, 0.5f, 0.5f); // Luz ambiental de d�a
		glm::vec3 nightAmbient(0.1f, 0.1f, 0.3f); // Luz ambiental de noche (m�s azul)

		// Lerp para la luz difusa
		glm::vec3 dayDiffuse(0.8f, 0.8f, 0.8f); // Luz difusa de d�a
		glm::vec3 nightDiffuse(0.2f, 0.2f, 0.5f); // Luz difusa de noche

		// Lerp para la luz especular
		glm::vec3 daySpecular(0.5f, 0.5f, 0.5f); // Luz especular de d�a
		glm::vec3 nightSpecular(0.2f, 0.2f, 0.5f); // Luz especular de noche

		// Lerp para la brillos del material
		float dayShininess = 32.0f;
		float nightShininess = 16.0f;

		// Luz direccional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
			glm::mix(dayLightDir, nightLightDir, lerpFactor).x,
			glm::mix(dayLightDir, nightLightDir, lerpFactor).y,
			glm::mix(dayLightDir, nightLightDir, lerpFactor).z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
			glm::mix(dayAmbient, nightAmbient, lerpFactor).x,
			glm::mix(dayAmbient, nightAmbient, lerpFactor).y,
			glm::mix(dayAmbient, nightAmbient, lerpFactor).z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
			glm::mix(dayDiffuse, nightDiffuse, lerpFactor).x,
			glm::mix(dayDiffuse, nightDiffuse, lerpFactor).y,
			glm::mix(dayDiffuse, nightDiffuse, lerpFactor).z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),
			glm::mix(daySpecular, nightSpecular, lerpFactor).x,
			glm::mix(daySpecular, nightSpecular, lerpFactor).y,
			glm::mix(daySpecular, nightSpecular, lerpFactor).z);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"),
			glm::mix(dayShininess, nightShininess, lerpFactor));


		// Configuraci�n de la luz puntual 1
		float lightsOffFactor = lightsOff ? 0.0f : 1.0f; // Factor de luz apagada
		float lerpFactor2 = lightsOff * 1.0f; // Factor de interpolaci�n

		// Luz puntual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
			pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
			0.2f * lightsOffFactor, 0.2f * lightsOffFactor, 0.2f * lightsOffFactor);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
			1.0f * lightsOffFactor, 1.0f * lightsOffFactor, 1.0f * lightsOffFactor);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
			1.0f * lightsOffFactor, 1.0f * lightsOffFactor, 1.0f * lightsOffFactor);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Configuraci�n de la luz puntual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"),
			pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);

		// Color amarillento (m�s intenso en el componente rojo y verde, menos azul)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"),
			0.5f * lightsOffFactor, 0.5f * lightsOffFactor, 0.1f * lightsOffFactor);  // Amarillo suave ambiental

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"),
			3.0f * lightsOffFactor, 2.5f * lightsOffFactor, 1.0f * lightsOffFactor);  // Amarillo brillante difuso

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"),
			1.5f * lightsOffFactor, 1.2f * lightsOffFactor, 0.3f * lightsOffFactor);  // Amarillo especular

		// Par�metros de atenuaci�n para mayor alcance (valores m�s bajos = mayor alcance)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.05f);    // Reducido para mayor alcance
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.01f); // Reducido para mayor alcance

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

		glm::mat4 model(1);

		//Carga de modelos
		view = camera.GetViewMatrix();

		/////////////////////////-Ambientacion-////////////////////////////////
		//Arbusto central
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(58.0f, -10.0f, 20.0f));
		model = glm::scale(model, glm::vec3(18.0f, 20.0f, 18.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto.Draw(lightingShader);
		//Arbusto izquierdo
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(45.0f, -10.0f, 22.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto.Draw(lightingShader);
		//Arbusto derecho
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(75.0f, -10.0f, 21.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto.Draw(lightingShader);
		//Arbusto derecho 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(85.0f, -10.0f, 21.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto.Draw(lightingShader);

		//Arbol derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(90.0f, -10.0f, -30.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(lightingShader);
		//Arbol izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-90.0f, -10.0f, 30.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(lightingShader);
		//Arbol 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-80.0f, -10.0f, -30.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(lightingShader);
		//Arbol 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-150.0f, -10.0f, -70.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(lightingShader);
		//Arbol 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-230.0f, -10.0f, -70.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Arbol.Draw(lightingShader);

		//Faro
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-17.0f, -10.0f, 80.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Farol.Draw(lightingShader);

		//Cesped
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-17.0f, -13.0f, 80.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cesped.Draw(lightingShader);

		//Cielo
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-17.0f, -14.0f, 80.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cielo.Draw(lightingShader);

		/////////////////////////-Fachada-////////////////////////////////
		//Casa
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Casa.Draw(lightingShader);
		//Piso general (provisional)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.0f, 5.55f, 3.0f));
		model = glm::scale(model, glm::vec3(2.7f, 1.0f, 1.9f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso_Cocina.Draw(lightingShader);
		//Techo
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.75f, 13.7f, 2.65f));
		model = glm::scale(model, glm::vec3(2.55f, 1.0f, 1.75));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso_Cocina.Draw(lightingShader);

		//Garage
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.4f, -4.35f, 23.0f)); // 3. Mover de vuelta
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Garage.Draw(lightingShader);

		///////////////////////////-Sala-////////////////////////////////
		//Cuarto completo de la sala
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.91f, 4.1f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0, 1.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sala.Draw(lightingShader);

		////////////////////////-Cocina-////////////////////////////////
		////////////////////////-Estructura-////////////////////////////////
		//Piso
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.2f, 4.1f, -12.6f));
		model = glm::scale(model, glm::vec3(1.28, 1.0f, 0.84f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso_Cocina.Draw(lightingShader);
		//Techo
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.2f, 13.0f, -12.7f));
		model = glm::scale(model, glm::vec3(1.28, 1.0f, 0.84f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso_Cocina.Draw(lightingShader);
		//Pared izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-15.57f, 3.3f, -9.6f));
		model = glm::scale(model, glm::vec3(1.0f, 2.06f, 0.85f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pared_Cocina.Draw(lightingShader);
		//Pared derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 3.3f, -9.6f));
		model = glm::scale(model, glm::vec3(1.0f, 2.06f, 0.85f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pared_Cocina.Draw(lightingShader);
		//Pared posterior
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.2f, 3.3f, -32.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 2.06f, 1.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pared_Cocina.Draw(lightingShader);
		//Pared anterior
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.2f, 3.3f, -14.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 2.06f, 1.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pared_Cocina_Anterior.Draw(lightingShader);
		////Ventana
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-21.2f, 10.2f, -20.37f));
		model = glm::scale(model, glm::vec3(3.35f, 3.35f, 1.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ventana_Cocina.Draw(lightingShader);
		//Puerta interior
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.8f, 7.2f, -20.15f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_Cocina.Draw(lightingShader);
		//Puerta exterior
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.8f, 7.2f, -21.35f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_Cocina.Draw(lightingShader);
		//Pared Madera 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.9f, 6.2f, -20.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pared_Madera.Draw(lightingShader);
		//Pared Madera 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.6f, 6.2f, -20.3f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pared_Madera.Draw(lightingShader);

		//////////////////////////-Objetos-////////////////////////////////
		////Alacena 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-19.0f, 4.74f, -17.57f));
		model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena.Draw(lightingShader);
		////Alacena 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-21.1f, 4.74f, -14.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena.Draw(lightingShader);
		////Alacena 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-19.0f, 4.74f, -10.5f));
		model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena.Draw(lightingShader);
		////Alacena 4
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-17.1f, 4.74f, -14.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena.Draw(lightingShader);
		////Alacena 5
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.1f, 4.74f, -14.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena.Draw(lightingShader);
		//Alacena 6
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-15.0f, 11.5f, -19.1f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena_Superior.Draw(lightingShader);
		//Alacena 7
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.5f, 9.8f, -17.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alacena_Grande.Draw(lightingShader);

		////Estufa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-21.15f, 5.91f, -23.85f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Estufa.Draw(lightingShader);
		////Campana
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-19.2f, 6.1f, -23.4f));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 200.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Campana.Draw(lightingShader);
		//Refrigerador
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-16.8f, 4.7f, -19.1f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Refrigerador.Draw(lightingShader);
		//Mesa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.3, 6.82f, -11.75));
		model = glm::scale(model, glm::vec3(0.85f, 0.9f, 0.85f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(lightingShader);
		//Silla 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-15.3, 6.4f, -5.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);
		//Silla 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-24.3, 6.4f, -9.6f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);
		//Tostadora
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.5f, 4.74f, -16.5f));
		model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tostadora.Draw(lightingShader);

		//////////////////////////-Animaciones-////////////////////////////////
		// Puerta abriendo/cerrandose
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(pivotOffset+2.0f, 10.0f, 17.0f)); // 3. Mover de vuelta
		model = glm::scale(model, glm::vec3(3.3f, 3.3f, 3.3f));
		model = glm::rotate(model, glm::radians(-rotPuerta), glm::vec3(0, 1, 0)); // 2. Rotar
		model = glm::translate(model, glm::vec3(-pivotOffset, 0.0f, 0.0f)); // 1. Mover al origen
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		//Puerta garage abriendo arriba/abajo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(pivotOffsetGarage + 23.0f, 0.35f, 24.0f)); // 3. Mover de vuelta
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotPuerta_Garage), glm::vec3(0, 1, 0)); // 2. Rotar
		model = glm::translate(model, glm::vec3(-pivotOffsetGarage, 0.0f, 0.0f)); // 1. Mover al origen
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_Garage.Draw(lightingShader);

		//Carrito golf moviendose adelante/atras
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(23.6f, -6.65f, 13.0f)); // Posición base
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, posCarrito));
		if (girando) {
			model = glm::rotate(model, glm::radians(anguloGiro), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		model = glm::scale(model, glm::vec3(1.9f, 1.9f, 1.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Carrito.Draw(lightingShader);

		//Tele prediendo
		//Dibujar el cuerpo de la TV
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-23.4f, 7.85f, 8.2f));
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tele.Draw(lightingShader);

		// Draw TV screen (with appropriate texture)
		tvShader.Use();
		modelLoc = glGetUniformLocation(tvShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tvShader.setMat4("view", view);
		tvShader.setMat4("projection", projection);
		tvShader.setFloat("time", animationTime);
		tvShader.setFloat("noiseIntensity", tvNoiseIntensity);
		tvShader.setBool("tvOn", tvOn);

		// Bind textures for TV
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tvOn ? tvScreenTexture : tvOffTexture);
		tvShader.setInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tvNoiseTexture);
		tvShader.setInt("noiseTexture", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tvOffTexture);
		tvShader.setInt("offTexture", 2);

		// Draw TV screen
		PantallaTele.Draw(tvShader);

		// Unbind textures to prevent leakage
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		lightingShader.Use();
		modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
		
		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Dibujar ejes de coordenadas
		lampShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		for (int i = 0; i < 2; i++) {
			glm::mat4 model(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 1.0f, 1.0f);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}



		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	float multiplier = 1.0f;
	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		multiplier = 6.0f;
	}

	if (keys[GLFW_KEY_W]) {
		camera.ProcessKeyboard(FORWARD, deltaTime * multiplier);
	}
	if (keys[GLFW_KEY_S]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime * multiplier);
	}
	if (keys[GLFW_KEY_A]) {
		camera.ProcessKeyboard(LEFT, deltaTime * multiplier);
	}
	if (keys[GLFW_KEY_D]) {
		camera.ProcessKeyboard(RIGHT, deltaTime * multiplier);
	}

	if (keys[GLFW_KEY_SPACE]) {
		camera.ProcessKeyboard(UP, deltaTime * multiplier);
	}
	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		camera.ProcessKeyboard(DOWN, deltaTime * multiplier);
	}

	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[0].x += 0.3f;
	}
	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].x -= 0.3f;
	}

	if (keys[GLFW_KEY_UP])
	{
		pointLightPositions[0].y += 0.3f;
	}

	if (keys[GLFW_KEY_DOWN])
	{
		pointLightPositions[0].y -= 0.3f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.3f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.3f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
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
	if (keys[GLFW_KEY_N] && !keyPressed) {
		isNight = !isNight; // Alterna entre noche y d�a
		keyPressed = true; // Marca que la tecla ha sido presionada
	}
	else if (!keys[GLFW_KEY_N]) {
		keyPressed = false; // Si la tecla se ha soltado, resetea el estado
	}

	if (keys[GLFW_KEY_L] && !keyPressed) {
		lightsOff = !lightsOff; // Alterna entre noche y d�a
		keyPressed2 = true; // Marca que la tecla ha sido presionada
	}
	else if (!keys[GLFW_KEY_L]) {
		keyPressed2 = false; // Si la tecla se ha soltado, resetea el estado
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		animPuerta = !animPuerta;
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		animGarage = !animGarage;
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		animCarrito = true; // Inicia la animación con cada pulsación
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		tvOn = !tvOn;
	}

}

void Animation(float deltaTime) {
	if (!animPuerta) return;

	float rotation = doorSpeed * deltaTime;
	rotPuerta += abriendo ? rotation : -rotation;

	// Limitar rotación y alternar estado
	if (abriendo && rotPuerta >= 90.0f) {
		rotPuerta = 90.0f;
		abriendo = false;
		animPuerta = false; // Opcional: detener animación al llegar
	}
	else if (!abriendo && rotPuerta <= 0.0f) {
		rotPuerta = 0.0f;
		abriendo = true;
		animPuerta = false; // Opcional: detener animación al llegar
	}
}

void Animation_garage(float deltaTime) {
	if (!animGarage) return;

	float rotation = doorSpeed * deltaTime;
	rotPuerta_Garage += abriendoGarage ? rotation : -rotation;

	// Limitar rotación y alternar estado
	if (abriendoGarage && rotPuerta_Garage >= 90.0f) {
		rotPuerta_Garage = 90.0f;
		abriendoGarage = false;
		animGarage = false;
	}
	else if (!abriendoGarage && rotPuerta_Garage <= 0.0f) {
		rotPuerta_Garage = 0.0f;
		abriendoGarage = true;
		animGarage = false; 
	}
}

void AnimationCarrito(float deltaTime) {
	if (!animCarrito) return;

	// Fase 1: Movimiento hacia el destino
	if (!enPosicionFinal && !girando) {
		posCarrito += velocidadCarrito * deltaTime;
		if (posCarrito >= distanciaTotal) {
			posCarrito = distanciaTotal;
			girando = true; // Activa la fase de giro
		}
	}
	// Fase 2: Giro en el destino
	else if (girando) {
		anguloGiro += velocidadGiro * deltaTime;

		// Comprueba si completó las vueltas
		if (anguloGiro >= 360.0f * vueltasRequeridas) {
			anguloGiro = 0.0f;
			girando = false;
			enPosicionFinal = true; // Prepara el regreso
		}
	}
	// Fase 3: Regreso al origen
	else if (enPosicionFinal) {
		posCarrito -= velocidadCarrito * deltaTime;
		if (posCarrito <= 0.0f) {
			posCarrito = 0.0f;
			enPosicionFinal = false;
			animCarrito = false; // Fin del ciclo
		}
	}
}

//Function to update procedural noise texture
void updateNoiseTexture() {
	std::vector<unsigned char> noiseData(256 * 256 * 3);
	for (int i = 0; i < 256 * 256 * 3; ++i) {
		noiseData[i] = rand() % 256;
	}
	glBindTexture(GL_TEXTURE_2D, tvNoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, noiseData.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
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