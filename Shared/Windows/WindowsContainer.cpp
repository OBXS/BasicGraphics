#include "WindowsContainer.h"

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

bool WindowsContainer::Initialize(TCHAR * szTitle, unsigned int uWidth, unsigned int uHeight)
{
	// Get Instance Handle
	m_HInstance = GetModuleHandle(NULL);

	// Setup Window Class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_HInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szTitle;
	wcex.hIconSm = NULL;

	// Register said class, return false when it fails
	if (!RegisterClassEx(&wcex))
		return false;

	// Create a new window, size is determined by parameter
	RECT rc = { 0, 0, (LONG)uWidth, (LONG)uHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_HWND = CreateWindow(szTitle, szTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_HInstance,
		nullptr);

	// Return false when window creation failed
	if (!m_HWND)
		return false;

	// Save Width, Height Data
	m_Width = uWidth;
	m_Height = uHeight;

	// Show the window
	ShowWindow(m_HWND, SW_SHOWDEFAULT);

	return true;
}

int WindowsContainer::Run()
{
	// Simple (bad) loop
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_UpdateCallback();
		}
	}

	return (int)msg.wParam;
}
