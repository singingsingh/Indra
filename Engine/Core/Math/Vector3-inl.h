#include <Engine\Util\Assert.h>
#include <Engine\Core\Math\Vector4.h>

#include <math.h>

namespace Engine
{
	namespace Math
	{
		inline Vector3::Vector3( float i_x, float i_y, float i_z ) :
			m_x( i_x ),
			m_y( i_y ),
			m_z( i_z )
		{
			// lets not create a Vector element with an invalid float
			Assert( !MathUtils::IsNAN( i_x ) );
			Assert( !MathUtils::IsNAN( i_y ) );
			Assert( !MathUtils::IsNAN( i_z ) );
		}

		inline Vector3::Vector3( const Vector3 & i_rhs ) :
			m_x( i_rhs.m_x ),
			m_y( i_rhs.m_y ),
			m_z( i_rhs.m_z )
		{
			// lets not create a Vector element with an invalid float
			Assert( !MathUtils::IsNAN( i_rhs.m_x ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_y ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_z ) );
		}

		// operators
		inline const Vector3 & Vector3::operator=( const Vector3 & i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs.m_x ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_y ) );
			Assert( !MathUtils::IsNAN( i_rhs.m_z ) );

			m_x = i_rhs.m_x;
			m_y = i_rhs.m_y;
			m_z = i_rhs.m_z;

			return *this;
		}

		// accessors
		inline float Vector3::x( void ) const
		{
			return m_x;
		}

		inline float Vector3::y( void ) const
		{
			return m_y;
		}

		inline float Vector3::z( void ) const
		{
			return m_z;
		}

		inline void Vector3::x( float i_x )
		{
			Assert( !MathUtils::IsNAN( i_x ) );
	
			m_x = i_x;
		}

		inline void Vector3::y( float i_y )
		{
			Assert( !MathUtils::IsNAN( i_y ) );
	
			m_y = i_y;
		}

		inline void Vector3::z( float i_z )
		{
			Assert( !MathUtils::IsNAN( i_z ) );
	
			m_z = i_z;
		}

		inline const Vector3 & Vector3::operator+=( const Vector3 & i_rhs )
		{
			m_x += i_rhs.m_x;
			m_y += i_rhs.m_y;
			m_z += i_rhs.m_z;

			return *this;
		}

		inline const Vector3 & Vector3::operator-=( const Vector3 & i_rhs )
		{
			m_x -= i_rhs.m_x;
			m_y -= i_rhs.m_y;
			m_z -= i_rhs.m_z;

			return *this;
		}

		inline const Vector3 & Vector3::operator*=( float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ) );

			m_x *= i_rhs;
			m_y *= i_rhs;
			m_z *= i_rhs;

			return *this;
		}

		inline const Vector3 & Vector3::operator/=( float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ) );
			Assert( !MathUtils::IsZero( i_rhs ) );

			m_x /= i_rhs;
			m_y /= i_rhs;
			m_z /= i_rhs;

			return *this;
		}

		inline const Vector3 Vector3::operator-( void )
		{
			return Vector3( -m_x, -m_y, -m_z );
		}

		inline float Vector3::LengthSq() const
		{
			return ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z );
		}

		inline float Vector3::Length() const
		{
			return sqrtf( LengthSq() );
		}

		// stand alone operators
		inline Vector3 operator+( const Vector3 & i_lhs, const Vector3 & i_rhs )
		{
			return Vector3( i_lhs.x() + i_rhs.x(), i_lhs.y() + i_rhs.y(), i_lhs.z() + i_rhs.z() );
		}

		inline Vector3 operator-( const Vector3 & i_lhs, const Vector3 & i_rhs )
		{
			return Vector3( i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y(), i_lhs.z() - i_rhs.z() );
		}

		inline Vector3 operator*( const Vector3 & i_lhs, float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ));

			return Vector3( i_lhs.x() * i_rhs, i_lhs.y() * i_rhs, i_lhs.z() * i_rhs );
		}

		inline Vector3 operator*( float i_lhs, const Vector3 & i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_lhs ));

			return Vector3( i_rhs.x() * i_lhs, i_rhs.y() * i_lhs, i_rhs.z() * i_lhs );
		}

		inline Vector3 operator/( const Vector3 & i_lhs, float i_rhs )
		{
			Assert( !MathUtils::IsNAN( i_rhs ));
			Assert( !MathUtils::IsZero( i_rhs ));

			return Vector3( i_lhs.x() / i_rhs, i_lhs.y() / i_rhs, i_lhs.z() / i_rhs );
		}

		inline bool operator==( const Vector3 & i_lhs, const Vector3 & i_rhs )
		{
			return MathUtils::almostEqual( i_lhs.x(), i_rhs.x() ) && MathUtils::almostEqual( i_lhs.y(), i_rhs.y() ) && MathUtils::almostEqual( i_lhs.z(), i_rhs.z() );
		}

		inline bool operator!=( const Vector3 & i_lhs, const Vector3 & i_rhs )
		{
			return !MathUtils::almostEqual( i_lhs.x(), i_rhs.x() ) || !MathUtils::almostEqual( i_lhs.y(), i_rhs.y() ) || !MathUtils::almostEqual( i_lhs.z(), i_rhs.z() );
		}

		inline float dot( const Vector3 & i_lhs, const Vector3 & i_rhs )
		{
			return i_lhs.x() * i_rhs.x() + i_lhs.y() * i_rhs.y() + i_lhs.z() * i_rhs.z();
		}

		inline Vector3 cross( const Vector3 & i_lhs, const Vector3 & i_rhs )
		{
			return Vector3( i_lhs.y() * i_rhs.z() - i_lhs.z() * i_rhs.y(),
							i_lhs.z() * i_rhs.x() - i_lhs.x() * i_rhs.z(),
							i_lhs.x() * i_rhs.y() - i_lhs.y() * i_rhs.x() );
		}
	} // namespace Math
} // namespace Engine