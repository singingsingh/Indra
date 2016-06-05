#ifndef __VECTOR3_H__
#define __VECTOR3_H__

namespace Engine
{
	namespace Math
	{
		class Vector4;
		class Vector3
		{
			private:
				float	m_x, m_y, m_z;

			public:
				// default constructor - good for calling new Vector3[];
				Vector3( void )			// we define the body here. this is the same as making it an inline.
										// in an optimized build it should be a no-op.
				{
				}

				// standard constructor
				inline Vector3( float i_x, float i_y, float i_z );
	
				// copy constructor - for completeness
				inline Vector3( const Vector3 & i_other );

				// assignment operator
				inline const Vector3 & operator=( const Vector3 & i_rhs );

				// element accessors
				// - get individual element
				inline float x( void ) const;
				inline float y( void ) const;
				inline float z( void ) const;
				// - set individual element
				inline void x( float i_x );
				inline void y( float i_y );
				inline void z( float i_z );

				const Vector3 & operator+=( const Vector3 & i_rhs );
				const Vector3 & operator-=( const Vector3 & i_rhs );

				const Vector3 & operator*=( float i_val );
				const Vector3 & operator/=( float i_val );

				// negate
				const Vector3 operator-( void );

				void Normalize();
				Vector3 Normalized() const;

				float Length() const;
				float LengthSq() const;
	
				// - ( 0.0f, 0.0f, 0.0f )
				static const Vector3 Zero;
		} ;

		inline Vector3 operator+( const Vector3 & i_lhs, const Vector3 & i_rhs );
		inline Vector3 operator-( const Vector3 & i_lhs, const Vector3 & i_rhs );

		inline Vector3 operator*( const Vector3 & i_lhs, float i_rhs );
		inline Vector3 operator*( float i_lhs, const Vector3 & i_rhs );
		inline Vector3 operator/( const Vector3 & i_lhs, float i_rhs );

		inline bool operator==( const Vector3 & i_lhs, const Vector3 & i_rhs );
		inline bool operator!=( const Vector3 & i_lhs, const Vector3 & i_rhs );

		inline float dot( const Vector3 & i_lhs, const Vector3 & i_rhs );
		inline Vector3 cross( const Vector3 & i_lhs, const Vector3 & i_rhs );

	} // namespace Math
} // namespace Engine

#include <Engine\Core\Math\Vector3-inl.h>
#endif // __VECTOR3_H__