#include <sycl/sycl.hpp>
#include <oneapi/dpl/execution>
#include <oneapi/dpl/algorithm>
#include <cstring>
#include "tables.h"

typedef unsigned int uint;
typedef unsigned char uchar;


// textures containing look-up tables
sycl::buffer<uint, 1> *triTableBuf;
sycl::buffer<uint, 1> *numVertsTableBuf;

// volume data
sycl::buffer<uchar, 1> *volumeBuf;

extern "C" void allocateTextures(sycl::queue &q, 
		                         uint **d_edgeTable, 
				                 uint **d_triTable,
                                 uint **d_numVertsTable) 
{
  *d_edgeTable = static_cast<uint *>(sycl::malloc_device(256 * sizeof(uint), q));
  q.memcpy(*d_edgeTable, edgeTable, 256 * sizeof(uint)).wait();

  *d_triTable = static_cast<uint *>(sycl::malloc_device(256 * 16 * sizeof(uint), q));
  q.memcpy(*d_triTable, triTable, 256 * 16 * sizeof(uint)).wait();

  triTableBuf = new sycl::buffer<uint, 1>(*d_triTable, sycl::range<1>(256 * 16));

  *d_numVertsTable = static_cast<uint *>(sycl::malloc_device(256 * sizeof(uint), q));
  q.memcpy(*d_numVertsTable, numVertsTable, 256 * sizeof(uint)).wait();

  numVertsTableBuf = new sycl::buffer<uint, 1>(*d_numVertsTable, sycl::range<1>(256));
}

extern "C" void createVolumeTexture(uchar *d_volume, 
		                            size_t buffSize) 
{
  volumeBuf = new sycl::buffer<uchar, 1>(d_volume, sycl::range<1>(buffSize));
}

extern "C" void destroyAllTextureObjects() 
{
  delete triTableBuf;
  delete numVertsTableBuf;
  delete volumeBuf;
}

float tangle(float x, 
	         float y, 
	         float z) 
{
  x *= 3.0f;
  y *= 3.0f;
  z *= 3.0f;
  return (x * x * x * x - 5.0f * x * x + y * y * y * y - 5.0f * y * y +
          z * z * z * z - 5.0f * z * z + 11.8f) * 0.2f + 0.5f;
}

float fieldFunc(sycl::float3 p) 
{
  return tangle(p.x(), p.y(), p.z());
}

sycl::float4 fieldFunc4(sycl::float3 p) 
{
  float v = tangle(p.x(), p.y(), p.z());
  const float d = 0.001f;
  float dx = tangle(p.x() + d, p.y(), p.z()) - v;
  float dy = tangle(p.x(), p.y() + d, p.z()) - v;
  float dz = tangle(p.x(), p.y(), p.z() + d) - v;
  return sycl::float4{dx, dy, dz, v};
}

float sampleVolume(sycl::accessor<uchar, 1, 
                   sycl::access_mode::read> volumeAcc, 
		           uchar *data, 
		           sycl::uint3 p, 
		           sycl::uint3 gridSize) 
{
  p.x() = sycl::min(p.x(), gridSize.x() - 1);
  p.y() = sycl::min(p.y(), gridSize.y() - 1);
  p.z() = sycl::min(p.z(), gridSize.z() - 1);
  uint i = (p.z() * gridSize.x() * gridSize.y()) + (p.y() * gridSize.x()) + p.x();
  return volumeAcc[i];
}

sycl::uint3 calcGridPos(uint i, 
		                sycl::uint3 gridSizeShift, 
			            sycl::uint3 gridSizeMask) 
{
  sycl::uint3 gridPos;
  gridPos.x() = i & gridSizeMask.x();
  gridPos.y() = (i >> gridSizeShift.y()) & gridSizeMask.y();
  gridPos.z() = (i >> gridSizeShift.z()) & gridSizeMask.z();
  return gridPos;
}

sycl::float3 vertexInterp(float isolevel, 
		                  sycl::float3 p0, 
			              sycl::float3 p1, 
			              float f0, 
			              float f1) 
{
  float t = (isolevel - f0) / (f1 - f0);
  return p0 + t * (p1 - p0);
}

void vertexInterp2(float isolevel, 
		           sycl::float3 p0, 
		           sycl::float3 p1, 
		           sycl::float4 f0, 
		           sycl::float4 f1, 
		           sycl::float3 &p, 
		           sycl::float3 &n) 
{
  float t = (isolevel- f0.w()) / (f1.w() - f0.w());
  p = p0 + t * (p1 - p0);
  n.x() = f0.x() + t * (f1.x() - f0.x());
  n.y() = f0.y() + t * (f1.y() - f0.y());
  n.z() = f0.z() + t * (f1.z() - f0.z());
}

extern "C" void ThrustScanWrapper(unsigned int *output, 
		                          unsigned int *input,
                                  unsigned int numElements) 
{
  oneapi::dpl::exclusive_scan(oneapi::dpl::execution::dpcpp_default, input, input + numElements, output, 0);
}

/********************************************************
 * classifyVoxel kernel here
 * *****************************************************/

#include "kernelClassifyVoxel.cpp"

/********************************************************
 * launch_classifyVoxel here
 * *****************************************************/

#include "kernelLaunchClassifyVoxel.cpp"

/********************************************************
 * compactVoxels kernel here
 * *****************************************************/

#include "kernelCompactVoxels.cpp"

/********************************************************
 * launch compactVoxels kernel here
 * *****************************************************/

#include "kernelLaunchCompactVoxels.cpp"

/********************************************************
 * generateTriangles kernel here
 * *****************************************************/

#include "kernelGenerateTriangles.cpp"

/********************************************************
 * launch generateTriangles kernel here
 * *****************************************************/

#include "kernelLaunchGenerateTriangles.cpp"


