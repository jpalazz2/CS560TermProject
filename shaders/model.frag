#version 330 core

precision highp float;
vec3 eye = vec3(0.0f, 0.0f, 2.5f);
in vec3 init_ray;
uniform float texture_weight;
uniform float time;
uniform vec3 light;
uniform sampler2D tex;

uniform vec3 sphere0;
uniform float radius0;
uniform vec3 sphere1;
uniform float radius1;
uniform vec3 room_min = vec3(-1.0, -1.0, -1.0);
uniform vec3 room_max = vec3(1.0, 1.0, 1.0);

out vec4 color;

int shinyValue = 32;

bool sphere_intersect(in vec3 origin, in vec3 direction, in vec3 center, in float radius,
                      out float t) {
  // Calculate the quadratic equation solution to sphere/ray intersection
  float a, b, c;
  vec3 oc_diff = origin - center;
  a = dot(direction, direction);
  b = 2 * dot(direction, oc_diff);
  c = dot(oc_diff, oc_diff) - radius*radius;

  // Check determinate
  float det = b*b - 4.0*a*c;
  if (det > 0.0) {
    float t0, t1;
    t = (-b - sqrt(det)) / (2.0 * a);
    return t > 0.0;
  } else {
    return false;
  }
}

vec3 sphere_normal(vec3 surface_pt, vec3 center, float radius) {
  return (surface_pt - center) / radius;
}

bool cube_intersect(in vec3 origin, in vec3 direction, in vec3 b_min, in vec3 b_max,
                    out float t) {
  vec3 t_min = (b_min - origin) / direction;
  vec3 t_max = (b_max - origin) / direction;
  vec3 t0 = min(t_min, t_max);
  vec3 t1 = max(t_min, t_max);
  float near = max(max(t0.x, t0.y), t0.z);
  float far = min(min(t1.x, t1.y), t1.z);
  t = far;
  return far >= near;
}

float shadow(in vec3 origin, in vec3 direction) {
	float sphere0_t, sphere1_t;
	bool sphere0_hit = sphere_intersect(origin, direction, sphere0, radius0, sphere0_t);
	bool sphere1_hit = sphere_intersect(origin, direction, sphere1, radius1, sphere1_t);
	if (sphere0_hit || sphere1_hit) {
		return 0.0;
	}
	return 1.0;
}

vec3 cube_normal(vec3 surface_pt, vec3 b_min, vec3 b_max) {
  float epsilon = 0.001;
  if (surface_pt.x < b_min.x + epsilon) return vec3(-1.0, 0.0, 0.0);
  else if (surface_pt.x > b_max.x - epsilon) return vec3(1.0, 0.0, 0.0);
  else if (surface_pt.y < b_min.y + epsilon) return vec3(0.0, -1.0, 0.0);
  else if (surface_pt.y > b_max.y - epsilon) return vec3(0.0, 1.0, 0.0);
  else if (surface_pt.z < b_min.z + epsilon) return vec3(0.0, 0.0, -1.0);
  else return vec3(0.0, 0.0, 1.0);
}

float random(vec3 scale, float seed) {
  return fract(sin(dot(gl_FragCoord.xyz + seed, scale)) * 43758.5453 + seed);
}
vec3 cosineWeightedDirection(float seed, vec3 normal) {
  float u = random(vec3(12.9898, 78.233, 151.7182), seed);
  float v = random(vec3(63.7264, 10.873, 623.6736), seed);
  float r = sqrt(u);
  float angle = 6.283185307179586 * v;
  vec3 sdir, tdir;
  if (abs(normal.x) < .5) {
    sdir = cross(normal, vec3(1, 0, 0));
  } else {
    sdir = cross(normal, vec3(0, 1, 0));
  }
  tdir = cross(normal, sdir);
  return r * cos(angle) * sdir + r * sin(angle) * tdir + sqrt(1. - u) * normal;
}

vec3 colorize(vec3 origin, vec3 direction, vec3 light) {
  float INF = 100000.0;
  vec3 color_mask = vec3(1.0);
  vec3 color_acc = vec3(0.0);

  for (int ref = 0; ref < 5; ref++) {
    // Intersect with everything
    float room_t;
    float sphere0_t, sphere1_t;
    bool room_hit = cube_intersect(origin, direction, room_min, room_max, room_t);
    bool sphere0_hit =  sphere_intersect(origin, direction, sphere0, radius0, sphere0_t);
    bool sphere1_hit = sphere_intersect(origin, direction, sphere1, radius1, sphere1_t);

    // Find the min intersection
    float t = INF;
    if (room_hit) t = room_t;
    if (sphere0_hit && sphere0_t < t) t = sphere0_t;
    if (sphere1_hit && sphere1_t < t) t = sphere1_t;

    if (t == INF) break;

    vec3 hit = origin + direction * t;
    vec3 normal;
    vec3 surface_color;

    // Calculate normal
    if (t == room_t) {
      normal = -cube_normal(hit, room_min, room_max);
      if (normal == vec3(1.0, 0.0, 0.0)) {
				surface_color = vec3(0.7, 0.0, 0.0);
      } else {
				surface_color = vec3(0.7, 0.7, 0.7);
			}
    } else if (t == sphere0_t) {
      normal = sphere_normal(hit, sphere0, radius0);
      surface_color = vec3(0.0, 0.7, 0.0);
    } else if (t == sphere1_t) {
      normal = sphere_normal(hit, sphere1, radius1);
      surface_color = vec3(0.7, 0.0, 0.7);
    } else {
    	normal = vec3(0.0, 1.0, 0.0);
    	surface_color = vec3(1.0, 0.0, 1.0);
		}

    direction = cosineWeightedDirection(time + float(ref), normal);

    // Calculate light contribution from hit
    vec3 to_light = light - hit;
    float diffuse = max(0.0, dot(normalize(to_light), normal));
    color_mask *= surface_color;
    float shadowIntensity = shadow(hit + normal * 0.0001, to_light);
    vec3 reflected = normalize(reflect(light - hit, normal));
    float specular = pow(max(dot(normalize(hit - origin), reflected), 0.0), shinyValue);

    color_acc += color_mask * shadowIntensity * ((0.5 * diffuse) + (0.5 * specular));

    // The diffuse value for a sphere is 1.0?
    // The floor and back wall have the same normal

    origin = hit;
  }

  return color_acc;
}

vec3 uniformlyRandomDirection(float seed) {
  float u = random(vec3(12.9898, 78.233, 151.7182), seed);
  float v = random(vec3(63.7264, 10.873, 623.6736), seed);
  float z = 1.0 - 2.0 * u;
  float r = sqrt(1.0 - z * z);
  float angle = 6.283185307179586 * v;
  return vec3(r * cos(angle), r * sin(angle), z);
}

vec3 uniformlyRandomVector(float seed) {
  return uniformlyRandomDirection(seed) * sqrt(random(vec3(36.7539, 50.3658, 306.2759),
                                                      seed));
}

void main() {
  vec3 new_light = light + uniformlyRandomVector(time - 53.0) * 0.1;
  vec3 textur = texture(tex, gl_FragCoord.xy / 512.0).rgb;
  color = vec4(mix(colorize(eye, init_ray, new_light), textur, texture_weight), 1.0);
}
