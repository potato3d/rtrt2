#ifndef _RTP_UNIFORMGRIDACCSTRUCT_H_
#define _RTP_UNIFORMGRIDACCSTRUCT_H_

#include <rti/IAccStruct.h>
#include <rt/Geometry.h>
#include <rt/RayTriIntersection.h>

namespace rtp {

class UniformGridAccStruct : public rti::IAccStruct
{
public:
	UniformGridAccStruct();
	~UniformGridAccStruct();

	virtual void clear();
	virtual void traceNearestGeometry( const rt::Instance& instance, rt::Ray& ray, rt::Hit& hit );

	// Must have a valid bounding box first!
	void setResolution( uint32 nCellsX, uint32 nCellsY, uint32 nCellsZ );
	void getResolution( uint32& nCellsX, uint32& nCellsY, uint32& nCellsZ ) const;

	const vr::vec3f& getCellSize() const;
	const vr::vec3f& getInvCellSize() const;	

	inline const std::vector<uint32>* at( uint32 x, uint32 y, uint32 z ) const;
	inline std::vector<uint32>* at( uint32 x, uint32 y, uint32 z );

	inline int32 worldToVoxel( float value, RTenum axis );
	inline float voxelToWorld( int32 voxel, RTenum axis );

private:
	// Main intersection routine, called whenever we find a non-empty cell
	// Updates ray.tfar to avoid false intersections that lie outside cell boundaries
	inline bool intersectTriangles( const std::vector<rt::TriAccel>& triangles, const std::vector<uint32>& cell, 
		                            float maxValidDistance, rt::Ray& ray, rt::Hit& hit, float& bestDistance );

	// Number of cells in each dimension
	uint32 _nx;
	uint32 _ny;
	uint32 _nz;

	std::vector< std::vector<uint32> > _data;
	vr::vec3f _cellSize;
	vr::vec3f _invCellSize;
};

inline const std::vector<uint32>* UniformGridAccStruct::at( uint32 x, uint32 y, uint32 z ) const
{
	return &_data[x + y * _nx + z * _nx * _ny];
}

inline std::vector<uint32>* UniformGridAccStruct::at( uint32 x, uint32 y, uint32 z )
{
	return &_data[x + y * _nx + z * _nx * _ny];
}

inline int32 UniformGridAccStruct::worldToVoxel( float value, RTenum axis )
{
	return (int32)( ( value - _bbox.minv[axis] ) * _invCellSize[axis] );
}

inline float UniformGridAccStruct::voxelToWorld( int32 voxel, RTenum axis )
{
	return (float)voxel * _cellSize[axis] + _bbox.minv[axis];
}

// Private
inline bool UniformGridAccStruct::intersectTriangles( const std::vector<rt::TriAccel>& triangles, 
													  const std::vector<uint32>& cell, 
													  float maxValidDistance, rt::Ray& ray, rt::Hit& hit, 
													  float& bestDistance )
{
	// Ray.tfar keeps an upper bound on intersection distances we find
	// This avoids false intersections when the ray actually hits the primitive in another cell outside current one
	ray.tfar = maxValidDistance;

	// Intersect triangles in cell and keep closest hit
	for( uint32 t = 0, limit = cell.size(); t < limit; ++t )
	{
		rt::RayTriIntersection::hitWald( triangles[cell[t]], ray, hit, bestDistance );
	}

	// return ( found hit )
	return ( bestDistance < hit.distance );
}

} // namespace rtp

#endif // _RTP_UNIFORMGRIDACCSTRUCT_H_
