#ifndef _RTP_HEADLIGHTMATERIALCOLOR_H_
#define _RTP_HEADLIGHTMATERIALCOLOR_H_

#include <rti/IMaterial.h>

namespace rtp {

class HeadlightMaterialColor : public rti::IMaterial
{
public:
	HeadlightMaterialColor();

	virtual void shade( rt::Sample& sample );

	void setAmbient( float r, float g, float b );
	void setDiffuse( float r, float g, float b );
	void setUseFlatNormals( bool enable );

private:
	vr::vec3f _ambient;
	vr::vec3f _diffuse;
	bool _useFlatNormals;
};

} // namespace rtp

#endif // _RTP_HEADLIGHTMATERIALCOLOR_H_
