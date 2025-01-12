#include <glad.h>
#include <logging.h>
#include <global.h>
#include <shader.h>
#include <GLFW/glfw3.h>

//----------------------------------------------------------------------------//

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    GLFWwindow* window;
} DrawState;

//----------------------------------------------------------------------------//

static void init_glfw(DrawState* s);
static void cleanup(DrawState* s);
static void glfw_error_callback(int32_t err, const char* msg);

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
    uint32_t shaderProgram = get_shader_program();

    f32 vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // f32 vertices[] = {
    //  0.5f,  0.5f, 0.0f,  // top right
    //  0.5f, -0.5f, 0.0f,  // bottom right
    // -0.5f, -0.5f, 0.0f,  // bottom left
    // -0.5f,  0.5f, 0.0f   // top left 
    // };

    // uint32_t indices[] = {  // note that we start from 0!
    //     0, 1, 3,   // first triangle
    //     1, 2, 3    // second triangle
    // }; 

    // weird 32 bit obj ? need to study on these
    uint32_t VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // This is cleanup lol ??? STUDY
    glBindBuffer(GL_ARRAY_BUFFER, 0); // <--- This clear the buffer?
    glBindVertexArray(0);


    while(!glfwWindowShouldClose(state->window))
    {
        process_input(state->window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(state->window);
        glfwPollEvents();
    }

    // Cleanup for GL Lib
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    cleanup(state);

    return 0;
}
