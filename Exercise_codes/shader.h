#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <std_image.h>

using namespace std;

class Shader {
public:
    unsigned int programID; // Store shader program ID

    Shader(const char *vertex_sha_path, const char *fragment_sha_path) {
        ifstream vrtx_shader(vertex_sha_path);
        ifstream frgmnt_shader(fragment_sha_path);

        // Check if file opened successfully
        if (!vrtx_shader.is_open() || !frgmnt_shader.is_open()) {
            cerr << "Failed to open shader files!" << endl;
            return;
        }

        stringstream vrtx_shader_stream, frgmnt_shader_stream;
        vrtx_shader_stream << vrtx_shader.rdbuf();
        frgmnt_shader_stream << frgmnt_shader.rdbuf();
        
        vrtx_shader.close();
        frgmnt_shader.close();

        string vrtx_code = vrtx_shader_stream.str();
        string frgmnt_code = frgmnt_shader_stream.str();

        programID = createShader(vrtx_code.c_str(), frgmnt_code.c_str());
    }

    void load_data(const char* image_path, unsigned char *data) {
        unsigned int width, height, channels;
        data


    }

    void use() {
        glUseProgram(programID);
    }

private:
    unsigned int createShader(const char *vertexShader, const char *fragmentShader) {
        unsigned int vertxSha = compileShader(vertexShader, GL_VERTEX_SHADER);
        unsigned int fragmSha = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
        
        if (vertxSha == 0 || fragmSha == 0) {
            cerr << "Shader compilation failed. Cannot create shader program." << endl;
            return 0;
        }

        unsigned int programID = glCreateProgram();
        glAttachShader(programID, vertxSha);
        glAttachShader(programID, fragmSha);
        glLinkProgram(programID);

        int status;
        char logInfo[512];
        glGetProgramiv(programID, GL_LINK_STATUS, &status);
        if (!status) {
            glGetProgramInfoLog(programID, sizeof(logInfo), NULL, logInfo);
            cerr << "Failed to link shader program. Reason: " << logInfo << endl;
            return 0;
        }

        glDeleteShader(vertxSha);
        glDeleteShader(fragmSha);
        return programID;
    }

    unsigned int compileShader(const char *shader, unsigned int shaderType) {
        unsigned int shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shader, NULL);
        glCompileShader(shaderID);

        int status;
        char logInfo[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
        if (!status) {
            glGetShaderInfoLog(shaderID, sizeof(logInfo), NULL, logInfo);
            cerr << "Failed to compile shader. Reason: " << logInfo << endl;
            return 0;
        }
        return shaderID;
    }
};

#endif
