#version 330 core

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float H;

in vec3 in_Position;
in vec3 in_VertexN;

out vec3 fragVert;
out vec3 normal;
out vec3 fragColor;

vec3	makeColor(float h)
{
	float x, r, g, b;
	
	if (h <= 0.3)
	{
		x = h * 10 / 3;
		r = -0.25 * x + 0.75;
		g = -0.25 * x + 0.75;
		b = 0;
	}
	else if (h <= 0.6)
	{
		x = (h - 0.3) * 10 / 3;
		r = 0.5;
		g = -0.25 * x + 0.5;
		b = 0;
	}
	else
	{
		x = (h - 0.6) * 10 / 4;
		r = 0.5 * x + 0.5;
		g = 0.75 * x + 0.25;
		b = x * x;
	}
	return vec3(r, g, b);
}

void		main()
{
	float height = H * 1.5;

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    normal = normalize(normalMatrix * in_VertexN);
	fragVert  = vec3(M * vec4(in_Position, 1.0));

	fragColor = makeColor(clamp(in_Position.y / height, 0, 1.0));
	gl_Position = P * V * vec4(fragVert, 1.0);
}
