#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "linmath.h"

#include "engine.hpp"
#include "entity.hpp"
#include "render_component.hpp"
#include "render_engine.hpp"

extern "C" {
    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

RenderEngine::RenderEngine(void)
{
    setup();
}

RenderEngine::~RenderEngine(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void RenderEngine::resize(unsigned int width, unsigned int height)
{
    glViewport(0, 0, width, height);
}

/* TODO move to separate thread */
void RenderEngine::update(void)
{
    if (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(m_vaoID[0]);        // select first VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);    // draw first object

        glBindVertexArray(m_vaoID[1]);        // select second VAO
        glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
        glDrawArrays(GL_TRIANGLES, 0, 3);    // draw second object

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

RenderEngine::operator std::string() const
{
}

void RenderEngine::entity_added(Entity* entity)
{

}

void RenderEngine::create_vertex_buffer()
{
    // First simple object
    float* vert = new float[9];    // vertex array
    float* col  = new float[9];    // color array

    vert[0] =-0.3; vert[1] = 0.5; vert[2] =-1.0;
    vert[3] =-0.8; vert[4] =-0.5; vert[5] =-1.0;
    vert[6] = 0.2; vert[7] =-0.5; vert[8]= -1.0;

    col[0] = 1.0; col[1] = 0.0; col[2] = 0.0;
    col[3] = 0.0; col[4] = 1.0; col[5] = 0.0;
    col[6] = 0.0; col[7] = 0.0; col[8] = 1.0;

    // Second simple object
    float* vert2 = new float[9];    // vertex array

    vert2[0] =-0.2; vert2[1] = 0.5; vert2[2] =-1.0;
    vert2[3] = 0.3; vert2[4] =-0.5; vert2[5] =-1.0;
    vert2[6] = 0.8; vert2[7] = 0.5; vert2[8]= -1.0;

    // Two VAOs allocation
    glGenVertexArrays(2, &m_vaoID[0]);

    // First VAO setup
    glBindVertexArray(m_vaoID[0]);

    glGenBuffers(2, m_vboID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), col, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Second VAO setup    
    glBindVertexArray(m_vaoID[1]);

    glGenBuffers(1, &m_vboID[2]);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert2, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    delete [] vert;
    delete [] vert2;
    delete [] col;
}


void RenderEngine::setup()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        throw;

    setup_window();

    // NOTE: OpenGL error checks have been omitted for brevity

    create_vertex_buffer();

    prepare_scene();

}
void RenderEngine::setup_window()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(RENDER_ENGINE_WINDOW_WIDTH,
            RENDER_ENGINE_WINDOW_HEIGHT,
            RENDER_ENGINE_WINDOW_TITLE,
            NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        throw;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
}


void RenderEngine::prepare_scene()
{
    GLuint vertex_shader = create_shader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragment_shader = create_shader("fragment_shader.glsl", GL_FRAGMENT_SHADER);

    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");

    glLinkProgram(program);
}

std::string RenderEngine::load_shader(std::string path)
{
     std::ifstream shader_file (path);
     std::stringstream buffer;

     if (shader_file.is_open())
     {
         buffer << shader_file.rdbuf();
     }

     return buffer.str();
}

GLuint RenderEngine::create_shader(std::string path, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    std::string shader_str = load_shader(path);
    const char* shader_c_str = shader_str.c_str();
    glShaderSource(shader, 1, &shader_c_str, NULL);
    glCompileShader(shader);
    shaders.push_back(shader);

    return shader;
}

void RenderEngine::display()
{

}
