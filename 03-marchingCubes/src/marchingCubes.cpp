/****************************************************
  This code is ported to SYCL from the original 
  Marching Cubes implementation in the Nvidia CUDA SDK
  https://github.com/NVIDIA/cuda-samples/tree/master/Samples/5_Domain_Specific/marchingCubes
*****************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sycl/sycl.hpp>

typedef unsigned int uint;
typedef unsigned char uchar;

// The number of threads to use for triangle generation (limited by shared memory size)
#define NTHREADS 32

// SYCL kernel launchers
extern "C" void launch_classifyVoxel(sycl::queue &q, 
		sycl::range<3> globalRange,
        uint *voxelVerts, 
		uint *voxelOccupied, 
		uchar *volume,
        sycl::uint3 gridSize, 
		sycl::uint3 gridSizeShift,
        sycl::uint3 gridSizeMask, 
		uint numVoxels,
        sycl::float3 voxelSize, 
		float isoValue);

extern "C" void launch_compactVoxels(sycl::queue &q, 
		sycl::range<3> globalRange,
        uint *compactedVoxelArray, 
		uint *voxelOccupied,
        uint *voxelOccupiedScan, 
		uint numVoxels);

extern "C" void launch_generateTriangles(sycl::queue &q, 
		sycl::range<3> globalRange,
        sycl::float4 *pos, 
		sycl::float4 *norm,
        uint *compactedVoxelArray,
		uint *numVertsScanned,
        sycl::uint3 gridSize, 
		sycl::uint3 gridSizeShift,
        sycl::uint3 gridSizeMask, 
		sycl::float3 voxelSize,
        float isoValue, 
		uint activeVoxels, 
		uint maxVerts);

extern "C" void allocateTextures(sycl::queue &q, 
		uint **d_edgeTable, 
		uint **d_triTable,
        uint **d_numVertsTable);

extern "C" void createVolumeTexture(uchar *d_volume, size_t buffSize);
extern "C" void destroyAllTextureObjects();
extern "C" void ThrustScanWrapper(unsigned int *output, 
		unsigned int *input,
        unsigned int numElements);

// forward declarations
void initMC(int argc, char **argv, sycl::queue q);
void computeIsosurface(sycl::queue q);
void cleanup(sycl::queue q);
void SaveToCSV(sycl::queue &q, const char *filename);

const char *volumeFilename = "../data/Bucky.raw";

sycl::uint3 gridSizeLog2 = sycl::uint3(5, 5, 5);
sycl::uint3 gridSizeShift;
sycl::uint3 gridSize;
sycl::uint3 gridSizeMask;

sycl::float3 voxelSize;
uint numVoxels = 0;
uint maxVerts = 0;
uint activeVoxels = 0;
uint totalVerts = 0;

float isoValue = 100.0f;

sycl::float4 *d_pos = nullptr, *d_normal = nullptr;

uchar *d_volume = nullptr;
uint  *d_voxelVerts = nullptr;
uint  *d_voxelVertsScan = nullptr;
uint  *d_voxelOccupied = nullptr;
uint  *d_voxelOccupiedScan = nullptr;
uint  *d_compVoxelArray = nullptr;

// tables
uint *d_numVertsTable = nullptr;
uint *d_edgeTable = nullptr;
uint *d_triTable = nullptr;

bool g_bValidate = false;

////////////////////////////////////////////////////////////////////////////////
// Load raw data from disk
////////////////////////////////////////////////////////////////////////////////
uchar *loadRawFile(const char *filename, int size) {
  FILE *fp = fopen(filename, "rb");

  if (!fp) {
    fprintf(stderr, "Error opening file '%s'\n", filename);
    return 0;
  }

  uchar *data = (uchar *)malloc(size);
  size_t read = fread(data, 1, size, fp);
  fclose(fp);

  printf("Read '%s', %d bytes\n", filename, (int)read);

  return data;
}

void dumpFile(void *dData, int data_bytes, const char *file_name) {
  void *hData = malloc(data_bytes);
  sycl::queue q;
  q.memcpy(hData, dData, data_bytes).wait();
  FILE *fp = fopen(file_name, "wb");
  fwrite(hData, 1, data_bytes, fp);
  fclose(fp);
  free(hData);
}

template <class T>
void dumpBuffer(T *d_buffer, int nelements, int size_element) {
  uint bytes = nelements * size_element;
  T *h_buffer = (T *)malloc(bytes);
  sycl::queue q;
  q.memcpy(h_buffer, d_buffer, bytes).wait();

  for (int i = 0; i < nelements; i++) {
    printf("%d: %u\n", i, h_buffer[i]);
  }

  printf("\n");
  free(h_buffer);
}

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {

  printf("[%s] - Starting...\n", argv[0]);

  // Create SYCL queue
  //auto myQueue = sycl::queue{sycl::gpu_selector_v};   
  auto myQueue = sycl::queue{sycl::default_selector_v}; 
    
  // Initialize buffers for Marching Cubes
  initMC(argc, argv, myQueue);

  computeIsosurface(myQueue);
    
  cleanup(myQueue);

  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
// initialize marching cubes
////////////////////////////////////////////////////////////////////////////////
void initMC(int argc, char **argv, sycl::queue q) {
  printf("Starting `initMC`\n");
    
  gridSize = sycl::uint3(1 << gridSizeLog2.x(), 1 << gridSizeLog2.y(), 1 << gridSizeLog2.z());
  gridSizeMask = sycl::uint3(gridSize.x() - 1, gridSize.y() - 1, gridSize.z() - 1);
  gridSizeShift = sycl::uint3(0, gridSizeLog2.x(), gridSizeLog2.x() + gridSizeLog2.y());

  numVoxels = gridSize.x() * gridSize.y() * gridSize.z();
  voxelSize = sycl::float3(2.0f / gridSize.x(), 2.0f / gridSize.y(), 2.0f / gridSize.z());
  maxVerts = gridSize.x() * gridSize.y() * 100;

  printf("grid: %d x %d x %d = %d voxels\n", gridSize.x(), gridSize.y(), gridSize.z(), numVoxels);
  printf("max verts = %d\n", maxVerts);

  // load volume data
  printf("Loading volume data\n");
  int size = gridSize.x() * gridSize.y() * gridSize.z() * sizeof(uchar);
  uchar *volume = loadRawFile(volumeFilename, size);

  d_volume = static_cast<uchar *>(sycl::malloc_device(size, q));
  q.memcpy(d_volume, volume, size).wait();
  free(volume);

  createVolumeTexture(d_volume, size);
   
  // allocate textures
  allocateTextures(q, &d_edgeTable, &d_triTable, &d_numVertsTable);

  // allocate device memory
  unsigned int memSize = sizeof(uint) * numVoxels;
  d_voxelVerts = static_cast<uint *>(sycl::malloc_device(memSize, q));
  d_voxelVertsScan = static_cast<uint *>(sycl::malloc_device(memSize, q));
  d_voxelOccupied = static_cast<uint *>(sycl::malloc_device(memSize, q));
  d_voxelOccupiedScan = static_cast<uint *>(sycl::malloc_device(memSize, q));
  d_compVoxelArray = static_cast<uint *>(sycl::malloc_device(memSize, q));
  d_pos = static_cast<sycl::float4 *>(sycl::malloc_device(maxVerts * sizeof(float) * 4, q));
  d_normal = static_cast<sycl::float4 *>(sycl::malloc_device(maxVerts * sizeof(float) * 4, q));
    
  printf("Finished `initMC`\n");
}

#define DEBUG_BUFFERS 0

////////////////////////////////////////////////////////////////////////////////
//! Run the **SYCL** part of the computation
////////////////////////////////////////////////////////////////////////////////
void computeIsosurface(sycl::queue q) {
  
  int maxThreadsPerBlock = 1024;
  int threads = std::min(128, maxThreadsPerBlock);
  int numBlocks = (numVoxels + threads - 1) / threads;

  numBlocks = std::min(numBlocks, 65535);
  
  sycl::range<3> globalRange(numBlocks, 1, threads);

  printf("isoValue is: %f\n", isoValue);
    
  printf("Starting `launch_classifyVoxel`\n");
  // calculate number of vertices need per voxel
  launch_classifyVoxel(q, globalRange, d_voxelVerts, d_voxelOccupied, d_volume,
                       gridSize, gridSizeShift, gridSizeMask, numVoxels,
                       voxelSize, isoValue);
  printf("Finished `launch_classifyVoxel`\n");

#if DEBUG_BUFFERS
  printf("voxelVerts:\n");
  dumpBuffer(d_voxelVerts, numVoxels, sizeof(uint));
#endif

  // scan voxel occupied array
  ThrustScanWrapper(d_voxelOccupiedScan, d_voxelOccupied, numVoxels);

#if DEBUG_BUFFERS
  printf("voxelOccupiedScan:\n");
  dumpBuffer(d_voxelOccupiedScan, numVoxels, sizeof(uint));
#endif

  // read back values to calculate total number of non-empty voxels
  // since we are using an exclusive scan, the total is the last value of
  // the scan result plus the last value in the input array
  {
    uint lastElement, lastScanElement;
    q.memcpy(&lastElement, d_voxelOccupied + numVoxels - 1, sizeof(uint)).wait();
    q.memcpy(&lastScanElement, d_voxelOccupiedScan + numVoxels - 1, sizeof(uint)).wait();
    activeVoxels = lastElement + lastScanElement;
    printf("Number of active voxels: %d\n", activeVoxels);
  }

  if (activeVoxels == 0) {
    // return if there are no full voxels
    totalVerts = 0;
    return;
  }

  // compact voxel index array
  printf("Starting `launch_compactVoxels`\n");  
  launch_compactVoxels(q, 
             globalRange, 
             d_compVoxelArray, 
             d_voxelOccupied,
             d_voxelOccupiedScan, 
             numVoxels);
  printf("Finished `launch_compactVoxels`\n");

  // scan voxel vertex count array
  ThrustScanWrapper(d_voxelVertsScan, d_voxelVerts, numVoxels);

#if DEBUG_BUFFERS
  printf("voxelVertsScan:\n");
  dumpBuffer(d_voxelVertsScan, numVoxels, sizeof(uint));
#endif

  // readback total number of vertices
  {
    uint lastElement, lastScanElement;
    q.memcpy(&lastElement, d_voxelVerts + numVoxels - 1, sizeof(uint)).wait();
    q.memcpy(&lastScanElement, d_voxelVertsScan + numVoxels - 1, sizeof(uint)).wait();
    totalVerts = lastElement + lastScanElement;
 
    printf("Total number of vertices: %d\n", totalVerts);
  }

  // generate triangles
#if SKIP_EMPTY_VOXELS
  sycl::range<3> globalRange2((int)ceil(activeVoxels / (float)NTHREADS), 1, NTHREADS);
#else
  sycl::range<3> globalRange2((int)ceil(numVoxels / (float)NTHREADS), 1, NTHREADS);
#endif

  while (globalRange2[0] > 65535) {
    globalRange2[0] /= 2;
    globalRange2[1] *= 2;
  }

  printf("Starting `launch_generateTriangles`\n");
  launch_generateTriangles(q, globalRange2, d_pos, d_normal, d_compVoxelArray,
                           d_voxelVertsScan, gridSize, gridSizeShift,
                           gridSizeMask, voxelSize, isoValue, activeVoxels,
                           maxVerts);
  printf("Finished `launch_generateTriangles`\n");      
}

void cleanup(sycl::queue q) 
{
  sycl::free(d_pos, q);
  sycl::free(d_normal, q);
  destroyAllTextureObjects();
  sycl::free(d_edgeTable, q);
  sycl::free(d_triTable, q);
  sycl::free(d_numVertsTable, q);
  sycl::free(d_voxelVerts, q);
  sycl::free(d_voxelVertsScan, q);
  sycl::free(d_voxelOccupied, q);
  sycl::free(d_voxelOccupiedScan, q);
  sycl::free(d_compVoxelArray, q);

  if (d_volume) 
  {
    sycl::free(d_volume, q);
  }
}



