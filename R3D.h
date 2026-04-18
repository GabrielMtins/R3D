#ifndef R3D_H
#define R3D_H

/*
 *	  This file is part of R3D.
 *	  R3D is free software: you can redistribute it and/or modify it
 *	  under the terms of the GNU General Public License as published by the
 *	  Free Software Foundation, either version 3 of the License, or (at
 *	  your option) any later version.
 *
 *	  R3D is distributed in the hope that it will be useful,
 *	  but WITHOUT ANY WARRANTY; without even the implied warranty
 *	  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *	  See the GNU General Public License for more details.
 *
 *	  You should have received a copy of the GNU General Public License
 *	  along with R3D. If not, see <https://www.gnu.org/licenses/>.
 *	  Copyright	Gabriel Martins (C) 2026
*/

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	float x, y, z;
} R3D_Vec3;

typedef struct {
	float x, y;
} R3D_Vec2;

typedef struct {
	float r, g, b;
} R3D_Color;

typedef struct {
	float x, y, w, h;
} R3D_Rect;

typedef struct {
	R3D_Vec3 pos;
	R3D_Color color;
	R3D_Vec2 uv;
	float tex_index;
} R3D_Vertex;

/**
 * Initialize the system.
 * Returns -1 at fail, 0 at success.
 */
int R3D_Init(SDL_Window *window);

/**
 * Initialize the texture array with a given width, height and number of
 * layers. Returns -1 at fail, 0 at success.
 */
int R3D_InitTextureArray(int width, int height, int layers);

/**
 * Uploads a texture to a layer of the texture array. The data must
 * be in the format RGBA32.
 * Returns -1 at fail, 0 at success.
 */
int R3D_UploadToTextureArray(int layer, const void *data);

/**
 * Ends the setup of the texture array, it must be called after uploading
 * all the textures to the texture array. It will generate data like
 * mipmapping and finish its setup.
 */
void R3D_EndSetupTextureArray(void);

/**
 * Load a texture to a given index. The data must be in the format
 * RGBA32, and must have the size in bytes of 4 * width * height.
 * Returns -1 at fail, 0 at success.
 */
int R3D_LoadTexture(int index, int width, int height, const void *data);

/** 
 * Delete a texture of a given index.
 * Returns -1 at fail, 0 at success.
 */
int R3D_DeleteTexture(int index);

/**
 * Returns the last set error message.
 */
const char * R3D_GetError(void);

/**
 * Set the viewport based on screen coordinates.
 */
void R3D_SetViewport(int x, int y, int w, int h);

/**
 * Set the clear (background) color.
 */
void R3D_SetClearColor(float r, float g, float b);

/**
 * Set the fog color.
 */
void R3D_SetFogColor(float r, float g, float b);

/**
 * Set the fog start and its end.
 * Objects at distance <= start have no fog.
 * Objects at distance >= end are fully fogged.
 */
void R3D_SetFogDistance(float start, float end);

void R3D_DisableFog(void);

/**
 * Enable skybox based on a texture with a given index.
 * It will try to render a skybox based on the 6 textures
 * that must be loaded before hand. They also must be adjacent in memory.
 * The textures must be in the following order:
 * Index 0: right
 * Index 1: left
 * Index 2: top
 * Index 3: bottom
 * Index 4: front
 * Index 5: back
 * Returns -1 at fail, 0 at success.
 */
int R3D_EnableSkybox(int skybox_index_start);

void R3D_DisableSkybox(void);

/**
 * Set the 2D orthographic projection space.
 * Typical usage: R3D_Set2DViewSpace(0, window_width, window_height, 0);
 */
void R3D_Set2DViewSpace(float left, float right, float bottom, float top);

void R3D_Set3DViewSpace(float fov_y, float aspect, float znear, float zfar);

void R3D_SetCameraView(float x, float y, float z, float yaw, float pitch);

/**
 * Pushes a triangle to a geometry buffer. All triangles pushed with R3D_PushTriangle
 * will use the default texture array, they cannot use individual textures. 
 * The points must be non NULL.
 * Returns -1 at fail, 0 at success.
 */
int R3D_PushTriangle(const R3D_Vertex *v1, const R3D_Vertex *v2, const R3D_Vertex *v3);

/**
 * Pushes a quad to a geometry buffer. All quads pushed with R3D_PushQuad
 * will use the default texture array, they cannot use individual textures. 
 * The points must be non NULL.
 * Returns -1 at fail, 0 at success.
 */
int R3D_PushQuad(const R3D_Vertex *v1, const R3D_Vertex *v2, const R3D_Vertex *v3, const R3D_Vertex *v4);

/**
 * Pushes a convex polygon to a geometry buffer. All polygons pushed with R3D_PushConvexPolygon
 * will use the default texture array, they cannot use individual textures. 
 * They must be in order (clockwise or counterclockwise).
 * The points must be non NULL.
 * Returns -1 at fail, 0 at success.
 */
int R3D_PushConvexPolygon(const R3D_Vertex *vertices, size_t num);

/**
 * Pushes a 2d sprite to a buffer. It will be rendered as a simple 2d sprite.
 * on the 2d view, on top of the 3d view.
 * If src is set to NULL, it will render the entire texture.
 * If dst is set to NULL, it will render the texture with its default size.
 * The texture used by the sprite must be loaded beforehand on the given
 * index with R3D_LoadTexture().
 * Returns -1 at fail, 0 at success.
 */
int R3D_PushSprite2D(int texture_index, const R3D_Rect *src, const R3D_Rect *dst, const R3D_Color *color);

/**
 * Pushes a 3d sprite to a buffer. It will be rendered as a simple 3d sprite
 * on the 2d view, with proper use of depth and transparency.
 * IMPORTANT: Sprites use alpha blending, NOT alpha testing.
 * For correct use of transparency, sprites must be drawn front-to-back.
 * If src is set to NULL, it will render the entire texture.
 * If color is set to NULL, its color will be white.
 * The texture used by the sprite must be loaded beforehand on the given
 * index with R3D_LoadTexture().
 * Rotation angle must be in radians.
 * Returns -1 at fail, 0 at success.
 */
int R3D_PushSprite3D(int texture_index, const R3D_Rect *src, const R3D_Vec3 *position, const R3D_Vec2 *size, const R3D_Color *color, float rotation);

/** 
 * Begins the render process: resets all the buffers and cleans
 * the screen.
 */
void R3D_Begin(void);

/**
 * Ends the render process: render all geometry, skybox, sprites et cetera.
 */
void R3D_End(void);

/**
 * Cleans up its inicialization.
 */
void R3D_Quit(void);

#ifdef __cplusplus
}
#endif

#ifdef R3D_IMPLEMENTATION

#ifndef R3D_MAX_VERTICES
	#define R3D_MAX_VERTICES 65536
#endif

#ifndef R3D_MAX_TEXTURES
	#define R3D_MAX_TEXTURES 1024
#endif

#ifndef R3D_MAX_SPRITES_2D
	#define R3D_MAX_SPRITES_2D 1024
#endif

#ifndef R3D_MAX_SPRITES_3D
	#define R3D_MAX_SPRITES_3D 256
#endif

#ifndef GLAD_PATH
	#define GLAD_PATH "glad/glad.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include GLAD_PATH

#define FOR_ERROR_CODE(DO) \
	DO(ERROR_NOT_ENOUGH_MEMORY, "Couldn't allocate not enough memory.") \
	DO(ERROR_SHADER_CREATE, "Failed to create shaders.") \
	DO(ERROR_POINTER_NULL, "Pointer is NULL.") \
	DO(ERROR_GLCONTEXT_CREATE, "Coudn't create GL Context.") \
	DO(ERROR_GLAD_START, "GLAD couldn't start.") \
	DO(ERROR_COMPILE_VERTEX_SHADER, "Couldn't compile Vertex Shader. Use Debug mode for more information.") \
	DO(ERROR_COMPILE_FRAGMENT_SHADER, "Couldn't compile Fragment Shader. Use Debug mode for more information.") \
	DO(ERROR_LINK_SHADER, "Couldn't compile link Shader Program. Use Debug mode for more information.") \
	DO(ERROR_INVALID_TEXTURE_INDEX, "Texture index is out of bounds.") \
	DO(ERROR_TEXTURE_NOT_FOUND, "Texture index is not loaded.") \
	DO(ERROR_SPRITE_2D_LIMIT, "Reached the maximum capacity of 2d sprites.") \
	DO(ERROR_SPRITE_3D_LIMIT, "Reached the maximum capacity of 3d sprites.") \
	DO(ERROR_TEXTURE_ALREADY_LOADED, "Texture index already has a texture.") \
	DO(ERROR_TEXTURE_NOT_LOADED, "Texture index does not have a texture.") \
	DO(ERROR_NOT_ENOUGH_POINTS, "Polygon must have at least three points.") \
	DO(ERROR_SKYBOX_TEXTURE_MISSING, "One or more skybox textures not loaded.")

typedef struct {
	unsigned int id;

	int width, height;
} R3D_Texture;

typedef struct {
	int first, count;
	int id;
} R3D_TextureBatch;

typedef struct {
	int texture_index;
	R3D_Rect src, dst;
	R3D_Color color;
} R3D_Sprite2D;

typedef struct {
	int texture_index;

	R3D_Rect src;
	R3D_Color color;

	R3D_Vec3 position;
	R3D_Vec2 size;

	float rotation;
} R3D_Sprite3D;

typedef struct {
	float r, g, b;
	float start, end;
} R3D_FogState;

typedef struct {
	SDL_Window *window;
	SDL_GLContext context;
	R3D_Vertex vertices[R3D_MAX_VERTICES];
	int vertex_count;

	R3D_Texture textures[R3D_MAX_TEXTURES];
	R3D_Sprite2D sprites_2D[R3D_MAX_SPRITES_2D];
	int sprites_2D_count;

	R3D_Sprite3D sprites_3D[R3D_MAX_SPRITES_3D];
	int sprites_3D_count;

	int skybox_texture_index;

	struct {
		float r, g, b;
	} clear_color;

	int render_mode;

	/* OpenGL ids */
	unsigned int vao, vbo;
	unsigned int world_shader;
	unsigned int sprite_shader;

	struct {
		unsigned int id;
		int width, height;
		int num_layers;
	} texture_array;

	float perspective[16];
	float orthographic[16];
	float view[16];
	float model[16];
	float identity[16];

	struct {
		int model;
		int view;
		int projection;
		int texture;
		int fog_start;
		int fog_end;
		int fog_color;
	} world_uniform, sprite_uniform;

	R3D_FogState fog;
} R3D_Context;

static int error_code;
static R3D_Context *ctx;

static const char *vertex_shader_src = "#version 330 core\n"
	"layout (location = 0) in vec3 a_position;\n"
	"layout (location = 1) in vec3 a_color;\n"
	"layout (location = 2) in vec2 a_uv;\n"
	"layout (location = 3) in float a_tex_index;\n"
	"uniform mat4 u_model;\n"
	"uniform mat4 u_view;\n"
	"uniform mat4 u_projection;\n"
	"out vec3 v_color;\n"
	"out vec2 v_uv;\n"
	"out float v_tex_index;\n"
	"out vec3 v_transformed;\n"
	"void main()\n"
	"{\n"
	"   mat4 mvp = u_projection * u_view * u_model;\n"
	"   v_transformed = vec3(u_view * u_model * vec4(a_position, 1.0f));"
	"   gl_Position = mvp * vec4(a_position, 1.0f);\n"
	"   v_color = a_color;\n"
	"   v_uv = a_uv;\n"
	"   v_tex_index = a_tex_index;\n"
	"}\0";

// Código fonte do Fragment Shader (calcula a cor dos pixels)
static const char *fragment_shader_world_src = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 v_color;\n"
	"in vec2 v_uv;\n"
	"in float v_tex_index;\n"
	"in vec3 v_transformed;\n"
	"uniform sampler2DArray u_texture;\n"
	"uniform float u_fog_start;\n"
	"uniform float u_fog_end;\n"
	"uniform vec3 u_fog_color;\n"
	"void main()\n"
	"{\n"
	"   float distance = (length(v_transformed) - u_fog_start) / (u_fog_end - u_fog_start);\n"
	"   distance = clamp(distance, 0.0, 1.0);\n"
	"   FragColor = texture(u_texture, vec3(v_uv, v_tex_index)) * vec4(v_color, 1.0);\n"
	"   FragColor.xyz = mix(FragColor.xyz, u_fog_color, distance);"
	"}\n\0";

static const char *fragment_shader_spr_src = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 v_color;\n"
	"in vec2 v_uv;\n"
	"in vec3 v_transformed;\n"
	"uniform sampler2D u_texture;\n"
	"uniform float u_fog_start;\n"
	"uniform float u_fog_end;\n"
	"uniform vec3 u_fog_color;\n"
	"void main()\n"
	"{\n"
	"   float distance = (length(v_transformed) - u_fog_start) / (u_fog_end - u_fog_start);\n"
	"   distance = clamp(distance, 0.0, 1.0);\n"
	"   FragColor = texture(u_texture, v_uv) * vec4(v_color, 1.0f);\n"
	"   FragColor.xyz = mix(FragColor.xyz, u_fog_color, distance);"
	"   \n"
	"}\n\0";

#define EXPAND_AS_ENUM(code, msg) code,

#define PI 3.141592f

enum ErrorTypes {
	FOR_ERROR_CODE(EXPAND_AS_ENUM)
	NUM_ERRORS
};

enum RenderMode {
	RENDERMODE_SKYBOX,
	RENDERMODE_ARRAY_GEOMETRY,
	RENDERMODE_2D_GEOMETRY,
	RENDERMODE_TEX_GEOMETRY,
};

#undef EXPAND_AS_ENUM

#define EXPAND_AS_MSGS(code, msg) msg,

static const char *error_msgs[] = {
	FOR_ERROR_CODE(EXPAND_AS_MSGS)
};

#undef EXPAND_AS_MSGS

static int R3D_CreateBuffers(void);
static int R3D_CompileShaders(void);
static void R3D_MatrixIdentity(float *m);
static void R3D_MatrixOrtho(float left, float right, float bottom, float top, float znear, float zfar);
static void R3D_MatrixPerspective(float fov_y, float aspect, float znear, float zfar);
static void R3D_MatrixViewFPS(float x, float y, float z, float pitch, float yaw);
static void R3D_Sprites2DToVertices(void);
static void R3D_Sprites3DToVertices(void);
static void R3D_SkyboxToVertices(void);
static void R3D_PrepareSkyboxFlush(void);
static void R3D_CopyVerticesToGpu(void);
static void R3D_Flush2DGeometry(void);
static void R3D_FlushTexGeometry(void);
static void R3D_FlushArrayGeometry(void);
static void R3D_Flush(void);
static void R3D_ApplyFog(R3D_FogState *fog);

static int R3D_CreateBuffers(void) {
	glGenVertexArrays(1, &ctx->vao);
	glGenBuffers(1, &ctx->vbo);

	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);

	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(R3D_Vertex) * R3D_MAX_VERTICES,
			NULL,
			GL_STREAM_DRAW
			);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(R3D_Vertex), (void*) offsetof(R3D_Vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(R3D_Vertex), (void*) offsetof(R3D_Vertex, color));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(R3D_Vertex), (void*) offsetof(R3D_Vertex, uv));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(R3D_Vertex), (void*) offsetof(R3D_Vertex, tex_index));

	glBindVertexArray(0);

	return 0;
}

static int R3D_CompileShaders(void) {
	unsigned int vs, w_fs, s_fs;
	int success;

	#ifdef R3D_DEBUG
	char info_log[512];
	#endif

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

	if(!success) {
		#ifdef R3D_DEBUG
		glGetShaderInfoLog(vs, 512, NULL, info_log);
		printf("Error VS: %s\n", info_log);
		#endif

		error_code = ERROR_COMPILE_VERTEX_SHADER;
		return -1;
	}

	w_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(w_fs, 1, &fragment_shader_world_src, NULL);
	glCompileShader(w_fs);

	glGetShaderiv(w_fs, GL_COMPILE_STATUS, &success);

	if(!success) {
		#ifdef R3D_DEBUG
		glGetShaderInfoLog(w_fs, 512, NULL, info_log);
		printf("Error FS: %s\n", info_log);
		#endif

		error_code = ERROR_COMPILE_FRAGMENT_SHADER;
		return -1;
	}

	s_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(s_fs, 1, &fragment_shader_spr_src, NULL);
	glCompileShader(s_fs);

	glGetShaderiv(s_fs, GL_COMPILE_STATUS, &success);

	if(!success) {
		#ifdef R3D_DEBUG
		glGetShaderInfoLog(s_fs, 512, NULL, info_log);
		printf("Error FS: %s\n", info_log);
		#endif

		error_code = ERROR_COMPILE_FRAGMENT_SHADER;
		return -1;
	}

	ctx->world_shader = glCreateProgram();

	glAttachShader(ctx->world_shader, vs);
	glAttachShader(ctx->world_shader, w_fs);
	glLinkProgram(ctx->world_shader);

	glGetProgramiv(ctx->world_shader, GL_LINK_STATUS, &success);

	if(!success) {
		#ifdef R3D_DEBUG
		glGetProgramInfoLog(ctx->world_shader, 512, NULL, info_log);
		printf("Error Linking: %s\n", info_log);
		#endif

		error_code = ERROR_LINK_SHADER;
		return -1;
	}

	ctx->sprite_shader = glCreateProgram();

	glAttachShader(ctx->sprite_shader, vs);
	glAttachShader(ctx->sprite_shader, s_fs);
	glLinkProgram(ctx->sprite_shader);

	glGetProgramiv(ctx->sprite_shader, GL_LINK_STATUS, &success);

	if(!success) {
		#ifdef R3D_DEBUG
		glGetProgramInfoLog(ctx->sprite_shader, 512, NULL, info_log);
		printf("Error Linking: %s\n", info_log);
		#endif

		error_code = ERROR_LINK_SHADER;
		return -1;
	}
	
	glDeleteShader(vs);
	glDeleteShader(w_fs);
	glDeleteShader(s_fs);

	ctx->world_uniform.model = glGetUniformLocation(ctx->world_shader, "u_model");
	ctx->world_uniform.view = glGetUniformLocation(ctx->world_shader, "u_view");
	ctx->world_uniform.projection = glGetUniformLocation(ctx->world_shader, "u_projection");
	ctx->world_uniform.texture = glGetUniformLocation(ctx->world_shader, "u_texture");
	ctx->world_uniform.fog_start = glGetUniformLocation(ctx->world_shader, "u_fog_start");
	ctx->world_uniform.fog_end = glGetUniformLocation(ctx->world_shader, "u_fog_end");
	ctx->world_uniform.fog_color = glGetUniformLocation(ctx->world_shader, "u_fog_color");

	ctx->sprite_uniform.model = glGetUniformLocation(ctx->sprite_shader, "u_model");
	ctx->sprite_uniform.view = glGetUniformLocation(ctx->sprite_shader, "u_view");
	ctx->sprite_uniform.projection = glGetUniformLocation(ctx->sprite_shader, "u_projection");
	ctx->sprite_uniform.texture = glGetUniformLocation(ctx->sprite_shader, "u_texture");
	ctx->sprite_uniform.fog_start = glGetUniformLocation(ctx->sprite_shader, "u_fog_start");
	ctx->sprite_uniform.fog_end = glGetUniformLocation(ctx->sprite_shader, "u_fog_end");
	ctx->sprite_uniform.fog_color = glGetUniformLocation(ctx->sprite_shader, "u_fog_color");

	return 0;
}

static void R3D_MatrixIdentity(float *m) {
	memset(m, 0, sizeof(float) * 16);

	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10] = 1.0f;
	m[15] = 1.0f;
}

static void R3D_MatrixOrtho(float left, float right, float bottom, float top, float znear, float zfar) {
	memset(ctx->orthographic, 0, sizeof(float) * 16);

	ctx->orthographic[0] = 2.0f / (right - left);
	ctx->orthographic[5] = 2.0f / (top - bottom);
	ctx->orthographic[10] = -2.0f / (zfar - znear);

	ctx->orthographic[12] = -(right + left) / (right - left);
	ctx->orthographic[13] = -(top + bottom) / (top - bottom);
	ctx->orthographic[14] = -(zfar + znear) / (zfar - znear);
	ctx->orthographic[15] = 1.0f;
}

static void R3D_MatrixPerspective(float fov_y, float aspect, float znear, float zfar) {
	memset(ctx->perspective, 0, sizeof(float) * 16);

	float tan_half_fovy = tanf(fov_y / 2.0f);

	ctx->perspective[0] = 1.0f / (aspect * tan_half_fovy);
	
	ctx->perspective[5] = 1.0f / (tan_half_fovy);
	
	ctx->perspective[10] = -(zfar + znear) / (zfar - znear);
	ctx->perspective[11] = -1.0f; 
	
	ctx->perspective[14] = -(2.0f * zfar * znear) / (zfar - znear);
}

static void R3D_MatrixViewFPS(float x, float y, float z, float pitch, float yaw) {
	memset(ctx->view, 0, sizeof(float) * 16);
	ctx->view[0] = 1.0f; ctx->view[5] = 1.0f; ctx->view[10] = 1.0f; ctx->view[15] = 1.0f;

	float cos_p = cosf(pitch);
	float sin_p = sinf(pitch);
	
	ctx->view[5]  = cos_p;
	ctx->view[6]  = sin_p;
	ctx->view[9]  = -sin_p;
	ctx->view[10] = cos_p;

	float cos_y = cosf(yaw);
	float sin_y = sinf(yaw);

	float m0 = ctx->view[0], m1 = ctx->view[1], m2 = ctx->view[2];
	float m8 = ctx->view[8], m9 = ctx->view[9], m10 = ctx->view[10];

	ctx->view[0] = cos_y * m0 + sin_y * m8;
	ctx->view[1] = cos_y * m1 + sin_y * m9;
	ctx->view[2] = cos_y * m2 + sin_y * m10;

	ctx->view[8] = -sin_y * m0 + cos_y * m8;
	ctx->view[9] = -sin_y * m1 + cos_y * m9;
	ctx->view[10] = -sin_y * m2 + cos_y * m10;

	ctx->view[12] = -ctx->view[0] * x - ctx->view[4] * y - ctx->view[8] * z;
	ctx->view[13] = -ctx->view[1] * x - ctx->view[5] * y - ctx->view[9] * z;
	ctx->view[14] = -ctx->view[2] * x - ctx->view[6] * y - ctx->view[10] * z;
}

static void R3D_Sprites2DToVertices(void) {
	R3D_Vertex vertices[4];
	R3D_Sprite2D *sprite;

	for(int i = 0; i < ctx->sprites_2D_count; i++) {
		sprite = &ctx->sprites_2D[i];

		vertices[0] = (R3D_Vertex) {
			.pos = {sprite->dst.x, sprite->dst.y, 0.0f},
			.uv = {sprite->src.x, sprite->src.y},
			.color = sprite->color,
			.tex_index = sprite->texture_index,
		};

		vertices[1] = (R3D_Vertex) {
			.pos = {sprite->dst.x + sprite->dst.w, sprite->dst.y, 0.0f},
			.uv = {sprite->src.x + sprite->src.w, sprite->src.y},
			.color = sprite->color,
			.tex_index = sprite->texture_index
		};

		vertices[2] = (R3D_Vertex) {
			.pos = {sprite->dst.x + sprite->dst.w, sprite->dst.y + sprite->dst.h, 0.0f},
			.uv = {sprite->src.x + sprite->src.w, sprite->src.y + sprite->src.h},
			.color = sprite->color,
			.tex_index = sprite->texture_index
		};

		vertices[3] = (R3D_Vertex) {
			.pos = {sprite->dst.x, sprite->dst.y + sprite->dst.h, 0.0f},
			.uv = {sprite->src.x, sprite->src.y + sprite->src.h},
			.color = sprite->color,
			.tex_index = sprite->texture_index
		};

		R3D_PushQuad(vertices + 0, vertices + 1, vertices + 2, vertices + 3);
	}

	ctx->sprites_2D_count = 0;
}

static void R3D_Sprites3DToVertices(void) {
	R3D_Vertex vertices[4];
	R3D_Sprite3D *sprite;
	float cos_ang, sin_ang;

	for(int i = 0; i < ctx->sprites_3D_count; i++) {
		sprite = &ctx->sprites_3D[i];
		
		cos_ang = cosf(sprite->rotation);
		sin_ang = sinf(sprite->rotation);

		vertices[0] = (R3D_Vertex) {
			.pos = {-0.5f, 0.0f, 0.0f},
			.uv = {sprite->src.x, sprite->src.y + sprite->src.h},
			.color = sprite->color,
			.tex_index = sprite->texture_index,
		};

		vertices[1] = (R3D_Vertex) {
			.pos = {0.5f, 0.0f, 0.0f},
			.uv = {sprite->src.x + sprite->src.w, sprite->src.y + sprite->src.h},
			.color = sprite->color,
			.tex_index = sprite->texture_index
		};

		vertices[2] = (R3D_Vertex) {
			.pos = {0.5f, 1.0f, 0.0f},
			.uv = {sprite->src.x + sprite->src.w, sprite->src.y},
			.color = sprite->color,
			.tex_index = sprite->texture_index
		};

		vertices[3] = (R3D_Vertex) {
			.pos = {-0.5f, 1.0f, 0.0f},
			.uv = {sprite->src.x, sprite->src.y},
			.color = sprite->color,
			.tex_index = sprite->texture_index
		};

		for(int j = 0; j < 4; j++) {
			float new_x, new_z;

			vertices[j].pos.x *= sprite->size.x;
			vertices[j].pos.y *= sprite->size.y;

			new_x = vertices[j].pos.x * cos_ang - vertices[j].pos.z * sin_ang;
			new_z = vertices[j].pos.x * sin_ang + vertices[j].pos.z * cos_ang;

			vertices[j].pos.x = new_x;
			vertices[j].pos.z = new_z;

			vertices[j].pos.x += sprite->position.x;
			vertices[j].pos.y += sprite->position.y;
			vertices[j].pos.z += sprite->position.z;

		}

		R3D_PushQuad(vertices + 0, vertices + 1, vertices + 2, vertices + 3);
	}

	ctx->sprites_3D_count = 0;
}

static void R3D_SkyboxToVertices(void) {
	static const R3D_Vertex skybox_default_vertices[24] = {
		// right
		{
			.pos = {+1.0f, -1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f},
			.tex_index = 0.0f,
		},
		{
			.pos = {+1.0f, +1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f},
			.tex_index = 0.0f,
		},
		{
			.pos = {+1.0f, +1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f},
			.tex_index = 0.0f,
		},
		{
			.pos = {+1.0f, -1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
			.tex_index = 0.0f,
		},

		// left
		{
			.pos = {-1.0f, -1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
			.tex_index = 1.0f,
		},
		{
			.pos = {-1.0f, +1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f},
			.tex_index = 1.0f,
		},
		{
			.pos = {-1.0f, +1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f},
			.tex_index = 1.0f,
		},
		{
			.pos = {-1.0f, -1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f},
			.tex_index = 1.0f,
		},

		// top
		{
			.pos = {-1.0f, +1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f},
			.tex_index = 2.0f,
		},
		{
			.pos = {+1.0f, +1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
			.tex_index = 2.0f,
		},
		{
			.pos = {+1.0f, +1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f},
			.tex_index = 2.0f,
		},
		{
			.pos = {-1.0f, +1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f},
			.tex_index = 2.0f,
		},

		// bottom
		{
			.pos = {-1.0f, -1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f},
			.tex_index = 3.0f,
		},
		{
			.pos = {+1.0f, -1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f},
			.tex_index = 3.0f,
		},
		{
			.pos = {+1.0f, -1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
			.tex_index = 3.0f,
		},
		{
			.pos = {-1.0f, -1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f},
			.tex_index = 3.0f,
		},

		// front
		{
			.pos = {-1.0f, -1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f},
			.tex_index = 4.0f,
		},
		{
			.pos = {+1.0f, -1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
			.tex_index = 4.0f,
		},
		{
			.pos = {+1.0f, +1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f},
			.tex_index = 4.0f,
		},
		{
			.pos = {-1.0f, +1.0f, -1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f},
			.tex_index = 4.0f,
		},

		// back
		{
			.pos = {-1.0f, -1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f},
			.tex_index = 5.0f,
		},
		{
			.pos = {+1.0f, -1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f},
			.tex_index = 5.0f,
		},
		{
			.pos = {+1.0f, +1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f},
			.tex_index = 5.0f,
		},
		{
			.pos = {-1.0f, +1.0f, +1.0f},
			.color = {1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f},
			.tex_index = 5.0f,
		},
	};

	R3D_Vertex vertices[24];

	memcpy(vertices, skybox_default_vertices, sizeof(R3D_Vertex) * 24);

	for(int i = 0; i < 24; i++) {
		vertices[i].tex_index += ctx->skybox_texture_index;
		vertices[i].pos.x *= 5.0f;
		vertices[i].pos.y *= 5.0f;
		vertices[i].pos.z *= 5.0f;

		vertices[i].color = (R3D_Color) {1.0f, 1.0f, 1.0f};
	}

	for(int i = 0; i < 6; i++) {
		R3D_PushQuad(
				vertices + i * 4 + 0,
				vertices + i * 4 + 1,
				vertices + i * 4 + 2,
				vertices + i * 4 + 3
				);
	}
}

static void R3D_PrepareSkyboxFlush(void) {
	R3D_FogState fog = ctx->fog;
	float old_view[16];

	memcpy(old_view, ctx->view, sizeof(float) * 16);

	ctx->view[12] = 0.0f;
	ctx->view[13] = 0.0f;
	ctx->view[14] = 0.0f;

	fog.end = -1.0f;
	fog.start = 1.0f;

	ctx->render_mode = RENDERMODE_SKYBOX;

	R3D_ApplyFog(&fog);
	R3D_SkyboxToVertices();

	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
	R3D_Flush();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	memcpy(ctx->view, old_view, sizeof(float) * 16);
}

static void R3D_CopyVerticesToGpu(void) {
	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);

	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(R3D_Vertex) * R3D_MAX_VERTICES,
			NULL,
			GL_STREAM_DRAW
			);

	glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			sizeof(R3D_Vertex) * ctx->vertex_count,
			ctx->vertices
			);
}

static void R3D_Flush2DGeometry(void) {
	int start, count, last_texture_id, current_texture_id;
	R3D_FogState fog;

	if(ctx->vertex_count == 0) {
		return;
	}

	fog = ctx->fog;
	fog.start = 1;
	fog.end = -1;

	R3D_ApplyFog(&fog);

	glDisable(GL_DEPTH_TEST);

	R3D_CopyVerticesToGpu();

	glUseProgram(ctx->sprite_shader);

	glUniformMatrix4fv(
			ctx->sprite_uniform.model,
			1,
			GL_FALSE,
			ctx->identity
			);

	glUniformMatrix4fv(
			ctx->sprite_uniform.view,
			1,
			GL_FALSE,
			ctx->identity
			);

	glUniformMatrix4fv(
			ctx->sprite_uniform.projection,
			1,
			GL_FALSE,
			ctx->orthographic
			);

	start = 0;
	count = 0;
	last_texture_id = 0;

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(ctx->sprite_uniform.texture, 0);

	for(int i = 0; i < ctx->vertex_count; i += 3) {
		current_texture_id = (int) (ctx->vertices[i].tex_index);


		if(i == 0) {
			start = 0;
			last_texture_id = current_texture_id;
			count += 3;
			continue;
		}

		if(last_texture_id != current_texture_id) {
			glBindTexture(GL_TEXTURE_2D, ctx->textures[last_texture_id].id);
			glDrawArrays(GL_TRIANGLES, start, count);

			start = count;
			last_texture_id = current_texture_id;
		}

		count += 3;
	}

	glBindTexture(GL_TEXTURE_2D, ctx->textures[last_texture_id].id);
	glDrawArrays(GL_TRIANGLES, start, count);

	ctx->vertex_count = 0;
	glEnable(GL_DEPTH_TEST);
}

static void R3D_FlushTexGeometry(void) {
	int start, count, last_texture_id, current_texture_id;

	if(ctx->vertex_count == 0) {
		return;
	}

	R3D_CopyVerticesToGpu();

	glUseProgram(ctx->sprite_shader);

	glUniformMatrix4fv(
			ctx->sprite_uniform.model,
			1,
			GL_FALSE,
			ctx->identity
			);

	glUniformMatrix4fv(
			ctx->sprite_uniform.view,
			1,
			GL_FALSE,
			ctx->view
			);

	glUniformMatrix4fv(
			ctx->sprite_uniform.projection,
			1,
			GL_FALSE,
			ctx->perspective
			);

	start = 0;
	count = 0;
	last_texture_id = 0;

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(ctx->sprite_uniform.texture, 0);

	for(int i = 0; i < ctx->vertex_count; i += 3) {
		current_texture_id = (int) (ctx->vertices[i].tex_index);

		if(i == 0) {
			start = 0;
			last_texture_id = current_texture_id;
			count += 3;
			continue;
		}

		if(last_texture_id != current_texture_id) {
			glBindTexture(GL_TEXTURE_2D, ctx->textures[last_texture_id].id);
			glDrawArrays(GL_TRIANGLES, start, count);

			start = count;
			last_texture_id = current_texture_id;
		}

		count += 3;
	}

	glBindTexture(GL_TEXTURE_2D, ctx->textures[last_texture_id].id);
	glDrawArrays(GL_TRIANGLES, start, count);

	ctx->vertex_count = 0;
}

static void R3D_FlushArrayGeometry(void) {
	if(ctx->vertex_count == 0) {
		return;
	}

	R3D_ApplyFog(&ctx->fog);

	R3D_CopyVerticesToGpu();

	glUseProgram(ctx->world_shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, ctx->texture_array.id);
	glUniform1i(ctx->world_uniform.texture, 0);
	
	glUniformMatrix4fv(
			ctx->world_uniform.model,
			1,
			GL_FALSE,
			ctx->model
			);

	glUniformMatrix4fv(
			ctx->world_uniform.view,
			1,
			GL_FALSE,
			ctx->view
			);

	glUniformMatrix4fv(
			ctx->world_uniform.projection,
			1,
			GL_FALSE,
			ctx->perspective
			);

	glDrawArrays(
			GL_TRIANGLES,
			0,
			ctx->vertex_count
			);

	ctx->vertex_count = 0;
}

static void R3D_Flush(void) {
	switch(ctx->render_mode) {
		case RENDERMODE_ARRAY_GEOMETRY:
			R3D_FlushArrayGeometry();
			break;

		case RENDERMODE_2D_GEOMETRY:
			R3D_Flush2DGeometry();
			break;

		case RENDERMODE_SKYBOX:
			R3D_FlushTexGeometry();
			break;

		case RENDERMODE_TEX_GEOMETRY:
			R3D_ApplyFog(&ctx->fog);
			glDepthMask(GL_FALSE);
			R3D_FlushTexGeometry();
			glDepthMask(GL_TRUE);
			break;
	}
}

static void R3D_ApplyFog(R3D_FogState *fog) {
	glUseProgram(ctx->world_shader);

	glUniform3f(ctx->world_uniform.fog_color, fog->r, fog->g, fog->b);
	glUniform1f(ctx->world_uniform.fog_start, fog->start);
	glUniform1f(ctx->world_uniform.fog_end, fog->end);

	glUseProgram(ctx->sprite_shader);

	glUniform3f(ctx->sprite_uniform.fog_color, fog->r, fog->g, fog->b);
	glUniform1f(ctx->sprite_uniform.fog_start, fog->start);
	glUniform1f(ctx->sprite_uniform.fog_end, fog->end);
}

int R3D_Init(SDL_Window *window) {
	ctx = (R3D_Context *) malloc(sizeof(R3D_Context));

	if(ctx == NULL) {
		error_code = ERROR_NOT_ENOUGH_MEMORY;
		return -1;
	}

	#ifdef R3D_DEBUG
	printf("R3D_Context size: %lu KB\n", sizeof(R3D_Context) / 1024);
	#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	*ctx = (R3D_Context) {
		.window = window,
		.clear_color = {0.1f, 0.2f, 0.3f},
		.skybox_texture_index = -1
	};

	memset(ctx->textures, 0, sizeof(R3D_Texture) * R3D_MAX_TEXTURES);

	R3D_MatrixIdentity(ctx->identity);
	R3D_MatrixIdentity(ctx->model);
	R3D_MatrixIdentity(ctx->view);

	R3D_MatrixViewFPS(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	ctx->context = SDL_GL_CreateContext(window);

	if(ctx->context == NULL) {
		error_code = ERROR_GLCONTEXT_CREATE;
		return -1;
	}

	if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
		error_code = ERROR_GLAD_START;
		return -1;
	}

	R3D_CreateBuffers();

	if(R3D_CompileShaders() < 0) {
		error_code = ERROR_SHADER_CREATE;
		return -1;
	}

	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0, 800, 600);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

int R3D_InitTextureArray(int width, int height, int layers) {
	glGenTextures(1, &ctx->texture_array.id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, ctx->texture_array.id);

	ctx->texture_array.width = width;
	ctx->texture_array.height = height;
	ctx->texture_array.num_layers = layers;

	glTexImage3D(
			GL_TEXTURE_2D_ARRAY,
			0,
			GL_RGBA8,
			width,
			height,
			layers,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			NULL
			);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return 0;
}

int R3D_UploadToTextureArray(int layer, const void *data) {
	if(layer < 0 || layer >= ctx->texture_array.num_layers) {
		error_code = ERROR_INVALID_TEXTURE_INDEX;

		return -1;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, ctx->texture_array.id);

	glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY,
			0, 0, 0,
			layer,
			ctx->texture_array.width, ctx->texture_array.height,
			1, GL_RGBA, GL_UNSIGNED_BYTE,
			data
			);

	return 0;
}

void R3D_EndSetupTextureArray(void) {
	glBindTexture(GL_TEXTURE_2D_ARRAY, ctx->texture_array.id);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

int R3D_LoadTexture(int index, int width, int height, const void *data) {
	R3D_Texture *texture;

	if(index < 0 || index >= R3D_MAX_TEXTURES) {
		error_code = ERROR_INVALID_TEXTURE_INDEX;
		return -1;
	}

	texture = ctx->textures + index;

	if(texture->id != 0) {
		error_code = ERROR_TEXTURE_ALREADY_LOADED;

		return -1;
	}

	texture->width = width;
	texture->height = height;

	glGenTextures(1, &texture->id);

	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data
			);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

int R3D_DeleteTexture(int index) {
	R3D_Texture *texture;

	if(index < 0 || index >= R3D_MAX_TEXTURES) {
		error_code = ERROR_INVALID_TEXTURE_INDEX;

		return -1;
	}

	texture = &ctx->textures[index];

	if(texture->id == 0) {
		error_code = ERROR_TEXTURE_NOT_LOADED;

		return -1;
	}

	glDeleteTextures(1, &texture->id);

	return 0;
}


const char * R3D_GetError(void) {
	if(error_code >= NUM_ERRORS || error_code < 0) {
		return "INVALID ERROR";
	}

	return error_msgs[error_code];
}

void R3D_SetViewport(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
}

void R3D_SetClearColor(float r, float g, float b) {
	ctx->clear_color.r = r;
	ctx->clear_color.g = g;
	ctx->clear_color.b = b;
}

void R3D_SetFogColor(float r, float g, float b) {
	ctx->fog.r = r;
	ctx->fog.g = g;
	ctx->fog.b = b;
}

void R3D_SetFogDistance(float start, float end) {
	ctx->fog.start = start;
	ctx->fog.end = end;
}

void R3D_DisableFog(void) {
	R3D_SetFogDistance(1.0f, -1.0f);
}

int R3D_EnableSkybox(int skybox_index_start) {
	if(skybox_index_start + 6 >= R3D_MAX_TEXTURES) {
		error_code = ERROR_SKYBOX_TEXTURE_MISSING;
		return -1;
	}

	for(int i = 0; i < 6; i++) {
		if(ctx->textures[skybox_index_start + i].id == 0) {
			error_code = ERROR_SKYBOX_TEXTURE_MISSING;
			return -1;
		}
	}

	ctx->skybox_texture_index = skybox_index_start;

	return 0;
}

void R3D_DisableSkybox(void) {
	ctx->skybox_texture_index = -1;
}

void R3D_Set2DViewSpace(float left, float right, float bottom, float top) {
	R3D_MatrixOrtho(left, right, bottom, top, -1.0f, 1.0f);
}

void R3D_Set3DViewSpace(float fov_y, float aspect, float znear, float zfar) {
	R3D_MatrixPerspective(fov_y, aspect, znear, zfar);
}

void R3D_SetCameraView(float x, float y, float z, float yaw, float pitch) {
	R3D_MatrixViewFPS(x, y, z, yaw, pitch);
}

int R3D_PushTriangle(const R3D_Vertex *v1, const R3D_Vertex *v2, const R3D_Vertex *v3) {
	if(!(v1 && v2 && v3)) {
		error_code = ERROR_POINTER_NULL;
		return -1;
	}

	if(ctx->vertex_count + 3 > R3D_MAX_VERTICES) {
		R3D_Flush();
	}

	ctx->vertices[ctx->vertex_count++] = *v1;
	ctx->vertices[ctx->vertex_count++] = *v2;
	ctx->vertices[ctx->vertex_count++] = *v3;

	return 0;
}

int R3D_PushQuad(const R3D_Vertex *v1, const R3D_Vertex *v2, const R3D_Vertex *v3, const R3D_Vertex *v4) {
	if(!(v1 && v2 && v3 && v4)) {
		error_code = ERROR_POINTER_NULL;
		return -1;
	}

	R3D_PushTriangle(v1, v2, v3);
	R3D_PushTriangle(v3, v4, v1);

	return 0;
}

int R3D_PushConvexPolygon(const R3D_Vertex *vertices, size_t num) {
	if(num < 3) {
		error_code = ERROR_NOT_ENOUGH_POINTS;
		return -1;
	}

	for(size_t i = 0; i < num - 2; i++) {
		R3D_PushTriangle(vertices + 0, vertices + i + 1, vertices + i + 2);
	}

	return 0;
}

int R3D_PushSprite2D(int texture_index, const R3D_Rect *src, const R3D_Rect *dst, const R3D_Color *color) {
	R3D_Sprite2D *sprite;
	R3D_Texture *texture;

	if(texture_index < 0 || texture_index >= R3D_MAX_TEXTURES) {
		error_code = ERROR_INVALID_TEXTURE_INDEX;
		return -1;
	}

	texture = &ctx->textures[texture_index];

	if(!texture->id) {
		error_code = ERROR_TEXTURE_NOT_FOUND;
		return -1;
	}

	if(ctx->sprites_2D_count == R3D_MAX_SPRITES_2D) {
		error_code = ERROR_SPRITE_2D_LIMIT;
		return -1;
	}

	sprite = &ctx->sprites_2D[ctx->sprites_2D_count++];

	*sprite = (R3D_Sprite2D) {
		.texture_index = texture_index,
		.src = {0, 0, texture->width, texture->height},
		.dst = {0, 0, texture->width, texture->height},
		.color = {1.0f, 1.0f, 1.0f}
	};

	if(src != NULL) {
		sprite->src = *src;
	}

	sprite->src.x /= texture->width;
	sprite->src.w /= texture->width;
	sprite->src.y /= texture->height;
	sprite->src.h /= texture->height;

	if(dst != NULL) {
		sprite->dst = *dst;
	}

	if(color != NULL) {
		sprite->color = *color;
	}

	return 0;
}

int R3D_PushSprite3D(int texture_index, const R3D_Rect *src, const R3D_Vec3 *position, const R3D_Vec2 *size, const R3D_Color *color, float rotation) {
	R3D_Sprite3D *sprite;
	R3D_Texture *texture;

	if(texture_index < 0 || texture_index >= R3D_MAX_TEXTURES) {
		error_code = ERROR_INVALID_TEXTURE_INDEX;
		return -1;
	}

	texture = &ctx->textures[texture_index];

	if(texture->id == 0) {
		error_code = ERROR_TEXTURE_NOT_FOUND;
		return -1;
	}

	if(ctx->sprites_3D_count == R3D_MAX_SPRITES_3D) {
		error_code = ERROR_SPRITE_3D_LIMIT;
		return -1;
	}

	sprite = &ctx->sprites_3D[ctx->sprites_3D_count++];

	*sprite = (R3D_Sprite3D) {
		.texture_index = texture_index,
		.src = {0, 0, texture->width, texture->height},
		.position = {0.0f, 0.0f, 0.0f},
		.size = {1.0f, 1.0f},
		.color = {1.0f, 1.0f, 1.0f},
		.rotation = 0.0f
	};

	if(src != NULL) {
		sprite->src = *src;
	}

	sprite->src.x /= texture->width;
	sprite->src.w /= texture->width;
	sprite->src.y /= texture->height;
	sprite->src.h /= texture->height;

	if(position != NULL) {
		sprite->position = *position;
	}

	if(size != NULL) {
		sprite->size = *size;
	}

	if(color != NULL) {
		sprite->color = *color;
	}

	sprite->rotation = rotation;

	return 0;
}

void R3D_Begin(void) {
	glClearColor(ctx->clear_color.r, ctx->clear_color.g, ctx->clear_color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ctx->render_mode = RENDERMODE_ARRAY_GEOMETRY;
	ctx->sprites_2D_count = 0;
	ctx->sprites_3D_count = 0;
	ctx->vertex_count = 0;

	if(ctx->skybox_texture_index >= 0) {
		R3D_PrepareSkyboxFlush();
	}
}

void R3D_End(void) {
	ctx->render_mode = RENDERMODE_ARRAY_GEOMETRY;
	R3D_Flush();

	ctx->render_mode = RENDERMODE_TEX_GEOMETRY;
	R3D_Sprites3DToVertices();
	R3D_Flush();

	ctx->render_mode = RENDERMODE_2D_GEOMETRY;
	R3D_Sprites2DToVertices();
	R3D_Flush();

	SDL_GL_SwapWindow(ctx->window);
}

void R3D_Quit(void) {
	glDeleteProgram(ctx->world_shader);
	glDeleteProgram(ctx->sprite_shader);
	glDeleteBuffers(1, &ctx->vbo);
	glDeleteVertexArrays(1, &ctx->vao);
	glDeleteTextures(1, &ctx->texture_array.id);

	SDL_GL_DeleteContext(ctx->context);
	free(ctx);
}

#endif /* R3D_IMPLEMENTATION */

#endif /* R3D_H */
