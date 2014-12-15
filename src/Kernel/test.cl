float distanceSq(float3 f1, float3 f2)
{
    return distance(f1, f2)*distance(f1, f2);
}

__kernel void calculate_density(
    __global float4* position,
    __global float* density,
    __global float* pressure,
    __global float3* velocity,
    __global float3* acceleration,
    float mass,
    unsigned int width,
    unsigned int height,
    float time)
{
    unsigned int id = get_global_id(0);
    float rcut = 1 * pow(165.0 / (4*M_PI*100),1.0/3.0);
    int i, j;
    float3 pos = position[id].xyz;
    for(i=0;i<get_global_size(0);i++)
    {
        if(distance(position[id].xyz, position[i].xyz) < rcut)
        {
            density[id] += mass * 315.0/(64.0*M_PI*pow(rcut, 9))
                            *pow(rcut*rcut-distanceSq(pos, position[i].xyz), 3);
        }
    }
}




