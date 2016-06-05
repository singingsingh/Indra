#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\MathUtils.h>
#include <Engine\Util\Assert.h>
#include <Engine\Core\Math\Vector2f.h>
#include <Engine\Util\MathUtils.h>

#include <math.h>

namespace Engine
{
	namespace Math
	{
		INLINE_FUNCTION_DEFINE_FIXEDSIZE_ALLOCATOR_8(Vector2f);

		Vector2f::Vector2f()
		{
			x = 0;
			y = 0;

			//DEBUG_PRINT("Creating Vector2f instance row = %d, col = %d\n", x, y);
		}

		Vector2f::~Vector2f()
		{
			//DEBUG_PRINT("Destroying Vector2f instance row = %d, col = %d\n", x, y);
		}

		Vector2f::Vector2f(float i_x, float i_y)
		{
			x = i_x;
			y = i_y;

			//DEBUG_PRINT("Creating Vector2f instance row = %f, col = %f\n", x, y);
		}

		Vector2f operator+(const Vector2f &lhs, const Vector2f &rhs)
		{
			return Vector2f(lhs.x + rhs.x, lhs.y + rhs.y);
		}

		Vector2f operator-(const Vector2f &lhs, const Vector2f &rhs)
		{
			return Vector2f(lhs.x - rhs.x, lhs.y - rhs.y);
		}

		Vector2f& Vector2f::operator +=(float i_value)
		{
			x += i_value;
			y += i_value;

			return *this;
		}

		Vector2f& Vector2f::operator *=(float i_value)
		{
			x *= i_value;
			y *= i_value;

			return *this;
		}

		Vector2f Vector2f::operator/(float i_value)
		{
			MessagedAssert(i_value != 0, "Divisin by Zero.\n");
			return Vector2f(x / i_value, y / i_value);
		}

		inline void Vector2f::normalize()
		{
			float len = length();

			if (MathUtils::IsZero(len))
			{
				return;
			}

			float lenInv = 1.0f / len;

			x = x*lenInv;
			y = y*lenInv;
		}

		inline Vector2f Vector2f::Normalized(const Vector2f& vec)
		{
			Vector2f temp(vec);
			temp.normalize();
			return temp;
		}

		inline float Vector2f::length()
		{
			return sqrtf( x*x + y*y );
		}

		inline float Vector2f::lengthSQ()
		{
			return (x*x + y*y);
		}

		Vector2f& Vector2f::operator -=(float value)
		{
			x -= value;
			y -= value;

			return *this;
		}

		Vector2f operator+(const Vector2f &lhs, float value)
		{
			return Vector2f(lhs.x + value, lhs.y + value);
		}

		Vector2f operator+(float value, const Vector2f &rhs)
		{
			return Vector2f(rhs.x + value, rhs.y + value);
		}

		Vector2f operator-(const Vector2f &lhs, float value)
		{
			return Vector2f(lhs.x - value, lhs.y - value);
		}

		Vector2f operator-(float value, const Vector2f &rhs)
		{
			return Vector2f(value - rhs.x, value - rhs.y);
		}

		Vector2f operator*(const Vector2f &lhs, float value)
		{
			return Vector2f(lhs.x * value, lhs.y * value);
		}

		Vector2f operator*(float value, const Vector2f &rhs)
		{
			return Vector2f(value * rhs.x, value * rhs.y);
		}

		float dot(const Vector2f &lhs, const Vector2f &rhs)
		{
			return lhs.x*rhs.x + lhs.y*rhs.y;
		}

		Vector2f& Vector2f::operator +=(const Vector2f &rhs)
		{
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		Vector2f& Vector2f::operator -=(const Vector2f &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}

		bool operator==(const Vector2f &lhs, const Vector2f &rhs)
		{
			return MathUtils::almostEqual(lhs.x, rhs.x) && MathUtils::almostEqual(lhs.y, rhs.y);
		}
	}	// namespace Math
}	// namespace Engine