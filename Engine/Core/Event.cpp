#include <Engine\Core\Event.h>

#include <Engine\Util\Assert.h>

namespace Engine
{
	ManualResetEvent::ManualResetEvent(const char * i_pName, bool i_bInitiallySignaled)
	{
		m_Handle = CreateEvent(NULL, TRUE, i_bInitiallySignaled, i_pName);
		Assert(m_Handle != INVALID_HANDLE_VALUE);
	}

	ManualResetEvent::~ManualResetEvent()
	{
		CloseHandle(m_Handle);
	}

	void ManualResetEvent::Reset(void)
	{
		ResetEvent(m_Handle);
	}

	bool ManualResetEvent::WaitFor(void)
	{
		DWORD result = WaitForSingleObject(m_Handle, INFINITE);
		Assert(result == WAIT_OBJECT_0);

		return true;
	}

	void ManualResetEvent::Signal(void)
	{
		BOOL result = SetEvent(m_Handle);
		Assert(result == TRUE);
	}

	AutoResetEvent::AutoResetEvent(const char * i_pName, bool i_bInitiallySignaled)
	{
		m_Handle = CreateEvent(NULL, FALSE, i_bInitiallySignaled, i_pName);
		Assert(m_Handle != INVALID_HANDLE_VALUE);
	}

	AutoResetEvent::~AutoResetEvent()
	{
		CloseHandle(m_Handle);
	}

	bool AutoResetEvent::WaitFor(void)
	{
		DWORD result = WaitForSingleObject(m_Handle, INFINITE);
		Assert(result == WAIT_OBJECT_0);

		return true;
	}

	void AutoResetEvent::Signal(void)
	{
		BOOL result = SetEvent(m_Handle);
		Assert(result == TRUE);
	}

} // namespace Engine