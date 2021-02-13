#include <rti/ILight.h>

using namespace rti;

bool ILight::illuminate( rt::Sample& sample )
{
	// avoid warnings
	sample;
	return false;
}
