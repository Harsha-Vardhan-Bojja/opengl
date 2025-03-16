#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    ifstream vertexFile(vertexPath);
    ifstream fragmentFile(fragmentPath);

    if (!vertexFile.is_open() || !fragmentFile.is_open()) {
        cerr << "Failed to open shader files!\n";
        return;
    }

    stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    vertexFile.close();
    fragmentFile.close();

    string vertexCode = vertexStream.str();
    string fragmentCode = fragmentStream.str();

    programID = createShader(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::use() {
    glUseProgram(programID);
}

unsigned int Shader::createShader(const char* vertexShader, const char* fragmentShader) {
    unsigned int vertex = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fragment = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
 if (vertex == 0 || fragment == 0) {
        return 0; // Shader compilation failed
    }
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
        cerr << "Shader Compilation Error: " << log << "\n";
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
        cout << "Loaded texture: " << image_path << " (" << width << "x" << height << ")\n";
    } else {
        cerr << "Failed to load texture: " << image_path << "\n";
    }
    
    // Free the image memory
    stbi_image_free(data);
}

/*
void Shader::processInput(GLFWwindow *window)  {
    const float cameraspeed = 0.05f * ;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraspeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraspeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraspeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraspeed;
}

void Shader::scroll_callback(GLFWwindow* widow, double xpos, double ypos){
    fov -= (float)ypos;
    if(fov < 1.0f)
        fov = 1.0f;
    if(fov > 45.0f)
        fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstmouse) {
        lastX = xpos;
        lastY = ypos;
        firstmouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitive = 0.05f;
    xoffset *= sensitive;
    yoffset *= sensitive;
    
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
        
    glm::vec3 direction;
    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    cameraFront = glm::normalize(direction);
}
*/