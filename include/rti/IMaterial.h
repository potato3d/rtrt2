#ifndef _RTI_IMATERIAL_H_
#define _RTI_IMATERIAL_H_

#include <rti/IPlugin.h>
#include <rt/Sample.h>

namespace rti {

class IMaterial : public IPlugin
{
public:
	// Default implementation: do nothing
	virtual void shade( rt::Sample& sample );
};

} // namespace rti

#endif // _RTI_IMATERIAL_H_
