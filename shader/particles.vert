#version 330 core

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec3 in_Position;
in vec3 in_VertexN;
in vec3 instancePosition;

out vec3 fragVert;
out vec3 normal;
out vec3 fragColor;

vec3	instanceMove(vec3 pos, vec3 trans)
{
	vec3	v = pos + trans;
	return v;
}

void		main()
{
	float cr = float(((gl_InstanceID % 1600) % 40)) / 40.0 + 0.2;
	float cb = float(((gl_InstanceID % 1600) / 40)) / 40.0 + 0.2;
	float cg = float((gl_InstanceID / 1600)) / 40.0 + 0.2;

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    normal = normalize(normalMatrix * in_VertexN);
	fragVert  = vec3(M * vec4(in_Position, 1.0));
	fragVert = instanceMove(fragVert, instancePosition);

	fragColor = vec3(cr, cg, cb);
	gl_Position = P * V * vec4(fragVert, 1.0);
}
