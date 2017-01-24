#include <Engine\Graphics\DiffuseLight.h>

namespace Engine
{
	namespace Graphics
	{
		DiffuseLight::DiffuseLight()
		{
		}

		DiffuseLight::~DiffuseLight()
		{
		}

		void DiffuseLight::setAmbientColor(float r, float g, float b, float a)
		{
			_ambientColor = D3DXVECTOR4(r, g, b, a);
		}

		void DiffuseLight::setDiffuseColor(float i_r, float i_g, float i_b, float i_a)
		{
			_diffuseColor = D3DXVECTOR4(i_r, i_g, i_b, i_a);
		}

		void DiffuseLight::setDirection(float i_x, float i_y, float i_z)
		{
			_direction = D3DXVECTOR3(i_x, i_y, i_z);
		}

		void DiffuseLight::SetPosition(float x, float y, float z)
		{
			m_position = D3DXVECTOR3(x, y, z);
		}

		void DiffuseLight::SetLookAt(float x, float y, float z)
		{
			m_lookAt.x = x;
			m_lookAt.y = y;
			m_lookAt.z = z;
		}

		void DiffuseLight::setSpecularColor(float i_red, float i_green, float i_blue, float i_alpha)
		{
			_specularColor.x = i_red;
			_specularColor.y = i_green;
			_specularColor.z = i_blue;
			_specularColor.w = i_alpha;
		}

		void DiffuseLight::setSpecularPower(float i_specularPower)
		{
			_specularPower = i_specularPower;
		}

		D3DXVECTOR4 DiffuseLight::getDiffuseColor() const
		{
			return _diffuseColor;
		}

		D3DXVECTOR4 DiffuseLight::getAmbientColor() const
		{
			return _ambientColor;
		}

		D3DXVECTOR3 DiffuseLight::getDirection() const
		{
			return _direction;
		}

		D3DXVECTOR3 DiffuseLight::getPosition()
		{
			return m_position;
		}

		D3DXVECTOR4 DiffuseLight::getSpecularColor() const
		{
			return _specularColor;
		}

		float DiffuseLight::getSpecularPower() const
		{
			return _specularPower;
		}

		void DiffuseLight::GenerateViewMatrix()
		{
			D3DXVECTOR3 up;


			// Setup the vector that points upwards.
			up.x = 0.0f;
			up.y = 1.0f;
			up.z = 0.0f;

			// Create the view matrix from the three vectors.
			D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);
		}

		void DiffuseLight::GenerateProjectionMatrix(float screenDepth, float screenNear)
		{
			float fieldOfView, screenAspect;


			// Setup field of view and screen aspect for a square light source.
			fieldOfView = (float)D3DX_PI / 2.0f;
			screenAspect = 1.0f;

			// Create the projection matrix for the light.
			D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
		}

		void DiffuseLight::generateDirection()
		{
			_direction = m_lookAt - m_position;
			D3DXVec3Normalize(&_direction, &_direction);
		}

		void DiffuseLight::GetViewMatrix(D3DXMATRIX& viewMatrix)
		{
			viewMatrix = m_viewMatrix;
		}

		void DiffuseLight::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
		{
			projectionMatrix = m_projectionMatrix;
		}
	}
}