#include <glad.h>
#include <logging.h>
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------//

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct DrawState
{
    GLFWwindow* window;
};

//----------------------------------------------------------------------------//

static void cleanup(DrawState* s);
static void glfw_error_callback(int32_t err, const char* msg);
static void init_glfw(DrawState* s);

static void framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height);

static void process_input(GLFWwindow* window);

//----------------------------------------------------------------------------//

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window ,GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
}

//----------------------------------------------------------------------------//

void framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

void glfw_error_callback(int32_t err, const char* msg)
{
    GLFW_ERR_LOG_MAC(err, msg);
}

void init_glfw(DrawState* s)
{
    if (glfwInit() != GLFW_TRUE)
    {
        ERROR_LOG_MAC("Unable to Initialize GLFW");
    }

    // Set common settings
    glfwSetErrorCallback(glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    s->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GL-Space", nullptr, nullptr);

    if (s->window == nullptr)
    {
        cleanup(s);
        ERROR_LOG_MAC("Unable to create window");
    }

    glfwMakeContextCurrent(s->window);

    // Load OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cleanup(s);
        ERROR_LOG_MAC("Unable to load OpenGL");
    }

    // Initial viewport set
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(s->window, framebuffer_size_callback);
}

//----------------------------------------------------------------------------//

void cleanup(DrawState* s)
{
    glfwTerminate();
    delete s;

    MSG_LOG_MAC("Cleanp up done");
}

int main()
{
    DrawState* state = new DrawState();

    init_glfw(state);

    while(!glfwWindowShouldClose(state->window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        process_input(state->window);

        glfwSwapBuffers(state->window);
        glfwPollEvents();    
    }
    
    cleanup(state);
    return 0;
}
