#ifndef _RTP_SUPERSAMPLEJITTEREDRENDERER_H_
#define _RTP_SUPERSAMPLEJITTEREDRENDERER_H_

#include <rti/IRenderer.h>

namespace rtp {

class SuperSampleJitteredRenderer : public rti::IRenderer
{
public:
	enum GridResolution
	{
		TWO_BY_TWO,
		FOUR_BY_FOUR,
		// TODO: EIGHT_BY_EIGHT
	};

	SuperSampleJitteredRenderer();

	virtual void render();

	void setGridResolution( GridResolution res );

private:
	GridResolution _gridRes;
};

} // namespace rtp

#endif // _RTP_SUPERSAMPLEJITTEREDRENDERER_H_
