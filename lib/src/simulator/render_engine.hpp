#ifndef RENDER_ENGINE_CLASS_HPP
#define RENDER_ENGINE_CLASS_HPP

#include <algorithm>
#include <utility>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "simulator/render_component.hpp"

#define RENDER_ENGINE_WINDOW_WIDTH (640)
#define RENDER_ENGINE_WINDOW_HEIGHT (480)
#define RENDER_ENGINE_WINDOW_TITLE ("Porthos - Simulation")

extern "C" {
    static void glfw_error_callback(int error, const char* description) {
             std::cerr << "GLFW returned error: \n\t" << description << std::endl;
    }
}

static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

class RenderEngine : public Engine {

    std::vector< RenderComponent * > components;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLFWwindow* window;

    public:
        RenderEngine(void){
             setup();
        }
        ~RenderEngine(void){
             glfwDestroyWindow(window);
             glfwTerminate();
        }

        virtual void update(void) {

            if(glfwWindowShouldClose(window))
                throw;
            display();
        }

        virtual operator std::string() const {
            return "Render Engine";
        }

        virtual void entity_added(Entity* entity) {

            RenderComponent* render_ptr = entity->component<RenderComponent>();

            if ( render_ptr != NULL )
            {
                components.push_back(render_ptr);
            }
        }

   private:
        void setup() {

            if(!glfwInit())
                throw;

            // Makes 3D drawing work when something is in front of something else
            glEnable(GL_DEPTH_TEST);

            glfwSetErrorCallback(glfw_error_callback);

            window = glfwCreateWindow(RENDER_ENGINE_WINDOW_WIDTH,
                                      RENDER_ENGINE_WINDOW_HEIGHT,
                                      RENDER_ENGINE_WINDOW_TITLE,
                                      NULL, NULL);
            if (!window)
            {
               throw;
            }
            glfwMakeContextCurrent(window);

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);

            glfwSwapInterval(1);

            std::cout << glGetString(GL_VERSION) << "\n";

//            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//            glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//            glCompileShader(vertex_shader);
//            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//            glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//            glCompileShader(fragment_shader);
//            program = glCreateProgram();
//            glAttachShader(program, vertex_shader);
//            glAttachShader(program, fragment_shader);
//            glLinkProgram(program);
        }

        void display()
        {
            glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); //clear background screen to black

            //Clear information from last draw
            glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
            glLoadIdentity(); //Reset the drawing perspective

            glBegin(GL_QUADS); //Begin quadrilateral coordinates

            //Trapezoid
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-0.7f, -1.5f, -5.0f);
            glVertex3f(0.7f, -1.5f, -5.0f);
            glVertex3f(0.4f, -0.5f, -5.0f);
            glVertex3f(-0.4f, -0.5f, -5.0f);

            glEnd(); //End quadrilateral coordinates

            glBegin(GL_TRIANGLES); //Begin triangle coordinates

            //Triangle
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, 0.5f, -5.0f);
            glVertex3f(-1.0f, 1.5f, -5.0f);
            glVertex3f(-1.5f, 0.5f, -5.0f);

            glEnd(); //End triangle coordinates

            glfwSwapBuffers(window);
        }
};


#endif /* RENDER_ENGINE_CLASS_HPP */
