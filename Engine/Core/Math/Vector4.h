#ifndef __VECTOR4_H__
#define __VECTOR4_H__

namespace Engine
{
	namespace Math
	{
		class Vector3;

		class Vector4
		{
			public:
				// default constructor - good for calling new Vector4[];
				// Since we're made up of POD types, which have no default constructor this does nothing.
				// But we need it. If no constructors exist the compiler will generate one for us. One that
				// takes no parameters and just call the default constructors for it's member variables.
				// Since we're going to define a constructor we also need to define a default constructor
				// so things like Vector4 myVectorArray[] work.
				Vector4( void ) {}	// we define the body here. this is the same as making it an inline.

				// standard constructor
				inline Vector4( float i_x, float i_y, float i_z, float i_w );

				// copy constructor
				inline Vector4( const Vector4 & i_other );
				// copy constructor from Vector3
				inline Vector4( const Vector3 & i_other, float i_w ); 

				// assignment operator
				inline Vector4 & operator=( const Vector4 & i_rhs );

				// element accessors
				// - get individual element
				inline float x( void ) const;
				inline float y( void ) const;
				inline float z( void ) const;
				inline float w( void ) const;
	
				// - set individual element
				inline void x( float i_x );
				inline void y( float i_y );
				inline void z( float i_z );
				inline void w( float i_w );
	
				Vector4 & operator+=( const Vector4 & i_rhs );
				Vector4 & operator-=( const Vector4 & i_rhs );

				Vector4 & operator*=( float i_val );
				Vector4 & operator/=( float i_val );

				// negate
				Vector4 operator-( void );

				void Normalize();
				Vector4 Normalized() const;
	
				float Length() const;
				float LengthSq() const;
	
				// - ( 0.0f, 0.0f, 0.0f )
				static const Vector4 Zero;
			private:
				float					m_x, m_y, m_z, m_w;
		} ;

		inline Vector4 operator+( const Vector4 & i_lhs, const Vector4 & i_rhs );
		inline Vector4 operator-( const Vector4 & i_lhs, const Vector4 & i_rhs );

		inline Vector4 operator*( const Vector4 & i_lhs, float i_rhs );
		inline Vector4 operator/( const Vector4 & i_lhs, float i_rhs );

		inline bool operator==( const Vector4 & i_lhs, const Vector4 & i_rhs );
		inline bool operator!=( const Vector4 & i_lhs, const Vector4 & i_rhs );

		inline float dot( const Vector4 & i_lhs, const Vector4 & i_rhs );
	} // namespace Math
} // namespace Engine

#include <Engine\Core\Math\Vector4-inl.h>

#endif // __VECTOR4_H__