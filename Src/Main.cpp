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
#include "Graphics/Triangle.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Timing.h"

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

//    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearColor(0.38f, 0.08f, 0.137f, 1.f);
    
    // Fixed time steps.
    Timing* timing = new Timing(60);
    
    // Camera.
    Camera* cam = new Camera((float)width / height);
    cam->setPosition(Vector3f(0.f, 0.f, -5.f));

    // Shaders.
    Shader* shd  = new Shader("default/");
    shd->addVec3VertexInput("position");
    cam->addShader(shd);

    // Define and upload geometry to the GPU here ...
    Triangle* tri = new Triangle(shd);

    // Entering Main Loop
    while(!glfwWindowShouldClose(window)) {
        // Detect inputs
        glfwPollEvents();
        
        while (timing->tickReady()) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
                break;
            }
            
            timing->subtractTick();
        }

        // Draw code.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam->update();

        // Draw rainbow triangle.
        tri->render();

        // End frame
        glfwSwapBuffers(window);
        
        // Get elapsed seconds since last run.
        double secondsPassed = timing->getElapsedSeconds();
        timing->addSecondsToAccumulator(secondsPassed);
    }

    delete cam;
    delete tri;
    delete shd;

    // Shutdown GLFW
    glfwTerminate();

	return 0;
}
