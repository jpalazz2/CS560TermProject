#version 330 core

layout (location = 0) in vec3 position;

vec3 eye = vec3(0.0f, 0.0f, 0.0f);
vec3 ray00 = vec3(-0.10402260303233442f, -0.10395006700834782f, -0.1998001998001997f);
vec3 ray01 = vec3(-0.10402260303233442f, 0.10406873441093129f, -0.1998001998001997f);
vec3 ray10 = vec3(0.10399619838694468f, -0.10395006700834782f, -0.1998001998001997f);
vec3 ray11 = vec3(0.10399619838694468f, 0.10406873441093129f, -0.1998001998001997f);

out vec3 init_ray;

void main() {
	vec2 percent = position.xy * 0.5f + 0.5f;
	init_ray = mix(mix(ray00, ray01, percent.y), mix(ray10, ray11, percent.y), percent.x);
	gl_Position = vec4(position, 1.0f);
}
