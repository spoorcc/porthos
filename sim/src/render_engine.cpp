#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
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

using namespace std;

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

    static void show_info_log(
        GLuint object,
        PFNGLGETSHADERIVPROC glGet__iv,
        PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
    )
    {
        GLint log_length;
        char *log;

        glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
        log = (char*) malloc(log_length);
        glGet__InfoLog(object, log_length, NULL, log);
        std::cerr << log << std::endl;
        free(log);
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
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);

        glm::mat4 model(1.0);

        for( auto obj : render_objects)
        {
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
            glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

            glBindVertexArray(obj.vao);
            //glDrawArrays(obj.draw_array.mode, obj.draw_array.first, obj.draw_array.count);
            glDrawElements(obj.draw_elements.mode, obj.draw_elements.count,
                           obj.draw_elements.type, obj.draw_elements.indices);
        }

        // Release vertex array
        glBindVertexArray(0);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    assert(glGetError() == GL_NO_ERROR);
}

RenderEngine::operator std::string() const
{
}

void RenderEngine::entity_added(shared_ptr<Entity> entity)
{
    PositionComponent* pos_ptr = entity->component<PositionComponent>();
    RenderComponent* render_ptr = entity->component<RenderComponent>();

    if (render_ptr != NULL)
    {
        create_render_object(render_ptr->vertices, render_ptr->elements);
    }
}

void copy_to_vbo(std::vector<vec3f> data, GLuint vbo, GLuint attribPointer)
{
    unsigned int count = data.size() * 3;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(GLfloat), (float*)&data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(attribPointer);
    glVertexAttribPointer(attribPointer, 3, GL_FLOAT, GL_FALSE, 0, 0);

    assert(glGetError() == GL_NO_ERROR);
}

void copy_xyz_rgb_to_vbo(const std::vector<xyz_rgb_t>& vertices, GLuint vbo, GLuint program)
{
    const unsigned int element_count = sizeof(xyz_rgb_t) / sizeof(float);
    unsigned int count = vertices.size() * element_count;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(GLfloat), (float*)&vertices[0], GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(program, "in_Position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, element_count*sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(program, "in_Color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                           element_count*sizeof(float), (void*)(3*sizeof(float)));
}

void copy_elements_to_vbo(std::vector<unsigned int> elements, GLuint vbo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 elements.size() * sizeof(unsigned int), (GLuint*)&elements[0], GL_STATIC_DRAW);
}


void RenderEngine::create_render_object(const std::vector<vec3f>& vertices, vec3f color,
                                        const std::vector<unsigned int>& elements)
{
    // Create VAO
    RenderObject obj;
    unsigned int count = vertices.size() * 3;
    GLint posAttrib = glGetAttribLocation(program, "in_Position");

    glGenVertexArrays(1, &obj.vao);
    glBindVertexArray(obj.vao); //Make it the actual one

    obj.vbos.resize(2, 0);
    glGenBuffers(2, &(obj.vbos[0]));

    copy_to_vbo(vertices, obj.vbos[0], posAttrib);
    copy_elements_to_vbo(elements, obj.vbos[1]);

    // Set fixed color
    glVertexAttrib3f((GLuint)1, color.x, color.y, color.z); // set constant color attribute

    // Release Vertex Array
    glBindVertexArray(0);
    assert(glGetError() == GL_NO_ERROR);

    render_objects.push_back( obj );
}

void RenderEngine::create_render_object(const std::vector<xyz_rgb_t>& vertices,
                                        const std::vector<unsigned int>& elements)
{
    assert(vertices.size() <= elements.size());

    // Create VAO
    RenderObject obj;
    unsigned int count = vertices.size() * 6;

    glGenVertexArrays(1, &obj.vao);
    glBindVertexArray(obj.vao); //Make it the actual one

    obj.vbos.resize(2, 0);
    glGenBuffers(2, &(obj.vbos[0]));

    copy_xyz_rgb_to_vbo(vertices, obj.vbos[0], program);
    copy_elements_to_vbo(elements, obj.vbos[1]);

    obj.draw_elements.count = elements.size();

    // Release Vertex Array
    glBindVertexArray(0);
    assert(glGetError() == GL_NO_ERROR);

    render_objects.push_back( obj );
}

void RenderEngine::setup()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        throw;

    setup_window();
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

    assert(glGetError() == GL_NO_ERROR);
}


void RenderEngine::prepare_scene()
{
    GLint program_ok = 0;
    GLuint vertex_shader = create_shader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragment_shader = create_shader("fragment_shader.glsl", GL_FRAGMENT_SHADER);

    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");

    glLinkProgram(program);
    uniModel = glGetUniformLocation(program, "model");

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);

    if (!program_ok)
    {
        std::cerr << "Shader program linking failed" << std::endl;
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        throw std::runtime_error("Linking shader program failed");
    }
    assert(glGetError() == GL_NO_ERROR);

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
    GLint shader_ok;
    GLuint shader = glCreateShader(shaderType);
    std::string shader_str = load_shader(path);
    const char* shader_c_str = shader_str.c_str();
    glShaderSource(shader, 1, &shader_c_str, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        std::cerr << "Failed to compile " <<  path << std::endl;
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        throw std::runtime_error("Failed to compile shaders");
    }

    shaders.push_back(shader);

    return shader;
}

void RenderEngine::display()
{

}
