#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

/**
 * This function is called whenever an error occurs within GLFW.
 * @param error The error code.
 * @param description A human-readable description of the error.
 */
static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

/**
 * This funtion is called whenever a key is pressed or released.
 * @param window The window that received the event.
 * @param key The keyboard key that was pressed or released.
 * @param scancode The system-specific scancode of the key.
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
 */
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/**
 * This function is called whenever the window is resized.
 * @param window The window that received the event.
 * @param width The new width, in pixels, of the window.
 * @param height The new height, in pixels, of the window.
 */
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  glViewport(0, 0, width, height);
}

/**
 * Convert a color from 0-255 range to 0.0-1.0 range.
 * @param color The color value in 0-255 range.
 * @return The color value in 0.0-1.0 range.
 */
static float convert_to_rgba(unsigned short color) {
  return (float)color / 255.0f;
}

int main() {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  glfwSetErrorCallback(error_callback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    exit(EXIT_FAILURE);
  }
  glViewport(0, 0, 800, 600);

  float r = convert_to_rgba(255);
  float g = convert_to_rgba(165);
  float b = convert_to_rgba(0);
  while (!glfwWindowShouldClose(window)) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
