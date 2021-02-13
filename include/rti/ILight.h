#ifndef _RTI_ILIGHT_H_
#define _RTI_ILIGHT_H_

#include <rti/IPlugin.h>
#include <rt/Sample.h>

namespace rti {

class ILight : public IPlugin
{
public:
	// Default implementation: do nothing
	virtual bool illuminate( rt::Sample& sample );
};

} // namespace rti

#endif // _RTI_ILIGHT_H_
