#include <chrono>  // Include the header for time measurement
#include "../include/shader.h"

int main () {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "texture_ex", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create the window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);

    Shader myshader("../shaders/vertex_shader.vs", "../shaders/fragment_shader.fs");

    float vertices[] = {
        // positions         // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // top left 
    };

    GLuint indices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    GLuint texture;
    myshader.texturing("../images/road.png", texture);
    

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    

    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);

    myshader.use();
    glUniform1i(glGetUniformLocation(myshader.programID, "ourTexture1"), 0);
    
    
    //I just added this timer for my purpose. You can avoid it.
    // Variables for timing
    std::chrono::high_resolution_clock::time_point start, end;
    double frameTime = 0.0;

    while (!glfwWindowShouldClose(window)) {
        // Record start time
        start = std::chrono::high_resolution_clock::now();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
       

        myshader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Record end time
        end = std::chrono::high_resolution_clock::now();

        // Compute frame time in milliseconds
        frameTime = std::chrono::duration<double, std::milli>(end - start).count();

        // Print frame time and FPS
        std::cout << "Frame Time: " << frameTime << " ms | FPS: " << (1000.0 / frameTime) << std::endl;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(myshader.programID);
    return 0;
}
