#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <stdint.h>

namespace Engine
{
	class MathUtils
	{
		public:
			static const float MAX_RELATIVE_ERROR;
			static const float MAX_ABSOLUTE_ERROR;
			static const float PI;
			static const float DegToRad;
			static const float RadToDeg;

			static bool almostEqual(float lhs, float rhs);
			static bool almostEqual(float lhs, float rhs, float maxRelativeError);
			static bool almostEqual(float lhs, float rhs, float maxRelativeError, float maxAbsoluteError);

			static bool IsNAN( float value );
			static bool IsZero(float i_val);

		private:
			MathUtils();
	};
}

#endif