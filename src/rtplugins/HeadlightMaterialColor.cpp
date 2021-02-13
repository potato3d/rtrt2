#include <rtp/HeadlightMaterialColor.h>

using namespace rtp;

HeadlightMaterialColor::HeadlightMaterialColor()
{
	_ambient.set( 0.1f, 0.1f, 0.1f );
	_diffuse.set( 1.0f, 1.0f, 1.0f );
	_useFlatNormals = false;
}

void HeadlightMaterialColor::shade( rt::Sample& sample )
{
	sample.ray.dir.normalize();
	if( _useFlatNormals )
		sample.computeFlatNormal();
	else
		sample.computeShadingNormal();

	const float nDotD = -( sample.normal.dot( sample.ray.dir ) );

	sample.color = ( _ambient + ( _diffuse - _ambient ) * nDotD ) * _diffuse;
}

void HeadlightMaterialColor::setAmbient( float r, float g, float b )
{
	_ambient.set( r, g, b );
}

void HeadlightMaterialColor::setDiffuse( float r, float g, float b )
{
	_diffuse.set( r, g, b );
}

void HeadlightMaterialColor::setUseFlatNormals( bool enable )
{
	_useFlatNormals = enable;
}
