int     getCellId(int x, int y, int z, int nbParticlePerCell, int gridX, int sizePlane)
{
    return (nbParticlePerCell + 1) * (x + y * gridX + z * sizePlane);
}

int    findNeighborsPi(__global int *gridParticles,
                    int             currentCell,
                    __global int    *neighbours,//point to current case in neighbors
                    __global float4 *particles,//List of all particles
                    int             nbNeighbors,
                    int             currentParticleId,
                    float4          currentParticle,
                    int             display,
                    int             x,
                    int             y,
                    int             z)
{
    int     workingId;
    float4  neighbor;
    float       dist;

    int nbParticleInCell = gridParticles[currentCell];
    
    for (int i = 1; i <= nbParticleInCell; i++) {
        workingId = gridParticles[currentCell + i];
        if (workingId == currentParticleId)
            continue ;
        if (nbNeighbors > 198)
            return nbNeighbors;
        dist = distance(currentParticle, particles[workingId]);
        if (dist > RANGE)
            continue ;
        neighbours[nbNeighbors + 1] = workingId;
        nbNeighbors++;
    }
    return nbNeighbors;
}

__kernel void   findNeighbors(
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
        __global int        *neighbors
        )
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return;
    int     x = particlesProjection[gid].x / coef + (float)gridX / 2.0f;
    int     y = particlesProjection[gid].y / coef + (float)gridX / 2.0f;
    int     z = particlesProjection[gid].z / coef + (float)gridX / 2.0f;
    int     sizePlane = gridX * gridY;
    int     currentCell;
    float3  currentParticle;
    int     xGrid;
    int     yGrid;
    int     zGrid;
    int     xGridStop;
    int     yGridStop;
    int     zGridStop;
    int     nbNeighbors = 0;

    xGridStop = min(x + 1, gridX - 1);
    yGridStop = min(y + 1, gridY - 1);
    zGridStop = min(z + 1, gridZ - 1);

    xGrid = max(x - 1, 0);
    for (; xGrid <= xGridStop; xGrid++) {
        yGrid = max(y - 1, 0);
        for (; yGrid <= yGridStop; yGrid++) {
            zGrid = max(z - 1, 0);
            for (; zGrid <= zGridStop; zGrid++) {
                currentCell = getCellId(
                                        xGrid,
                                        yGrid,
                                        zGrid,
                                        nbParticlePerCell,
                                        gridX,
                                        sizePlane);
                nbNeighbors = findNeighborsPi(
                    gridParticles,
                    currentCell,
                    neighbors + gid * 200,
                    particlesProjection,
                    nbNeighbors,
                    gid,
                    particlesProjection[gid],
                    yGrid == 10 && zGrid == 10,
                    xGrid, yGrid, zGrid);
            }
        }
    }
    neighbors[gid * 200] = nbNeighbors;
}
