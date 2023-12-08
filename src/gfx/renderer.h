#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "../util/util.h"

struct Renderer
{
    Camera camera;
    Shader shader;
    VAO vao;
    VBO vbo;
};

typedef struct Renderer Renderer;

void renderer_init(vec2f viewport_size);
void renderer_camera_move(vec3f direction);
void renderer_camera_turn(vec2f offset);
void render();

#endif