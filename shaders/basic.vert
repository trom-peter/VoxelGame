#version 450 core

//vertex attributes
layout(location = 0) in vec3 a_position;
layout(location = 1) in ivec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec2 a_tex_id;
layout(location = 4) in float a_ambientOcclusion;

out vec2 v_tex_id;
out vec3 v_normal;
out vec2 v_tex_coord;
out smooth float v_ambientOcclusion;

uniform vec2 u_chunkPosition;
uniform mat4 u_modelViewProj;
uniform mat4 u_invModelView;
uniform int u_chunkWidth;
uniform int u_chunkLength;

void main()
{
    vec3 worldPos = vec3(a_position.x + u_chunkPosition.x * u_chunkWidth, 
                         a_position.y, 
                         a_position.z + u_chunkPosition.y * u_chunkLength);
    gl_Position = u_modelViewProj * vec4(worldPos, 1.0f);
    v_tex_id = a_tex_id;
    v_normal = normalize(mat3(u_invModelView) * a_normal);
    v_tex_coord = a_tex_coord;
    v_ambientOcclusion = a_ambientOcclusion / 3.0;
}