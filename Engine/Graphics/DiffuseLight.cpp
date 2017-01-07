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

		D3DXVECTOR4 DiffuseLight::getDiffuseColor()
		{
			return _diffuseColor;
		}

		D3DXVECTOR4 DiffuseLight::getAmbientColor()
		{
			return _ambientColor;
		}

		D3DXVECTOR3 DiffuseLight::getDirection()
		{
			return _direction;
		}
	}
}