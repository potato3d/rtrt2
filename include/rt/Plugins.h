#ifndef _RT_PLUGINS_H_
#define _RT_PLUGINS_H_

#include <rt/common.h>
#include <rti/IAccStructBuilder.h>
#include <rti/ICamera.h>
#include <rti/IEnvironment.h>
#include <rti/IRenderer.h>
#include <rti/ILight.h>
#include <rti/IMaterial.h>
#include <rti/ITexture.h>

namespace rt {

struct Plugins
{
	vr::ref_ptr<rti::IAccStructBuilder> accStructBuilder;
	vr::ref_ptr<rti::ICamera> camera;
	vr::ref_ptr<rti::IEnvironment> environment;
	vr::ref_ptr<rti::IRenderer> renderer;

	std::vector< vr::ref_ptr<rti::ILight> > lights;
	std::vector< vr::ref_ptr<rti::IMaterial> > materials;
	std::vector< vr::ref_ptr<rti::ITexture> > textures;
};

} // namespace rt

#endif // _RT_PLUGINS_H_
