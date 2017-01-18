#include "vector.hpp"

t_vecf				normalize(t_vecf v)
{
	t_vecf			v2;
	float			len;
	float			e = 10E-6;

	len = v.x * v.x + v.y * v.y + v.z * v.z;
	if (len < e )
		return v;
	len = sqrt(len);
	v2.x = v.x / len;
	v2.y = v.y / len;
	v2.z = v.z / len;
	return v2;
}

t_vecf				add(t_vecf v1, t_vecf v2)
{
	t_vecf			res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return res;
}

t_vecf				sub(t_vecf v1, t_vecf v2)
{
	t_vecf			res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return res;
}

t_vecf				scale(t_vecf v, float s)
{
	t_vecf			res;

	res.x = v.x * s;
	res.y = v.y * s;
	res.z = v.z * s;
	return res;
}

t_vecf				cross(t_vecf v1, t_vecf v2)
{
	t_vecf			res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return res;
}

float				dot(t_vecf v1, t_vecf v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vecf				inverse(t_vecf v)
{
	t_vecf			res;

	res.x = v.x * -1;
	res.y = v.y * -1;
	res.z = v.z * -1;
	return res;
}

t_vecf				translate(t_vecf point, t_vecf trVec)
{
	t_vecf			res;

	res.x = point.x + trVec.x;
	res.y = point.y + trVec.y;
	res.z = point.z + trVec.z;
	return res;
}
