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

#include "Utils/String.h"
#include "Graphics/Car.h"
#include "Graphics/Grid.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Timing.h"

void updateInputs(float timestep, GLFWwindow* window, Car* car);

int main() {
    // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    int width = 1024;
    int height = 768;
    GLFWwindow* window = glfwCreateWindow(width, height, "Comp371 - Lab 01", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
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
    cam->setPosition(Vector3f(0.f, 5.f, -5.f));

    // Shaders.
    Shader* shd  = new Shader("default/");
    shd->addVec3VertexInput("position");
    cam->addShader(shd);

    // Models.
    Car* car = new Car(shd);
    Grid* grid = new Grid(shd);

    while (!glfwWindowShouldClose(window)) {
        // Detect inputs.
        glfwPollEvents();
        
        while (timing->tickReady()) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
                break;
            }
            
            updateInputs((float)timing->getTimeStep(), window, car);
            
            timing->subtractTick();
        }

        // Draw code.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam->update();

        car->render();
        grid->render();

        glfwSwapBuffers(window);
        
        // Get elapsed seconds since last run.
        double secondsPassed = timing->getElapsedSeconds();
        timing->addSecondsToAccumulator(secondsPassed);
    }

    delete cam;
    delete car;
    delete grid;
    delete shd;

    // Shutdown GLFW
    glfwTerminate();

	return 0;
}

void updateInputs(float timestep, GLFWwindow* window, Car* car) {
    // Movement.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        car->walk(true, timestep * 5.f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        car->walk(true, timestep * -5.f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        car->addRotationY(timestep * 5.f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        car->addRotationY(timestep * -5.f);
    }
    
    // Scale.
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        car->addScale(timestep * 2.f);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        car->addScale(timestep * -2.f);
    }
}
