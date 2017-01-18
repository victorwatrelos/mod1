#ifndef PROCMAP_HPP
# define PROCMAP_HPP

# include <cstdlib>
# include <vector>
# include "Noise.hpp"
# define MAP					std::vector< std::vector<float> >
# define P(X)						((X) * (X))

typedef struct					s_fp
{
	float						v;
	float						d;
}								t_fp;

typedef struct					s_cPoint
{
	int							x;
	int							y;
	float						v;
}								t_cPoint;


class								ProcMap
{
	public:
									ProcMap(int size);
		void						createMap();
		void						scaleMap(float sc);
		MAP							getMap();

	private:
		std::vector< t_cPoint >		seedMap(size_t size, int sub);
		float						VoronoiFn(std::vector<t_cPoint> sPoints, int size, int sub, int i, int j);

		float						NoiseFn(int i, int j, float z);

		float						clamp(float x, float a, float b);
		float						random(float a, float b);
		float						stretch(float x, float a, float b);
		void						hdr(MAP &map, size_t size, float min, float max);
		MAP							addMap(MAP m1, MAP m2, size_t size, float c1, float c2);

	private:
		MAP							_procMap;
		int							_size;
	
};


#endif
