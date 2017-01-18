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

void		main()
{
	float height = H;

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    normal = normalize(normalMatrix * in_VertexN);
	fragVert  = vec3(M * vec4(in_Position, 1.0));

	fragColor = vec3(0, 0, in_Position.y / height);
	gl_Position = P * V * vec4(fragVert, 1.0);
}
