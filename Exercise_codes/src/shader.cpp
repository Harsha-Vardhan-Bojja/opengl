#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if (!vertexFile.is_open() || !fragmentFile.is_open()) {
        std::cerr << "Failed to open shader files!\n";
        return;
    }

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    vertexFile.close();
    fragmentFile.close();

    std::string vertexCode = vertexStream.str();
    std::string fragmentCode = fragmentStream.str();

    programID = createShader(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::use() {
    glUseProgram(programID);
}

unsigned int Shader::createShader(const char* vertexShader, const char* fragmentShader) {
    unsigned int vertex = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fragment = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    int success;
    char log[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        std::cerr << "Shader Program Linking Error: " << log << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}

unsigned int Shader::compileShader(const char* shaderCode, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << "Shader Compilation Error: " << log << "\n";
    }

    return shader;
}

void Shader::texturing(const char* image_path, GLuint& textureID) {
    // Generate the texture ID before binding
    glGenTextures(1, &textureID);  
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image data
    int width, height, channels;
    unsigned char* data = stbi_load(image_path, &width, &height, &channels, 0);
    
    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Loaded texture: " << image_path << " (" << width << "x" << height << ")\n";
    } else {
        std::cerr << "Failed to load texture: " << image_path << "\n";
    }
    
    // Free the image memory
    stbi_image_free(data);
}
