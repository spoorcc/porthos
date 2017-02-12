#ifndef RENDER_ENGINE_CLASS_HPP
#define RENDER_ENGINE_CLASS_HPP

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render_component.hpp"

#define RENDER_ENGINE_WINDOW_WIDTH (640)
#define RENDER_ENGINE_WINDOW_HEIGHT (480)
#define RENDER_ENGINE_WINDOW_TITLE ("Porthos - Simulation")

struct xy_rgb_t {
    float x, y;
    float r, g, b;
};

class RenderEngine : public Engine {

    std::vector< RenderComponent * > components;
    std::vector< GLuint > vertex_buffers;
    std::vector< GLuint > vaos;
    std::vector< GLuint > vbos;
    std::vector< GLuint > shaders;
    GLuint program;
    GLint mvp_location;
    GLFWwindow* window;

    GLuint m_vaoID[2];            // two vertex array objects, one for each drawn object
    GLuint m_vboID[3];            // three VBOs

    public:
        RenderEngine(void);
        ~RenderEngine(void);

        virtual void update(void);
        virtual operator std::string() const;
        virtual void entity_added(Entity* entity);

   private:
        void resize(unsigned int width, unsigned int height); 
        void setup(); 
        void display();
        void setup_window();
        void prepare_scene();

        void create_vertex_buffer();

        std::string load_shader(std::string path);
        GLuint create_shader(std::string path,
                                           GLenum shaderType);
};


#endif /* RENDER_ENGINE_CLASS_HPP */
