#ifndef _RTI_IACCSTRUCTBUILDER_H_
#define _RTI_IACCSTRUCTBUILDER_H_

#include <rt/common.h>
#include <rt/Geometry.h>
#include <rt/Instance.h>
#include <rti/IPlugin.h>
#include <rti/IAccStruct.h>

namespace rti {

class IAccStructBuilder : public IPlugin
{
public:
	// Default implementation: do nothing
	virtual void buildGeometry( rt::Geometry* geometry );
	virtual IAccStruct* buildInstance( const std::vector<rt::Instance> instances );
};

} // namespace rti

#endif // _RTI_IACCSTRUCTBUILDER_H_
