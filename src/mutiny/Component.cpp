#include "Component.h"

namespace mutiny
{

namespace engine
{

Component::~Component()
{

}

ref<GameObject> Component::getGameObject()
{
  return gameObject;
}

void Component::awake()
{

}

void Component::start()
{

}

void Component::update()
{

}

void Component::destroy()
{

}

void Component::render()
{

}

void Component::postRender()
{

}

void Component::gui()
{

}

void Component::levelWasLoaded()
{

}

void Component::collisionEnter(Collision& collision){}
void Component::collisionStay(Collision& collision){}
void Component::collisionExit(Collision& collision){}

}

}

