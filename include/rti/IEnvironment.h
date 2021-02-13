#ifndef _RTI_IENVIRONMENT_H_
#define _RTI_IENVIRONMENT_H_

#include <rti/IPlugin.h>
#include <rt/Sample.h>

namespace rti {

class IEnvironment : public IPlugin
{
public:
	// Default implementation: do nothing
	virtual void shade( rt::Sample& sample );
};

} // namespace rti

#endif // _RTI_IENVIRONMENT_H_
