#include <Engine\Util\Assert.h>
#include <Engine\Util\MathUtils.h>

#include "Matrix4.h"
#include "Vector3.h"

#include "Quaternion.h"

namespace Engine
{
	namespace Math
	{
		Quaternion operator*( const Quaternion & i_lhs, const Quaternion & i_rhs )
		{
			return Quaternion( 
							i_lhs.real() * i_rhs.real() - i_lhs.imag().x() * i_rhs.imag().x() - i_lhs.imag().y() * i_rhs.imag().y() - i_lhs.imag().z() * i_rhs.imag().z(),
							i_lhs.real() * i_rhs.imag().x() + i_lhs.imag().x() * i_rhs.real() - i_lhs.imag().y() * i_rhs.imag().z() + i_lhs.imag().z() * i_rhs.imag().y(),
							i_lhs.real() * i_rhs.imag().y() + i_lhs.imag().x() * i_rhs.imag().z() + i_lhs.imag().y() * i_rhs.real() - i_lhs.imag().z() * i_rhs.imag().x(),
							i_lhs.real() * i_rhs.imag().z() - i_lhs.imag().x() * i_rhs.imag().y() + i_lhs.imag().y() * i_rhs.imag().x() + i_lhs.imag().z() * i_rhs.real() );
		}

		void Quaternion::Normalize( void )
		{
			const float magnitude = sqrtf( m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z );
			Assert( !MathUtils::IsZero( magnitude ) );
	
			const float inv_magnitude = 1.0f / magnitude;
	
			m_w = m_w * inv_magnitude;
			m_x = m_x * inv_magnitude;
			m_y = m_y * inv_magnitude;
			m_z = m_z * inv_magnitude;
		}

		Quaternion Quaternion::Normalized( void ) const
		{
			const float magnitude = sqrtf( m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z );
			Assert( !MathUtils::IsZero( magnitude ) );
	
			const float inv_magnitude = 1.0f / magnitude;
	
			return Quaternion( m_w * inv_magnitude, m_x * inv_magnitude, m_y * inv_magnitude, m_z * inv_magnitude );
		}

		Matrix4 Quaternion::ToRotationMatrix( void ) const
		{
			Quaternion normalized = Normalized();

			const float w = normalized.m_w;
			const float x = normalized.m_x;
			const float y = normalized.m_y;
			const float z = normalized.m_z;
	
			return Matrix4(
				1.0f - 2.0f*y*y - 2.0f*z*z,	2.0f*x*y - 2.0f*z*w,				2.0f*x*z + 2.0f*y*w, 			0.0f,
				2.0f*x*y + 2.0f*z*w, 			1.0f - 2.0f*x*x - 2.0f*z*z, 	2.0f*y*z - 2.0f*x*w, 			0.0f,
				2.0f*x*z - 2.0f*y*w, 			2.0f*y*z + 2.0f*x*w, 			1.0f - 2.0f*x*x - 2.0f*y*y,	0.0f,
				0.0f, 								0.0f, 								0.0f, 								1.0f );
		}
	} // namespace Math
} // namespace Engine