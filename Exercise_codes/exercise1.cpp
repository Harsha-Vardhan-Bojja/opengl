#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

const char *vertexshader = "#version 330 core\n"
  "layout (location = 0) in vec3 apos;\n"
  "void main() {\n"
      "gl_Position = vec4(apos.x, apos.y, apos.z, 1.0f);\n" 
  "}\0";

const char *fragmentshader = "#version 330 core\n"
  "out vec4 Fragcolor;\n"  
  "void main() {\n"
      "Fragcolor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

unsigned int compileshader(const char *shader, unsigned int shadertype) {
  unsigned int shaderid = glCreateShader(shadertype);
  glShaderSource(shaderid, 1, &shader, NULL);
  glCompileShader(shaderid);

  int status;
  char loginfo[512];
  glGetShaderiv(shaderid,GL_COMPILE_STATUS,&status);
  if(!status) {
    glGetShaderInfoLog(shaderid,sizeof(loginfo),NULL, loginfo);
    cout << "FAILED to compile shader REASON: " << loginfo << endl;
    return 0;
  }
  return shaderid;
}

unsigned int createshader(const char *vertexshader, const char *fragmentshader) {
  unsigned int vertxsha = compileshader(vertexshader,GL_VERTEX_SHADER);
  unsigned int fragmsha = compileshader(fragmentshader,GL_FRAGMENT_SHADER);

  unsigned int programid = glCreateProgram();
  glAttachShader(programid, vertxsha);
  glAttachShader(programid, fragmsha);
  glLinkProgram(programid);

  int status;
  char loginfo[512];
  glGetProgramiv(programid,GL_LINK_STATUS,&status);
  if(!status) {
    glGetProgramInfoLog(programid,sizeof(loginfo),NULL,loginfo);
    cout << "FAILED to link the program REASON: " << loginfo << endl;
    return 0;
  }
  glDeleteShader(vertxsha);
  glDeleteShader(fragmsha);
  return programid;
}

int main () {

  if (!glfwInit()) {
        cout << "Failed to initialize GLFW\n";
        return -1;
    }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "exercise1", NULL, NULL);
  if(!window){
    cout<<"Failed to create the window\n";
    return -1;
  }
  glfwMakeContextCurrent(window);
  

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD\n";
        return -1;
    }

  glViewport(0,0,800,600);
  
  unsigned int programid = createshader(vertexshader,fragmentshader);

  GLfloat vertices[] = {
    // first triangle
    -0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
    // second triangle
      0.0f, -0.5f, 0.0f,  // left
      0.9f, -0.5f, 0.0f,  // right
      0.45f, 0.5f, 0.0f   // top 
  };


  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1,&VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);


  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programid);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,6);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(programid);
  return 0;
}