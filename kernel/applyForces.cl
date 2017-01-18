#define MAX_VEL 5.0f
#define MIN_VEL -MAX_VEL

__kernel void   applyForces(
        __global float4     *particles,
        __global float4     *particlesVelocity,
        __global float4     *particlesProjection,
        __global int        *gridParticles,
        float               coef,
        int                 gridX,
        int                 gridY,
        int                 gridZ,
        int                 nbParticlePerCell,
        int                 maxGID)
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return ;
    particlesVelocity[gid] = max(min(particlesVelocity[gid] + ((float4)(0.0f, -0.05f, 0.0f, 0.0f)), MAX_VEL), MIN_VEL);
    particlesProjection[gid] = particles[gid] + particlesVelocity[gid];
}
