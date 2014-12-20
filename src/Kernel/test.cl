__kernel void hash_particles(
    __global float4* position,
    __global int2* voxelparticle,
    float4 blf,
    float4 trb,
    float h
    )
{
    unsigned int id = get_global_id(0);

    int voxelsX = fabs((float)((trb.x - blf.x)/(2*h))) + 0.5;
    int voxelsY = fabs((float)((trb.y - blf.y)/(2*h))) + 0.5;
    int voxelsZ = fabs((float)((trb.z - blf.z)/(2*h))) + 0.5;

    int voxelX = min((int)((position[id].x-blf.x)/(2*h)), voxelsX-1);
    int voxelY = min((int)((position[id].y-blf.y)/(2*h)), voxelsY-1);
    int voxelZ = min((int)((position[id].z-blf.z)/(2*h)), voxelsZ-1);

    int voxelId = voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
    position[id].w = voxelId;
    voxelparticle[id] = (int2)(voxelId, id);
}

__kernel void sort_post_pass(
    __global float4* position,
    __global float4* velocity,
    __global float4* sortedPosition,
    __global float4* sortedVelocity,
    __global int2* voxelparticle
    )
{
    unsigned int id = get_global_id(0);
    sortedPosition[id] = position[voxelparticle[id].y];
    sortedVelocity[id] = velocity[voxelparticle[id].y];
}

unsigned int binsearch(
    const unsigned int voxelId,
    const int2* array,
    const unsigned int length
    )
{
    int l = 0, r = length - 1, s;
    while(l <= r)
    {
        s = (l + r) / 2;
        if(array[s].x >= voxelId)
            p = s - 1;
        else
            l = s + 1;
    }
    return l;
}

__kernel void indexx(
    __global uint* gridVoxelIndex,
    __global int2* voxelparticle,
    const unsigned int length
    )
{
    unsigned int id = get_global_id(0);
    unsigned int lowestParticle = binsearch(id, voxelparticle, length);
    if(voxelparticle[lowestParticle].x == id)
        gridVoxelIndex[id] = lowestParticle;
    else
        gridVoxelIndex[id] = -1;
}

__kernel void index_post_pass(
    __global uint* gridVoxelIndex,
    const unsigned int length
    )
{
    unsigned int id = get_global_id(0);
    if(gridVoxelIndex[id] = -1)
    {
        int i;
        for(i = id + 1; i < length && gridVoxelIndex[i] == -1; ++i);
        if(i < length)
            gridVoxelIndex[id] = gridVoxelIndex[i];
    }
}

__kernel void histogram(
    __global int2* voxelparticle,
    __global int* histogram,
    const int length,
    const int iteration)
{
    unsigned int id = get_global_id(0);
    int val = (voxelparticle[id].x>>(iteration*length))%(1<<length);
    histogram[val]+=1;
}





