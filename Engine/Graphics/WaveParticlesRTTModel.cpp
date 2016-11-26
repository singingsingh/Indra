#include <Engine\Graphics\WaveParticlesRTTModel.h>

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Core\MemoryMgr.h>
#include <Engine\Util\MathUtils.h>
#include <Engine\System\Timer.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	namespace Graphics
	{

		WaveParticlesRTTModel::WaveParticlesRTTModel()
			:_numParticles(400),
			_waveParticleMemPool(static_cast<WaveParticle*>(MemoryMgr::getInstance()->allocMemory(_numParticles * sizeof(WaveParticle))))
			
		{
			_vertexBuffer = nullptr;
			_activeParticles = 0;

			_freeList = nullptr;
			_activeList = nullptr;

			//initializeWaveParticlesList();
		}

		WaveParticlesRTTModel::~WaveParticlesRTTModel()
		{
			MemoryMgr::getInstance()->dellocMemory(static_cast<void*>(const_cast<WaveParticle*>(_waveParticleMemPool)));
		}

		bool WaveParticlesRTTModel::initialize()
		{
			bool result;

			result = initializeBuffers();
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
			_currentTick = System::Timer::GetCurrentTick();

			//subDivideParticles();
			//updateBuffers();

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
			return _vertexCount;
		}

		void WaveParticlesRTTModel::spawnParticles()
		{
			WaveParticle* newParticles = getFreePartices(3);
			WaveParticle* first = newParticles;

			newParticles->origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			newParticles->velocity = 0.002f;
			newParticles->amplitude = 0.2f;
			newParticles->radius = 0.5f;
			newParticles->angle = 30.0f;
			newParticles->spawnTick = System::Timer::GetCurrentTick();
			newParticles->actionTick = newParticles->spawnTick + System::Timer::ConvertMilliSecToTick(newParticles->radius / (4.0 * sin((newParticles->angle / 2.0)*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;
			newParticles->origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR3((float)sin(30.0f*MathUtils::DegToRad), 0.0f, (float)cos(30.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.002f;
			newParticles->amplitude = 0.2f;
			newParticles->radius = 0.5f;
			newParticles->angle = 30.0f;
			newParticles->spawnTick = System::Timer::GetCurrentTick();
			newParticles->actionTick = newParticles->spawnTick + System::Timer::ConvertMilliSecToTick(newParticles->radius / (4.0 * sin((newParticles->angle / 2.0)*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;
			newParticles->origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR3((float)sin(-30.0f*MathUtils::DegToRad), 0.0f, (float)cos(-30.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.002f;
			newParticles->amplitude = 0.2f;
			newParticles->radius = 0.5f;
			newParticles->angle = 30.0f;
			newParticles->spawnTick = System::Timer::GetCurrentTick();
			newParticles->actionTick = newParticles->spawnTick + System::Timer::ConvertMilliSecToTick(newParticles->radius / (4.0 * sin((newParticles->angle / 2.0)*MathUtils::DegToRad) * newParticles->velocity));
			newParticles->next = nullptr;

			pushToActiveList(first);
		}

		void WaveParticlesRTTModel::subDivideParticles()
		{
			WaveParticle* currentParticle = _activeList;

			while (currentParticle && currentParticle->actionTick < _currentTick)
			{
				_activeList = currentParticle->next;

				WaveParticle* newParticles = getFreePartices(2);
				currentParticle->next = newParticles;

				currentParticle->amplitude = currentParticle->amplitude * 0.33333333f;
				currentParticle->angle = currentParticle->angle * 0.33333333f;
				currentParticle->actionTick = currentParticle->actionTick +
					System::Timer::ConvertMilliSecToTick(currentParticle->radius / (4.0 * sin((currentParticle->angle * 0.5)*MathUtils::DegToRad) * currentParticle->velocity));

				WaveParticle* temp = newParticles;
				while (temp)
				{
					temp->origin = currentParticle->origin;
					temp->spawnTick = currentParticle->spawnTick;
					temp->actionTick = currentParticle->actionTick;
					temp->angle = currentParticle->angle;
					temp->amplitude = currentParticle->amplitude;
					temp->radius = currentParticle->radius;
					temp->velocity = currentParticle->velocity;

					temp = temp->next;
				}

				D3DXVECTOR3 direction = currentParticle->direction;
				D3DXVECTOR3 newDirection = direction;
				float angle = currentParticle->angle*MathUtils::DegToRad;
				newDirection.x = direction.x * (float)cos(angle) - direction.z * (float)sin(angle);
				newDirection.z = direction.x * (float)sin(angle) + direction.z * (float)cos(angle);
				temp = newParticles;
				temp->direction = newDirection;

				temp = temp->next;

				direction = currentParticle->direction;
				newDirection = currentParticle->direction;
				angle = -currentParticle->angle*MathUtils::DegToRad;
				newDirection.x = direction.x * (float)cos(angle) - direction.z * (float)sin(angle);
				newDirection.z = direction.x * (float)sin(angle) + direction.z * (float)cos(angle);
				temp->direction = newDirection;

				pushToActiveList(currentParticle);

				currentParticle = _activeList;
			}
		}

		void WaveParticlesRTTModel::updateBuffers()
		{
			//WaveParticle* currentParticle = _activeList;
			//bool first = true;

			//while (currentParticle)
			//{
			//	double elapsedTime = System::Timer::GetElapsedTimeMilliSec(currentParticle->spawnTick, currentTick, false);
			//	D3DXVECTOR3 wavePos = currentParticle->origin + (currentParticle->direction * currentParticle->velocity * (float)elapsedTime);
			//	int vertexCount = 0;
			//	float invRadius = 1.0f / currentParticle->radius;

			//	for (uint8_t row = 0; row <= _gridRows; row++)
			//	{
			//		for (uint8_t col = 0; col <= _gridCols; col++)
			//		{
			//			if (first)
			//			{
			//				_vertices[vertexCount].position = D3DXVECTOR3(col*_gridGap, 0.0f, -row*_gridGap) + _corner;
			//			}

			//			float dist = sqrt(((_vertices[vertexCount].position.x - wavePos.x) * (_vertices[vertexCount].position.x - wavePos.x))
			//				+ ((_vertices[vertexCount].position.z - wavePos.z) * (_vertices[vertexCount].position.z - wavePos.z)));

			//			float rectFunc = dist * invRadius;
			//			if (rectFunc <= 1.0f)
			//			{
			//				_vertices[vertexCount].position.y += currentParticle->amplitude * 0.5f * (cos(MathUtils::PI * invRadius * dist) + 1.0f);
			//			}

			//			vertexCount++;
			//		}
			//	}

			//	first = false;
			//	currentParticle = currentParticle->next;
			//}

			//// update the buffers

			//ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			//HRESULT result;
			//D3D11_MAPPED_SUBRESOURCE mappedResource;

			//result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			//if (FAILED(result))
			//{
			//	MessagedAssert(false, "Not able to map vertex buffer in water model.");
			//}

			//memcpy(mappedResource.pData, _vertices, sizeof(VertexType)*_vertexCount);
			//deviceContext->Unmap(_vertexBuffer, 0);
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
				return first;
			}
			else
			{
				MessagedAssert(false, "Free particles exhausted.");
				return nullptr;
			}
		}

		void WaveParticlesRTTModel::pushToActiveList(WaveParticle * i_waveParticle)
		{
			WaveParticle* currentParticle = i_waveParticle;
			WaveParticle* nextParticle = nullptr;

			while (currentParticle)
			{
				nextParticle = currentParticle->next;
				WaveParticle* currentPtr = _activeList;
				WaveParticle* prevPtr = nullptr;

				while (currentPtr && currentParticle->actionTick > currentPtr->actionTick)
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
					_activeList = currentParticle;
					currentParticle->next = nullptr;
				}

				currentParticle = nextParticle;
			}
		}

		void WaveParticlesRTTModel::recycleParticles(WaveParticle* i_waveParticle)
		{
			if (i_waveParticle == nullptr)
			{
				return;
			}

			WaveParticle* last = i_waveParticle;

			while (last->next != nullptr)
			{
				last = last->next;
			}

			last->next = _freeList;
			_freeList = i_waveParticle;
		}

		bool WaveParticlesRTTModel::initializeBuffers()
		{
			VertexType* vertices;
			D3D11_BUFFER_DESC vertexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData;
			HRESULT result;

			ID3D11Device* device = GraphicsDX::GetDevice();
			_vertexCount = 3;

			vertices = new VertexType[_vertexCount];
			if (!vertices)
			{
				return false;
			}

			vertices[0].position = D3DXVECTOR2(0.0f, 0.0f);
			vertices[0].size = D3DXVECTOR2(0.5f, 0.5f);

			vertices[1].position = D3DXVECTOR2(0.0f, 0.6f);
			vertices[1].size = D3DXVECTOR2(0.5f, 0.5f);

			vertices[2].position = D3DXVECTOR2(0.1f, 0.0f);
			vertices[2].size = D3DXVECTOR2(0.5f, 0.5f);

			 // Set up the description of the vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Now create the vertex buffer.
			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Release the arrays now that the vertex and index buffers have been created and loaded.
			delete[] vertices;
			vertices = 0;

			return true;
		}
	}	// namespace Graphics
}	// namespace Engine
