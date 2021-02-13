#ifndef _RTP_PHONGMATERIALCOLOR_H_
#define _RTP_PHONGMATERIALCOLOR_H_

#include <rti/IMaterial.h>
#include <rti/ITexture.h>

namespace rtp {

class PhongMaterialColor : public rti::IMaterial
{
public:
	PhongMaterialColor();

	virtual void shade( rt::Sample& sample );

	void setAmbient( float r, float g, float b );
	void setDiffuse( float r, float g, float b );
	void setSpecularExponent( float expn );
	void setReflexCoeff( float coeff );
	void setRefractionIndex( float index );
	void setOpacity( float opacity );
	void setTexture( rti::ITexture* texture );

private:
	vr::vec3f _ambient;
	vr::vec3f _diffuse;
	vr::vec3f _specularColor;
	float _specularExponent;
	float _reflexCoeff;
	float _refractionIndex;
	float _opacity;
	rti::ITexture* _texture;
};

} // namespace rtp

#endif // _RTP_PHONGMATERIALCOLOR_H_
