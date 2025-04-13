#include <iostream>
#include <vector>
#include <thread>

#include "CAD/general/version.hpp"


#include "glad/gl.h"
#include "glfw/include/GLFW/glfw3.h"

#include "bbmodel.hpp"

typedef float BB_TYPE;

typedef CAD::geometry::BoundingBox::BB<BB_TYPE> BB;
typedef CAD::geometry::BoundingBox::Box<BB_TYPE> BBox;
typedef CAD::geometry::BoundingBox::Sphere<BB_TYPE> BSphere;
typedef CAD::geometry::visualizer::BoundingBoxVisualizer<BB_TYPE> BVis;

void animateBoundingBoxes(std::vector<BB*> *bbs, bool *stop){
    while(!stop){

    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if(version == 0){
        glfwTerminate();
        return -1;
    }

    #include "shader.hpp"

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glClearColor(0, 0, 0, 255);



    std::vector<BB*> boundingBoxes;
    std::vector<BVis> boundingBoxVisualizers;
    for(int i=0;i<5;++i){
        boundingBoxes.push_back(new CAD::geometry::BoundingBox::Box<float>{
            {0.0f,      (float)i,   0.0f},
            {1.0f,      1.0f,       1.0f}
        });
        boundingBoxes.push_back(new CAD::geometry::BoundingBox::Sphere<float>{
            {0.0f,      0.0f,       (float)i},
            1.0f+((float)i*0.1f)
        });
    }
    for(BB *bb : boundingBoxes){
        boundingBoxVisualizers.push_back({*bb});
    }
    for(BVis &vis : boundingBoxVisualizers){
        vis.generateMesh();
        vis.uploadMesh();
    }

    bool stop = false;
    std::thread boxAnimation{&animateBoundingBoxes, &boundingBoxes, &stop};

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        for(BVis &vis : boundingBoxVisualizers){
            vis.render();
        }
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    stop = true;
    if(boxAnimation.joinable()){
        boxAnimation.join();
    }
    for(auto *bb : boundingBoxes){
        delete bb;
    }

    glfwTerminate();
    return 0;
}