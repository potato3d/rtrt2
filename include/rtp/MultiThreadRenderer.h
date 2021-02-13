#ifndef _RTP_MULTITHREADRENDERER_H_
#define _RTP_MULTITHREADRENDERER_H_

#include <rti/IRenderer.h>

namespace rtp {

class MultiThreadRenderer : public rti::IRenderer
{
public:
	virtual void render();
};

} // namespace rtp

#endif // _RTP_MULTITHREADRENDERER_H_
