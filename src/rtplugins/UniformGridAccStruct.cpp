#include <rtp/UniformGridAccStruct.h>
#include <rt/Context.h>
#include <rti/IEnvironment.h>
#include <rt/AabbIntersection.h>
#include <rt/RayTriIntersection.h>

using namespace rtp;

UniformGridAccStruct::UniformGridAccStruct()
{
	_nx = 0;
	_ny = 0;
	_nz = 0;
	_cellSize.set( 0,0,0 );
	_invCellSize.set( 0,0,0 );
}

UniformGridAccStruct::~UniformGridAccStruct()
{
	clear();
}

void UniformGridAccStruct::clear()
{
	rt::vectorFreeMemory( _data );
}

void UniformGridAccStruct::traceNearestGeometry( const rt::Instance& instance, rt::Ray& ray, rt::Hit& hit )
{
	// If don't hit bbox in local space, no need to trace underlying triangles
	if( !rt::AabbIntersection::clipRay( _bbox, ray ) )
		return;

	/************************************************************************/
	/* Initial setup                                                        */
	/************************************************************************/
	// 1. Find initial cell where ray begins

	// Since ray was already clipped against bbox (grid), 
	// ray.tnear gives us the starting t (thus the start point as well)
	const vr::vec3f startPoint = ray.orig + ray.dir * ray.tnear;

	// TODO: check if this is the best solution
	int32 x = vr::clampTo( worldToVoxel( startPoint.x, RT_AXIS_X ), 0, (int32)_nx-1 );
	int32 y = vr::clampTo( worldToVoxel( startPoint.y, RT_AXIS_Y ), 0, (int32)_ny-1 );
	int32 z = vr::clampTo( worldToVoxel( startPoint.z, RT_AXIS_Z ), 0, (int32)_nz-1 );

	// 2. Compute stepX, stepY, stepZ
	const int32 stepX = ray.dirSigns[RT_AXIS_X];
	const int32 stepY = ray.dirSigns[RT_AXIS_Y];
	const int32 stepZ = ray.dirSigns[RT_AXIS_Z];

	// TODO: check if it is faster to use logic operations or to branch based on ray.dir signs for each dimension

	// 3 Compute out of grid limits
	const int32 outX = -ray.dirSignBits[RT_AXIS_X] + _nx * !ray.dirSignBits[RT_AXIS_X];
	const int32 outY = -ray.dirSignBits[RT_AXIS_Y] + _ny * !ray.dirSignBits[RT_AXIS_Y];
	const int32 outZ = -ray.dirSignBits[RT_AXIS_Z] + _nz * !ray.dirSignBits[RT_AXIS_Z];

	// 4. Compute tDeltaX, tDeltaY, tDeltaZ
	const float tDeltaX = vr::abs( _cellSize.x * ray.invDir.x );
	const float tDeltaY = vr::abs( _cellSize.y * ray.invDir.y );
	const float tDeltaZ = vr::abs( _cellSize.z * ray.invDir.z );

	// 5. Compute tNextX, tNextY, tNextZ
	float tMaxX = ( voxelToWorld( x + !ray.dirSignBits[RT_AXIS_X], RT_AXIS_X ) - ray.orig.x ) * ray.invDir.x;
	float tMaxY = ( voxelToWorld( y + !ray.dirSignBits[RT_AXIS_Y], RT_AXIS_Y ) - ray.orig.y ) * ray.invDir.y;
	float tMaxZ = ( voxelToWorld( z + !ray.dirSignBits[RT_AXIS_Z], RT_AXIS_Z ) - ray.orig.z ) * ray.invDir.z;

	/************************************************************************/
	/* Trace ray through grid                                               */
	/************************************************************************/
	const std::vector<rt::TriAccel>& triangles = instance.geometry->triAccel;

	// Best distance considers any previously found hits to prevent false hits in this geometry
	float bestDistance = hit.distance;

	/**/
	// Version 1: Code easier to understand
	std::vector<uint32>* cell;
	
	// While inside grid
	while( x != outX && y != outY && z != outZ )
	{
		// Get current cell
		cell = at( x, y, z );

		// If cell contains triangles, test intersection.
		// We send the lesser tMax as the maximum valid distance. This avoids false intersections outside current cell.
		if( !cell->empty() && 
			intersectTriangles( triangles, *cell, vr::min( vr::min( tMaxX, tMaxY ), tMaxZ ), ray, hit, bestDistance ) )
		{
			hit.distance = bestDistance;
			hit.instance = &instance;
			return;
		}

		// Go to next cell, need to decide which dimension is next
		// TODO: could do this without branches? is it worth it?
		if( tMaxX < tMaxY && tMaxX < tMaxZ )
		{
			x += stepX;
			tMaxX += tDeltaX;
		}
		else if( tMaxY < tMaxZ )
		{
			y += stepY;
			tMaxY += tDeltaY;
		}
		else
		{
			z += stepZ;
			tMaxZ += tDeltaZ;
		}
	}
	/**/

	/**
	// Version 2: A little more complicated. Kept as reference.
	std::vector<uint32>* cell = at( x, y, z );
	
	while( true )
	{
		if( intersectTriangles( triangles, *cell, vr::min( vr::min( tMaxX, tMaxY ), tMaxZ ), ray, hit, bestDistance ) )
		{
			hit.distance = bestDistance;
			hit.instance = &instance;
			return;
		}

		// Find next non-empty cell
		do
		{
			if( tMaxX < tMaxY && tMaxX < tMaxZ )
			{
				x += stepX;

				if( x == outX )
					return;

				tMaxX += tDeltaX;
			}
			else if( tMaxY < tMaxZ )
			{
				y += stepY;

				if( y == outY )
					return;

				tMaxY += tDeltaY;
			}
			else
			{
				z += stepZ;

				if( z == outZ )
					return;

				tMaxZ += tDeltaZ;
			}

			// Get next cell
			cell = at( x, y, z );

		} while( cell->empty() );
	}
	/**/
}

void UniformGridAccStruct::setResolution( uint32 nCellsX, uint32 nCellsY, uint32 nCellsZ )
{
	// Just in case
	if( _bbox.isDegenerate() )
		return;

	_data.resize( nCellsX*nCellsY*nCellsZ );
	_nx = nCellsX;
	_ny = nCellsY;
	_nz = nCellsZ;

	// Update cell sizes
	_cellSize.set( ( _bbox.maxv.x - _bbox.minv.x ) / (float)_nx, 
		           ( _bbox.maxv.y - _bbox.minv.y ) / (float)_ny,
		           ( _bbox.maxv.z - _bbox.minv.z ) / (float)_nz );

	_invCellSize.x = 1.0f / _cellSize.x;
	_invCellSize.y = 1.0f / _cellSize.y;
	_invCellSize.z = 1.0f / _cellSize.z;
}

void UniformGridAccStruct::getResolution( uint32& nCellsX, uint32& nCellsY, uint32& nCellsZ ) const
{
	nCellsX = _nx;
	nCellsY = _ny;
	nCellsZ = _nz;
}

const vr::vec3f& UniformGridAccStruct::getCellSize() const
{
	return _cellSize;
}

const vr::vec3f& UniformGridAccStruct::getInvCellSize() const
{
	return _invCellSize;
}
