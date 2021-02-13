#include <rti/ITexture.h>

using namespace rti;

void ITexture::shade( rt::Sample& sample )
{
	// avoid warnings
	sample;
}

void ITexture::setTextureImage2D( uint32 width, uint32 height, unsigned char* texels )
{
	// avoid warnings
	width; height; texels;
}
