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





