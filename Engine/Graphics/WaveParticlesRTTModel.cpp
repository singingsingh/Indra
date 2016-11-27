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
			_waveParticleMemPool(static_cast<WaveParticle*>(MemoryMgr::getInstance()->allocMemory(_numParticles * sizeof(WaveParticle)))),
			_vertices (static_cast<VertexType*>(MemoryMgr::getInstance()->allocMemory(_numParticles*sizeof(VertexType))))
		{
			_vertexBuffer = nullptr;
			_activeParticles = 0;

			_freeList = nullptr;
			_activeList = nullptr;
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
			WaveParticle* newParticles = getFreePartices(12);
			WaveParticle* first = newParticles;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2(0.0f, 1.0f);
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS + 
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(30.0f*MathUtils::DegToRad), (float)cos(30.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS + 
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(-30.0f*MathUtils::DegToRad), (float)cos(-30.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS + 
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(60.0f*MathUtils::DegToRad), (float)cos(60.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(-60.0f*MathUtils::DegToRad), (float)cos(-60.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(90.0f*MathUtils::DegToRad), (float)cos(90.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(-90.0f*MathUtils::DegToRad), (float)cos(-90.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(120.0f*MathUtils::DegToRad), (float)cos(120.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(-120.0f*MathUtils::DegToRad), (float)cos(-120.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(150.0f*MathUtils::DegToRad), (float)cos(150.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(-150.0f*MathUtils::DegToRad), (float)cos(-150.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;

			newParticles->origin = D3DXVECTOR2(0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR2((float)sin(180.0f*MathUtils::DegToRad), (float)cos(180.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.0001f;
			newParticles->amplitude = 1.5f;
			newParticles->radius = 0.2f;
			newParticles->angle = 30.0f;
			newParticles->spawnTimeMS = static_cast<float>(System::Timer::GetCurrentTimeMS());
			newParticles->actionTimeMS = newParticles->spawnTimeMS +
				(newParticles->radius / (4.0f * (float)sin(newParticles->angle / 2.0f*MathUtils::DegToRad) * newParticles->velocity));

			newParticles->next = nullptr;

			pushToActiveList(first);
			updateBuffers();
		}

		float WaveParticlesRTTModel::getCurrentTime()
		{
			return _currentTimeMS;
		}

		void WaveParticlesRTTModel::subDivideParticles()
		{
			WaveParticle* currentParticle = _activeList;

			bool particlesSubdiveded = false;

			while (currentParticle && currentParticle->actionTimeMS < _currentTimeMS)
			{
				particlesSubdiveded = true;
				_activeList = currentParticle->next;

				// check to see if the amplitude is less than threshold and discard accordingly
				if (currentParticle->amplitude < 0.5)
				{
					currentParticle->next = nullptr;
					recycleParticles(currentParticle);

					currentParticle = _activeList;
					continue;
				}

				WaveParticle* newParticles = getFreePartices(2);
				currentParticle->next = newParticles;

				currentParticle->amplitude = currentParticle->amplitude * 0.33333333f;
				currentParticle->angle = currentParticle->angle * 0.33333333f;
				currentParticle->actionTimeMS = currentParticle->actionTimeMS +
					(currentParticle->radius / (4.0f * (float)sin((currentParticle->angle * 0.5f)*MathUtils::DegToRad) * currentParticle->velocity));

				WaveParticle* temp = newParticles;
				while (temp)
				{
					temp->origin = currentParticle->origin;
					temp->spawnTimeMS = currentParticle->spawnTimeMS;
					temp->actionTimeMS = currentParticle->actionTimeMS;
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

				temp = temp->next;

				direction = currentParticle->direction;
				newDirection = currentParticle->direction;
				angle = -currentParticle->angle*MathUtils::DegToRad;
				newDirection.x = direction.x * (float)cos(angle) - direction.y * (float)sin(angle);
				newDirection.y = direction.x * (float)sin(angle) + direction.y * (float)cos(angle);
				temp->direction = newDirection;

				pushToActiveList(currentParticle);

				currentParticle = _activeList;
			}

			if (particlesSubdiveded)
			{
				updateBuffers();
			}
		}

		void WaveParticlesRTTModel::updateBuffers()
		{
			WaveParticle* currentParticle = _activeList;
			uint32_t particleCount = 0;
			
			while (currentParticle)
			{
				_vertices[particleCount].data.x = currentParticle->spawnTimeMS;
				_vertices[particleCount].data.y = currentParticle->amplitude;
				_vertices[particleCount].data.z = currentParticle->radius;
				_vertices[particleCount].data.w = currentParticle->velocity;
				_vertices[particleCount].origin = currentParticle->origin;
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

		void WaveParticlesRTTModel::pushToActiveList(WaveParticle * i_waveParticle)
		{
			WaveParticle* currentParticle = i_waveParticle;
			WaveParticle* nextParticle = nullptr;

			while (currentParticle)
			{
				nextParticle = currentParticle->next;
				WaveParticle* currentPtr = _activeList;
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
					currentParticle->next = _activeList;
					_activeList = currentParticle;
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
