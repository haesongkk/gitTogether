#include "Component.h"
#include <typeinfo>

CBEngine::Component::Component()
	: name(typeid(*this).name()),
	gameObject(nullptr)
{

}

CBEngine::Component::~Component()
{

}
