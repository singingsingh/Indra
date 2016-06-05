#ifndef __THREADED_FILE_PROCESSOR_H__
#define __THREADED_FILE_PROCESSOR_H__

#include <Engine\Core\Mutex.h>
#include <Engine\Core\Semaphore.h>
#include <Engine\Core\Event.h>
#include <Engine\Util\Assert.h>

#include <stdint.h>
#include <queue>
#include <Windows.h>

namespace Engine
{
	class ThreadedFileProcessor
	{
	public:
		class LoadTask
		{
			public:
				LoadTask( const char * m_FileName );
				virtual ~LoadTask();

				const char * GetFileName( void );
				virtual void ProcessFileContents( uint8_t * i_pBuffer, uint32_t i_BufferBytes ) = 0;
				virtual void processReleaseQueue() = 0;

			private:
				const char *		m_FileName;
		};

		void AddToLoadQueue( LoadTask & i_LoadTask );
		ThreadedFileProcessor::LoadTask * GetFromReleaseQueue();

		static ThreadedFileProcessor & GetInstance( void );
		static void	Shutdown( void );

	private:
		struct _LoadTaskData
		{
			uint8_t *				m_pFileContents;
			uint32_t 				m_FileContentsSize;

			LoadTask *				m_pLoadTask;

			_LoadTaskData( uint8_t * i_pFileContents, uint32_t i_FileContentsSize, LoadTask * i_pLoadTask ) :
				m_pFileContents( i_pFileContents ),
				m_FileContentsSize( i_FileContentsSize ),
				m_pLoadTask( i_pLoadTask )
			{
				Assert( i_pFileContents != NULL );
				Assert( i_FileContentsSize > 0 );
				Assert( i_pLoadTask != NULL );
			}

		};

		HANDLE m_LoadThreadHandle;
		HANDLE m_ProcessThreadHandle;

		Engine::ManualResetEvent m_ShutdownRequestEvent;
		Engine::Semaphore m_LoadQueueSemaphore;
		Engine::Mutex m_LoadQueueMutex;

		Engine::Semaphore m_ProcessQueueSemaphore;
		Engine::Mutex m_ProcessQueueMutex;

		Engine::Mutex m_releaseQueueMutex;

		std::queue<LoadTask *>	m_LoadQueue;
		std::queue<struct _LoadTaskData *>	m_ProcessQueue;
		std::queue<LoadTask *> _releaseQueue;

		ThreadedFileProcessor( HANDLE i_LoadThreadHandle, HANDLE i_ProcessThreadHandle );

		void AddToProcessQueue( struct _LoadTaskData & i_LoadTaskData );
		void AddToReleaseQueue(LoadTask & i_LoadTask);

		LoadTask * GetFromLoadQueue( void );
		struct _LoadTaskData * GetFromProcessQueue( void );

		static DWORD WINAPI LoadThreadFunction( void * i_pThreadData );
		static DWORD WINAPI ProcessThreadFunction( void * i_pThreadData );
	};
} // namespace Engine

#endif // __THREADED_FILE_PROCESSOR_H__