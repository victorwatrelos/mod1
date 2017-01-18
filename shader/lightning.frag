#version 330 core
#define M_PI 3.1415926535897932384626433832795

uniform mat4 M;
uniform mat4 V;

in vec3 fragVert;
in vec3 normal;
in vec3	fragColor;

out vec4 finalColor;

float	diffuse(vec3 N, vec3 L, float Kd)
{
    float d = dot(N, L);
	return (d * Kd);
}

float	specular(vec3 N, vec3 L, vec3 V, float Ks, float alpha)
{
	vec3 R = 2 * dot(N, L) * N - L;
	float s = dot(R, V);
	s = pow(s, alpha);
	return (s * Ks);
}

void main()
{
	vec3 light_position = vec3(0.0, 60.0, 120.0);
	vec3 light_color = vec3(1.0, 1.0, 1.0);

	light_position = vec3(M * vec4(light_position, 1.0));
    vec3 lightRay = normalize(light_position - fragVert);

	vec3 viewDir = normalize(vec3(V[0][2], V[1][2], V[2][2]));

	float d = diffuse(normal, lightRay, 1.);
	float s = specular(normal, lightRay, viewDir, 0.4, 500.);
	float a = 0.2;
    finalColor = vec4(s * light_color + ( a + d) * fragColor, 1.0);
}
