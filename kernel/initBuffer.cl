__kernel void   initBuffer(__global int *grid,
                            int maxGID)
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return ;
    grid[gid] = 0;
}
