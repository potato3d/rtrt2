#ifndef _RTI_IRENDERER_H_
#define _RTI_IRENDERER_H_

#include <rti/IPlugin.h>

namespace rti {

class IRenderer : public IPlugin
{
public:
	// Default implementation: do nothing
	virtual void render();
};

} // namespace rti

#endif // _RTI_IRENDERER_H_
