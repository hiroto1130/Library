#ifndef COLLISION_H
#define COLLISION_H

#include "../Render/Render.h"
#include "../Box/Box.h"

using Library::Thing;

namespace Library
{

	class Collision
	{
	public:

		bool Sphere(const Thing& left_thing, const Thing& right_thing);

		bool Box(const Thing& left_thing,const BBOX& left_box, const Thing& right_thing, const BBOX& right_box);

	private:

		FLOAT GetRadius(const Thing& thing);


	};
}

#endif
