#include <rt/Geometry.h>

using namespace rt;

void Geometry::clear()
{
	if( accStruct.valid() )
		accStruct->clear();

	vectorFreeMemory( triAccel );
	vectorFreeMemory( triDesc );
	vectorFreeMemory( vertices );
	vectorFreeMemory( normals );
	vectorFreeMemory( colors );
	vectorFreeMemory( texCoords );
}
