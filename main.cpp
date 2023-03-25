#include <cassert>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include "cubeverts.h"
#include "asteroid.h"

/////////////////////////////////////////////////////
// I truly am sorry if you have to read this code  //
// I'm serious.                         Eli Lewis  //
/////////////////////////////////////////////////////

// significant help from docs at https://learnopengl.com/

// global vars
unsigned int shaderProgram;
unsigned int VBO, VAO, EBO;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const std::string vertex_shader_source_file = "./vertexshader.glsl";
const std::string fragment_shader_source_file = "./fragmentshader.glsl";

glm::vec3 cameraPos; 
glm::vec3 direction;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = (int)SCR_WIDTH/2, lastY = (int)SCR_HEIGHT/2;
float yaw, pitch, roll;
bool firstMouse = true;

Asteroid * asteroids;

// function defs
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int compile_shaders();
void create_vaos();
void create_vbos();
void bind_buffer_data(int buffer, float * vertices, int vertices_size);
void loadShader(std::string filename, std::string &shader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "main", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // compile shaders
    // ------------------------------------------------------------------
    if (compile_shaders())
    {
        std::cout << "Failed to initialize shaders!" << std::endl; 
        return -1;
    }

    glEnable(GL_DEPTH_TEST); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, mouse_callback);  

    create_vaos();
    create_vbos();
    bind_buffer_data(VBO, vertices, vertices_size);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);


    cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 projection    = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(65.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    int mpvLoc = glGetUniformLocation(shaderProgram, "mpv");

    /*int number_of_asteroids = 10;
    asteroids = (Asteroid*)malloc(sizeof(Asteroid) * number_of_asteroids);
    for (int i = 0; i < number_of_asteroids; i++){
        asteroids[i] = new Asteroid(glm::vec3(1.0f * i + 5.0f, 1.0f * i + 5.0f, 1.0f * i + 5.0f), glm::vec3(0.0f));
    }*/
    
    Asteroid a(1.0f, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f);
    std::cout << a.location[0] << std::endl;
    std::cout << a.location[1] << std::endl;
    std::cout << a.location[2] << std::endl;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //time calcs
        // -----
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
        
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //for(unsigned int i = 0; i < number_of_asteroids; i++)
        {
           
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, a.location);
            //model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 mpv = projection * view * model;
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glUniformMatrix4fv(mpvLoc, 1, GL_FALSE, glm::value_ptr(mpv));
        }

        // handle camera
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    free(asteroids);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;

};

// Handle mouse events
// ---------------------------------------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos){  

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    const float cameraSpeed = deltaTime * 2.5f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int compile_shaders(){

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    
    std::string fragmentShaderString;
    std::string vertexShaderString;

    loadShader(vertex_shader_source_file, vertexShaderString);
    loadShader(fragment_shader_source_file, fragmentShaderString);

    const char * fragmentShaderSource = fragmentShaderString.c_str();
    const char * vertexShaderSource = vertexShaderString.c_str();


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}

void create_vbos(){
    glGenBuffers(1, &VBO);
}

void bind_buffer_data(int buffer, float * vertices, int vertices_size){
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices_size, vertices, GL_STATIC_DRAW);
}

void create_vaos(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void create_ebos(){
    glGenBuffers(1, &EBO);
}

void loadShader(std::string filename, std::string &shader){
    std::ifstream fin(filename.c_str());
    assert(fin.is_open());
    
    std::string tmp;
    while (std::getline(fin, tmp)){
        shader += tmp + "\n";
    }

    fin.close();
}

