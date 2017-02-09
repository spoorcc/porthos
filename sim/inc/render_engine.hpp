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
    std::vector< GLuint > shaders;
    GLuint program;
    GLint mvp_location;
    GLFWwindow* window;

    public:
        RenderEngine(void);
        ~RenderEngine(void);

        virtual void update(void);
        virtual operator std::string() const;
        virtual void entity_added(Entity* entity);

   private:
        void setup(); 
        void display();
        void setup_window();
        void compile_shaders();

        void create_vertex_buffer(const xy_rgb_t* array,
                                  const long unsigned int size);
};


#endif /* RENDER_ENGINE_CLASS_HPP */
