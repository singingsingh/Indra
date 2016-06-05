#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include <Engine\Core\Math\Vector3.h>

namespace Engine
{
	namespace Math
	{
		class Matrix4;

		class Matrix3
		{
			// This is a very coordinate transform centric Matrix class.
		public:
			// constructors
			// Default constructor - Does Nothing. Initializing 16 floating point numbers
			// 	can be time consuming so we have a default constructor that does nothing.
			//	Most of the time we will be initializing via one of the create functions
			//	below (i.e. CreateXRotationCV( )
			Matrix3();
			//  Value based constructor - This is good for things like creating const
			//	matrix (i.e. Identity matrix
			Matrix3(
				float i_11, float i_12, float i_13,
				float i_21, float i_22, float i_23,
				float i_31, float i_32, float i_33 );

			// Copy constructor & assignment operator
			Matrix3( const Matrix3 & i_other );
			Matrix3 & operator=(const Matrix3 & i_other);

			Matrix3 operator*( const Matrix3 & i_other ) const;

			Vector3 Row( size_t i_row ) const;
			Vector3 Col( size_t i_col ) const;

			Matrix4 ToTransform() const;

			// Useful matrix creators
			static Matrix3 CreateIdentity( void );
			static Matrix3 CreateScale( float i_scaleX, float i_scaleY, float i_scaleZ );

			// Layout for row vector multiplies ( V = V * M )
			static Matrix3 CreateXRotationRV( float i_RotationRadians );
			static Matrix3 CreateYRotationRV( float i_RotationRadians );
			static Matrix3 CreateZRotationRV( float i_RotationRadians );

			// Layout for column vector multiplies ( V = M * V )
			static Matrix3 CreateXRotationCV( float i_RotationRadians );
			static Matrix3 CreateYRotationCV( float i_RotationRadians );
			static Matrix3 CreateZRotationCV( float i_RotationRadians );

			// Transpose this instance
			void Transpose( void );
			// return a matrix that is a transpose of this instance but don't modify this instance
			Matrix3 GetTranspose( void ) const;
			void GetTranspose( Matrix3 & o_out ) const;

			// Multiply Matrix * Matrix
			// return *this * i_other
			void Multiply( const Matrix3 & i_other, Matrix3 & o_out ) const;

			// Multiply Vector by Matrix
			// return v * M
			Vector3 MultiplyLeft( const Vector3 & i_Vector ) const;
			// return M * v
			Vector3 MultiplyRight( const Vector3 & i_Vector ) const;

			static const Matrix3 Identity;

		private:
			float
				// m_<row><column>
				m_11, m_12, m_13, // row 1
				m_21, m_22, m_23, // row 2
				m_31, m_32, m_33; // row 3

			void MultiplyInt( const Matrix3 & i_other, Matrix3 & o_out ) const;

		};

		// Allows us to use V = M * V; (i.e. column vector)
		inline Vector3 operator*(const Matrix3 & i_mtx, const Vector3 & i_vec);
		// Allows us to use V = V * M; (i.e. row vector)
		inline Vector3 operator*(const Vector3 & i_vec, const Matrix3 & i_mtx);


	} // namespace Math
} // namespace Engine

#include <Engine\Core\Math\Matrix3-inl.h>

#endif // __MATRIX3_H__