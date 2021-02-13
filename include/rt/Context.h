#ifndef _RT_CONTEXT_H_
#define _RT_CONTEXT_H_

#include <rt/common.h>
#include <rt/MatrixStack.h>
#include <rt/PrimitiveBuilder.h>

// Forward declarations
namespace rti
{
	class IAccStruct;
	class IAccStructBuilder;
	class ICamera;
	class IEnvironment;
	class IRenderer;
	class ILight;
	class IMaterial;
	class ITexture;
}

namespace rt {

// Forward declarations
class Geometry;
struct Instance;
struct Plugins;
struct Scene;
class Sample;

class Context
{
public:
	static RTenum createNew();
	static RTenum makeCurrent( uint32 ctxId );
	static Context* get();
	static uint32 getNumActiveContexts();
	static uint32 getMaxActiveContexts();

	const Plugins* getPlugins() const;
	const Scene* getScene() const;

	MatrixStack& getMatrixStack();
	AttributeBindingStack& getBindingStack();

	void renderFrame();
	void traceNearest( Sample& sample );
	bool traceAny( Sample& sample );

	// Plugins
	void setAccStructBuilder( rti::IAccStructBuilder* accBuilder );
	rti::IAccStructBuilder* getAccStructBuilder() const;

	void setCamera( rti::ICamera* camera );
	rti::ICamera* getCamera() const;

	void setEnvironment( rti::IEnvironment* environment );
	rti::IEnvironment* getEnvironment() const;

	void setRenderer( rti::IRenderer* renderer );
	rti::IRenderer* getRenderer() const;

	uint32 createMaterials( uint32 count );
	void setMaterial( uint32 id, rti::IMaterial* material );
	void bindMaterial( uint32 id );
	rti::IMaterial* getMaterial( uint32 id ) const;
	uint32 getMaterialCount() const;

	uint32 createTextures( uint32 count );
	void setTexture( uint32 id, rti::ITexture* texture );
	rti::ITexture* getTexture( uint32 id ) const;
	uint32 getTextureCount() const;

	uint32 createLights( uint32 count );
	void setLight( uint32 id, rti::ILight* light );
	rti::ILight* getLight( uint32 id ) const;
	uint32 getLightCount() const;

	// Scene
	uint32 createGeometries( uint32 count );
	void beginGeometry( uint32 id );

	void beginPrimitive( RTenum primitiveType );

	void setColor( float r, float g, float b );
	void setColor( float const * const color );
	void setTexCoord( float s, float t, float p );
	void setTexCoord( float const * const texCoord );
	void setNormal( float x, float y, float z );
	void setNormal( float const * const normal );
	void addVertex( float x, float y, float z );
	void addVertex( float const * const vertex );

	void endPrimitive();

	void endGeometry();

	uint32 createInstances( uint32 count );
	void instantiateGeometry( uint32 instanceId, uint32 geometryId );

	Geometry* getGeometry( uint32 id ) const;
	uint32 getGeometryCount() const;
	Instance* getInstance( uint32 id ) const;
	uint32 getInstanceCount() const;

	// Miscellaneous
	void setFrameBuffer( float* buffer );
	float* getFrameBuffer() const;

	void setRayEpsilon( float epsilon );
	float getRayEpsilon() const;

	void setMaxRecursionDepth( uint32 count );
	uint32 getMaxRecursionDepth() const;

	void setMediumRefractionIndex( float index );
	float getMediumRefractionIndex() const;

private:
	// Singleton
	Context();
	~Context();

	Plugins* _plugins;
	Scene* _scene;
	MatrixStack _matrixStack;
	AttributeBindingStack _bindingStack;
	PrimitiveBuilder _primBuilder;

	bool _instancesDirty;
	uint32 _currentMaterialId;
	uint32 _currentGeometryId;

	float* _frameBuffer;
	float _rayEpsilon;
	uint32 _maxRecursionDepth;
	float _mediumRefractionIndex;
};

} // namespace rt

#endif // _RT_CONTEXT_H_
