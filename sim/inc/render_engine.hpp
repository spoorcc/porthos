#ifndef RENDER_ENGINE_CLASS_HPP
#define RENDER_ENGINE_CLASS_HPP

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define RENDER_ENGINE_WINDOW_WIDTH (640)
#define RENDER_ENGINE_WINDOW_HEIGHT (480)
#define RENDER_ENGINE_WINDOW_TITLE ("Porthos - Simulation")

class RenderEngine : public Engine {

    std::vector< RenderComponent * > components;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLFWwindow* window;

    public:
        RenderEngine(void);
        ~RenderEngine(void);

        virtual void update(void);
        virtual operator std::string();
        virtual void entity_added(Entity* entity);

   private:
        void setup(); 
        void display();
};


#endif /* RENDER_ENGINE_CLASS_HPP */
