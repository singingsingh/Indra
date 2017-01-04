#include <Engine\Graphics\SpecularLight.h>

namespace Engine
{
	namespace Graphics
	{

		SpecularLight::SpecularLight()
		{
		}

		SpecularLight::SpecularLight(const SpecularLight &)
		{
		}


		SpecularLight::~SpecularLight()
		{
		}

		void SpecularLight::setAmbientColor(float i_red, float i_green, float i_blue, float i_alpha)
		{
			_ambientColor = D3DXVECTOR4(i_red, i_green, i_blue, i_alpha);
		}

		void SpecularLight::setDiffuseColor(float i_red, float i_green, float i_blue, float i_alpha)
		{
			_diffuseColor = D3DXVECTOR4(i_red, i_green, i_blue, i_alpha);
		}

		void SpecularLight::setDirection(float i_x, float i_y, float i_z)
		{
			_direction = D3DXVECTOR3(i_x, i_y, i_z);
		}

		void SpecularLight::setSpecularColor(float i_red, float i_green, float i_blue, float i_alpha)
		{
			_specularColor = D3DXVECTOR4(i_red, i_green, i_blue, i_alpha);
		}

		void SpecularLight::setSpecularPower(float i_power)
		{
			_specularPower = i_power;
		}

		D3DXVECTOR4 SpecularLight::getDiffuseColor()
		{
			return _diffuseColor;
		}

		D3DXVECTOR3 SpecularLight::getDirection()
		{
			return _direction;
		}

		D3DXVECTOR4 SpecularLight::getSpecularColor()
		{
			return _specularColor;
		}

		float SpecularLight::getSpecularPower()
		{
			return _specularPower;
		}

		D3DXVECTOR4 SpecularLight::getAmbientColor()
		{
			return _ambientColor;
		}

	}
}