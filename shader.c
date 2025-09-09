#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/gl.h>

/**
 * Reads the entire contents of a file into a dynamically allocated string.
 */
char *read_file_to_string(const char *file_path) {
  FILE *file = fopen(file_path, "rb");
  if (!file) {
    printf("Failed to open file: %s\n", file_path);
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return NULL;
  }
  long size = ftell(file);
  if (size < 0) {
    fclose(file);
    return NULL;
  }
  rewind(file);

  char *buffer = malloc(size + 1);
  if (!buffer) {
    fclose(file);
    return NULL;
  }

  size_t read_size = fread(buffer, 1, size, file);
  buffer[read_size] = '\0';

  fclose(file);

  if (read_size != size) {
    free(buffer);
    return NULL;
  }

  return buffer;
}

/**
 * Checks for shader compilation and linking errors.
 */
void check_compile_errors(unsigned int shader, char *type) {
  int success;
  char infoLog[1024];
  if (strcmp("PROGRAM", type) != 0) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      fprintf(stderr,
              "ERROR::SHADER_COMPILATION_ERROR of type:\t%s\n%s\n"
              "-- --------------------------------------------------- -- \n",
              type, infoLog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      fprintf(stderr,
              "ERROR::PROGRAM_LINKING_ERROR of type:\t%s\n%s\n"
              "-- --------------------------------------------------- -- \n",
              type, infoLog);
    }
  }
}

/**
 * Generates a shader program from a vertex and a fragment shader file.
 * Returns the shader program ID.
 */
unsigned int generateShader(const char *vertexShaderPath,
                            const char *fragmentShaderPath) {
  unsigned int vertex, fragment;

  char *vertexShaderSrc = read_file_to_string(vertexShaderPath);
  if (vertexShaderSrc == NULL) {
    return EXIT_FAILURE;
  }

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, (const char **)&vertexShaderSrc, NULL);
  glCompileShader(vertex);
  check_compile_errors(vertex, "VERTEX");
  free(vertexShaderSrc);

  char *fragmentShaderSrc = read_file_to_string(fragmentShaderPath);
  if (fragmentShaderSrc == NULL) {
    glDeleteShader(vertex);
    return EXIT_FAILURE;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, (const char **)&fragmentShaderSrc, NULL);
  glCompileShader(fragment);
  check_compile_errors(fragment, "FRAGMENT");
  free(fragmentShaderSrc);

  unsigned int shaderProgramId = glCreateProgram();
  glAttachShader(shaderProgramId, vertex);
  glAttachShader(shaderProgramId, fragment);
  glLinkProgram(shaderProgramId);
  check_compile_errors(shaderProgramId, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return shaderProgramId;
}
