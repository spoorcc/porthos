#ifndef RENDER_ENGINE_CLASS_HPP
#define RENDER_ENGINE_CLASS_HPP

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "position_component.hpp"
#include "render_component.hpp"

#define RENDER_ENGINE_WINDOW_WIDTH (640)
#define RENDER_ENGINE_WINDOW_HEIGHT (480)
#define RENDER_ENGINE_WINDOW_TITLE ("Porthos - Simulation")

struct vec2f { float x,y; };
struct vec3f { float x,y,z; };

struct draw_array_t {
    GLenum mode = GL_TRIANGLES;
    GLint first = 0;
    GLsizei count = 3;
};

struct draw_elements_t {
    GLenum mode = GL_TRIANGLES;
    GLsizei count = 3;
    GLenum type = GL_UNSIGNED_INT;
    GLvoid * indices = 0;
};

class RenderObject {

    public:
        RenderObject(void) = default;
        ~RenderObject(void) = default;

    GLuint vao = 0;
    std::vector<GLuint> vbos;
    draw_array_t draw_array;
    draw_elements_t draw_elements;

    float rotation_degrees = 0.0f;
    vec2f scale = {1.0f, 1.f};
    vec3f translation = {0.0f, 0.0f, 0.0f};
};

class RenderEngine : public Engine {

    typedef std::pair<PositionComponent*, RenderComponent*> pos_render_pair_t;
    std::vector< pos_render_pair_t > pos_render_vect;

    std::vector< RenderComponent * > components;
    std::vector< RenderObject > render_objects;
    std::vector< GLuint > vbos;
    std::vector< GLuint > shaders;
    GLuint program;
    GLint uniModel;
    GLint mvp_location;
    GLFWwindow* window;

    GLuint m_vboID[3];            // three VBOs

    public:
        RenderEngine(void);
        ~RenderEngine(void);

        virtual void update(void);
        virtual operator std::string() const;
        virtual void entity_added(std::shared_ptr<Entity> entity);

   private:
        void resize(unsigned int width, unsigned int height); 
        void setup(); 
        void display();
        void setup_window();
        void prepare_scene();

        void create_render_object(const std::vector<vec3f>& vertices, vec3f color,
                                  const std::vector<unsigned int>& elements);
        void create_render_object(const std::vector<xyz_rgb_t>& vertices,
                                  const std::vector<unsigned int>& elements);

        std::string load_shader(std::string path);
        GLuint create_shader(std::string path,
                                           GLenum shaderType);
};


#endif /* RENDER_ENGINE_CLASS_HPP */
