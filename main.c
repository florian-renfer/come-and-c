#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;

unsigned int currentRenderingMode = GL_FILL;
unsigned int shaderProgram;

// clang-format off
float vertices[] = {
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};
// clang-format on

/**
 * Callback function executed when the window is created or resized.
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

/**
 * Processes input events received by the window.
 * - Pressing the ESC key will close the window.
 *   - Pressing the T key will toggle between wireframe and fill rendering
 * modes.
 */
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }

  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
    switch (currentRenderingMode) {
    case GL_LINE:
      currentRenderingMode = GL_FILL;
      break;
    case GL_FILL:
      currentRenderingMode = GL_LINE;
      break;
    }
    glPolygonMode(GL_FRONT_AND_BACK, currentRenderingMode);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Error creating window.\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    fprintf(stderr, "Error getting process address.\n");
    return EXIT_FAILURE;
  }

  shaderProgram =
      generateShader("../shaders/simple.vert", "../shaders/simple.frag");

  GLuint VBO;
  GLuint VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    int vertxOffsetLocation =
        glGetUniformLocation(shaderProgram, "horizontalOffset");
    glUniform1f(vertxOffsetLocation, 0.5f * sin(timeValue));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return EXIT_SUCCESS;
}
