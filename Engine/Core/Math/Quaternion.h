#ifndef __QUATERNION_H__
#define __QUATERNION_H__

namespace Engine
{
	namespace Math
	{
		class Matrix4;
		class Vector3;

		class Quaternion
		{
			public:
				Quaternion() {}
				Quaternion( float i_w, float i_x, float i_y, float i_z );
				Quaternion( float i_real, const Vector3 & i_imag );
	
				Quaternion( const Quaternion & i_other );
				Quaternion & operator=( const Quaternion & i_other );
	
				float real() const;
				Vector3 imag() const;
	
				Quaternion & operator-( void );
	
				Quaternion & operator*=( float i_other );
				Quaternion & operator/=( float i_other );
	
				Quaternion & operator+=( const Quaternion & i_other );
				Quaternion & operator-=( const Quaternion & i_other );
				Quaternion & operator*=( const Quaternion & i_other );
	
				float dot( const Quaternion & i_other );

				void Normalize( void );
				Quaternion Normalized( void ) const;
	
				float Magnitude( void ) const;
	
				Matrix4 ToRotationMatrix( void ) const;
	
				static Quaternion CreateRotation( const Vector3 & i_Axis, float i_Rotation );

			private:
				float		m_w, m_x, m_y, m_z;
		} ;

		// To / From Euler angle

		Quaternion operator*( const Quaternion & i_lhs, float i_rhs );
		Quaternion operator*( float i_lhs, const Quaternion & i_rhs );

		Quaternion operator/( const Quaternion & i_lhs, float i_rhs );

		Quaternion operator+( const Quaternion & i_lhs, const Quaternion & i_rhs );
		Quaternion operator-( const Quaternion & i_lhs, const Quaternion & i_rhs );
		Quaternion operator*( const Quaternion & i_lhs, const Quaternion & i_rhs );
			
		float dot( const Quaternion & i_lhs, const Quaternion & i_rhs );

		Quaternion slerp( const Quaternion & i_start, const Quaternion & i_end, float i_dist );
	
	} // namespace Math
} // namespace Engine

#include "Quaternion-inl.h"

#endif // __QUATERNION_H__