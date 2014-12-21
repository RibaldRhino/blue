__kernel void hash_particles(
    __global float4* position,
    __global int2* voxelparticle,
    float4 lbf, //left bottom front
    float4 rtb, //right top back
    float h
    )
{
    unsigned int id = get_global_id(0);

    int voxelsX = fabs((float)((rtb.x - lbf.x)/(2*h))) + 0.5;
    int voxelsY = fabs((float)((rtb.y - lbf.y)/(2*h))) + 0.5;
    int voxelsZ = fabs((float)((rtb.z - lbf.z)/(2*h))) + 0.5;

    int voxelX = min((int)((position[id].x-lbf.x)/(2*h)), voxelsX-1);
    int voxelY = min((int)((position[id].y-lbf.y)/(2*h)), voxelsY-1);
    int voxelZ = min((int)((position[id].z-lbf.z)/(2*h)), voxelsZ-1);

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
    while(l < r)
    {
        s = (l + r) / 2;
        if(array[s].x >= voxelId)
            r = s;
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
    __global int* gridVoxelIndex
    )
{
    unsigned int id = get_global_id(0);
    if(gridVoxelIndex[id] == -1)
    {
        int i;
        for(i = id + 1; gridVoxelIndex[i] == -1; ++i);
        gridVoxelIndex[id] = gridVoxelIndex[i];
    }
}


__kernel void find_voxel_neighbours(
    __global int* voxelNeighbourMap, //64 x voxelCount
    float4 lbf,
    float4 rtb,
    float h
    )
{
    int voxelId = get_global_id(0);

    int voxelsX = fabs((float)((rtb.x - lbf.x)/(2*h))) + 0.5;
    int voxelsY = fabs((float)((rtb.y - lbf.y)/(2*h))) + 0.5;
    int voxelsZ = fabs((float)((rtb.z - lbf.z)/(2*h))) + 0.5;

    int voxelZ =  voxelId / (voxelsX*voxelsY);
    int voxelY =  (voxelId / voxelsX) % voxelsY;
    int voxelX = voxelId % voxelsX;

    voxelNeighbourMap[64*voxelId+0] = (voxelY>0 && voxelX>0)?               (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+1] = (voxelY>0)?                           (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+2] = (voxelX>0)?                           (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+3] =                                       (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
    voxelNeighbourMap[64*voxelId+4] = (voxelY>0 && voxelX>0)?               voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+5] = (voxelY>0)?                           voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+6] = (voxelX>0)?                           voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+7] =                                       voxelId;

    voxelNeighbourMap[64*voxelId+8+0] = (voxelY>0 && voxelX>0)?             voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+8+1] = (voxelY>0)?                         voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+8+2] = (voxelX>0)?                         voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+8+3] =                                     voxelId;
    voxelNeighbourMap[64*voxelId+8+4] = (voxelY>0 && voxelX>0)?             (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+8+5] = (voxelY>0)?                         (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+8+6] = (voxelX>0)?                         (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+8+7] =                                     (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;

    voxelNeighbourMap[64*voxelId+16+0] = (voxelX>0)?                        (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+16+1] =                                    (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
    voxelNeighbourMap[64*voxelId+16+2] = (voxelY<voxelsY-1 && voxelX>0)?    (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+16+3] = (voxelY<voxelsY-1)?                (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+16+4] = (voxelX>0)?                        voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+16+5] =                                    voxelId;
    voxelNeighbourMap[64*voxelId+16+6] = (voxelY<voxelsY-1 && voxelX>0)?    voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+16+7] = (voxelY<voxelsY-1)?                voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;

    voxelNeighbourMap[64*voxelId+24+0] = (voxelX>0)?                        voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+24+1] =                                    voxelId;
    voxelNeighbourMap[64*voxelId+24+2] = (voxelY<voxelsY-1 && voxelX>0)?    voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+24+3] = (voxelY<voxelsY-1)?                voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+24+4] = (voxelX>0)?                        (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+24+5] =                                    (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
    voxelNeighbourMap[64*voxelId+24+6] = (voxelY<voxelsY-1 && voxelX>0)?    (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX-1 : -1;
    voxelNeighbourMap[64*voxelId+24+7] = (voxelY<voxelsY-1)?                (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;

    voxelNeighbourMap[64*voxelId+32+0] = (voxelY>0)?                        (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+32+1] = (voxelY>0 && voxelX<voxelsX-1)?    (voxelZ-1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+32+2] =                                    (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
    voxelNeighbourMap[64*voxelId+32+3] = (voxelX<voxelsX-1)?                (voxelZ-1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+32+4] = (voxelY>0)?                        voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+32+5] = (voxelY>0 && voxelX<voxelsX-1)?    voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+32+6] =                                    voxelId;
    voxelNeighbourMap[64*voxelId+32+7] = (voxelX<voxelsX-1)?                voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1 : -1;

    voxelNeighbourMap[64*voxelId+40+0] = (voxelY>0)?                        voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+40+1] = (voxelY>0 && voxelX<voxelsX-1)?    voxelZ * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+40+2] =                                    voxelId;
    voxelNeighbourMap[64*voxelId+40+3] = (voxelX<voxelsX-1)?                voxelZ * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+40+4] = (voxelY>0)?                        (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+40+5] = (voxelY>0 && voxelX<voxelsX-1)?    (voxelZ+1) * (voxelsX*voxelsY) + (voxelY-1) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+40+6] =                                    (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX;
    voxelNeighbourMap[64*voxelId+40+7] = (voxelX<voxelsX-1)?                (voxelZ+1) * (voxelsX*voxelsY) + voxelY * (voxelsX) + voxelX+1 : -1;

    voxelNeighbourMap[64*voxelId+48+0] =                                            (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
    voxelNeighbourMap[64*voxelId+48+1] = (voxelX<voxelsX-1)?                        (voxelZ-1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+48+2] = (voxelY<voxelsY-1)?                        (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+48+3] = (voxelX<voxelsX-1 && voxelY<voxelsY-1)?    (voxelZ-1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+48+4] =                                            voxelId;
    voxelNeighbourMap[64*voxelId+48+5] = (voxelX<voxelsX-1)?                        voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+48+6] = (voxelY<voxelsY-1)?                        voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+48+7] = (voxelX<voxelsX-1 && voxelY<voxelsY-1)?    voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1 : -1;

    voxelNeighbourMap[64*voxelId+56+0] =                                            voxelId;
    voxelNeighbourMap[64*voxelId+56+1] = (voxelX<voxelsX-1)?                        voxelZ * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+56+2] = (voxelY<voxelsY-1)?                        voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+56+3] = (voxelX<voxelsX-1 && voxelY<voxelsY-1)?    voxelZ * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+56+4] =                                            (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX;
    voxelNeighbourMap[64*voxelId+56+5] = (voxelX<voxelsX-1)?                        (voxelZ+1) * (voxelsX*voxelsY) + (voxelY) * (voxelsX) + voxelX+1 : -1;
    voxelNeighbourMap[64*voxelId+56+6] = (voxelY<voxelsY-1)?                        (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX : -1;
    voxelNeighbourMap[64*voxelId+56+7] = (voxelX<voxelsX-1 && voxelY<voxelsY-1)?    (voxelZ+1) * (voxelsX*voxelsY) + (voxelY+1) * (voxelsX) + voxelX+1 : -1;
    int i=0;
    for(i=0;i<64;i++)
    {
        if(voxelNeighbourMap[64*voxelId+i] > (voxelsX*voxelsY*voxelsZ-1) || voxelNeighbourMap[64*voxelId+i] < 0) voxelNeighbourMap[64*voxelId+i]=-1;
    }
}

__kernel void neighbour_map(
    __global int* neighbourMap,
    __global int2* voxelParticle,
    __global int* gridVoxelIndex,
    __global float4* sortedPositions,
    //__global int* randomInts, //should be particle_count elements
    __global int* voxelNeighbourMap,
    int neighboursToFind,
    float4 lbf,
    float4 rtb,
    float h
    )
{

    int id = get_global_id(0);
    int particle_count = get_global_size(0);

    uint foundCount = 0;

    int2 offsetToIndex[9];
    int count = 0;
    uint skip = 16777259;

    int i=0;
    int j=0;
    int iter=0;
    int voxelId = voxelParticle[id].x;


    int voxelsX = fabs((float)((rtb.x - lbf.x)/(2*h))) + 0.5;
    int voxelsY = fabs((float)((rtb.y - lbf.y)/(2*h))) + 0.5;
    int voxelsZ = fabs((float)((rtb.z - lbf.z)/(2*h))) + 0.5;

    int voxelZ = voxelId / (voxelsX*voxelsY);
    int voxelY = voxelId / voxelsX % (voxelsX*voxelsY);
    int voxelX = voxelId % voxelsX;

    int part = 0;
    if(voxelX * 2*h + h < sortedPositions[id].x) part+=4;
    if(voxelY * 2*h + h < sortedPositions[id].y) part+=2;
    if(voxelZ * 2*h + h < sortedPositions[id].z) part+=1;

    int voxelNeighbour;
    for(i=0;i<8;i++)
    {
        voxelNeighbour = voxelNeighbourMap[voxelId*64+8*part+i];
        if(voxelNeighbour != -1)
        {
            offsetToIndex[iter].x = gridVoxelIndex[voxelNeighbour+1] - gridVoxelIndex[voxelNeighbour];
            offsetToIndex[iter].y = gridVoxelIndex[voxelNeighbour];
            iter++;
        }
    }
    //neighbourMap[id*32+20] = iter;

    int tmp;
    count=offsetToIndex[0].x;
    offsetToIndex[0].x = 0;
    for(i=1;i<iter;i++)
    {
        tmp = offsetToIndex[i].x;
        offsetToIndex[i].x = count;
        count+=tmp;
    }
    offsetToIndex[i] = count;
    //for(i=0;i<9;i++) {
     //   neighbourMap[id*32+2*i]=offsetToIndex[i].x;
    //    neighbourMap[id*32+2*i+1]=offsetToIndex[i].y;
    //}
    //neighbourMap[id*32+21] = count;
    //iter=randomInts[id]%count;
    uint qq = 0;
    int particleIndex;
    for(i=0;i<count && foundCount < neighboursToFind;i++)
    {
        for(j=0;offsetToIndex[j].x<=qq;j++);
        j--;
        particleIndex = offsetToIndex[j].y+(qq-offsetToIndex[j].x);
        if(distance(sortedPositions[id], sortedPositions[particleIndex]) < h) {
           neighbourMap[id*neighboursToFind+foundCount]=particleIndex;
           foundCount=foundCount+1;
        }
        qq = (qq + skip)%count;
    }
}