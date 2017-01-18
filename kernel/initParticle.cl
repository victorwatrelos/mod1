#define BOX_SIZE2 BOX_SIZE * BOX_SIZE
#define BOX_HALF_SIZE (((float)BOX_SIZE) / 2.0f)

__kernel void   initParticle(__global float4    *particles,
                               __global float4  *particlesVelocity,
                               int              maxGID)
{
    int gid = get_global_id(0);
    if (gid > maxGID) {
        return ;
    }
    particles[gid].x = ((float)(gid % BOX_SIZE))
        - BOX_HALF_SIZE;
    particles[gid].y = ((float)((gid % (BOX_SIZE2)) / BOX_SIZE))
        - BOX_HALF_SIZE;
    particles[gid].z = ((float)((gid / (BOX_SIZE2))))
        - BOX_HALF_SIZE;
    particles[gid].w = 1.0f;

    particlesVelocity[gid].x = ((float)(gid % 100)) / 50.0f  - 1.0f;
    particlesVelocity[gid].y = ((float)(gid % 100)) / 50.0f  - 1.0f;
    particlesVelocity[gid].z = ((float)(gid % 100)) / 50.0f  - 1.0f;
    particlesVelocity[gid].w = 1.0f;
}
