#ifndef _RTI_ITEXTURE_H_
#define _RTI_ITEXTURE_H_

#include <rti/IPlugin.h>
#include <rt/Sample.h>

namespace rti {

class ITexture : public IPlugin
{
public:
	// Default implementation: do nothing
	virtual void shade( rt::Sample& sample );
	virtual void setTextureImage2D( uint32 width, uint32 height, unsigned char* texels );
};

} // namespace rts

#endif // _RTI_ITEXTURE_H_
