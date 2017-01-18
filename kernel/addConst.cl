float4   spikyGradientKernel(float4 currentParticle, float4 neighbor) {
    float4  res;
    float   length;

    res = currentParticle - neighbor;
    length = distance(currentParticle, neighbor);
  //  printf("delta: %f.%f.%f  %f %f %f, dist: %f\n", currentParticle.x, currentParticle.y, currentParticle.z, neighbor.x, neighbor.y, neighbor.z, length);
    float   hrTerm = RANGE - length; 
    float   gradientMagnitude = 45.0f / (MY_PI * pown(RANGE, 6)) * hrTerm * hrTerm;
    float   div = length + 0.001f;
    return gradientMagnitude * 1.0f / div * res;

}

float   poly6Kernel(float4 currentParticle, float4 neighbor, int gid) {
    float r = distance(currentParticle, neighbor);
    float hrTerm = (RANGE * RANGE - r * r);
    float div = 64.0f * MY_PI * pown(RANGE, 9);
    float res = 315.0f / div * hrTerm * hrTerm * hrTerm;
    return res;
}
#define PRESSURE_K 0.1
#define PRESSURE_N 1

float4      calcDelta(__global float4   *particles,
                      float4            currentParticle,
                      __global int      *neighbors,
                      int               nbNeighbors,
                      __global float    *lstLambda,
                      float             lambda,
                      int               gid) {
    float4  delta = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4  neighbor;
    float4  dQ;
    int     neighborId;
    float   kTerm;
    float   sCorr = 0.0f;

    dQ = (0.1f * RANGE) * ((float4)(1.0f, 1.0f, 1.0f, 1.0f)) + currentParticle;

    for (int i = 0; i < nbNeighbors; i++) {
        neighborId = neighbors[i];
        neighbor = particles[neighborId];

        float poly6PDQ = poly6Kernel(currentParticle, dQ, gid);
        if (poly6PDQ < EPSILON)
        {
            kTerm = 0.0f;
            sCorr = 0.0f;
        }
        else
        {
            kTerm = poly6Kernel(currentParticle, neighbor, gid) / poly6PDQ;
            sCorr = -1.0f * PRESSURE_K * pown(kTerm, PRESSURE_N);
        }

        delta += (lambda + lstLambda[neighborId] + sCorr) * spikyGradientKernel(currentParticle, neighbor);
    }
    return 1.0f / REST_DENSITY * delta;
}

void        boxCollision(__global float4 *particle, float4 currentParticle, __global float4 *particleVelocity, float step) {
    float4      normal = 0.0f;
    float4      tmp = 0.0f;
    float4      vel = 0.0f;
    float4      tmpVel = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4      tmpParticle = *particle;
    float       N = 1.0f;
    float       max = MAX;
    float       min = MIN;
    float       offset = 0.001f;
//    float       maxX = step / 100.0f + 15.0f;
    float       maxX = max;

    vel = *particleVelocity;

    if (tmpParticle.x < min) {
        tmpParticle.x = min + offset;
        normal.x = 1.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        tmpVel.x = tmp.x * N;
    }

    if (tmpParticle.x > maxX) {
        tmpParticle.x = maxX - offset;
        normal.x = -1.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        tmpVel.x = tmp.x * N;
    }

    if (tmpParticle.y < min) {
        tmpParticle.y = min + offset;
        normal.x = 0.0f;
        normal.y = 1.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        tmpVel.y = tmp.y;
    }

    if (tmpParticle.y > max) {
        tmpParticle.y = max - offset;
        normal.x = 0.0f;
        normal.y = -1.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        tmpVel.y = tmp.y;
    }

    if (tmpParticle.z < min) {
        tmpParticle.z = min + offset;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 1.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        tmpVel.z = tmp.z * N;
    }

    if (tmpParticle.z > max) {
        tmpParticle.z = max - offset;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = -1.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        tmpVel.z = tmp.z * N;
    }
    *particleVelocity = tmpVel;
    *particle = tmpParticle;

}

__kernel void   addConst (
        __global float4     *particles,
        __global float4     *particlesVelocity,
        __global float4     *particlesProjection,
        __global int        *gridParticles,
        float               coef,
        int                 gridX,
        int                 gridY,
        int                 gridZ,
        int                 nbParticlePerCell,
        int                 maxGID,
        __global float      *lambda,
        __global int        *neighbors,
        float               wallStep
        )
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return;
    float4  delta = calcDelta(
                            particlesProjection,
                            particlesProjection[gid],
                            neighbors + gid * 200 + 1,
                            neighbors[gid * 200],
                            lambda,
                            lambda[gid],
                            gid);
    if (particlesProjection[gid].x != particlesProjection[gid].x || delta.x != delta.x)
    {
    /*
        printf("gid: %d FAIL :: pos: %f, %f, %f delta: %f, %f, %f neighbors: %d \n",
            gid,
            particlesProjection[gid].x,
            particlesProjection[gid].y,
            particlesProjection[gid].z,
            delta.x, delta.y, delta.z,
            neighbors[gid * 200]);
        */
    }
    delta *= 2.5f;
    particlesProjection[gid] += delta;
    boxCollision(particlesProjection + gid, particlesProjection[gid], particlesVelocity + gid, wallStep);
}
