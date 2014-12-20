
char clCode_clppSort_BitonicSortGPU[]=
"#if KEYS_ONLY\n"
"	#define getKey(a) (a)\n"
"	#define getValue(a) (0)\n"
"	#define makeData(k,v) (k)\n"
"#else\n"
"	#define getKey(a) ((a).x)\n"
"	#define getValue(a) ((a).y)\n"
"	#define makeData(k,v) ((uint2)((k),(v)))\n"
"#endif\n"
"#ifndef BLOCK_FACTOR\n"
"#define BLOCK_FACTOR 1\n"
"#endif\n"
"#define ORDER(a,b) { bool swap = reverse ^ (getKey(a)<getKey(b)); KV_TYPE auxa = a; KV_TYPE auxb = b; a = (swap)?auxb:auxa; b = (swap)?auxa:auxb; }\n"
"__kernel\n"
"void ParallelBitonic_B2(__global KV_TYPE* data, int inc, int dir, uint datasetSize)\n"
"{\n"
"	int t = get_global_id(0); // thread index\n"
"	int low = t & (inc - 1); // low order bits (below INC)\n"
"	int i = (t<<1) - low; // insert 0 at position INC\n"
"	bool reverse = ((dir & i) == 0); // asc/desc order\n"
"	data += i; // translate to first value\n"
"	// Load\n"
"	KV_TYPE x0 = data[  0];\n"
"	KV_TYPE x1 = data[inc];\n"
"	// Sort\n"
"	ORDER(x0,x1)\n"
"	// Store\n"
"	data[0  ] = x0;\n"
"	data[inc] = x1;\n"
"}\n"
"__kernel\n"
"void ParallelBitonic_B4(__global KV_TYPE * data,int inc,int dir, uint datasetSize)\n"
"{\n"
"	inc >>= 1;\n"
"	int t = get_global_id(0); // thread index\n"
"	int low = t & (inc - 1); // low order bits (below INC)\n"
"	int i = ((t - low) << 2) + low; // insert 00 at position INC\n"
"	bool reverse = ((dir & i) == 0); // asc/desc order\n"
"	data += i; // translate to first value\n"
"	\n"
"	// Load\n"
"	KV_TYPE x0 = data[    0];\n"
"	KV_TYPE x1 = data[  inc];\n"
"	KV_TYPE x2 = data[2*inc];\n"
"	KV_TYPE x3 = data[3*inc];\n"
"	\n"
"	// Sort\n"
"	ORDER(x0,x2)\n"
"	ORDER(x1,x3)\n"
"	ORDER(x0,x1)\n"
"	ORDER(x2,x3)\n"
"	\n"
"	// Store\n"
"	data[    0] = x0;\n"
"	data[  inc] = x1;\n"
"	data[2*inc] = x2;\n"
"	data[3*inc] = x3;\n"
"}\n"
"#define ORDERV(x,a,b) { bool swap = reverse ^ (getKey(x[a])<getKey(x[b])); KV_TYPE auxa = x[a]; KV_TYPE auxb = x[b]; x[a] = (swap)?auxb:auxa; x[b] = (swap)?auxa:auxb; }\n"
"#define B2V(x,a) { ORDERV(x,a,a+1) }\n"
"#define B4V(x,a) { for (int i4=0;i4<2;i4++) { ORDERV(x,a+i4,a+i4+2) } B2V(x,a) B2V(x,a+2) }\n"
"#define B8V(x,a) { for (int i8=0;i8<4;i8++) { ORDERV(x,a+i8,a+i8+4) } B4V(x,a) B4V(x,a+4) }\n"
"#define B16V(x,a) { for (int i16=0;i16<8;i16++) { ORDERV(x,a+i16,a+i16+8) } B8V(x,a) B8V(x,a+8) }\n"
"__kernel\n"
"void ParallelBitonic_B8(__global KV_TYPE * data,int inc,int dir, uint datasetSize)\n"
"{\n"
"	inc >>= 2;\n"
"	int t = get_global_id(0); // thread index\n"
"	int low = t & (inc - 1); // low order bits (below INC)\n"
"	int i = ((t - low) << 3) + low; // insert 000 at position INC\n"
"	bool reverse = ((dir & i) == 0); // asc/desc order\n"
"	data += i; // translate to first value\n"
"	\n"
"	// Load\n"
"	KV_TYPE x[8];\n"
"	for (int k=0;k<8;k++) x[k] = data[k*inc];\n"
"	\n"
"	// Sort\n"
"	B8V(x,0)\n"
"	\n"
"	// Store\n"
"	for (int k=0;k<8;k++) data[k*inc] = x[k];\n"
"}\n"
"__kernel\n"
"void ParallelBitonic_B16(__global KV_TYPE * data,int inc,int dir, uint datasetSize)\n"
"{\n"
"	inc >>= 3;\n"
"	int t = get_global_id(0); // thread index\n"
"	int low = t & (inc - 1); // low order bits (below INC)\n"
"	int i = ((t - low) << 4) + low; // insert 0000 at position INC\n"
"	bool reverse = ((dir & i) == 0); // asc/desc order\n"
"	data += i; // translate to first value\n"
"	\n"
"	// Load\n"
"	KV_TYPE x[16];\n"
"	for (int k=0;k<16;k++) x[k] = data[k*inc];\n"
"	\n"
"	// Sort\n"
"	B16V(x,0)\n"
"	\n"
"	// Store\n"
"	for (int k=0;k<16;k++) data[k*inc] = x[k];\n"
"}\n"
"__kernel\n"
"void ParallelBitonic_C4(__global KV_TYPE * data, int inc0, int dir, __local KV_TYPE* aux, uint datasetSize)\n"
"{\n"
"	int t = get_global_id(0); // thread index\n"
"	int wgBits = 4 * get_local_size(0) - 1; // bit mask to get index in local memory AUX (size is 4*WG)\n"
"	int inc,low,i;\n"
"	bool reverse;\n"
"	KV_TYPE x[4];\n"
"	\n"
"	// First iteration, global input, local output\n"
"	inc = inc0>>1;\n"
"	low = t & (inc - 1); // low order bits (below INC)\n"
"	i = ((t - low) << 2) + low; // insert 00 at position INC\n"
"	reverse = ((dir & i) == 0); // asc/desc order\n"
"	for (int k = 0; k < 4; k++) x[k] = data[i+k*inc];\n"
"	B4V(x,0);\n"
"	for (int k = 0; k < 4; k++) aux[(i+k*inc) & wgBits] = x[k];\n"
"	barrier(CLK_LOCAL_MEM_FENCE);\n"
"	\n"
"	// Internal iterations, local input and output\n"
"	for(;inc > 1; inc >>= 2)\n"
"	{\n"
"		low = t & (inc - 1); // low order bits (below INC)\n"
"		i = ((t - low) << 2) + low; // insert 00 at position INC\n"
"		reverse = ((dir & i) == 0); // asc/desc order\n"
"		for (int k=0;k<4;k++) x[k] = aux[(i+k*inc) & wgBits];\n"
"		B4V(x,0);\n"
"		barrier(CLK_LOCAL_MEM_FENCE);\n"
"		for (int k=0;k<4;k++) aux[(i+k*inc) & wgBits] = x[k];\n"
"		barrier(CLK_LOCAL_MEM_FENCE);\n"
"	}\n"
"	\n"
"	// Final iteration, local input, global output, INC=1\n"
"	i = t << 2;\n"
"	reverse = ((dir & i) == 0); // asc/desc order\n"
"	for (int k = 0;k < 4; k++) x[k] = aux[(i+k) & wgBits];\n"
"	B4V(x,0);\n"
"	for (int k = 0;k < 4; k++) data[i+k] = x[k];\n"
"}\n"
;