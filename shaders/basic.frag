#version 450 core

layout(location = 0) out vec4 f_color;

in vec2 v_tex_id;
in vec3 v_normal;
in vec2 v_tex_coord;
in smooth float v_ambientOcclusion;

uniform sampler2D u_texture_atlas;

uniform vec3 u_sunDirection;
uniform vec3 u_sunColor;
uniform int u_atlasWidth;
uniform int u_atlasHeight;
uniform int u_tileWidth;
uniform int u_tileHeight;
uniform float u_near;
uniform float u_far;
uniform vec3 u_skyColor;

float fogStart = 20.0;              // Startdistanz des Nebels
float fogEnd = u_far;                // Enddistanz des Nebels

float linearizeDepth(float depth) 
{
	return (2.0 * u_near * u_far) / (u_far + u_near - (depth * 2.0 - 1.0) * (u_far - u_near));
}

void main()
{
	float u = ((v_tex_id.x + v_tex_coord.x) * u_tileWidth) / u_atlasWidth;
	float v = ((v_tex_id.y + v_tex_coord.y) * u_tileHeight) / u_atlasHeight;
    vec3 light = normalize(u_sunDirection);
	vec4 diffuseColor = texture(u_texture_atlas, vec2(u, v));
    vec3 finalColor = max(0.1, v_ambientOcclusion) * u_sunColor * max(dot(v_normal, light), 0.6) * diffuseColor.xyz;

	float depth = linearizeDepth(gl_FragCoord.z);
	float fogFactor = clamp((fogEnd - depth) / (fogEnd - fogStart), 0.0, 1.0);

	f_color = vec4(mix(u_skyColor, finalColor, fogFactor), 1.0); 
}