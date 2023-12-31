#include "chunk.h"
#include "world.h"

#pragma region raw_data
#define NUM_COMPONENTS 5

static f32 s_vertices[] = {
    0, 0, 0,
    1, 0, 0,
    1, 1, 0,
    0, 1, 0,
    0, 0, 1,
    1, 0, 1,
    1, 1, 1,
    0, 1, 1
};
/*
static u32 s_indices[] = {
    1, 0, 3, 1, 3, 2, // -z
    4, 5, 6, 4, 6, 7, // +z
    5, 1, 2, 5, 2, 6, // +x
    0, 4, 7, 0, 7, 3, // -x
    2, 3, 7, 2, 7, 6, // +y
    5, 4, 0, 5, 0, 1  // -y
};
*/
static u32 side_idxs[] = {
    2, 1, 0, 3, //-z
    7, 4, 5, 6, //+z
    6, 5, 1, 2, //+x
    3, 0, 4, 7, //-x
    2, 3, 7, 6, //+y
    5, 4, 0, 1  //-y
};

static u32 vertex_idxs[] = {
    0, 1, 2, 0, 2, 3
};

static f32 s_tex[] = {
    0, 0,
    0, 1,
    1, 1,
    1, 0
};

static s32 dirs[] = {
    0, 0, -1,  // -z
    0, 0, 1,  // +z
    1, 0, 0,  // +x
    -1, 0, 0, // -x
    0, 1, 0,  // +y
    0, -1, 0  // -y
};
#pragma endregion

#pragma region counts
static s32 vertex_count(Chunk* chunk)
{
    // each face has 4 vertices with components
    return chunk->face_count * 4 * NUM_COMPONENTS;
}

static s32 index_count(Chunk* chunk)
{
    // each face has 2 triangles
    return chunk->face_count * 6;
}

static s32 max_vertex_count(Chunk* chunk)
{
    return chunk->data_count * 6 * 4 * NUM_COMPONENTS;
}

static s32 max_index_count(Chunk* chunk)
{
    return chunk->data_count * 6 * 6;
}

#pragma endregion


static bool has_adjacent(const Chunk* chunk, const vec3i pos, const Side side)
{
    vec3i new = vec3i_initr(pos.x + dirs[3*side], pos.y + dirs[3*side+1], pos.z + dirs[3*side+2]);
    s32 idx = chunk_block_index(new.x, new.y, new.z);
    if (idx == -1)
    {
        return world_block_at(chunk->position, new);
    }
    return chunk->data[idx] != 0;
}   

static void fill_vertices(Chunk* chunk, BlockId block_id, vec3i pos)
{
    vec2i tex_coords[6];
    memcpy(tex_coords, BLOCKS[block_id].tex_coords, 6 * sizeof(vec2i));
    for (Side side = SIDE_FIRST; side <= SIDE_LAST; side++)
    {
        if (has_adjacent(chunk, pos, side))
            continue;
        
        for (s32 i = 0; i < 4; i++)
        {
            u32 index = side_idxs[4 * side + i];
            chunk->vertices[NUM_COMPONENTS * i + vertex_count(chunk)]     = s_vertices[3 * index]     + pos.x + CHUNK_SIZE_X * chunk->position.x;
            chunk->vertices[NUM_COMPONENTS * i + vertex_count(chunk) + 1] = s_vertices[3 * index + 1] + pos.y + CHUNK_SIZE_Y * chunk->position.y;
            chunk->vertices[NUM_COMPONENTS * i + vertex_count(chunk) + 2] = s_vertices[3 * index + 2] + pos.z + CHUNK_SIZE_Z * chunk->position.z;
            chunk->vertices[NUM_COMPONENTS * i + vertex_count(chunk) + 3] = (s_tex[2 * i]     + tex_coords[side].x) / BLOCK_ATLAS_WIDTH;
            chunk->vertices[NUM_COMPONENTS * i + vertex_count(chunk) + 4] = (s_tex[2 * i + 1] + tex_coords[side].y) / BLOCK_ATLAS_WIDTH;
        }
        for (s32 i = 0; i < 6; i++)
            chunk->indices[index_count(chunk) + i] = vertex_count(chunk) / NUM_COMPONENTS + vertex_idxs[i];

        chunk->face_count++;
    }
}

void chunk_init(Chunk* chunk, s32 x, s32 y, s32 z)
{
    block_init();

    vec3i_init(&chunk->position, x, y, z);
    chunk->data     = calloc(CHUNK_VOLUME, sizeof(u8));
    chunk->vertices = malloc(0);
    chunk->indices  = malloc(0);
    chunk->data_count = 0;
    assert(chunk->data != NULL);

    chunk->vao = vao_init();
    chunk->vbo = vbo_init(GL_ARRAY_BUFFER);
    chunk->ebo = vbo_init(GL_ELEMENT_ARRAY_BUFFER);

    chunk_generate_data(chunk);
}

s32 chunk_block_index(s32 x, s32 y, s32 z)
{
    if (x < 0 || y < 0 || z < 0) 
        return -1;
    if (x >= CHUNK_SIZE_X || y >= CHUNK_SIZE_Y || z >= CHUNK_SIZE_Z) 
        return -1;
    return x + CHUNK_SIZE_X * y + CHUNK_SIZE_X * CHUNK_SIZE_Y * z;
}

vec3i chunk_block_position(s32 idx)
{
    vec3i pos;
    pos.z = idx / (CHUNK_SIZE_X * CHUNK_SIZE_Y);
    idx -= pos.z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
    pos.y = idx / CHUNK_SIZE_X;
    idx -= pos.y * CHUNK_SIZE_X;
    pos.x = idx;
    return pos;
}

void chunk_generate_data(Chunk* chunk)
{
    for (s32 x = 0; x < CHUNK_SIZE_X; x++)
    {
        for (s32 z = 0; z < CHUNK_SIZE_Z; z++)
        {
            u8 height = noise_at(chunk->position, vec3i_initr(x, 0, z)) + 1;
            for (s32 y = 0; y < height; y++)
            {
                chunk->data[chunk_block_index(x, y, z)] = 1;
                chunk->data_count++;
            }
        }
    }
}

void chunk_generate_vertices(Chunk* chunk)
{
    free(chunk->vertices);
    free(chunk->indices);
    chunk->face_count = 0;
    chunk->vertices = calloc(max_vertex_count(chunk), sizeof(f32));
    chunk->indices  = calloc(max_index_count(chunk), sizeof(u32));
    assert(chunk->vertices != NULL);
    assert(chunk->indices != NULL);
    
    for (s32 i = 0; i < CHUNK_VOLUME; i++)
    {
        if (chunk->data[i] != 0)
            fill_vertices(chunk, (BlockId) chunk->data[i], chunk_block_position(i));
    }

    if (chunk->face_count > 0)
    {
        chunk->vertices = realloc(chunk->vertices, vertex_count(chunk) * sizeof(f32));
        chunk->indices = realloc(chunk->indices, index_count(chunk) * sizeof(s32));
        assert(chunk->vertices != NULL);
        assert(chunk->indices != NULL);

        vao_bind(chunk->vao);
        vbo_bind(chunk->vbo);
        vbo_bind(chunk->ebo);
        vbo_buffer(chunk->vbo, vertex_count(chunk) * sizeof(f32), chunk->vertices);
        vbo_buffer(chunk->ebo, index_count(chunk) * sizeof(s32), chunk->indices);
    }
}

void chunk_render(Chunk* chunk)
{
    vao_bind(chunk->vao);
    vbo_bind(chunk->vbo);
    vbo_bind(chunk->ebo);
    vao_attr(0, 3, NUM_COMPONENTS * sizeof(f32), 0);
    vao_attr(1, 2, NUM_COMPONENTS * sizeof(f32), (void*) (3 * sizeof(f32)));
    glDrawElements(GL_TRIANGLES, index_count(chunk), GL_UNSIGNED_INT, 0);
}

void chunk_reload(Chunk* chunk)
{
    chunk_generate_vertices(chunk);
}