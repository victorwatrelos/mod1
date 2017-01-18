#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <cmath>

typedef struct				s_vecf
{
	float					x;
	float					y;
	float					z;
}							t_vecf;

t_vecf						normalize(t_vecf v);
t_vecf						add(t_vecf v1, t_vecf v2);
t_vecf						sub(t_vecf v1, t_vecf v2);
t_vecf						scale(t_vecf v, float s);
t_vecf						cross(t_vecf v1, t_vecf v2);
float						dot(t_vecf v1, t_vecf v2);
t_vecf						inverse(t_vecf v);
t_vecf						translate(t_vecf point, t_vecf trVec);

#endif
