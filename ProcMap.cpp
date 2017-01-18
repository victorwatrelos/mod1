#include "ProcMap.hpp"

ProcMap::ProcMap(int size) : _procMap(size, std::vector<float>(size)), _size(size)
{
}

void          					ProcMap::createMap()
{
    int                         sub = 3;
    float                       nMin = 100, nMax = -100;
    float                       vMin = 100, vMax = -100;
    float                       n, v;
    std::vector<t_cPoint>		sPoints = seedMap(this->_size, sub);
	MAP                         nMap(this->_size, std::vector<float>(this->_size));
	MAP                         vMap(this->_size, std::vector<float>(this->_size));

	for (int i = 0 ; i < this->_size ; i++)
	{
		for (int j = 0 ; j < this->_size ; j++)
		{
			n = NoiseFn(i, j, 0.6);
			v = VoronoiFn(sPoints, this->_size, sub, i, j);
            nMap[i][j] = n;
            vMap[i][j] = v;
            if (n < nMin)
                nMin = n;
            else if (n > nMax)
                nMax = n;
            if (v < vMin)
                vMin = v;
            else if (v > vMax)
                vMax = v;
        }
    }
//  hdr(nMap, this->_size, nMin, nMax);
//  hdr(vMap, this->_size, vMin, vMax);
//    this->_procMap = addMap(nMap, vMap, this->_size, 0.33f, 0.67f);
    this->_procMap = addMap(nMap, vMap, this->_size, 0.67f, 0.33f);
}

void							ProcMap::scaleMap(float sc)
{
	for (int i = 0 ; i < this->_size ; i++)
	{
		for (int j = 0 ; j < this->_size ; j++)
		{
			this->_procMap[i][j] = sc * this->_procMap[i][j];
		}
	}
}

MAP								ProcMap::getMap() { return (this->_procMap); }

/*
 * 				VORONOI
 */

std::vector< t_cPoint >			ProcMap::seedMap(size_t size, int sub)
{
	float						step = size / sub;
	t_cPoint					point;
	std::vector< t_cPoint >		seedPoints;

	srand(666);
	for (int i = 0 ; i < sub ; i++)
	{
		for (int j = 0 ; j < sub ; j++)
		{
			point.x = random(j * step, j * step + step);
			point.y = random(i * step, i * step + step);
			point.v = random(0.f, 1.f);
			if (point.v < 0.3f)
				point.v = 0.f;
			else
				point.v = 1.f;
			seedPoints.push_back(point);
		}
	}
	return (seedPoints);
}

bool							sortFP(t_fp f1, t_fp f2) { return (f1.d < f2.d); }

float							ProcMap::VoronoiFn(std::vector<t_cPoint> sPoints, int size, int sub, int i, int j)
{
	float						step = static_cast<float>(size) / static_cast<float>(sub);
	float						n;
	std::vector<t_fp>			dl;
	t_fp						f;

	for (auto const &p : sPoints)
	{
		f.d = sqrt(P(p.x - j) + P(p.y - i));
		f.v = p.v;
		dl.push_back(f);
	}
	std::sort(dl.begin(), dl.end(), sortFP);
	n = (dl[1].d / (step * M_SQRT2)) - (dl[0].d / (step * M_SQRT2));
	n *= dl[0].v;
//	n -= (dl[0].v / 8.f);
	return (clamp(n, 0.f, 1.f));
}

/*
 * 				NOISE
 */

float							ProcMap::NoiseFn(int i, int j, float z)
{
	int							octaves = 6;
	float						p = 0.6;

	return ( Noise::sound3D(i, j, z, octaves, p) );
}

/*
 * 				UTILITIES
 */

float							ProcMap::clamp(float x, float a, float b)
{
	if (x < a)
		x = a;
	else if (x > b)
		x = b;
	return x;
}

//		return random value beween a and b
float							ProcMap::random(float a, float b)
{
	return ((b - a) * (static_cast<float>(rand()) / RAND_MAX) + a);
}

float							ProcMap::stretch(float x, float a, float b)
{
	return ((x - a) / (b - a));
}

void							ProcMap::hdr(MAP &map, size_t size, float min, float max)
{
	for (size_t i = 0 ; i < size ; i++)
	{
		for (size_t j = 0 ; j < size ; j++)
		{
			map[i][j] = stretch(map[i][j], min, max);
		}
	}
}

MAP								ProcMap::addMap(MAP m1, MAP m2, size_t size, float c1, float c2)
{
	MAP							mRes(size, std::vector<float>(size));
	float						n, min = 100, max = -100;

	for (size_t i = 0 ; i < size ; i++)
	{
		for (size_t j = 0 ; j < size ; j++)
		{
			n = c1 * m1[i][j] + c2 * m2[i][j];
			mRes[i][j] = n;
			if (n < min)
				min = n;
			else if (n > max)
				max = n;
		}
	}
	hdr(mRes, size, min, max);
	return (mRes);
}
