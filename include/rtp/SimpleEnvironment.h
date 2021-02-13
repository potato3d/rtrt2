#ifndef _RTP_SIMPLEENVIRONMENT_H_
#define _RTP_SIMPLEENVIRONMENT_H_

#include <rti/IEnvironment.h>

namespace rtp {

class SimpleEnvironment : public rti::IEnvironment
{
public:
	SimpleEnvironment();

	virtual void shade( rt::Sample& sample );

private:
	vr::vec3f _background;
};

} // namespace rtp

#endif // _RTP_SIMPLEENVIRONMENT_H_
