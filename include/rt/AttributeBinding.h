#ifndef _RT_ATTRIBUTEBINDING_H_
#define _RT_ATTRIBUTEBINDING_H_

#include <rt/common.h>
#include <rt/Stl.h>

namespace rt {

struct AttributeBinding 
{
	AttributeBinding();
	void reset();

	RTenum normalBinding;
	RTenum colorBinding;
	RTenum textureBinding;
};

class AttributeBindingStack
{
public:
	AttributeBindingStack();

	void pushAllAttrib();
	AttributeBinding& top();
	void popAllAttrib();
	
private:
	std::stack<AttributeBinding> _stack;
};

} // namespace rt

#endif // _RT_ATTRIBUTEBINDING_H_
