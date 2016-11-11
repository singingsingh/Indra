#include <Engine\Graphics\Camera.h>

#include <Engine\System\Window.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\System\Window.h>
#include <Engine\Util\MathUtils.h>
#include <Engine\System\Timer.h>

namespace Engine
{
	namespace Graphics
	{
		const float Camera::_angularSensitivity = 0.01f;

		Camera::Camera(float i_nearPlane, float i_farPlane, float i_fov, float i_aspectRatio)
		{
			_nearPlane = i_nearPlane;
			_farPlane = i_farPlane;
			_fov = i_fov;
			_aspectRatio = i_aspectRatio;

			D3DXMatrixPerspectiveFovLH(&_projectionMatrix, _fov, _aspectRatio, i_nearPlane, i_farPlane);
			D3DXMatrixOrthoLH(&_orthoProjMatrix, float(System::Window::GetWidth()), float(System::Window::GetHeight()), i_nearPlane, i_farPlane);

			_position.x = 0;
			_position.y = 0;
			_position.z = 0;

			_rotation.x = 0;
			_rotation.y = 0;
			_rotation.z = 0;

			D3DXVECTOR3 up, lookAt, center;

			up.x = 0.0f;
			up.y = 1.0f;
			up.z = 0.0f;

			lookAt.x = 0.0f;
			lookAt.y = 0.0f;
			lookAt.z = 1.0f;

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = -3.0f;

			// Translate the rotated camera position to the location of the viewer.
			lookAt = center + lookAt;

			// Finally create the view matrix from the three updated vectors.
			D3DXMatrixLookAtLH(&_orthoViewMatrix, &center, &lookAt, &up);

			Engine::KeyboardNotifier::RegisterKeyboardUpdate(this);
		}

		Camera::~Camera()
		{
			Engine::KeyboardNotifier::DeRegisterKeyboardUpdate(this);
		}

		void Camera::setPosition(float i_x, float i_y, float i_z)
		{
			_position.x = i_x;
			_position.y = i_y;
			_position.z = i_z;
		}

		void Camera::setRotation(float i_x, float i_y, float i_z)
		{
			_rotation.x = i_x;
			_rotation.y = i_y;
			_rotation.z = i_z;
		}

		D3DXVECTOR3 Camera::getPosition()
		{
			return _position;
		}

		D3DXVECTOR3 Camera::getRotation()
		{
			return _rotation;
		}

		D3DXVECTOR3 Camera::getRightVec()
		{
			D3DXVECTOR3 rightVec;
			rightVec.x = _viewMatrix._11;
			rightVec.y = _viewMatrix._21;
			rightVec.z = _viewMatrix._31;
			D3DXVec3Normalize(&rightVec, &rightVec);
			return rightVec;
		}

		D3DXVECTOR3 Camera::getForwardVec()
		{
			D3DXVECTOR3 forwardVec;
			forwardVec.x = _viewMatrix._13;
			forwardVec.y = _viewMatrix._23;
			forwardVec.z = _viewMatrix._33;
			D3DXVec3Normalize(&forwardVec, &forwardVec);
			return forwardVec;
		}

		D3DXVECTOR3 Camera::getUpVec()
		{
			D3DXVECTOR3 upVec;
			upVec.x = _viewMatrix._12;
			upVec.y = _viewMatrix._22;
			upVec.z = _viewMatrix._32;
			D3DXVec3Normalize(&upVec, &upVec);
			return upVec;
		}

		void Camera::update()
		{
			D3DXVECTOR3 up, lookAt;
			float yaw, pitch, roll;
			D3DXMATRIX rotationMatrix;

			up.x = 0.0f;
			up.y = 1.0f;
			up.z = 0.0f;

			lookAt.x = 0.0f;
			lookAt.y = 0.0f;
			lookAt.z = 1.0f;

			// Setup Look at again for view matrix
			lookAt.x = 0.0f;
			lookAt.y = 0.0f;
			lookAt.z = 1.0f;

			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
			pitch = _rotation.x * MathUtils::DegToRad;
			yaw = _rotation.y * MathUtils::DegToRad;
			roll = _rotation.z * MathUtils::DegToRad;

			// Create the rotation matrix from the yaw, pitch, and roll values.
			D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

			// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
			D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
			D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

			// Translate the rotated camera position to the location of the viewer.
			lookAt = _position + lookAt;

			// Finally create the view matrix from the three updated vectors.
			D3DXMatrixLookAtLH(&_viewMatrix, &_position, &lookAt, &up);
		}

		D3DXMATRIX Camera::getViewMatrix()
		{
			return _viewMatrix;
		}

		D3DXMATRIX Camera::getOrthoViewMatrix()
		{
			return _orthoViewMatrix;
		}

		D3DXMATRIX Camera::getProjMatrix()
		{
			return _projectionMatrix;
		}

		D3DXMATRIX Camera::getOrthoProjMatrix()
		{
			return _orthoProjMatrix;
		}
		void Camera::keyboardUpdate(uint8_t i_key, bool i_down, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("Key %d state = %d, Mouse Location x = %d, y = %d\n", i_key, i_down, i_x, i_y);
			switch (i_key)
			{
				case 0x57:	// w
					if (i_down)
					{
						D3DXVECTOR3 forwardVec = getForwardVec();
						_position += forwardVec * _angularSensitivity * (float)System::Timer::GetDeltaTime();
					}
					break;
				case 0x53:	// s
					if (i_down)
					{
						D3DXVECTOR3 forwardVec = getForwardVec();
						_position -= forwardVec * _angularSensitivity * (float)System::Timer::GetDeltaTime();
					}
					break;
				case 0x41:	// a
					if (i_down)
					{
						D3DXVECTOR3 rightVec = getRightVec();
						_position -= rightVec * _angularSensitivity * (float)System::Timer::GetDeltaTime();
					}
					break;
				case 0x44:	//d
					if (i_down)
					{
						D3DXVECTOR3 rightVec = getRightVec();
						_position += rightVec * _angularSensitivity * (float)System::Timer::GetDeltaTime();
					}
					break;
			}
		}

		void Camera::mouseClickUpdate(uint8_t i_button, bool i_down, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("Button %d state = %d, Mouse Location x = %d, y = %d\n", i_button, i_down, i_x, i_y);
		}

		void Camera::mouseMoveUpdate(bool i_leftBt, bool i_rightBt, bool i_middleBt, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("%d %d %d Mouse Location x = %d, y = %d\n", i_leftBt, i_rightBt, i_middleBt, i_x, i_y);
		}

		void Camera::mousePassiveMoveUpdate(uint16_t i_x, uint16_t i_y)
		{
			static int16_t centerX = static_cast<uint16_t>(System::Window::GetWidth()*0.5f);
			static int16_t centerY = static_cast<uint16_t>(System::Window::GetHeight()*0.5f);

			int16_t diffX = static_cast<int16_t>(i_x) - centerX;
			int16_t diffY = static_cast<int16_t>(i_y) - centerY;

			if (diffX != 0 || diffY != 0)
			{
				//DEBUG_PRINT("Mouse Location x = %d, y = %d\n", diffX, diffY);
				_rotation.x += diffY;
				_rotation.y += diffX;
			}

			//DEBUG_PRINT("Mouse Location x = %d, y = %d\n",i_x, i_y);
		}

		void Camera::mouseWheelUpdate(bool i_direction, uint16_t i_x, uint16_t i_y)
		{
			//DEBUG_PRINT("Roll %d Mouse Location x = %d, y = %d\n", i_direction, i_x, i_y);
		}
	}
}	// namespace Engine
