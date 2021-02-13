#include <rt/AttributeBinding.h>

using namespace rt;

AttributeBinding::AttributeBinding()
: normalBinding( RT_BIND_PER_VERTEX ), colorBinding( RT_BIND_PER_VERTEX ), textureBinding( RT_BIND_PER_VERTEX )
{
	// empty
}

void AttributeBinding::reset()
{
	normalBinding = RT_BIND_PER_VERTEX;
	colorBinding = RT_BIND_PER_VERTEX;
	textureBinding = RT_BIND_PER_VERTEX;
}

AttributeBindingStack::AttributeBindingStack()
{
	AttributeBinding ab;
	ab.reset();
	_stack.push( ab );
}

void AttributeBindingStack::pushAllAttrib()
{
	_stack.push( _stack.top() );
}

AttributeBinding& AttributeBindingStack::top()
{
	return _stack.top();
}

void AttributeBindingStack::popAllAttrib()
{
	if( _stack.size() <= 1 )
		return;

	_stack.pop();
}
