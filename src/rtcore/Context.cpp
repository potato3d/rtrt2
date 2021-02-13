#include <rt/Context.h>
#include <rt/Stl.h>

#include <rt/Plugins.h>
#include <rt/Scene.h>
#include <rt/Geometry.h>

using namespace rt;

static const uint32 s_maxActiveContexts = 8;
static Context* s_contexts[s_maxActiveContexts] = { NULL };
static uint32 s_activeContexts = 0;
static uint32 s_currentContext = 0;

RTenum Context::createNew()
{
	if( s_activeContexts < s_maxActiveContexts )
	{
		s_contexts[s_activeContexts++] = new Context();
		return RT_OK;
	}
	return RT_NO_MEMORY;
}

RTenum Context::makeCurrent( uint32 ctxId )
{
	if( ctxId < s_activeContexts )
	{
		s_currentContext = ctxId;
		return RT_OK;
	}
	return RT_INVALID_PARAM;
}

Context* Context::get()
{
	return s_contexts[s_currentContext];
}

uint32 Context::getNumActiveContexts()
{
	return s_activeContexts;
}

uint32 Context::getMaxActiveContexts()
{
	return s_maxActiveContexts;
}

const Plugins* Context::getPlugins() const
{
	return _plugins;
}

const Scene* Context::getScene() const
{
	return _scene;
}

MatrixStack& Context::getMatrixStack()
{
	return _matrixStack;
}

AttributeBindingStack& Context::getBindingStack()
{
	return _bindingStack;
}

void Context::renderFrame()
{
	// Call newFrame for everyone
	_plugins->camera->newFrame();
	_plugins->accStructBuilder->newFrame();
	_plugins->environment->newFrame();

	for( uint32 i = 0, size = _plugins->lights.size(); i < size; ++i )
	{
		_plugins->lights[i]->newFrame();
	}
	for( uint32 i = 0, size = _plugins->materials.size(); i < size; ++i )
	{
		_plugins->materials[i]->newFrame();
	}
	for( uint32 i = 0, size = _plugins->textures.size(); i < size; ++i )
	{
		_plugins->textures[i]->newFrame();
	}

	// Update instances, if needed
	if( _instancesDirty )
	{
		_scene->accStruct = _plugins->accStructBuilder->buildInstance( _scene->instances );
		_instancesDirty = false;
	}
	_scene->accStruct->newFrame();

	// Render current frame
	_plugins->renderer->newFrame();
	_plugins->renderer->render();
}

void Context::traceNearest( Sample& sample )
{
	_scene->accStruct->traceNearestInstance( _scene->instances, sample );
}

bool Context::traceAny( Sample& sample )
{
	return _scene->accStruct->traceAnyInstance( _scene->instances, sample );
}

// Plugins
void Context::setAccStructBuilder( rti::IAccStructBuilder* accBuilder )
{
	_plugins->accStructBuilder = accBuilder;
}

rti::IAccStructBuilder* Context::getAccStructBuilder() const
{
	return _plugins->accStructBuilder.get();
}

void Context::setCamera( rti::ICamera* camera )
{
	_plugins->camera = camera;
}

rti::ICamera* Context::getCamera() const
{
	return _plugins->camera.get();
}

void Context::setEnvironment( rti::IEnvironment* environment )
{
	_plugins->environment = environment;
}

rti::IEnvironment* Context::getEnvironment() const
{
	return _plugins->environment.get();
}

void Context::setRenderer( rti::IRenderer* renderer )
{
	_plugins->renderer = renderer;
}

rti::IRenderer* Context::getRenderer() const
{
	return _plugins->renderer.get();
}

uint32 Context::createMaterials( uint32 count )
{
	uint32 previousSize = _plugins->materials.size();
	uint32 newSize = previousSize + count;

	_plugins->materials.resize( newSize );

	// Populate with default implementations
	for( uint32 i = previousSize; i < newSize; ++i )
	{
		_plugins->materials[i] = new rti::IMaterial();
	}

	return previousSize;
}

void Context::setMaterial( uint32 id, rti::IMaterial* material )
{
	if( id >= _plugins->materials.size() )
		return;

	_plugins->materials[id] = material;
}

void Context::bindMaterial( uint32 id )
{
	if( id >= _plugins->materials.size() )
		return;

	_currentMaterialId = id;
}

rti::IMaterial* Context::getMaterial( uint32 id ) const
{
	if( id >= _plugins->materials.size() )
		return NULL;

	return _plugins->materials[id].get();
}

uint32 Context::getMaterialCount() const
{
	return _plugins->materials.size();
}

uint32 Context::createTextures( uint32 count )
{
	uint32 previousSize = _plugins->textures.size();
	uint32 newSize = previousSize + count;

	_plugins->textures.resize( newSize );

	// Populate with default implementations
	for( uint32 i = previousSize; i < newSize; ++i )
	{
		_plugins->textures[i] = new rti::ITexture();
	}

	return previousSize;
}

void Context::setTexture( uint32 id, rti::ITexture* texture )
{
	if( id >= _plugins->textures.size() )
		return;

	_plugins->textures[id] = texture;
}

rti::ITexture* Context::getTexture( uint32 id ) const
{
	if( id >= _plugins->textures.size() )
		return NULL;

	return _plugins->textures[id].get();
}

uint32 Context::getTextureCount() const
{
	return _plugins->textures.size();
}

uint32 Context::createLights( uint32 count )
{
	uint32 previousSize = _plugins->lights.size();
	uint32 newSize = previousSize + count;

	_plugins->lights.resize( newSize );

	// Populate with default implementations
	for( uint32 i = previousSize; i < newSize; ++i )
	{
		_plugins->lights[i] = new rti::ILight();
	}

	return previousSize;
}

void Context::setLight( uint32 id, rti::ILight* light )
{
	if( id >= _plugins->lights.size() )
		return;

	_plugins->lights[id] = light;
}

rti::ILight* Context::getLight( uint32 id ) const
{
	if( id >= _plugins->lights.size() )
		return NULL;

	return _plugins->lights[id].get();
}

uint32 Context::getLightCount() const
{
	return _plugins->lights.size();
}

// Scene
uint32 Context::createGeometries( uint32 count )
{
	uint32 previousSize = _scene->geometries.size();
	_scene->geometries.resize( previousSize + count );
	return previousSize;
}

void Context::beginGeometry( uint32 id )
{
	if( id >= _scene->geometries.size() )
		return;

	_currentGeometryId = id;
}

void Context::beginPrimitive( RTenum primitiveType )
{
	_primBuilder.begin( primitiveType, &_scene->geometries[_currentGeometryId] );
	_primBuilder.setMaterial( _plugins->materials[_currentMaterialId].get() );
	_primBuilder.getBindings() = _bindingStack.top();
	_primBuilder.getTransform().setMatrix( _matrixStack.top() );
}

void Context::setColor( float r, float g, float b )
{
	_primBuilder.setColor( r, g, b );
}

void Context::setColor( float const * const color )
{
	_primBuilder.setColor( color );
}

void Context::setTexCoord( float s, float t, float p )
{
	_primBuilder.setTexCoord( s, t, p );
}

void Context::setTexCoord( float const * const texCoord )
{
	_primBuilder.setTexCoord( texCoord );
}

void Context::setNormal( float x, float y, float z )
{
	_primBuilder.setNormal( x, y, z );
}

void Context::setNormal( float const * const normal )
{
	_primBuilder.setNormal( normal );
}

void Context::addVertex( float x, float y, float z )
{
	_primBuilder.addVertex( x, y, z );
}

void Context::addVertex( float const * const vertex )
{
	_primBuilder.addVertex( vertex );
}

void Context::endPrimitive()
{
	_primBuilder.end();
}

void Context::endGeometry()
{
	_plugins->accStructBuilder->buildGeometry( &_scene->geometries[_currentGeometryId] );
}

uint32 Context::createInstances( uint32 count )
{
	uint32 previousSize = _scene->instances.size();
	_scene->instances.resize( previousSize + count );
	return previousSize;
}

void Context::instantiateGeometry( uint32 instanceId, uint32 geometryId )
{
	if( instanceId >= _scene->instances.size() || geometryId >= _scene->geometries.size() )
		return;

	_instancesDirty = true;

	Instance& instance = _scene->instances[instanceId];
	Geometry& geometry = _scene->geometries[geometryId];

	// Setup instance
	instance.geometry = &geometry;
	instance.transform.setMatrix( _matrixStack.top() );

	// Update instance bounding box according to current matrix and geometry's original bounding box
	const Aabb& bbox = geometry.accStruct->getBoundingBox();

	// If geometry box is degenerate, we keep it as instance's box as well
	if( bbox.isDegenerate() )
	{
		instance.bbox = bbox;
		return;
	}
	
	// Transform geometry's bbox according to instance's matrix transform
	vr::vec3f boxVertices[8];
	bbox.computeVertices( boxVertices );

	for( uint32 v = 0; v < 8; ++v )
	{
		instance.transform.transformVertex( boxVertices[v] );
		instance.bbox.expandBy( boxVertices[v] );
	}

	// Avoid precision problems: we extend the box a little
	// The other option would be to add a similar tolerance to the clipRay method in AabbIntersection.
	// But since the clipRay method is called several times per ray, we opted to put that tolerance here.
	instance.bbox.scaleBy( 0.01f );
}

Geometry* Context::getGeometry( uint32 id ) const
{
	if( id >= _scene->geometries.size() )
		return NULL;

	return &_scene->geometries[id];
}

uint32 Context::getGeometryCount() const
{
	return _scene->geometries.size();
}

Instance* Context::getInstance( uint32 id ) const
{
	if( id >= _scene->instances.size() )
		return NULL;

	return &_scene->instances[id];
}

uint32 Context::getInstanceCount() const
{
	return _scene->instances.size();
}

void Context::setFrameBuffer( float* buffer )
{
	_frameBuffer = buffer;
}

float* Context::getFrameBuffer() const
{
	return _frameBuffer;
}

void Context::setRayEpsilon( float epsilon )
{
	_rayEpsilon = epsilon;
}

float Context::getRayEpsilon() const
{
	return _rayEpsilon;
}

void Context::setMaxRecursionDepth( uint32 count )
{
	_maxRecursionDepth = count;
}

uint32 Context::getMaxRecursionDepth() const
{
	return _maxRecursionDepth;
}

void Context::setMediumRefractionIndex( float index )
{
	_mediumRefractionIndex = index;
}

float Context::getMediumRefractionIndex() const
{
	return _mediumRefractionIndex;
}

/************************************************************************/
/* Private                                                              */
/************************************************************************/
Context::Context()
{
	_plugins = new Plugins();
	_scene = new Scene();

	_instancesDirty = true;
	_currentMaterialId = 0;
	_currentGeometryId = 0;

	// Default plugins
	setAccStructBuilder( new rti::IAccStructBuilder() );
	setCamera( new rti::ICamera() );
	setEnvironment( new rti::IEnvironment() );
	setRenderer( new rti::IRenderer() );
	createLights( 1 );
	setLight( 0, new rti::ILight() );
	createMaterials( 1 );
	setMaterial( 0, new rti::IMaterial() );
	createTextures( 1 );
	setTexture( 0, new rti::ITexture() );

	// Setup default ray tracing parameters
	setFrameBuffer( NULL );
	setMaxRecursionDepth( 3 );

	// TODO: Watch for conflicts with epsilon in raytracer class. Remove there and change here!
	// Currently, we use: ray.tnear = epsilon and hit.position += hit.shadingNormal * epsilon (init light sample).
	// This epsilon must be greater than any eventual epsilon used while tracing rays!
	setRayEpsilon( 2e-4f );

	// Approximation for air index
	setMediumRefractionIndex( 1.0f );
}

Context::~Context()
{
	delete _plugins;
	delete _scene;
}
