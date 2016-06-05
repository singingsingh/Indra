#ifndef _POSITION_2D_H_
#define _POSITION_2D_H_

#include <Engine\Core\FixedSizeAlloc.h>

namespace Engine
{
	namespace Math
	{
		class Vector2f
		{
		public:
			static const Vector2f Zero;
			float x, y;

			inline Vector2f();
			inline Vector2f(float x, float y);
			inline ~Vector2f();

			inline Vector2f& operator +=(float value);
			inline Vector2f& operator +=(const Vector2f &rhs);

			inline Vector2f& operator -=(float value);
			inline Vector2f& operator -=(const Vector2f &rhs);

			inline friend bool operator==(const Vector2f &lhs, const Vector2f &rhs);

			inline friend Vector2f operator+(const Vector2f &lhs, const Vector2f &rhs);
			inline friend Vector2f operator-(const Vector2f &lhs, const Vector2f &rhs);

			inline friend Vector2f operator+(const Vector2f &lhs, float value);
			inline friend Vector2f operator+(float value, const Vector2f &rhs);

			inline friend Vector2f operator-(const Vector2f &lhs, float value);
			inline friend Vector2f operator-(float value, const Vector2f &rhs);

			inline friend Vector2f operator*(const Vector2f &lhs, float value);
			inline Vector2f& operator *=(float value);

			inline friend Vector2f operator*(float value, const Vector2f &rhs);

			inline Vector2f operator/(float value);

			inline friend float dot(const Vector2f &lhs, const Vector2f &rhs);

			inline void normalize();
			inline Vector2f Normalized( const Vector2f& vec );
			inline float length();
			inline float lengthSQ();

		private:

			INLINE_DECLARE_FIXEDSIZE_ALLOCATOR_8;
		};
	}	// namespace Math
}	// namespace Engine

#include <Engine\Core\Math\Vector2f.inl.h>

#endif //_POSITION_2D_H_