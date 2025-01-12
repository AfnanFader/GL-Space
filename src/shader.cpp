#include <shader.h>
#include <glad.h>
#include <logging.h>
#include <fstream>
#include <sstream>

//----------------------------------------------------------------------------//

#ifdef _WIN32
    #define DIR_SEPARATOR "\\"
    #define VERTEX_FILE_PATH "\\Assets\\shader\\vertex_shader.glsl"
    #define FRAGMENT_FILE_PATH "\\Assets\\shader\\fragment_shader.glsl"
#else
    #define DIR_SEPARATOR "/"
    #define VERTEX_FILE_PATH "/Assets/shader/vertex_shader.glsl"
    #define FRAGMENT_FILE_PATH "/Assets/shader/fragment_shader.glsl"
#endif

//----------------------------------------------------------------------------//

static std::string read_shader_file(std::string file_path);
static uint32_t compile_shader(GLenum type, const char* source);
static uint32_t get_vertex_shader();
static uint32_t get_fragment_shader();

//----------------------------------------------------------------------------//

void show_path()
{
    // LOCAL DEBUG
    std::string filepath = PROJECT_DIRECTORY + (std::string)VERTEX_FILE_PATH;
    MSG_LOG_MAC(filepath);
}

std::string read_shader_file(std::string file_path)
{
    std::ifstream shaderFile(file_path);

    if (!shaderFile.is_open())
    {
        FILE_IO_ERR_LOG_MAC(file_path);
    }

    std::stringstream buffer;
    buffer << shaderFile.rdbuf();

    return buffer.str();
}

uint32_t compile_shader(GLenum type , const char* source)
{
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int32_t success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        ERROR_LOG_MAC(infoLog);
    }

    return shader;
}

uint32_t get_vertex_shader()
{
    std::string filepath = PROJECT_DIRECTORY + (std::string)VERTEX_FILE_PATH;
    std::string source = read_shader_file(filepath);
    // MSG_LOG_MAC(source);
    return compile_shader(GL_VERTEX_SHADER, source.c_str());
}

uint32_t get_fragment_shader()
{
    std::string filepath = PROJECT_DIRECTORY + (std::string)FRAGMENT_FILE_PATH;
    std::string source = read_shader_file(filepath);
    // MSG_LOG_MAC(source);
    return compile_shader(GL_FRAGMENT_SHADER, source.c_str());
}

uint32_t get_shader_program()
{
    // Get shaders
    uint32_t vertexShader = get_vertex_shader();
    uint32_t fragmentShader = get_fragment_shader();

    uint32_t shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if successful
    int32_t success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        ERROR_LOG_MAC(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
