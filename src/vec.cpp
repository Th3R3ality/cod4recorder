#include "vec.h"
#include "cod4/angles.h"

ivec3 Angles2Shorts(const fvec3& angles)
{
	return ivec3({
		ANGLE2SHORT(angles.x),
		ANGLE2SHORT(angles.y),
		ANGLE2SHORT(angles.z) });
}
