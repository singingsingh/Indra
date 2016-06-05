#include <Engine\Util\MathUtils.h>
#include <Engine\Util\Assert.h>

#include <math.h>

namespace Engine
{
	const float MathUtils::MAX_ABSOLUTE_ERROR = 0.00001f;
	const float MathUtils::MAX_RELATIVE_ERROR = 0.00001f;
	const float MathUtils::PI = 3.14159f;

	bool MathUtils::almostEqual(float lhs, float rhs, float maxRelativeError, float maxAbsoluteError)
	{
		MessagedAssert(!IsNAN(lhs), "lhs is NAN.\n");
		MessagedAssert(!IsNAN(rhs), "rhs is NAN.\n");

		if (fabs(lhs - rhs) < maxAbsoluteError)
		{
			return true;
		}
			
		float relativeError;
		if (fabs(rhs) > fabs(lhs))
		{
			relativeError = (float)fabs((lhs - rhs) / rhs);
		}
		else
		{
			relativeError = (float)fabs((lhs - rhs) / lhs);
		}
			
		if (relativeError <= maxRelativeError)
		{
			return true;
		}

		return false;
	}

	float MathUtils::ToDegrees(float i_radians)
	{
		return i_radians*(180.0f / PI);
	}

	float MathUtils::ToRadians(float i_degree)
	{
		return i_degree*( PI / 180.0f );
	}

	bool MathUtils::almostEqual(float lhs, float rhs, float maxRelativeError)
	{
		return almostEqual(lhs, rhs, maxRelativeError, MAX_ABSOLUTE_ERROR);
	}

	bool MathUtils::almostEqual(float lhs, float rhs)
	{
		return almostEqual(lhs, rhs, MAX_RELATIVE_ERROR, MAX_ABSOLUTE_ERROR);
	}

	bool MathUtils::IsNAN(float value)
	{
		if (value != value)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool MathUtils::IsZero(float i_val)
	{
		return almostEqual(0.0f, i_val, .000000001f);
	}
}