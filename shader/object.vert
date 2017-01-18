#version 330 core
#define M_PI 3.1415926535897932384626433832795

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float H;

in vec3 in_Position;
in vec3 in_VertexN;

out vec3 fragVert;
out vec3 normal;
out vec3 fragColor;

float wnoise(vec3 pos, vec3 n, float a, float b, float c) {
	vec3 light_position = vec3(4.0, 4.0, 4.0);
    vec3 lightRay = light_position - pos;

    float alpha = dot(n, lightRay) / (length(lightRay) * length(n));
    alpha = clamp(alpha, 0, 1);

	return a * sin( (alpha * 2 * M_PI + c) * b );
}

void		main()
{
	float a = 2.0;
	float height = H;

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    normal = normalize(normalMatrix * in_VertexN);
	fragVert  = vec3(M * vec4(in_Position, 1.0));

	float n = wnoise(fragVert, normal, a, 2.0, 5.0);
	float coln = ( (n + a) / 2 ) / a;
	float cr = ( sin( (coln / 32) * 2 * M_PI ) + 1 ) / 2;
	float cg = ( cos( (coln / 32) * 2 * M_PI ) + 1 ) / 2;
	fragColor = vec3(cr, 0.5, cg);
	gl_Position = P * V * vec4(fragVert, 1.0);
}
