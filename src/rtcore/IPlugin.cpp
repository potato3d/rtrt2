#include <rti/IPlugin.h>

using namespace rti;

void IPlugin::newFrame()
{
	// empty
}

void IPlugin::receiveParameter( int paramId, void* paramValue )
{
	// avoid warnings
	paramId; paramValue;
}

IPlugin::~IPlugin()
{
	// empty
}
