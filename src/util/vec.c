#include "vec.h"

char* hash_int(int x)
{
    char* hash = malloc(9 * sizeof(char));
    unsigned int ux;
    ux = (unsigned int)x;
    ux += 2147483647;
    for (int i = 0; i < 8; i++)
    {
        int m = ux % 16;
        if (m < 10)
        {
            hash[7-i] = m + '0';
        }
        else
        {
            hash[7-i] = (char) (m + 97 - 10);
        }
        ux /= 16;
    }
    hash[8] = '\0';
    return hash;
}

// vec2f
void vec2f_init(vec2f* p_vec, float x, float y)
{
    p_vec->x = x;
    p_vec->y = y;
}

void vec2f_init0(vec2f* p_vec)
{
    p_vec->x = 0;
    p_vec->y = 0;
}

void vec2f_normalize_ip(vec2f* p_vec)
{
    float x, y, mag;
    x = p_vec->x;
    y = p_vec->y;
    mag = sqrt(x*x + y*y);
    if (mag != 0)
    {
        p_vec->x /= mag;
        p_vec->y /= mag;
    }
}

void vec2f_scale_ip(vec2f* p_vec, float scale)
{
    p_vec->x *= scale;
    p_vec->y *= scale;
}

// vec2i
void vec2i_init(vec2i* p_vec, int x, int y)
{
    p_vec->x = x;
    p_vec->y = y;
}

void vec2i_init0(vec2i* p_vec)
{
    p_vec->x = 0;
    p_vec->y = 0;
}

// vec3f
void vec3f_init(vec3f* p_vec, float x, float y, float z)
{
    p_vec->x = x;
    p_vec->y = y;
    p_vec->z = z;
}

void vec3f_init0(vec3f* p_vec)
{
    p_vec->x = 0;
    p_vec->y = 0;
    p_vec->z = 0;
}

void vec3f_normalize_ip(vec3f* p_vec)
{
    float x, y, z, mag;
    x = p_vec->x;
    y = p_vec->y;
    z = p_vec->z;
    mag = sqrt(x*x + y*y + z*z);
    if (mag != 0)
    {
        p_vec->x /= mag;
        p_vec->y /= mag;
        p_vec->z /= mag;
    }
}

void vec3f_scale_ip(vec3f* p_vec, float scale)
{
    p_vec->x *= scale;
    p_vec->y *= scale;
    p_vec->z *= scale;
}

void vec3f_norm_scale_ip(vec3f* p_vec, float scale)
{
    vec3f_normalize_ip(p_vec);
    vec3f_scale_ip(p_vec, scale);
}

void vec3f_add_ip(vec3f* p_vec, vec3f vec)
{
    p_vec->x += vec.x;
    p_vec->y += vec.y;
    p_vec->z += vec.z;
}

vec3f vec3f_scale(const vec3f vec, float scale)
{
    vec3f ret;
    ret.x = vec.x * scale;
    ret.y = vec.y * scale;
    ret.z = vec.z * scale;
    return ret;
}

vec3f vec3f_add(const vec3f vec1, const vec3f vec2)
{
    vec3f ret;
    ret.x = vec1.x + vec2.x;
    ret.y = vec1.y + vec2.y;
    ret.z = vec1.z + vec2.z;
    return ret;
}

vec3f vec3f_cross(const vec3f vec1, const vec3f vec2)
{
    vec3f ret;
    ret.x = vec1.y * vec2.z - vec1.z * vec2.y;
    ret.y = vec1.z * vec2.x - vec1.x * vec2.z;
    ret.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return ret;
}

char* vec3f_hash(const vec3f vec)
{
    return malloc(sizeof(char));
}

void vec3f_print(const vec3f vec)
{
    printf("(%.2f, %.2f, %.2f)\n", vec.x, vec.y, vec.z);
}

// vec3i
void vec3i_init(vec3i* p_vec, int x, int y, int z)
{
    p_vec->x = x;
    p_vec->y = y;
    p_vec->z = z;
}

void vec3i_init0(vec3i* p_vec)
{
    p_vec->x = 0;
    p_vec->y = 0;
    p_vec->z = 0;
}

vec3i vec3i_add(const vec3i vec1, const vec3i vec2)
{
    vec3i ret;
    ret.x = vec1.x + vec2.x;
    ret.y = vec1.y + vec2.y;
    ret.z = vec1.z + vec2.z;
    return ret;
}

char* vec3i_hash(const vec3i vec)
{
    char* hash = malloc(25 * sizeof(char));
    char* hash_x = hash_int(vec.x);
    char* hash_y = hash_int(vec.y);
    char* hash_z = hash_int(vec.z);
    for (int i = 0; i < 8; i++)
    {
        hash[i] = hash_x[i];
        hash[i+8] = hash_y[i];
        hash[i+16] = hash_z[i];
    }
    hash[24] = '\0';
    free(hash_x);
    free(hash_y);
    free(hash_z);
    return hash;
}

void vec3i_print(const vec3i vec)
{
    printf("(%d, %d, %d)\n", vec.x, vec.y, vec.z);
}