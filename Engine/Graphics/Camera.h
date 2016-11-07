#ifndef _Camera_H_
#define _Camera_H_

#include <d3dx10math.h>

#include <Engine\Engine\KeyboardNotifier.h>

namespace Engine
{
	namespace Graphics
	{
		class Camera : public Engine::IKeyboardListener
		{
		public:
			Camera(float nearPlane, float farPlane, float fov, float aspectRatio);
			~Camera();

			void setPosition(float, float, float);
			void setRotation(float, float, float);

			D3DXVECTOR3 getPosition();
			D3DXVECTOR3 getRotation();

			void update();
			D3DXMATRIX getViewMatrix();
			D3DXMATRIX getProjectionMatrix();
			D3DXMATRIX getOrthogonalMatrix();

			virtual void keyboardUpdate(uint8_t key, bool down, uint16_t x, uint16_t y);
			virtual void mouseClickUpdate(uint8_t button, bool down, uint16_t x, uint16_t y);
			virtual void mouseMoveUpdate(bool leftBt, bool rightBt, bool middleBt, uint16_t x, uint16_t y);
			virtual void mousePassiveMoveUpdate(uint16_t x, uint16_t y);
			virtual void mouseWheelUpdate(bool direction, uint16_t x, uint16_t y);

		private:

			float _nearPlane;
			float _farPlane;
			float _fov;
			float _aspectRatio;

			D3DXVECTOR3 _position, _rotation;
			D3DXMATRIX _viewMatrix;
			D3DXMATRIX _orthoMatrix;
			D3DXMATRIX _projectionMatrix;
		};
	}
}

#endif	//_Camera_H_