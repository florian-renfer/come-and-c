#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_SHAPES 5

typedef struct {
  float x, y;
  float dx, dy;
  float r, g, b;
} Shape;

const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "uniform vec2 offset;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos + offset, 0.0, 1.0);\n"
    "}\n";

const char *fragment_shader_src = "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "uniform vec3 color;\n"
                                  "void main() {\n"
                                  "   FragColor = vec4(color, 1.0);\n"
                                  "}\n";

float vertices[] = {-0.05f, -0.05f, 0.05f, -0.05f, 0.05f, 0.05f, -0.05f, 0.05f};
unsigned int indices[] = {0, 1, 2, 2, 3, 0};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

float randf(float min, float max) {
  return min + (float)rand() / ((float)RAND_MAX / (max - min));
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

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    fprintf(stderr, "Error getting process address.\n");
    return EXIT_FAILURE;
  }

  // Compile shaders
  unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader_src, NULL);
  glCompileShader(vs);
  unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader_src, NULL);
  glCompileShader(fs);
  unsigned int shader = glCreateProgram();
  glAttachShader(shader, vs);
  glAttachShader(shader, fs);
  glLinkProgram(shader);
  glDeleteShader(vs);
  glDeleteShader(fs);

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
  Shape shapes[NUM_SHAPES];
  for (int i = 0; i < NUM_SHAPES; ++i) {
    shapes[i].x = randf(-0.8f, 0.8f);
    shapes[i].y = randf(-0.8f, 0.8f);
    shapes[i].dx = randf(-0.01f, 0.01f);
    shapes[i].dy = randf(-0.01f, 0.01f);
    shapes[i].r = randf(0.2f, 1.0f);
    shapes[i].g = randf(0.2f, 1.0f);
    shapes[i].b = randf(0.2f, 1.0f);
  }

  glUseProgram(shader);
  int offsetLoc = glGetUniformLocation(shader, "offset");
  int colorLoc = glGetUniformLocation(shader, "color");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < NUM_SHAPES; ++i) {
      shapes[i].x += shapes[i].dx;
      shapes[i].y += shapes[i].dy;
      if (fabsf(shapes[i].x) > 0.95f)
        shapes[i].dx *= -1;
      if (fabsf(shapes[i].y) > 0.95f)
        shapes[i].dy *= -1;

      glUniform2f(offsetLoc, shapes[i].x, shapes[i].y);
      glUniform3f(colorLoc, shapes[i].r, shapes[i].g, shapes[i].b);
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
