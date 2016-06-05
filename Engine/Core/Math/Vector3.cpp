#include <Engine\Core\Math\Vector3.h>

#include <Engine\Util\Assert.h>
#include <Engine\Util\MathUtils.h>

namespace Engine
{
	namespace Math
	{
		const Vector3 Vector3::Zero( 0.0f, 0.0f, 0.0f );

		Vector3 Vector3::Normalized() const
		{
			float length = Length();
			Assert( !MathUtils::IsZero( length ) );

			if(MathUtils::IsZero( length ) )
				return Zero;
			else
			{
				float inv_length = 1.0f / length;

				return *this * inv_length;
			}
		}
	} // namespace Math
} // namespace Engine