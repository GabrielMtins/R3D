# R3D

## What is R3D?

R3D is a **header-only** library designed to draw 3d and 2d graphics using SDL2
and OpenGL. The library is designed to be a simple **stateless** abstraction
over OpenGL, allowing the user to create simple retro 3d games.

## License

R3D is **free software**, published under **GNU GPL v3** or later.

## Features

- R3D is a **stateless** library, the user doesn't need to handle OpenGL state.
  He can call `R3D_Begin()`, draw geometry with `R3D_PushTriangle()` and call
  `R3D_End()` to finish rendering.
- R3D also is **single header and header-only**. The user must define `#define
  R3D_IMPLEMENTATION` on a source file.
- R3D also uses **texture arrays** for triangles and quads, allowing to draw
  basic geometry with a single draw call.
- R3D also has basic abstractions for **2D and 3D sprites**. They use textures
  that can be loaded with `R3D_LoadTexture`.
- R3D has **skybox support** with 6 faces.
- R3D has **fog support**.

## Dependencies

- SDL2
- OpenGL
- GLAD (included by `#include GLAD_PATH`. Default **GLAD_PATH** is
  **"glad/glad.h"**, but the user of the library can define the actual
  **GLAD_PATH** before its implementation).

## Set-Up

You can set up the following macros for the library:

- **R3D_MAX_VERTICES**, default: 65536
- **R3D_MAX_TEXTURES**, default: 1024
- **R3D_MAX_SPRITES_2D**, default: 1024
- **R3D_MAX_SPRITES_3D**, default: 256
- **GLAD_PATH**, default: "glad/glad.h"
