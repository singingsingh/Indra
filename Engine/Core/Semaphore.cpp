#include <Engine\Core\Semaphore.h>

#include <Engine\Util\Assert.h>

#include <Windows.h>

namespace Engine
{

Semaphore::Semaphore( const char * i_pName, unsigned int i_CountMax, unsigned int i_CountInit )
{
	Assert( i_CountInit <= i_CountMax );

	m_Handle = CreateSemaphore( NULL, i_CountInit, i_CountMax, i_pName );
	Assert( m_Handle != NULL );
}

Semaphore::~Semaphore()
{
	BOOL result = CloseHandle( m_Handle );
	Assert( result == TRUE );
}

void Semaphore::Increment( unsigned int i_count )
{
	BOOL result = ReleaseSemaphore( m_Handle, i_count, NULL );
	Assert( result == TRUE );
}

void Semaphore::WaitAndDecrement( void )
{
	DWORD result = WaitForSingleObject( m_Handle, INFINITE );
	Assert( result == WAIT_OBJECT_0 );
}

} // namespace Engine
