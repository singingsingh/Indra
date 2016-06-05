#include <Engine\Util\Assert.h>
#include <Engine\Util\MathUtils.h>
#include <Engine\Core\Math\Vector3.h>

namespace Engine
{
	namespace Math
	{
		inline Vector4::Vector4( float i_x, float i_y, float i_z, float i_w ) :
			m_x( i_x ),
			m_y( i_y ),
			m_z( i_z ),
			m_w( i_w )
		{
			// lets not create a Vector element with an invalid float
			Assert( !MathUtils::IsNAN( i_x ) );
			Assert( !MathUtils::IsNAN( i_y ) );
			Assert( !MathUtils::IsNAN( i_z ) );
			Assert( !MathUtils::IsNAN( i_w ) );
		}

		inline Vector4::Vector4( const Vector3 & i_other, float i_w ) :
			m_x( i_other.x() ),
			m_y( i_other.y() ),
			m_z( i_other.z() ),
			m_w( i_w )
		{
			// lets not create a Vector element with an invalid float
			Assert( !MathUtils::IsNAN( i_other.x() ) );
			Assert( !MathUtils::IsNAN( i_other.y() ) );
			Assert( !MathUtils::IsNAN( i_other.z() ) );
			Assert( !MathUtils::IsNAN( i_w ) );
		}

		inline Vector4::Vector4( const Vector4 & i_rhs ) :
			m_x( i_rhs.m_x ),
			m_y( i_rhs.m_y ),
			m_z( i_rhs.m_z ),
			m_w( i_rhs.m_w )
		{
			// lets not create a Vector element with an invalid float
			Assert( !MathUtils::IsNAN( i_rhs.m_x ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_y ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_z ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_w ) );
		}

		// operators
		inline Vector4 & Vector4::operator=( const Vector4 & i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs.m_x ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_y ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_z ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_w ) );

			m_x = i_rhs.m_x;
			m_y = i_rhs.m_y;
			m_z = i_rhs.m_z;
			m_w = i_rhs.m_w;

			return *this;
		}

		// accessors
		inline float Vector4::x( void ) const
		{
			return m_x;
		}

		inline float Vector4::y( void ) const
		{
			return m_y;
		}

		inline float Vector4::z( void ) const
		{
			return m_z;
		}

		inline float Vector4::w( void ) const
		{
			return m_w;
		}

		inline void Vector4::x( float i_x )
		{
			Assert( !MathUtils::IsNAN( i_x ) );
	
			m_x = i_x;
		}

		inline void Vector4::y( float i_y )
		{
			Assert( !MathUtils::IsNAN( i_y ) );
	
			m_y = i_y;
		}

		inline void Vector4::z( float i_z )
		{
			Assert( !MathUtils::IsNAN( i_z ) );
	
			m_z = i_z;
		}

		inline void Vector4::w( float i_w )
		{
			Assert( !MathUtils::IsNAN( i_w ) );
	
			m_w = i_w;
		}

		inline Vector4 & Vector4::operator+=( const Vector4 & i_rhs )
		{
			m_x += i_rhs.m_x;
			m_y += i_rhs.m_y;
			m_z += i_rhs.m_z;
			m_w += i_rhs.m_w;

			return *this;
		}

		inline Vector4 & Vector4::operator-=( const Vector4 & i_rhs )
		{
			m_x -= i_rhs.m_x;
			m_y -= i_rhs.m_y;
			m_z -= i_rhs.m_z;
			m_w -= i_rhs.m_w;

			return *this;
		}

		inline Vector4 & Vector4::operator*=( float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ) );

			m_x *= i_rhs;
			m_y *= i_rhs;
			m_z *= i_rhs;
			m_w *= i_rhs;

			return *this;
		}

		inline Vector4 & Vector4::operator/=( float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ) );
			Assert( !MathUtils::IsZero( i_rhs ) );

			float inv_rhs = 1.0f / i_rhs;
	
			m_x *= inv_rhs;
			m_y *= inv_rhs;
			m_z *= inv_rhs;
			m_w *= inv_rhs;

			return *this;
		}

		inline Vector4 Vector4::operator-( void )
		{
			return Vector4( -m_x, -m_y, -m_z, -m_w );
		}

		// stand alone operators
		inline Vector4 operator+( const Vector4 & i_lhs, const Vector4 & i_rhs )
		{
			return Vector4( i_lhs.x() + i_rhs.x(), i_lhs.y() + i_rhs.y(), i_lhs.z() + i_rhs.z(), i_lhs.w() + i_rhs.w() );
		}

		inline Vector4 operator-( const Vector4 & i_lhs, const Vector4 & i_rhs )
		{
			return Vector4( i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y(), i_lhs.z() - i_rhs.z(), i_lhs.w() -i_rhs.w() );
		}

		inline Vector4 operator*( const Vector4 & i_lhs, float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ));

			return Vector4( i_lhs.x() * i_rhs, i_lhs.y() * i_rhs, i_lhs.z() * i_rhs, i_lhs.w() * i_rhs );
		}

		inline Vector4 operator/( const Vector4 & i_lhs, float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ));
			Assert( !MathUtils::IsZero( i_rhs ));

			return Vector4( i_lhs.x() / i_rhs, i_lhs.y() / i_rhs, i_lhs.z() / i_rhs, i_lhs.w() / i_rhs );
		}

		inline bool operator==( const Vector4 & i_lhs, const Vector4 & i_rhs )
		{
			return MathUtils::almostEqual( i_lhs.x(), i_rhs.x() ) &&
				MathUtils::almostEqual( i_lhs.y(), i_rhs.y() ) &&
				MathUtils::almostEqual( i_lhs.z(), i_rhs.z() ) &&
				MathUtils::almostEqual( i_lhs.w(), i_rhs.w() );
		}

		inline bool operator!=( const Vector4 & i_lhs, const Vector4 & i_rhs )
		{
			return !MathUtils::almostEqual( i_lhs.x(), i_rhs.x() ) ||
					!MathUtils::almostEqual( i_lhs.y(), i_rhs.y() ) ||
					!MathUtils::almostEqual( i_lhs.z(), i_rhs.z() ) ||
					!MathUtils::almostEqual( i_lhs.w(), i_rhs.w() );
		}

		inline float dot( const Vector4 & i_lhs, const Vector4 & i_rhs )
		{
			return i_lhs.x() * i_rhs.x() + i_lhs.y() * i_rhs.y() + i_lhs.z() * i_rhs.z() + i_lhs.w() * i_rhs.w();
		}
	} // namespace Math
} // namespace Engine