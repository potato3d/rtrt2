#ifndef _RTI_ICAMERA_H_
#define _RTI_ICAMERA_H_

#include <rt/common.h>
#include <rti/IPlugin.h>
#include <vr/vec3.h>

namespace rti {

class ICamera : public IPlugin
{
public:
	// Default implementation: do nothing
	virtual void translate( float x, float y, float z );
	virtual void rotate( float radians, uint32 axis );

	virtual void setLookAt( float eyeX, float eyeY, float eyeZ, 
		 				 float centerX, float centerY, float centerZ, 
		                 float upX, float upY, float upZ );

	virtual void setPerspective( float fovY, float zNear, float zFar );

	virtual void setViewport( uint32 width, uint32 height );
	virtual void getViewport( uint32& width, uint32& height );

	virtual void getRayOrigin( vr::vec3f& origin, float x, float y );
	virtual void getRayDirection( vr::vec3f& dir, float x, float y );
};

} // namespace rti

#endif // _RTI_ICAMERA_H_
