#ifndef _RTP_DEPTHMATERIAL_H_
#define _RTP_DEPTHMATERIAL_H_

#include <rti/IMaterial.h>

namespace rtp {

class DepthMaterial: public rti::IMaterial
{
public:
	virtual void shade( rt::Sample& sample );
};

} // namespace rtp

#endif // _RTP_DEPTHMATERIAL_H_
