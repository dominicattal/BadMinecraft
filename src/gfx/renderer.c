#include "renderer.h"

Renderer renderer;

void renderer_init(vec2i viewport_size)
{
    renderer.shader = shader_init("src/shaders/vertex.sl", "src/shaders/fragment.sl");
    shader_use(renderer.shader);
    renderer.vao = vao_init();
    vao_bind(renderer.vao);
    renderer.vbo = vbo_init(GL_ARRAY_BUFFER);
    vbo_bind(renderer.vbo);
    renderer.ebo = vbo_init(GL_ELEMENT_ARRAY_BUFFER);
    vbo_bind(renderer.ebo);
    renderer.block_modelID = glGetUniformLocation(renderer.shader.ID, "model");
    renderer.blocks = ht_create();

    Block* block = malloc(sizeof(Block));
    block_init(block, 0, 2, 0);
    ht_set(renderer.blocks, block_hash(*block), block);
    
    camera_init(&renderer.camera, (float)viewport_size.x / viewport_size.y);
    shader_link_camera(renderer.shader, &renderer.camera);
}

void renderer_camera_move(vec3f moving, float dt)
{
    camera_move(&renderer.camera, moving, dt);
}

void renderer_camera_turn(vec2f offset)
{
    camera_turn(&renderer.camera, offset);
}

void render_color_blocks()
{
    float vertices[] = {
        0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 1
    };
    unsigned int indices[] = {
        1, 0, 3, 1, 3, 2, // -z
        4, 5, 6, 4, 6, 7, // +z
        5, 1, 2, 5, 2, 6, // +x
        0, 4, 7, 0, 7, 3, // -x
        2, 3, 7, 2, 7, 6, // +y
        5, 4, 0, 5, 0, 1  // -y
    };
    vbo_buffer(renderer.vbo, sizeof(vertices), vertices);
    vbo_buffer(renderer.ebo, sizeof(indices), indices);
    vao_attr();
    mat4f model = mat4f_init();
    hti iter = ht_iterator(renderer.blocks);
    while (ht_next(&iter))
    {
        Block* block = iter.value;
        vec3i pos = block->position;
        model[12] = pos.x;
        model[13] = pos.y;
        model[14] = pos.z;
        glUniformMatrix4fv(renderer.block_modelID, 1, GL_FALSE, model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void render_texture_blocks() {}

void render()
{
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    render_color_blocks();
}