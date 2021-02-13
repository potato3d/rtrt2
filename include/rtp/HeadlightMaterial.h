#ifndef _RTP_HEADLIGHTMATERIAL_H_
#define _RTP_HEADLIGHTMATERIAL_H_

#include <rti/IMaterial.h>

namespace rtp {

class HeadlightMaterial : public rti::IMaterial
{
public:
	HeadlightMaterial();

	virtual void shade( rt::Sample& sample );

private:
	vr::vec3f _ambient;
};

} // namespace rtp

#endif // _RTP_HEADLIGHTMATERIAL_H_
