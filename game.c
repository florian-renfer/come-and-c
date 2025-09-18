#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <cglm/cglm.h>
#include <cglm/mat4.h>

#include "shader.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_BOIDS 1

/**
 * Structure representing a boid in two-dimensional space.
 * Keeps track of the position vector (x, y), directional vector (dx, dy), and
 * color (r, g, b).
 */
struct Boid {
  float x, y;
  float dx, dy;
  float r, g, b;
};

// clang-format off
float vertices[] = {
  0.00f, 0.05f,
  -0.025f, -0.05f,
  0.00f, 0.00f,
  0.025f, -0.05f,
};

unsigned int indices[] = {
  0, 1, 2,
  0, 3, 2
};
// clang-format on

// TODO: move to global state struct
unsigned int currentRenderingMode = GL_FILL;
unsigned int fps = 0;

/**
 * Callback function executed when the window is created or resized.
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

/**
 * Processes input events received by the window.
 * - Pressing the ESC key will close the window.
 * - Pressing the T key will toggle between wireframe and fill rendering
 *   modes.
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

/**
 * Calculates a random float between min and max.
 */
float randf(float min, float max) {
  return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

/**
 * Calculates and prints the frames per second (FPS) to the standard output.
 */
void calculateFps(int *prev) {
  int curr = glfwGetTime();
  fps++;

  if (curr - *prev >= 1) {
    fprintf(stdout, "FPS:%d\n", fps);
    fps = 0;
    *prev = curr;
  }
}

int main() {
  srand((unsigned int)time(NULL));
  if (!glfwInit())
    return EXIT_FAILURE;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "2D Shapes Animation", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Disable v-sync (v-sync limits the FPS to the monitor refresh rate)
  glfwSwapInterval(0);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    fprintf(stderr, "Error getting process address.\n");
    return EXIT_FAILURE;
  }

  int prevtime = glfwGetTime();

  // Compile shaders
  GLuint shader = generateShader("shaders/boids.vert", "shaders/boids.frag");

  // Setup VAO/VBO/EBO
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Shapes
  struct Boid shapes[NUM_BOIDS];
  for (int i = 0; i < NUM_BOIDS; ++i) {
    shapes[i].x = 0.25;
    shapes[i].y = 0;
    shapes[i].dx = 0;
    shapes[i].dy = 0;
    shapes[i].r = 1;
    shapes[i].g = 0;
    shapes[i].b = 1;
  }

  glUseProgram(shader);
  unsigned int colorLoc = glGetUniformLocation(shader, "color");
  unsigned int transformLoc = glGetUniformLocation(shader, "transform");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    calculateFps(&prevtime);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < NUM_BOIDS; ++i) {
      glUniform3f(colorLoc, shapes[i].r, shapes[i].g, shapes[i].b);

      mat4 trans;
      glm_mat4_identity(trans);
      glm_translate(trans, (vec3){-0.5f, 0.5f, 0.0f});
      glm_rotate(trans, sin(glfwGetTime()), (vec3){0.0, 0.0, 1.0});
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float *)trans);

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader);

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
