#ifndef COLLISION_H
#define COLLISION_H

#include "../Render/Render.h"

using Library::Thing;

namespace Library
{

	class Collision
	{
	public:

		bool Impact(const Thing& left_thing, const Thing& right_thing);

	private:

		FLOAT GetRadius(const Thing& thing);


	};
}

#endif
