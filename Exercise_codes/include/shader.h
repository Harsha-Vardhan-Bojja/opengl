#ifndef SHADER_H
#define SHADER_H



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader {
public:
    unsigned int programID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();
    void texturing(const char* image_path, GLuint& textureID);

private:
    unsigned int createShader(const char* vertexShader, const char* fragmentShader);
    unsigned int compileShader(const char* shader, unsigned int shaderType);
};

#endif
