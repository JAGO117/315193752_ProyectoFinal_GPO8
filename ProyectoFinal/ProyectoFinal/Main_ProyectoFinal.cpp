// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animaSilla();
void animaPuerta();
void animaCoraje();
void animaReloj();
void animaFan();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


//Varuables para animar ojetos
//Anima silla
bool animSilla = false;
bool frente = false;
bool atras = false;
float rotSilla = 0.0;
//Anima puerta
bool abrePuerta = false;
bool cierraPuerta = false;
float rotPuerta = 0.0;
//Anima coraje
float movXCor = 0.0f;
float movZCor = 0.0f;
float rotCor = 0.0f;
float oscilaCor = 0.0f;
bool derCor = false;
bool izqCor = false;
bool animCoraje = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;

//Anima Fantasma
float movXFan = 0.0f;
float movZFan = 0.0f;
float rotFan = 0.0f;
float oscilaFan = 0.0f;
bool upFan = false;
bool downFan = false;
bool animFan = false;
bool recorrido1F = true;
bool recorrido2F = false;
bool recorrido3F = false;
bool recorrido4F = false;

//Anima Reloj
float rotMin = 0;
float rotHor = 0;
bool animReloj = false;

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Adaptacion,carga de modelos y camara sintetica", nullptr, nullptr);

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
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");

    // Load models
    Model tv((char*)"Models/Tv/TV.obj");
    Model mesita((char*)"Models/Mesita/mesita.obj");
    Model alfombra((char*)"Models/Alfombra/alfombra.obj");
    Model silla((char*)"Models/Silla/silla.obj");
    Model lampara((char*)"Models/Lampara/lampara.obj");
    Model sillon((char*)"Models/Sillon/sillon.obj");
    Model reloj((char*)"Models/Reloj/reloj.obj");
    Model minutero((char*)"Models/Reloj/minutero.obj");
    Model horero((char*)"Models/Reloj/horero.obj");
    Model casa((char*)"Models/Casa/casa.obj");
    Model puerta((char*)"Models/Puerta/puerta.obj");
    Model coraje((char*)"Models/Coraje/coraje.obj");
    Model fantasma((char*)"Models/Fantasma/fantasma.obj");

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
        animaSilla();
        animaPuerta();
        animaCoraje();
        animaReloj();
        animaFan();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        //casa
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(shader);

        //puerta
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::translate(model, glm::vec3(-1.6f, 4.2f, 11.0f));
        model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        puerta.Draw(shader);

        //alfombra
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::translate(model, glm::vec3(0.0f, 1.7f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        alfombra.Draw(shader);

        //silla
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::translate(model, glm::vec3(2.8f, 1.8f, -1.0f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        model = glm::rotate(model, glm::radians(rotSilla), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        silla.Draw(shader);

        //sillon
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-140.0F), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::translate(model, glm::vec3(0.5f, 1.85f, -2.1f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sillon.Draw(shader);

        //mesita
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.5f, 1.5f, -0.5f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        mesita.Draw(shader);

        //lampara
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.8f, 1.8f, 4.8f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lampara.Draw(shader);

        //TV
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-8.0f, 2.8f, 0.0f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tv.Draw(shader);

        //reloj
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 7.0f, 9.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        reloj.Draw(shader);
        //min
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 7.0f, 9.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        model = glm::rotate(model, glm::radians(-rotMin), glm::vec3(0.0f, 0.0f, 1.f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        minutero.Draw(shader);
        //hor
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 7.0f, 9.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        model = glm::rotate(model, glm::radians(-rotHor), glm::vec3(0.0f, 0.0f, 1.f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        horero.Draw(shader);

        //Coraje
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.0f, 1.5f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::translate(model, glm::vec3(movXCor, 0.0f, movZCor));
        model = glm::rotate(model, glm::radians(rotCor), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(oscilaCor), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        coraje.Draw(shader);

        //Fantasma
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.0f, 3.0f, -5.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::translate(model, glm::vec3(movXFan, 0.0f, movZFan));
        model = glm::rotate(model, glm::radians(rotFan), glm::vec3(0.0f, 1.0f, 0.f));
        model = glm::translate(model, glm::vec3(0.0f, oscilaFan, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fantasma.Draw(shader);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

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

    if (keys[GLFW_KEY_C]) {
        animSilla = true;
    }

    if (keys[GLFW_KEY_V])
    {
        animCoraje = true;
    }

    if (keys[GLFW_KEY_R])
    {
        animReloj = true;
    }
    if (keys[GLFW_KEY_F])
    {
        animFan = true;
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
    if (keys[GLFW_KEY_P])
    {
        if (rotPuerta <= 0.0f)
            abrePuerta = true;
        if (rotPuerta >= 70.0f)
            cierraPuerta = true;

    }

}

void animaSilla() {
    if (frente == false && atras == false) {
        frente = true;
    }
    if (animSilla) {
        if (frente) {
            rotSilla += 0.15;
            if (rotSilla > 10) {
                frente = false;
                atras = true;
            }
            
        }
        if (atras) {
            rotSilla -= 0.15;
            if (rotSilla < -18) {
                frente = true;
                atras = false;
            }

        }
       
    }
}

void animaPuerta() {

    if (abrePuerta)
    {
        if (rotPuerta < 70.0f) {
            rotPuerta += 0.12f;
        }

        else {
            abrePuerta = false;
        }
    }
    if (cierraPuerta)
    {
        if (rotPuerta > 0.0f) {
            rotPuerta -= 0.12f;
        }
        else {
            cierraPuerta = false;
        }
    }
}

void animaCoraje() {
    if (derCor == false && izqCor == false) {
        derCor = true;
    }
    if (animCoraje)
    {
        if (derCor) {
            oscilaCor += 0.25;
            if (oscilaCor > 10) {
                derCor = false;
                izqCor = true;
            }

        }
        if (izqCor) {
            oscilaCor -= 0.25;
            if (oscilaCor < -10) {
                derCor = true;
                izqCor = false;
            }

        }
        if (recorrido1)
        {
            movZCor += 0.05f;
            if (movZCor > 18)
            {
                recorrido1 = false;
                recorrido2 = true;
            }
        }
        if (recorrido2)
        {
            rotCor = 90;
            movXCor += 0.05f;
            if (movXCor > 22)
            {
                recorrido2 = false;
                recorrido3 = true;

            }
        }

        if (recorrido3)
        {
            rotCor = -180;
            movZCor -= 0.05f;
            if (movZCor < -18)
            {
                recorrido3 = false;
                recorrido4 = true;
            }
        }

        if (recorrido4)
        {
            rotCor = -90;
            movXCor -= 0.05f;
            if (movXCor < -1)
            {
                recorrido4 = false;
                recorrido1 = true;
                rotCor = 0;
            }
        }
    }
}

void animaFan() {
    if (upFan == false && downFan == false) {
        upFan = true;
    }
    if (animFan)
    {
        if (upFan) {
            oscilaFan += 0.02;
            if (oscilaFan > 1) {
                upFan = false;
                downFan = true;
            }

        }
        if (downFan) {
            oscilaFan -= 0.02;
            if (oscilaFan < -1) {
                upFan = true;
                downFan = false;
            }

        }
        if (recorrido1F)
        {
            movZFan += 0.025f;
            if (movZFan > 13)
            {
                recorrido1F = false;
                recorrido2F = true;
            }
        }
        if (recorrido2F)
        {
            rotFan = 90;
            movXFan += 0.025f;
            if (movXFan > 12.5)
            {
                recorrido2F = false;
                recorrido3F = true;

            }
        }

        if (recorrido3F)
        {
            rotFan = -180;
            movZFan -= 0.025f;
            if (movZFan < -3)
            {
                recorrido3F = false;
                recorrido4F = true;
            }
        }

        if (recorrido4F)
        {
            rotFan = -90;
            movXFan -= 0.025f;
            if (movXFan < -1)
            {
                recorrido4F = false;
                recorrido1F = true;
                rotFan = 0;
            }
        }
    }
}

void animaReloj() {
    if (animReloj) {
        rotMin += 0.03; 
        rotHor += 0.005;
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