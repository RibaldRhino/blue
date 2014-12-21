__kernel void hash_particles(
    __global float4* position,
    __global int2* voxelparticle,
    float4 blf, //bottom left front
    float4 trb, //top right back
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
    const uint voxelId,
    __global int2* array,
    const uint length
    )
{
    int l = 0, r = length - 1, s;
    while(l <= r)
    {
        s = (l + r) / 2;
        if(array[s].x >= voxelId)
            r = s - 1;
        else
            l = s + 1;
    }
    return l;
}

__kernel void indexx(
    __global int* gridVoxelIndex,
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
    __global int* gridVoxelIndex,
    const unsigned int length
    )
{
    unsigned int id = get_global_id(0);
    if(gridVoxelIndex[id] == -1)
    {
        int i;
        for(i = id + 1; i < length && gridVoxelIndex[i] == -1; ++i);
        if(i < length)
            gridVoxelIndex[id] = gridVoxelIndex[i];
        else
            gridVoxelIndex[id] = length;
    }
}

__constant int primes[7] = {67, 521, 4099, 32771, 262147, 2097169, 16777259};

__kernel void find_neighbours(
    __global int* gridVoxelIndex,
    __global int2* voxelParticle,
    __global float4* positions,
    __global int* neighbourMap,
    int neighboursToFind,
    float4 blf,
    float4 trb,
    float h
    )
{

    int lid = get_local_id(0);
    int lws = get_local_size(0);

    int id = get_global_id(0) / neighboursToFind;
    int particle_count = get_global_size(0) / neighboursToFind;

    __local uint found_count;
    //__local uint skip;
    //__local uint prime;
    __local int search_voxels[8];
    if(lid==0)
    {
        int i=0;
        /*uint A = 5293;
        uint B = 2345;
        uint C = 3665;
        skip = A*particle_count*particle_count + B*particle_count + C;

        for(i=6;i>=0;i--)
        {
            if(primes[i] > particle_count)
            {
                prime = primes[i];
                break;
            }
        }*/

        int voxelId = voxelParticle[id].x;

        int voxelsX = fabs((float)((trb.x - blf.x)/(2*h))) + 0.5;
        int voxelsY = fabs((float)((trb.y - blf.y)/(2*h))) + 0.5;
        int voxelsZ = fabs((float)((trb.z - blf.z)/(2*h))) + 0.5;

        int voxelZ =  voxelId / (voxelsX*voxelsY);
        int voxelY =  voxelId / voxelsX % (voxelsX*voxelsY);
        int voxelX = voxelId % voxelsX;

        int part = 0;
        if(voxelX * 2*h + h < positions[id].x) part+=4;
        if(voxelY * 2*h + h < positions[id].y) part+=2;
        if(voxelZ * 2*h + h < positions[id].z) part+=1;

        int maxVal = voxelsX*voxelsY*voxelsZ-1;

        switch(part)
        {
            case 0:
                search_voxels[0] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1;
                search_voxels[1] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[2] = (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1;
                search_voxels[3] = (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
                search_voxels[4] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1;
                search_voxels[5] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[6] = voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1;
                search_voxels[7] = voxelId;
                break;
            case 1:
                search_voxels[0] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1;
                search_voxels[1] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[2] = voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1;
                search_voxels[3] = voxelId;
                search_voxels[4] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1;
                search_voxels[5] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[6] = (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1;
                search_voxels[7] = (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
                break;
            case 2:
                search_voxels[0] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1;
                search_voxels[1] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
                search_voxels[2] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1;
                search_voxels[3] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                search_voxels[4] = voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1;
                search_voxels[5] = voxelId;
                search_voxels[6] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1;
                search_voxels[7] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                break;
            case 3:
                search_voxels[0] = voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1;
                search_voxels[1] = voxelId;
                search_voxels[2] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1;
                search_voxels[3] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                search_voxels[4] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1;
                search_voxels[5] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
                search_voxels[6] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1;
                search_voxels[7] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                break;
            case 4:
                search_voxels[0] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[1] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1;
                search_voxels[2] = (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
                search_voxels[3] = (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1;
                search_voxels[4] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[5] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1;
                search_voxels[6] = voxelId;
                search_voxels[7] = voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1;
                break;
            case 5:
                search_voxels[0] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[1] = voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1;
                search_voxels[2] = voxelId;
                search_voxels[3] = voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1;
                search_voxels[4] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX;
                search_voxels[5] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1;
                search_voxels[6] = (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
                search_voxels[7] = (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1;
                break;
            case 6:
                search_voxels[0] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
                search_voxels[1] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1;
                search_voxels[2] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                search_voxels[3] = (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1;
                search_voxels[4] = voxelId;
                search_voxels[5] = voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1;
                search_voxels[6] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                search_voxels[7] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1;
                break;
            case 7:
                search_voxels[0] = voxelId;
                search_voxels[1] = voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1;
                search_voxels[2] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                search_voxels[3] = voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1;
                search_voxels[4] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
                search_voxels[5] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1;
                search_voxels[6] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX;
                search_voxels[7] = (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1;
                break;
        }
        for(i=0;i<8;i++)
        {
            if(search_voxels[i] > maxVal || search_voxels[i] < 0) search_voxels[i]=-1;
        }
        neighbourMap[neighboursToFind*id+0] = h;
        neighbourMap[neighboursToFind*id+1] = voxelsX;
        neighbourMap[neighboursToFind*id+2] = voxelsY;
        neighbourMap[neighboursToFind*id+3] = voxelsZ;
        neighbourMap[neighboursToFind*id+4] = voxelId;
        neighbourMap[neighboursToFind*id+5] = search_voxels[0];
        neighbourMap[neighboursToFind*id+6] = search_voxels[1];
        neighbourMap[neighboursToFind*id+7] = search_voxels[2];
        neighbourMap[neighboursToFind*id+8] = search_voxels[3];
        neighbourMap[neighboursToFind*id+9] = search_voxels[4];
        neighbourMap[neighboursToFind*id+10] = search_voxels[5];
        neighbourMap[neighboursToFind*id+11] = search_voxels[6];
        neighbourMap[neighboursToFind*id+12] = search_voxels[7];
    }
    barrier(CLK_LOCAL_MEM_FENCE);

}
