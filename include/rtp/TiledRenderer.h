#ifndef _RTP_TILEDRENDERER_H_
#define _RTP_TILEDRENDERER_H_

#include <rti/IRenderer.h>

namespace rtp {

class TiledRenderer : public rti::IRenderer
{
public:
	virtual void render();
};

} // namespace rtp

#endif // _RTP_TILEDRENDERER_H_
