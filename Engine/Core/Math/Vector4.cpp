#include <Engine\Core\Math\Vector4.h>

#include <Engine\Util\Assert.h>
#include <Engine\Util\MathUtils.h>

namespace Engine
{
	namespace Math
	{
		const Vector4 Vector4::Zero( 0.0f, 0.0f, 0.0f, 0.0f );

		float Vector4::Length() const
		{
			return sqrtf( ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z ) + ( m_w * m_w ) );
		}

		float Engine::Math::Vector4::LengthSq() const
		{
			return ((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w));
		}

		Vector4 Vector4::Normalized() const
		{
			float length = Length();
			Assert( !MathUtils::IsZero( length ) );

			if (MathUtils::IsZero(length))
			{
				return Zero;
			}
			else
			{
				float inv_length = 1.0f / length;
				return *this * inv_length;
			}
		}

		void Engine::Math::Vector4::Normalize()
		{
			float length = Length();
			Assert(!MathUtils::IsZero(length));

			if (!MathUtils::IsZero(length))
			{
				float inv_length = 1.0f / length;
				(*this) = (*this)*inv_length;
			}
		}
	} // namespace Math
} // namespace Engine