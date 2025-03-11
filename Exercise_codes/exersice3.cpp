#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

// Vertex Shader Source Code
const char *vertexshader = "#version 330 core\n"
  "layout (location = 0) in vec3 apos;\n"
  "void main() {\n"
      "gl_Position = vec4(apos, 1.0f);\n"
  "}\0";

// Fragment Shader Source Code for orange color
const char *fragmentshader1 = "#version 330 core\n"
  "out vec4 Fragcolor;\n"
  "void main() {\n"
      "Fragcolor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";

// Fragment Shader Source Code for yellow color
const char *fragmentshader2 = "#version 330 core\n"
  "out vec4 Fragcolor;\n"
  "void main() {\n"
      "Fragcolor = vec4(1.0, 1.0, 0.0, 1.0);\n"
  "}\n\0";

// Function to compile a shader
unsigned int compileshader(const char *shader, unsigned int shadertype) {
  unsigned int shaderid = glCreateShader(shadertype);
  glShaderSource(shaderid, 1, &shader, NULL);
  glCompileShader(shaderid);

  int status;
  char loginfo[512];
  glGetShaderiv(shaderid, GL_COMPILE_STATUS, &status);
  if (!status) {
    glGetShaderInfoLog(shaderid, sizeof(loginfo), NULL, loginfo);
    cout << "FAILED to compile shader. REASON: " << loginfo << endl;
    return 0; // Return 0 to indicate failure
  }
  return shaderid;
}

// Function to create a shader program
unsigned int createshader(const char *vertexshader, const char *fragmentshader) {
  unsigned int vertxsha = compileshader(vertexshader, GL_VERTEX_SHADER);
  unsigned int fragmsha = compileshader(fragmentshader, GL_FRAGMENT_SHADER);

  if (vertxsha == 0 || fragmsha == 0) {
    cout << "Shader compilation failed, cannot create shader program\n";
    return 0; // Return 0 to indicate failure
  }

  unsigned int programid = glCreateProgram();
  glAttachShader(programid, vertxsha);
  glAttachShader(programid, fragmsha);
  glLinkProgram(programid);

  int status;
  char loginfo[512];
  glGetProgramiv(programid, GL_LINK_STATUS, &status);
  if (!status) {
    glGetProgramInfoLog(programid, sizeof(loginfo), NULL, loginfo);
    cout << "FAILED to link the program. REASON: " << loginfo << endl;
    return 0;
  }

  glDeleteShader(vertxsha);
  glDeleteShader(fragmsha);
  return programid;
}

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    cout << "Failed to initialize GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFW window
  GLFWwindow *window = glfwCreateWindow(800, 600, "exercise3", NULL, NULL);
  if (!window) {
    cout << "Failed to create the window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  // Set viewport
  glViewport(0, 0, 800, 600);

  // Create shader program or orange rendering
  unsigned int programid_1 = createshader(vertexshader, fragmentshader1);
  if (programid_1 == 0) {
    cout << "Failed to create shader program\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }
  
  // Create shader program or yellow rendering
  unsigned int programid_2 = createshader(vertexshader, fragmentshader2);
  if (programid_1 == 0) {
    cout << "Failed to create shader program\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  // Vertex data
  GLfloat vertices_1[] = {
    // First triangle
    -0.9f, -0.5f, 0.0f,  // Left
    -0.0f, -0.5f, 0.0f,  // Right
    -0.45f, 0.5f, 0.0f,  // Top 
    // Second triangle
  };  
  
  GLfloat vertices_2[] = {
    0.0f, -0.5f, 0.0f,   // Left
    0.9f, -0.5f, 0.0f,   // Right
    0.45f, 0.5f, 0.0f    // Top 
  };

  // Generate and bind VAO and VBO
  GLuint VAO[2], VBO[2];
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);


  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);


  // Render loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programid_1);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glUseProgram(programid_2);
    glBindVertexArray(VAO[2]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteVertexArrays(1, VAO);
  glDeleteBuffers(1, VBO);
  glDeleteProgram(programid_1);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
