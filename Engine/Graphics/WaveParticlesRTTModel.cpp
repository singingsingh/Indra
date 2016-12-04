#include <Engine\Graphics\WaveParticlesRTTModel.h>

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Core\MemoryMgr.h>
#include <Engine\Util\MathUtils.h>
#include <Engine\System\Timer.h>
#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>

namespace Engine
{
	namespace Graphics
	{

		WaveParticlesRTTModel::WaveParticlesRTTModel(float i_xMin, float i_xMax, float i_yMin, float i_yMax)
			:_numParticles(14000),
			_waveParticleMemPool(static_cast<WaveParticle*>(MemoryMgr::getInstance()->allocMemory(_numParticles * sizeof(WaveParticle)))),
			_vertices (static_cast<VertexType*>(MemoryMgr::getInstance()->allocMemory(_numParticles*sizeof(VertexType))))
		{
			_vertexBuffer = nullptr;
			_activeParticles = 0;

			_freeList = nullptr;
			_subDivList = nullptr;
			_reflecList = nullptr;

			_xMin = i_xMin;
			_xMax = i_xMax;
			_yMin = i_yMin;
			_yMax = i_yMax;

			_gridWidth = (_xMax - _xMin) * 0.5f;
			_gridHeight = (_yMax - _yMin) * 0.5f;

			_topRight = D3DXVECTOR2(_xMax, _yMax);
			_topLeft = D3DXVECTOR2(_xMin, _yMax);
			_bottomRight = D3DXVECTOR2(_xMax, _yMin);
			_bottomLeft = D3DXVECTOR2(_xMin, _yMin);

			_minAmp = 0.005f;

			_numSubDiv = 0;
			_numReflect = 0;
		}

		WaveParticlesRTTModel::~WaveParticlesRTTModel()
		{
			MemoryMgr::getInstance()->dellocMemory(static_cast<void*>(const_cast<WaveParticle*>(_waveParticleMemPool)));
			MemoryMgr::getInstance()->dellocMemory(static_cast<void*>(const_cast<VertexType*>(_vertices)));
		}

		bool WaveParticlesRTTModel::initialize()
		{
			bool result;

			initializeWaveParticlesList();

			result = createBuffers();
			if (!result)
			{
				return false;
			}

			return true;
		}

		void WaveParticlesRTTModel::shutdown()
		{
			_vertexBuffer->Release();
			_vertexBuffer = 0;
		}

		void WaveParticlesRTTModel::render()
		{
			_currentTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());

			reflectParticles();
			subDivideParticles();

			unsigned int stride;
			unsigned int offset;

			stride = sizeof(VertexType);
			offset = 0;

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		}

		int WaveParticlesRTTModel::getVertexCount()
		{
			return _activeParticles;
		}

		void WaveParticlesRTTModel::spawnParticles()
		{
			_currentTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());

			NextActionType actionType;

			WaveParticle* newParticles = getFreePartices(1);
			float waveVelocity = 0.008f;
			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2(0.0, 1.0);
			newParticles->velocity = waveVelocity;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			actionType = getNextActionTime(newParticles);
			newParticles->next = nullptr;

			if (actionType == NextActionType::REFLECTION)
			{
				pushToList(newParticles, _reflecList);
				_numReflect++;
			}
			else
			{
				pushToList(newParticles, _subDivList);
				_numSubDiv++;
			}


			newParticles = getFreePartices(1);
			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(90.0f*MathUtils::DegToRad), (float)cos(90.0f*MathUtils::DegToRad));
			newParticles->velocity = waveVelocity;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			actionType = getNextActionTime(newParticles);
			newParticles->next = nullptr;

			if (actionType == NextActionType::REFLECTION)
			{
				pushToList(newParticles, _reflecList);
				_numReflect++;
			}
			else
			{
				pushToList(newParticles, _subDivList);
				_numSubDiv++;
			}

			newParticles = getFreePartices(1);
			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(-90.0f*MathUtils::DegToRad), (float)cos(-90.0f*MathUtils::DegToRad));
			newParticles->velocity = waveVelocity;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			actionType = getNextActionTime(newParticles);
			newParticles->next = nullptr;

			if (actionType == NextActionType::REFLECTION)
			{
				pushToList(newParticles, _reflecList);
				_numReflect++;
			}
			else
			{
				pushToList(newParticles, _subDivList);
				_numSubDiv++;
			}

			newParticles = getFreePartices(1);
			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(180.0f*MathUtils::DegToRad), (float)cos(180.0f*MathUtils::DegToRad));
			newParticles->velocity = waveVelocity;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			actionType = getNextActionTime(newParticles);
			newParticles->next = nullptr;

			if (actionType == NextActionType::REFLECTION)
			{
				pushToList(newParticles, _reflecList);
				_numReflect++;
			}
			else
			{
				pushToList(newParticles, _subDivList);
				_numSubDiv++;
			}

			//newParticles = getFreePartices(1);
			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(-60.0f*MathUtils::DegToRad), (float)cos(-60.0f*MathUtils::DegToRad));
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//actionType = getNextActionTime(newParticles);
			//newParticles->next = nullptr;

			//if (actionType == NextActionType::REFLECTION)
			//{
			//	pushToList(newParticles, _reflecList);
			//	_numReflect++;
			//}
			//else
			//{
			//	pushToList(newParticles, _subDivList);
			//	_numSubDiv++;
			//}

			//newParticles = getFreePartices(1);
			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(60.0f*MathUtils::DegToRad), (float)cos(60.0f*MathUtils::DegToRad));
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//actionType = getNextActionTime(newParticles);
			//newParticles->next = nullptr;

			//if (actionType == NextActionType::REFLECTION)
			//{
			//	pushToList(newParticles, _reflecList);
			//	_numReflect++;
			//}
			//else
			//{
			//	pushToList(newParticles, _subDivList);
			//	_numSubDiv++;
			//}


			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(30.0f*MathUtils::DegToRad), (float)cos(30.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS + 
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(-30.0f*MathUtils::DegToRad), (float)cos(-30.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS + 
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(60.0f*MathUtils::DegToRad), (float)cos(60.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(-60.0f*MathUtils::DegToRad), (float)cos(-60.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(90.0f*MathUtils::DegToRad), (float)cos(90.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(-90.0f*MathUtils::DegToRad), (float)cos(-90.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(120.0f*MathUtils::DegToRad), (float)cos(120.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(-120.0f*MathUtils::DegToRad), (float)cos(-120.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(150.0f*MathUtils::DegToRad), (float)cos(150.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(-150.0f*MathUtils::DegToRad), (float)cos(-150.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles = newParticles->next;

			//newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			//newParticles->direction = D3DXVECTOR2((float)sin(180.0f*MathUtils::DegToRad), (float)cos(180.0f*MathUtils::DegToRad));
			//D3DXVec2Normalize(&newParticles->direction, &newParticles->direction);
			//newParticles->velocity = waveVelocity;
			//newParticles->amplitude = 1.5f;
			//newParticles->radius = 0.2f;
			//newParticles->angle = 30.0f;
			//newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			//newParticles->actionTimeMS = newParticles->spawnTimeMS +
			//	(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			//newParticles->next = nullptr;

			//pushToList(first, _subDivList);
			updateBuffers();
		}

		float WaveParticlesRTTModel::getCurrentTime()
		{
			return _currentTimeMS;
		}

		void WaveParticlesRTTModel::subDivideParticles()
		{
			WaveParticle* currentParticle = _subDivList;

			bool particlesSubdiveded = false;

			while (currentParticle && currentParticle->actionTimeMS < _currentTimeMS)
			{
				//static int i = 0;
				//i++;
				//DEBUG_PRINT("subdivided = %d\n", i);
				particlesSubdiveded = true;
				_subDivList = currentParticle->next;

				{
					D3DXVECTOR2 currentPos = currentParticle->origin +
						(currentParticle->direction * currentParticle->velocity * (currentParticle->actionTimeMS - currentParticle->spawnTimeMS));

					//DEBUG_PRINT("subdivide at x = %f y = %f\n", currentPos.x, currentPos.y);
				}

				// check to see if the amplitude is less than threshold and discard accordingly
				if (currentParticle->amplitude < _minAmp)
				{
					currentParticle->next = nullptr;
					recycleParticles(currentParticle);

					currentParticle = _subDivList;
					continue;
				}

				WaveParticle* newParticles = getFreePartices(2);
				currentParticle->next = newParticles;

				currentParticle->amplitude = currentParticle->amplitude * 0.33333333f;
				currentParticle->angle = currentParticle->angle * 0.33333333f;

				WaveParticle* temp = newParticles;
				while (temp)
				{
					temp->origin = currentParticle->origin;
					temp->spawnTimeMS = currentParticle->spawnTimeMS;
					temp->angle = currentParticle->angle;
					temp->amplitude = currentParticle->amplitude;
					temp->radius = currentParticle->radius;
					temp->velocity = currentParticle->velocity;

					temp = temp->next;
				}

				D3DXVECTOR2 direction = currentParticle->direction;
				D3DXVECTOR2 newDirection = direction;
				float angle = currentParticle->angle*MathUtils::DegToRad;
				newDirection.x = direction.x * (float)cos(angle) - direction.y * (float)sin(angle);
				newDirection.y = direction.x * (float)sin(angle) + direction.y * (float)cos(angle);
				temp = newParticles;
				temp->direction = newDirection;
				D3DXVec2Normalize(&temp->direction, &temp->direction);

				temp = temp->next;

				direction = currentParticle->direction;
				newDirection = currentParticle->direction;
				angle = -currentParticle->angle*MathUtils::DegToRad;
				newDirection.x = direction.x * (float)cos(angle) - direction.y * (float)sin(angle);
				newDirection.y = direction.x * (float)sin(angle) + direction.y * (float)cos(angle);
				temp->direction = newDirection;
				D3DXVec2Normalize(&temp->direction, &temp->direction);

				// push into the appropriate list
				{
					WaveParticle* nextParticle = nullptr;
					_numSubDiv--;
					while (currentParticle)
					{
						nextParticle = currentParticle->next;
						currentParticle->next = nullptr;
						NextActionType actionType = getNextActionTime(currentParticle);

						if (actionType == NextActionType::REFLECTION)
						{
							pushToList(currentParticle, _reflecList);
							_numReflect++;
						}
						else
						{
							pushToList(currentParticle, _subDivList);
							_numSubDiv++;
						}

						currentParticle = nextParticle;
					}
				}

				currentParticle = _subDivList;
			}

			if (particlesSubdiveded)
			{
				updateBuffers();
			}
		}

		void WaveParticlesRTTModel::reflectParticles()
		{
			WaveParticle* currentParticle = _reflecList;

			bool particlesReflected = false;

			while (currentParticle && currentParticle->actionTimeMS < _currentTimeMS)
			{
				particlesReflected = true;
				_reflecList = _reflecList->next;

				D3DXVECTOR2 currentPos = currentParticle->origin + 
					(currentParticle->direction * currentParticle->velocity * (currentParticle->actionTimeMS - currentParticle->spawnTimeMS));

				//DEBUG_PRINT("reflect at x = %f y = %f\n", currentPos.x, currentPos.y);

				float value = (currentParticle->direction.y / currentParticle->direction.x);
				float invValue = (currentParticle->direction.x / currentParticle->direction.y);
				D3DXVECTOR2 rightInterSec = D3DXVECTOR2(_xMax, value*_xMax);
				D3DXVECTOR2 leftInterSec = D3DXVECTOR2(_xMin, value*_xMin);
				D3DXVECTOR2 topInterSec = D3DXVECTOR2(_yMax*invValue, _yMax);
				D3DXVECTOR2 bottomInterSec = D3DXVECTOR2(_yMin*invValue, _yMin);

				float rightDist = D3DXVec2Length(&(rightInterSec - currentPos));
				float leftDist = D3DXVec2Length(&(leftInterSec - currentPos));
				float topDist = D3DXVec2Length(&(topInterSec - currentPos));
				float bottomDist = D3DXVec2Length(&(bottomInterSec - currentPos));

				// hit the bottom
				if (bottomDist < topDist && bottomDist < leftDist && bottomDist < rightDist)
				{
					//currentParticle->origin.y = currentParticle->origin.y + 2 * (_yMin - currentParticle->origin.y);
					currentParticle->origin.y = _yMin - (currentParticle->origin.y - _yMin);
					D3DXVECTOR2 normalVec = D3DXVECTOR2(0.0, 1.0);
					currentParticle->direction = reflect(currentParticle->direction, normalVec);
				}
				// hit the top
				else if (topDist < bottomDist && topDist < leftDist && topDist < rightDist)
				{
					//currentParticle->origin.y = currentParticle->origin.y + 2 * (_yMax - currentParticle->origin.y);
					currentParticle->origin.y = _yMax + (_yMax - currentParticle->origin.y);
					D3DXVECTOR2 normalVec = D3DXVECTOR2(0.0, -1.0);
					currentParticle->direction = reflect(currentParticle->direction, normalVec);
				}
				// hit the right
				else if (rightDist < leftDist && rightDist < bottomDist && rightDist < topDist)
				{
					//currentParticle->origin.x = currentParticle->origin.x + 2 * (_xMax - currentParticle->origin.x);
					currentParticle->origin.x = _xMax + (_xMax - currentParticle->origin.x);
					D3DXVECTOR2 normalVec = D3DXVECTOR2(-1.0, 0.0);
					currentParticle->direction = reflect(currentParticle->direction, normalVec);
				}
				//hit the left
				else
				{
					//currentParticle->origin.x = currentParticle->origin.x + 2 * (_xMin - currentParticle->origin.x);
					currentParticle->origin.x = _xMin - (currentParticle->origin.x -_xMin);
					D3DXVECTOR2 normalVec = D3DXVECTOR2(1.0, 0.0);
					currentParticle->direction = reflect(currentParticle->direction, normalVec);
				}

				//DEBUG_PRINT("reflect direction at x = %f y = %f\n", currentParticle->direction.x, currentParticle->direction.y);
				//DEBUG_PRINT("reflect origin at x = %f y = %f\n", currentParticle->origin.x, currentParticle->origin.y);
				_numReflect--;

				NextActionType actionType = getNextActionTime(currentParticle);
				currentParticle->next = nullptr;

				if (actionType == NextActionType::REFLECTION)
				{
					pushToList(currentParticle, _reflecList);
					_numReflect++;
				}
				else
				{
					pushToList(currentParticle, _subDivList);
					_numSubDiv++;
				}

				currentParticle = _reflecList;
			}

			if (particlesReflected)
			{
				updateBuffers();
			}
		}

		void WaveParticlesRTTModel::updateBuffers()
		{
			WaveParticle* currentParticle = _subDivList;
			uint32_t particleCount = 0;

			while (currentParticle)
			{
				_vertices[particleCount].data.x = currentParticle->spawnTimeMS;
				_vertices[particleCount].data.y = currentParticle->amplitude;
				_vertices[particleCount].data.z = currentParticle->radius;
				_vertices[particleCount].data.w = currentParticle->velocity/ _gridWidth;
				_vertices[particleCount].origin.x = currentParticle->origin.x/ _gridWidth;
				_vertices[particleCount].origin.y = currentParticle->origin.y / _gridHeight;
				_vertices[particleCount].direction = currentParticle->direction;

				currentParticle = currentParticle->next;
				particleCount++;
			}

			currentParticle = _reflecList;
			while (currentParticle)
			{
				_vertices[particleCount].data.x = currentParticle->spawnTimeMS;
				_vertices[particleCount].data.y = currentParticle->amplitude;
				_vertices[particleCount].data.z = currentParticle->radius;
				_vertices[particleCount].data.w = currentParticle->velocity / _gridWidth;
				_vertices[particleCount].origin.x = currentParticle->origin.x / _gridWidth;
				_vertices[particleCount].origin.y = currentParticle->origin.y / _gridHeight;
				_vertices[particleCount].direction = currentParticle->direction;

				currentParticle = currentParticle->next;
				particleCount++;
			}

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				MessagedAssert(false, "Not able to map vertex buffer in water model.");
			}

			memcpy(mappedResource.pData, _vertices, sizeof(VertexType)*_activeParticles);
			deviceContext->Unmap(_vertexBuffer, 0);
		}

		D3DXVECTOR2 WaveParticlesRTTModel::reflect(const D3DXVECTOR2& i_incidentVec, const D3DXVECTOR2& i_normalVec)
		{
			D3DXVECTOR2 reflectVec;

			reflectVec = i_incidentVec - ((2.0f * D3DXVec2Dot(&i_incidentVec, &i_normalVec)) * i_normalVec);

			return reflectVec;
		}

		void WaveParticlesRTTModel::initializeWaveParticlesList()
		{
			_freeList = const_cast<WaveParticle*>(_waveParticleMemPool);
			WaveParticle* currentPtr = _freeList;
			WaveParticle* nextPtr = currentPtr + 1;

			for (uint32_t i = 0; i < _numParticles - 1; i++)
			{
				currentPtr->next = nextPtr;
				currentPtr = nextPtr;
				nextPtr += 1;
			}

			currentPtr->next = nullptr;
		}

		WaveParticlesRTTModel::WaveParticle * WaveParticlesRTTModel::getFreePartices(uint32_t i_numParticles)
		{
			if (_freeList == nullptr)
			{
				MessagedAssert(false, "Free particles exhausted.");
				return nullptr;
			}

			WaveParticle* first = _freeList;
			WaveParticle* current = first;
			WaveParticle* prev = nullptr;

			uint32_t i = 0;
			for (; i < i_numParticles && current != nullptr; i++)
			{
				prev = current;
				current = current->next;
			}

			if (i == i_numParticles)
			{
				prev->next = nullptr;
				_freeList = current;
				_activeParticles += i_numParticles;
				return first;
			}
			else
			{
				MessagedAssert(false, "Free particles exhausted.");
				return nullptr;
			}
		}

		void WaveParticlesRTTModel::pushToList(WaveParticle * i_waveParticle, WaveParticle*& i_list)
		{
			WaveParticle* currentParticle = i_waveParticle;
			WaveParticle* nextParticle = nullptr;

			while (currentParticle)
			{
				nextParticle = currentParticle->next;
				WaveParticle* currentPtr = i_list;
				WaveParticle* prevPtr = nullptr;

				while (currentPtr && currentParticle->actionTimeMS > currentPtr->actionTimeMS)
				{
					prevPtr = currentPtr;
					currentPtr = currentPtr->next;
				}

				if (prevPtr)
				{
					prevPtr->next = currentParticle;
					currentParticle->next = currentPtr;
				}
				else
				{
					currentParticle->next = i_list;
					i_list = currentParticle;
				}

				currentParticle = nextParticle;
			}
		}

		void WaveParticlesRTTModel::recycleParticles(WaveParticle* i_waveParticle)
		{
			if (i_waveParticle == nullptr)
			{
				MessagedAssert(false, "Trying to recylce nullptr.");
				return;
			}

			WaveParticle* last = i_waveParticle;
			uint32_t numParticles = 1;

			while (last->next != nullptr)
			{
				last = last->next;
				numParticles++;
			}

			_activeParticles -= numParticles;
			last->next = _freeList;
			_freeList = i_waveParticle;
		}

		char WaveParticlesRTTModel::getLineIntersection(const D3DXVECTOR2& p0, const D3DXVECTOR2& p1, const D3DXVECTOR2& p2, const D3DXVECTOR2& p3, D3DXVECTOR2&  i_intersection)
		{
			float s1_x, s1_y, s2_x, s2_y;
			s1_x = p1.x - p0.x;     s1_y = p1.y - p0.y;
			s2_x = p3.x - p2.x;     s2_y = p3.y - p2.y;

			float s, t;
			s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
			t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

			if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
			{
				// Collision detected
				i_intersection.x = p0.x + (t * s1_x);
				i_intersection.y = p0.y + (t * s1_y);
				return 1;
			}

			return 0; // No collision
		}

		WaveParticlesRTTModel::NextActionType WaveParticlesRTTModel::getNextActionTime(WaveParticle * i_particle)
		{
			float subDivTime = 0.0f;
			float reflectTime = 0.0f;
			float angle = 30.0f;
			NextActionType retVal;

			// calculate next subdivision time
			
			float currentTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			subDivTime = i_particle->spawnTimeMS;

			while (subDivTime <= currentTimeMS)
			{
				subDivTime += (i_particle->radius / (4.0f * (float)sin((angle * 0.5f)*MathUtils::DegToRad) * i_particle->velocity));
				angle *= 0.333333333f;
			}

			// calculate next reflection time
			
			D3DXVECTOR2 currentPos = i_particle->origin + (i_particle->direction * i_particle->velocity * (currentTimeMS - i_particle->spawnTimeMS));
			D3DXVECTOR2 nextSubDivPos = i_particle->origin + (i_particle->direction * i_particle->velocity * (subDivTime - i_particle->spawnTimeMS));
			D3DXVECTOR2 interSection;

			// test intersection for right edge
			if (getLineIntersection(currentPos, nextSubDivPos, _topRight, _bottomRight, interSection))
			{
				reflectTime = currentTimeMS + (D3DXVec2Length(&(interSection - currentPos)) / i_particle->velocity);
			}
			// test intersection for left edge
			else if (getLineIntersection(currentPos, nextSubDivPos, _topLeft, _bottomLeft, interSection))
			{
				reflectTime = currentTimeMS + (D3DXVec2Length(&(interSection - currentPos)) / i_particle->velocity);
			}
			// test intersection for top edge
			else if (getLineIntersection(currentPos, nextSubDivPos, _topLeft, _topRight, interSection))
			{
				reflectTime = currentTimeMS + (D3DXVec2Length(&(interSection - currentPos)) / i_particle->velocity);
			}
			// test intersection for bottom edge
			else if (getLineIntersection(currentPos, nextSubDivPos, _bottomRight, _bottomLeft, interSection))
			{
				reflectTime = currentTimeMS + (D3DXVec2Length(&(interSection - currentPos)) / i_particle->velocity);
			}

			if (reflectTime == 0.0 || subDivTime < reflectTime)
			{
				i_particle->actionTimeMS = subDivTime;
				retVal = NextActionType::SUB_DIVISION;
			}
			else
			{
				i_particle->actionTimeMS = reflectTime;
				retVal = NextActionType::REFLECTION;
			}

			return retVal;
		}

		bool WaveParticlesRTTModel::createBuffers()
		{
			D3D11_BUFFER_DESC vertexBufferDesc;
			HRESULT result;

			ID3D11Device* device = GraphicsDX::GetDevice();

			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * _numParticles;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			result = device->CreateBuffer(&vertexBufferDesc, nullptr, &_vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}
	}	// namespace Graphics
}	// namespace Engine
