#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"
#include "mat.h"
#include "type.h"

#define NEAR_CLIP_DISTANCE 0.1
#define FAR_CLIP_DISTANCE 10000

struct Camera
{
    f32 yaw, pitch, speed, sensitivity, aspect_ratio, fov;
    mat4f view, proj;
    vec3f position, facing, right, up;
    u8 viewID, projID;
};

typedef struct Camera Camera;

void camera_set_aspect_ratio(Camera* camera, float aspect_ratio);
void camera_init(Camera* camera, float aspect_ratio);
void camera_turn(Camera* camera, vec2f offset);
void camera_move(Camera* camera, vec3f direction, float dt);
void camera_update_view(Camera* camera);
void camera_update_proj(Camera* camera);

#endif