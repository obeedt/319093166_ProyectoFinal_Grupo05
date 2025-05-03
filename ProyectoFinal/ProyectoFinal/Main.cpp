// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final - Computacion Grafica", nullptr, nullptr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

    // En la sección de shader (después de cargar los shaders)
    shader.Use();
    // Configuración de la luz direccional
    // Después de cargar los shaders y antes del game loop
    shader.Use();

    // Configuración de la luz direccional
    glm::vec3 lightDir(0.0f, -1.0f, 0.0f); // Luz que viene desde arriba
    glUniform3f(glGetUniformLocation(shader.Program, "light.direction"), lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(glGetUniformLocation(shader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

    // En el game loop, antes de dibujar los modelos
    shader.Use();
    glUniform3f(glGetUniformLocation(shader.Program, "viewPos"),
        camera.GetPosition().x,
        camera.GetPosition().y,
        camera.GetPosition().z);

    // También necesitas pasar la posición de la cámara para el cálculo de specular
    //glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

    // Load models
    Model alacena((char*)"Models/alacena.obj");
    Model campana((char*)"Models/campana.obj");
    Model estufa((char*)"Models/estufa.obj");
    Model mesa((char*)"Models/mesa.obj");
    Model refrigerador((char*)"Models/refrigerador.obj");
    Model silla((char*)"Models/silla.obj");
	Model tostadora((char*)"Models/tostadora.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);




    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // En el game loop, antes de dibujar los modelos
        shader.Use();
        // Actualizar la posición/dirección de la luz si es necesario
        glm::vec3 lightDir = glm::vec3(0.0f, -1.0f, 0.0f); // Luz que viene directamente desde arriba
        glUniform3f(glGetUniformLocation(shader.Program, "light.direction"), lightDir.x, lightDir.y, lightDir.z);
        glm::vec3 cameraPos = camera.GetPosition();
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
        
        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        alacena.Draw(shader);

        glm::mat4 model2(1);
        model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        campana.Draw(shader);

        glm::mat4 model3(1);
        model3 = glm::translate(model3, glm::vec3(0.0f, 0.0f, 0.0f));
        model3 = glm::scale(model3, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model3));
        estufa.Draw(shader);

        glm::mat4 model4(1);
        model4 = glm::translate(model4, glm::vec3(0.0f, 0.0f, 0.0f));
        model4 = glm::scale(model4, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model4));
        mesa.Draw(shader);

		glm::mat4 model5(1);
		model5 = glm::translate(model5, glm::vec3(0.0f, 0.0f, 0.0f));
		model5 = glm::scale(model5, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model5));
		refrigerador.Draw(shader);

		glm::mat4 model6(1);
		model6 = glm::translate(model6, glm::vec3(0.0f, 0.0f, 0.0f));
		model6 = glm::scale(model6, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model6));
		silla.Draw(shader);

		glm::mat4 model7(1);
		model7 = glm::translate(model7, glm::vec3(0.0f, 0.0f, 0.0f));
		model7 = glm::scale(model7, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model7));
		tostadora.Draw(shader);



        // Swap the buffers
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
GLfloat modelY = 0.0f;  // Variable para almacenar la posición en Y del modelo
float moveSpeed = 5.0f;  // Velocidad de movimiento en unidades por segundo

void DoMovement()
{
    // Camera controls (Modificando la velocidad de la cámara)
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime * moveSpeed);  // Aumenta la velocidad con moveSpeed
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime * moveSpeed);  // Aumenta la velocidad con moveSpeed
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime * moveSpeed);  // Aumenta la velocidad con moveSpeed
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime * moveSpeed);  // Aumenta la velocidad con moveSpeed
    }

    // Model controls (Mover el modelo hacia arriba y abajo)
    if (keys[GLFW_KEY_Q]) // Mover el modelo hacia arriba
    {
        modelY += moveSpeed * deltaTime;  // Mueve el modelo más rápido
    }

    if (keys[GLFW_KEY_E]) // Mover el modelo hacia abajo
    {
        modelY -= moveSpeed * deltaTime;  // Mueve el modelo más rápido
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