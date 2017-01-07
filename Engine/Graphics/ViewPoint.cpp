#include <Engine\Graphics\ViewPoint.h>

namespace Engine
{
	namespace Graphics
	{
		ViewPoint::ViewPoint()
		{
		}

		ViewPoint::~ViewPoint()
		{
		}

		void ViewPoint::SetPosition(float x, float y, float z)
		{
			m_position = D3DXVECTOR3(x, y, z);
		}

		void ViewPoint::SetLookAt(float x, float y, float z)
		{
			m_lookAt = D3DXVECTOR3(x, y, z);
		}

		void ViewPoint::SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
		{
			m_fieldOfView = fieldOfView;
			m_aspectRatio = aspectRatio;
			m_nearPlane = nearPlane;
			m_farPlane = farPlane;
		}

		void ViewPoint::GenerateViewMatrix()
		{
			D3DXVECTOR3 up;

			// Setup the vector that points upwards.
			up.x = 0.0f;
			up.y = 1.0f;
			up.z = 0.0f;

			// Create the view matrix from the three vectors.
			D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);
		}

		void ViewPoint::GenerateProjectionMatrix()
		{
			D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
		}

		void ViewPoint::GetViewMatrix(D3DXMATRIX& viewMatrix)
		{
			viewMatrix = m_viewMatrix;
		}

		void ViewPoint::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
		{
			projectionMatrix = m_projectionMatrix;
		}
	}	// namespace Graphics
}	// namespace Engine