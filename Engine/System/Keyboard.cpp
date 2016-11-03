#include <Engine\System\Keyboard.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

#include <Engine\System\Window.h>

#include <stdint.h>
#include <Windows.h>


namespace Engine
{
	KeyStateChange		m_pKeyStateChangeCallback = nullptr;

	void SetKeyStateChangeCallback(KeyStateChange i_Callback)
	{
		m_pKeyStateChangeCallback = i_Callback;
	}

	namespace Keyboard
	{
		const size_t	m_bytesInitReadBuffer = 128;
		
		size_t			m_bytesReadBuffer = 0;
		LPBYTE			m_pReadBuffer = nullptr;

		const size_t	m_sizeReadTable = 256;
		uint8_t			m_VKeyStates[m_sizeReadTable];

		void ResizeReadBuffer(size_t i_bytes)
		{
			Assert(i_bytes);

			if (i_bytes > m_bytesReadBuffer)
			{
				if (m_pReadBuffer)
					delete[] m_pReadBuffer;

				m_pReadBuffer = new BYTE[i_bytes];
				Assert(m_pReadBuffer);

				m_bytesReadBuffer = i_bytes;
			}
		}

		bool Service(HRAWINPUT i_Input)
		{
			bool quit = false;
			UINT bytesData = 0;
			UINT ret = GetRawInputData(i_Input, RID_INPUT, NULL, &bytesData, sizeof(RAWINPUTHEADER));
			Assert(ret == 0);

			if (bytesData)
			{
				ResizeReadBuffer(bytesData);
				Assert(m_pReadBuffer);

				ret = GetRawInputData(i_Input, RID_INPUT, m_pReadBuffer, &bytesData, sizeof(RAWINPUTHEADER));
				Assert(ret == bytesData);

				RAWINPUT * pRawInput = reinterpret_cast<RAWINPUT *>(m_pReadBuffer);

				if (pRawInput->header.dwType == RIM_TYPEKEYBOARD)
				{
					if (pRawInput->data.keyboard.Flags == RI_KEY_BREAK)
					{ // up
						bool bChanged = m_VKeyStates[pRawInput->data.keyboard.VKey] != 0;

						m_VKeyStates[pRawInput->data.keyboard.VKey] = 0;

						if (bChanged)
						{
							if (m_pKeyStateChangeCallback)
								m_pKeyStateChangeCallback(pRawInput->data.keyboard.VKey, false);

							DEBUG_PRINT("Key 0x%02x changed state to UP\n", pRawInput->data.keyboard.VKey);
						}
					}
					else if (pRawInput->data.keyboard.Flags == RI_KEY_MAKE)
					{ // down
						bool bChanged = m_VKeyStates[pRawInput->data.keyboard.VKey] != 1;

						m_VKeyStates[pRawInput->data.keyboard.VKey] = 1;

						if (m_VKeyStates[VK_ESCAPE] == 1)
						{
							quit = true;
						}

						if (bChanged)
						{
							if (m_pKeyStateChangeCallback)
								m_pKeyStateChangeCallback(pRawInput->data.keyboard.VKey, true);

							DEBUG_PRINT("Key 0x%02x changed state to DOWN\n", pRawInput->data.keyboard.VKey);
						}
					}
					else
					{
						DEBUG_PRINT("Key 0x%02x Flags: 0x%04x\n", pRawInput->data.keyboard.VKey, pRawInput->data.keyboard.Flags);
					}
				}
			}

			return quit;
		}

		bool KeyboardUpdate()
		{
			bool quit = false;

			BOOL				bGotMsg = FALSE;

			do
			{
				MSG					msg;

				msg.message = WM_NULL;

				bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

				if (bGotMsg)
				{
					//if (TranslateAccelerator(System::Window::GetWindwsHandle(), NULL, &msg) == 0)
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					switch (msg.message)
					{
					case WM_QUIT:
					{
						quit = true;
						break;
					}
					case WM_INPUT:
					{
						quit = Service(HRAWINPUT(msg.lParam));
						break;
					}
					default:
					{
						//DEBUG_PRINT("message: 0x%04x\n", msg.message);
					}
					}
				}
			} while ((bGotMsg == TRUE) && (quit == false));
			return quit;
		}



		bool Initialize()
		{
			Assert(m_pReadBuffer == nullptr);

			ResizeReadBuffer(m_bytesInitReadBuffer);

			RAWINPUTDEVICE keyboard;

			keyboard.usUsagePage = 0x01;		// Generic desktop page
			keyboard.usUsage = 0x06;			// Keyboard
			keyboard.dwFlags = RIDEV_NOLEGACY;	// Turn off keyboard input to the Windows message handlers
			keyboard.hwndTarget = NULL;

			BOOL success = RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard));
			Assert(success == TRUE);

			memset(m_VKeyStates, sizeof(m_VKeyStates), 0);
			
			return true;
		}

		void Shutdown()
		{
			if (m_pReadBuffer)
				delete[] m_pReadBuffer;
		}
	} // namespace Input
} // namespace GraphicsDX