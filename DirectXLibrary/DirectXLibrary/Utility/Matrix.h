#ifndef MATRIX_H
#define MATRIX_H

namespace Utility
{
	struct Rotation
	{
		Rotation(float Yaw,float Pitch,float Roll):yaw(Yaw),pitch(Pitch),roll(Roll) {}

		float yaw;
		float pitch;
		float roll;
	};

}

#endif
