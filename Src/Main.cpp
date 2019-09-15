//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>
#include <fstream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include "Timing.h"
#include "Graphics/Car.h"
#include "Graphics/Grid.h"
#include "Graphics/Axis.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Math/MathUtil.h"

int width = 1024;
int height = 768;

void updateInputs(float timestep, GLFWwindow* window, Car* car);
static void cursorPositionCallback(GLFWwindow* window, double x, double y);
float mouseXDiff = 0.f;
float mouseYDiff = 0.f;

int main() {
    // Give std::rand the current time as a seed.
    std::srand((unsigned)std::time(NULL));
    
    // Initialize GLFW and OpenGL version.
    glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create Window and rendering context using GLFW.
    GLFWwindow* window = glfwCreateWindow(width, height, "Comp371 - Lab 01", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetCursorPos(window, width / 2.f, height / 2.f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Initialize GLEW.
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    
    // Fixed time steps.
    Timing* timing = new Timing(60);
    
    // Camera.
    Camera* cam = new Camera((float)width / height);
    cam->setPosition(Vector3f(0.f, 5.f, -10.f));

    // Shaders.
    Shader* shd  = new Shader("default/");
    shd->addVec3VertexInput("position");
    cam->addShader(shd);

    // Models.
    Car* car = new Car(shd);
    car->addRotationY(MathUtil::PI / -2.f);
    Grid* grid = new Grid(shd);
    grid->scale = Vector3f(50.f, 1.f, 50.f); // 100x100 grid.
    
    Axis* xAxis = new Axis(shd);
    xAxis->color = Vector4f(1.f, 0.f, 0.f, 1.f);
    xAxis->rotation = Vector3f(0.f, MathUtil::PI / 2.f, 0.f);
    Axis* yAxis = new Axis(shd);
    yAxis->color = Vector4f(0.f, 0.f, 1.f, 1.f);
    yAxis->rotation = Vector3f(MathUtil::PI / -2.f, 0.f, 0.f);
    Axis* zAxis = new Axis(shd);
    zAxis->color = Vector4f(0.f, 0.75f, 0.f, 1.f);

    while (!glfwWindowShouldClose(window)) {
        
        while (timing->tickReady()) {
            // Detect inputs.
            glfwPollEvents();
            
            updateInputs((float)timing->getTimeStep(), window, car);
            
            timing->subtractTick();
        }

        // Draw code.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam->update();

        car->render();
        grid->render();
        
        // Disable depth buffer here to the cartesian axes always show up over everything else.
        glDisable(GL_DEPTH_TEST);
        xAxis->render();
        yAxis->render();
        zAxis->render();
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        
        // Get elapsed seconds since last run.
        double secondsPassed = timing->getElapsedSeconds();
        timing->addSecondsToAccumulator(secondsPassed);
    }

    delete cam;
    delete car;
    delete grid;
    delete zAxis;
    delete shd;

    // Shutdown GLFW
    glfwTerminate();

	return 0;
}

void cursorPositionCallback(GLFWwindow* window, double x, double y) {
    float centerX = width / 2.f;
    float centerY = height / 2.f;
    mouseXDiff = (x - centerX) / 300.f;
    mouseYDiff = (y - centerY) / 300.f;
    glfwSetCursorPos(window, centerX, centerY);
}

bool spaceHit = false; // Used to determine whether the spacebar was HIT, as opposed to just pressed.

void updateInputs(float timestep, GLFWwindow* window, Car* car) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    
    // Teleport.
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spaceHit) {
        car->addPositionXZ(Vector2f((std::rand() % 4) - 1.5f, (std::rand() % 4) - 1.5f));
        spaceHit = true;
    }
    if (spaceHit) { spaceHit = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS; }
    
    // Movement.
    float speed = 5.f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed += 15.f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        speed = 2.f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        car->walk(true, timestep * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        car->walk(false, timestep * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        car->addRotationY(timestep * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        car->addRotationY(timestep * -speed);
    }
    
    // Scale.
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        car->addScale(timestep * 2.f);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        car->addScale(timestep * -2.f);
    }
    
    // Change how the car is rendered.
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        car->setRenderingMode(GL_POINT);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        car->setRenderingMode(GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        car->setRenderingMode(GL_FILL);
    }
}
