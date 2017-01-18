__kernel void   endSim(
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
        __global float      *particlesVBO)
{
    int gid = get_global_id(0);
    if (gid > maxGID)
       return;
    int pos = gid * 3;
    particlesVelocity[gid] += particlesProjection[gid] - particles[gid];
    particles[gid] = particlesProjection[gid];
    particlesVBO[pos] = particles[gid].x;
    particlesVBO[pos + 1] = particles[gid].y;
    particlesVBO[pos + 2] = particles[gid].z;
   // printf("%f, %f, %f\n", particlesVBO[pos], particlesVBO[pos + 1], particlesVBO[pos + 2]);
}
