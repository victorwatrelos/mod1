#ifndef NOISE_HPP
# define NOISE_HPP

# include <cmath>

class					Noise
{
	public:
		static double noise3D(double x, double y, double z);
		static double sound3D(double x, double y, double z, int octaves, double persistence);

	private:
		static double fade(double t);
		static double lerp(double t, double a, double b);
		static double slerp(double t, double a, double b);
		static double grad(int hash, double x, double y, double z);

		static int p[];
};

#endif
