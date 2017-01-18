__kernel void   particleInGrid(
        __global float4     *particles,
        __global float4     *particlesVelocity,
        __global float4     *particleProjection,
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
        return;

    int x = (particles[gid].x / coef) + (float)gridX / 2.0f;
    int y = (particles[gid].y / coef) + (float)gridY / 2.0f;
    int z = (particles[gid].z / coef) + (float)gridZ / 2.0f;
    if (x >= gridX | x < 0 | y >= gridY | y < 0 | z >= gridZ | z < 0 
          | particles[gid].x != particles[gid].x) {
       /* printf("gid: %d :: %f, %f, %f\n", gid, particles[gid].x, particles[gid].y, particles[gid].z);
        particles[gid].x = 0.0f;
        particles[gid].y = 10.0f;
        particles[gid].z = 0.0f;
        particlesVelocity[gid].x = 0.0f;
        particlesVelocity[gid].y = -1.0f;
        particlesVelocity[gid].z = 0.0f;*/
        return;
    }
    int id = (nbParticlePerCell + 1) * (x + y * gridX + z * gridX * gridY);
    int offset = atomic_inc(gridParticles + id) + 1;//Update particle counter for cell
    if (offset >= nbParticlePerCell) {
   /*     printf("gid: %d, FAILED offset(%d, %d) :: %d, %d, %d\n \
               %f, %f, %f :: %f, %f, %f\n",
           gid,
           offset,
           id,
           x,
           y,
           z,
           particles[gid].x,
           particles[gid].y,
           particles[gid].z,
           particlesVelocity[gid].x,
           particlesVelocity[gid].y,
           particlesVelocity[gid].z
           );*/
        return ;
    }

    gridParticles[id + offset] = gid;
}
