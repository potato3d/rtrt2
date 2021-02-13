#ifndef _RTI_IPLUGIN_H_
#define _RTI_IPLUGIN_H_

#include <rt/common.h>
#include <vr/refcounting.h>

namespace rti {

class IPlugin : public vr::RefCounted
{
public:
	// Default implementation: do nothing
	virtual void newFrame();
	virtual void receiveParameter( int paramId, void* paramValue );

protected:
	virtual ~IPlugin();
};

} // namespace rti

#endif // _RTI_IPLUGIN_H_
