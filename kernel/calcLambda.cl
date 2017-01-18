float   poly6Kernel(float4 currentParticle, float4 neighbor) {
    float r = distance(currentParticle, neighbor);
    float hrTerm = (RANGE * RANGE - r * r);
    return 315.0f / (64.0f * MY_PI * pown(RANGE, 9)) * hrTerm * hrTerm * hrTerm;
}

float4   spikyGradientKernel(float4 currentParticle, float4 neighbor) {
    float   length;

    length = distance(currentParticle, neighbor);
    float   hrTerm = RANGE - length; 
    return 45.0f / (((float)(MY_PI)) * pown(RANGE, 6)) * hrTerm * hrTerm
            * 1.0f / (length + 0.001f) * (currentParticle - neighbor);

}

float4  calcCiGradient(float4 currentParticle, float4 neighbor) {
    return -1.0f / REST_DENSITY * spikyGradientKernel(currentParticle, neighbor);
}

float4  calcCiGradientATI(__global float4 *particles, float4 currentParticle, __global int *neighbors, int nbNeighbors) {
    float4  accum = float4(0.0f);
    float4  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
        accum += spikyGradientKernel(currentParticle, particles[neighbors[i]]);
    }
    return 1.0f / REST_DENSITY * accum;
}

float   calcRo(__global float4 *particles, float4 currentParticle, __global int *neighbors, int nbNeighbors)
{
    float   ro = 0.0f;
    float4  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
        neighbor = particles[neighbors[i]];
        ro += poly6Kernel(currentParticle, neighbor);
    }
    return ro;
}

float   calcLambdaPi(__global float4 *particles, float4 currentParticle, __global int *neighbors, int nbNeighbors, int gid)
{
    float   cI = (calcRo(particles, currentParticle, neighbors, nbNeighbors) / REST_DENSITY) - 1.0f;
    float   cIGradient, sumGradient = 0.0f;

    for (int i = 0; i < nbNeighbors; i++) {
        cIGradient = length(calcCiGradient(currentParticle, particles[neighbors[i]]));
        sumGradient += cIGradient * cIGradient;
    }
    cIGradient = length(calcCiGradientATI(particles, currentParticle, neighbors, nbNeighbors));
    sumGradient += cIGradient * cIGradient + 0.01;
    return -1.0f * (cI / sumGradient);
}

__kernel void   calcLambda (
        __global float4    *particles,
        __global float4    *particlesVelocity,
        __global float4    *particlesProjection,
        __global int        *gridParticles,
        float               coef,
        int                 gridX,
        int                 gridY,
        int                 gridZ,
        int                 nbParticlePerCell,
        int                 maxGID,
        __global float      *lambda,
        __global int        *neighbors
        )
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return;

    lambda[gid] = calcLambdaPi(
                                    particlesProjection,
                                    particlesProjection[gid],
                                    neighbors + gid * 200 + 1,
                                    neighbors[gid * 200],
                                    gid);
}
