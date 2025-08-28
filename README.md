# Come and C

`Come and C` is an educational project aiming to serve as playground to learn _C_ and _OpenGL_ on the go.

## Installation

To install the project, clone the repository in your preferred IDE.

> [!IMPORTANT]
> Remember to include required _headers_. The process might differ depending on the IDE in use.

### Requirements

- [Clang](https://clang.llvm.org)
- [OpenGL](https://www.opengl.org)
- [GLFW](https://www.glfw.org)
- [GLAD](https://glad.dav1d.de)

### Compiling and running

In order to `build` or `run` the application, a `Makefile` is provided in the project's root directory. It contains the tasks `all`, `build` and `run`.

> [!CAUTION]
> In order to compile and link required libraries, the paths have to be updated to comply to _your_ local environment.

```bash
cd come-and-c
make
```

## Features

- Create a window using _OpenGL_
- Graceful shutdown on `GLFW_PRESS` + `ESC` or `Q`
- Handle events regarding the window's size

## Roadmap

- Display current amount of frames per second (FPS)
- Add rendering for geometric shapes in a 2D space
- Transform geometric shapes in a 2D space

## Documentation

For now, all documentation lives in the code base. It's planned to document learned principles and key aspects of the language _C_ and _OpenGL_ separately in the future.

## Appendix

This project is not going to be maintained on a regular basis. The application is developed just for fun and learning purposes - call it a side-project if you want.
