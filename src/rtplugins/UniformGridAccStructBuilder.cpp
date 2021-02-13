#include <rtp/UniformGridAccStructBuilder.h>
#include <rtp/UniformGridAccStruct.h>
#include <rt/AabbIntersection.h>

using namespace rtp;

void UniformGridAccStructBuilder::buildGeometry( rt::Geometry* geometry )
{
	// Compute scene box
	rt::Aabb bbox;
	bbox.buildFrom( &geometry->vertices[0], geometry->vertices.size() );

	// Add a tolerance to planar box dimension(s) to guarantee that grid has a volume > 0
	// Also avoid precision problems during triangle insertion, ray traversal and ray intersection
	bbox.scaleBy( 0.01f );

	// Create uniform grid
	UniformGridAccStruct* grid = new UniformGridAccStruct();
	geometry->accStruct = grid;

	// Compute total geometry volume
	const vr::vec3f diagonal = bbox.maxv - bbox.minv;
	float V = ( diagonal.x * diagonal.y * diagonal.z );

	// TODO: improve this equation / heuristic
	// TODO: magic user-supplied number
	uint32 k = 6;
	uint32 triCount = geometry->triDesc.size();
	float factor = powf( (float)( k * triCount ) / V, 1.0f / 3.0f );

	// Compute actual number of cells in each dimension
	uint32 nx = (uint32)ceilf( diagonal.x * factor );
	uint32 ny = (uint32)ceilf( diagonal.y * factor );
	uint32 nz = (uint32)ceilf( diagonal.z * factor );

	// Store grid data
	grid->setBoundingBox( bbox );
	grid->setResolution( nx, ny, nz );

	// Reference cell box for triangle overlap test
	rt::Aabb cellBox;
	rt::Aabb triBox;

	// Version 1: Main algorithm
	// For each triangle, compute which cells it could overlap and check overlap only for them O(n)
	for( uint32 t = 0; t < triCount; ++t )
	{
		// Set initial triangle Aabb to v0 and expand to include other 2 vertices
		const vr::vec3f& v0 = geometry->getVertex( t, 0 );
		const vr::vec3f& v1 = geometry->getVertex( t, 1 );
		const vr::vec3f& v2 = geometry->getVertex( t, 2 );
		triBox.minv = v0;
		triBox.maxv = v0;
		triBox.expandBy( v1 );
		triBox.expandBy( v2 );

		// Now that we have the triangle box, need to find which grid cells it overlaps
		const int32 xStart = grid->worldToVoxel( triBox.minv.x, RT_AXIS_X );
		const int32 yStart = grid->worldToVoxel( triBox.minv.y, RT_AXIS_Y );
		const int32 zStart = grid->worldToVoxel( triBox.minv.z, RT_AXIS_Z );

		const int32 xEnd = grid->worldToVoxel( triBox.maxv.x, RT_AXIS_X );
		const int32 yEnd = grid->worldToVoxel( triBox.maxv.y, RT_AXIS_Y );
		const int32 zEnd = grid->worldToVoxel( triBox.maxv.z, RT_AXIS_Z );

		// Loop over all cells overlapped by AABB and check if triangle actually overlaps any
		for( int32 z = zStart; z <= zEnd; ++z )
		{
			for( int32 y = yStart; y <= yEnd; ++y )
			{
				for( int32 x = xStart; x <= xEnd; ++x )
				{
					// Update cell box to check for triangle overlap
					cellBox.minv = bbox.minv + vr::vec3f( x, y, z ) * grid->getCellSize();
					cellBox.maxv = cellBox.minv + grid->getCellSize();

					if( rt::AabbIntersection::triangleOverlaps( cellBox, v0, v1, v2 ) )
						grid->at( x, y, z )->push_back( t );
				}
			}
		}
	}

	/**
	// Version 2: Kept only as reference
	// For each cell, check all triangles and store the ones that overlap cell O(n^2)
	for( uint32 z = 0; z < nz; ++z )
	{
		for( uint32 y = 0; y < ny; ++y )
		{
			for( uint32 x = 0; x < nx; ++x )
			{
				// Update cell box to check for triangle overlap
				cellBox.minv = bbox.minv + vr::vec3f( x, y, z ) * grid->getCellSize();
				cellBox.maxv = cellBox.minv + grid->getCellSize();

				// For each triangle
				for( uint32 t = 0; t < triCount; ++t )
				{
					const vr::vec3f& v0 = geometry->getVertex( t, 0 );
					const vr::vec3f& v1 = geometry->getVertex( t, 1 );
					const vr::vec3f& v2 = geometry->getVertex( t, 2 );

					if( rt::AabbIntersection::triangleOverlaps( cellBox, v0, v1, v2 ) )
						grid->at( x, y, z )->push_back( t );
				}
			}
		}
	}
	/**/
}
