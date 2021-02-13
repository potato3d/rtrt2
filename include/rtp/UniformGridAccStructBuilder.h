#ifndef _RTP_UNIFORMGRIDACCSTRUCTBUILDER_H_
#define _RTP_UNIFORMGRIDACCSTRUCTBUILDER_H_

#include <rti/IAccStructBuilder.h>

namespace rtp {

class UniformGridAccStructBuilder : public rti::IAccStructBuilder
{
public:
	virtual void buildGeometry( rt::Geometry* geometry );
	// TODO: 
	//virtual IAccStruct* buildInstance( const std::vector<rt::Instance> instances );
};

} // namespace rtp

#endif // _RTP_UNIFORMGRIDACCSTRUCTBUILDER_H_
