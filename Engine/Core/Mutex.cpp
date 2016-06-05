#include <Engine\Core\Mutex.h>

#include <Engine\Util\Assert.h>

#include <windows.h>

namespace Engine
{

Mutex::Mutex( const char * i_pName )
{
	m_Handle = CreateMutex( NULL, FALSE, i_pName );
	Assert( m_Handle != NULL );
}

Mutex::~Mutex()
{
	BOOL result = CloseHandle( m_Handle );
	Assert( result == TRUE );
}

bool Mutex::TryLock( void )
{
	DWORD result = WaitForSingleObject( m_Handle, 0 );

	return result == WAIT_OBJECT_0;
}

void Mutex::Lock( void )
{
	DWORD result = WaitForSingleObject( m_Handle, INFINITE );
	Assert( result == WAIT_OBJECT_0 );
}

void Mutex::Release( void )
{
	BOOL result = ReleaseMutex( m_Handle );
	Assert( result == TRUE );
}

} // namespace Engine