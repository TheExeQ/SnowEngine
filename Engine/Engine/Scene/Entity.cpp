#include "Entity.h"

namespace Snow
{
	Entity::Entity(entt::entity aEntityHandle, Scene* aScene)
		: myEntityHandle(aEntityHandle), myScene(aScene)
	{

	}
}