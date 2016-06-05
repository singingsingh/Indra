#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include <Engine\Core\Math\Vector4.h>

namespace Engine
{
	namespace Math
	{
		class Matrix4;
		// create an alias for cache alignment
		typedef __declspec(align(64)) Matrix4 FastMatrix4;

		class Matrix4
		{
			// This is a very coordinate transform centric Matrix class.
		public:
			// constructors
			// Default constructor - Does Nothing. Initializing 16 floating point numbers
			// 	can be time consuming so we have a default constructor that does nothing.
			//	Most of the time we will be initializing via one of the create functions
			//	below (i.e. CreateTranslation( )
			Matrix4();
			//  Value based constructor - This is good for things like creating const
			//	matrix (i.e. Identity matrix
			Matrix4(
				float i_11, float i_12, float i_13, float i_14,
				float i_21, float i_22, float i_23, float i_24,
				float i_31, float i_32, float i_33, float i_34,
				float i_41, float i_42, float i_43, float i_44 );

			// Copy constructor & assignment operator
			Matrix4( const Matrix4 & i_other );
			Matrix4 & operator=(const Matrix4 & i_other);

			Matrix4 operator*( const Matrix4 & i_other ) const;

			Vector3 TransformPointRV( const Vector3 & i_Point ) const;
			Vector3 TransformPointCV( const Vector3 & i_Point ) const;

			Vector3 TransformNormalRV( const Vector3 & i_Normal ) const;
			Vector3 TransformNormalCV( const Vector3 & i_Normal ) const;

			Vector4 Row( size_t i_row ) const;
			Vector4 Col( size_t i_col ) const;

			// Useful matrix creators
			static Matrix4 CreateIdentity( void );
			static Matrix4 CreateScale( float i_scaleX, float i_scaleY, float i_scaleZ );

			// Layout for row vector multiplies ( V = V * M )
			static Matrix4 CreateXRotationRV( float i_RotationRadians );
			static Matrix4 CreateYRotationRV( float i_RotationRadians );
			static Matrix4 CreateZRotationRV( float i_RotationRadians );

			static Matrix4 CreateTranslationRV( float i_transX, float i_transY, float i_transZ );
			inline static Matrix4 CreateTranslationRV( const Vector3 & i_trans );

			// Layout for column vector multiplies ( V = M * V )
			static Matrix4 CreateXRotationCV( float i_RotationRadians );
			static Matrix4 CreateYRotationCV( float i_RotationRadians );
			static Matrix4 CreateZRotationCV( float i_RotationRadians );

			static Matrix4 CreateTranslationCV( float i_transX, float i_transY, float i_transZ );
			inline static Matrix4 CreateTranslationCV( const Vector3 & i_trans );

			// Invert this instance
			void Invert( void );
			// Return a matrix that is the inverse of this instance but don't modify this instance
			Matrix4 GetInverse( void ) const;
			void GetInverse( Matrix4 & o_out ) const;

			// Return a matrix that is the inverse ASSUMING this matrix has axis rotation and translation only.
			Matrix4 GetInverseRotTransCV() const;
			Matrix4 GetInverseRotTransRV() const;
			
			// Transpose this instance
			void Transpose( void );
			// return a matrix that is a transpose of this instance but don't modify this instance
			Matrix4 GetTranspose( void ) const;
			void GetTranspose( Matrix4 & o_out ) const;

			// Multiply Matrix * Matrix
			// return *this * i_other
			void Multiply( const Matrix4 & i_other, Matrix4 & o_out ) const;

			// Multiply Vector by Matrix
			// return v * M
			Vector4 MultiplyLeft( const Vector4 & i_Vector ) const;
			// return M * v
			Vector4 MultiplyRight( const Vector4 & i_Vector ) const;

			static const FastMatrix4 Identity;

		private:
			float
				// m_<row><column>
				m_11, m_12, m_13, m_14, // row 1
				m_21, m_22, m_23, m_24, // row 2
				m_31, m_32, m_33, m_34, // row 3
				m_41, m_42, m_43, m_44; // row 4

			void MultiplyInt( const Matrix4 & i_other, Matrix4 & o_out ) const;

			// internal inverse function used by Invert() and GetInverse()
			void InverseSSE( Matrix4 & o_out ) const;
			void MultiplySSE( const Matrix4 & i_other, Matrix4 & o_out ) const;
		};

		// Allows us to use V = M * V; (i.e. column vector)
		inline Vector4 operator*(const Matrix4 & i_mtx, const Vector4 & i_vec);
		// Allows us to use V = V * M; (i.e. row vector)
		inline Vector4 operator*(const Vector4 & i_vec, const Matrix4 & i_mtx);
	} // namespace Math
} // namespace Engine

#include <Engine\Core\Math\Matrix4-inl.h>

#endif // __MATRIX4_H__