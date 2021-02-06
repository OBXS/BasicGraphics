#pragma once
#include <Windows.h>

/// <summary>
/// Class that handles window related actions in Windows
/// </summary>
class WindowsContainer
{
public:
	/// <summary>
	/// Registers and Creates a Window
	/// </summary>
	/// <param name="szTitle">Text to be shown in Title bar</param>
	/// <param name="uWidth">Width of client region, in pixels</param>
	/// <param name="uHeight">Height of client region, in pixels</param>
	/// <returns>true if Succeeded, false otherwise</returns>
	bool Initialize(TCHAR * szTitle, unsigned int uWidth, unsigned int uHeight);

	/// <summary>
	/// Runs the main loop
	/// </summary>
	/// <returns>Message passed by the internal processes when the loop exits</returns>
	int Run();

	/// <summary>
	/// Sets which function to call every loop cycle
	/// </summary>
	/// <param name="f_Callback">Update callback, Must be a void</param>
	void SetUpdateCallback(void (*f_Callback)())
	{
		m_UpdateCallback = f_Callback;
	}

	/// <summary>
	/// Get window handle for this window
	/// </summary>
	/// <returns>Window handle</returns>
	HWND GetHWND() const
	{
		return m_HWND;
	}

	/// <summary>
	/// Get client width in pixels
	/// </summary>
	/// <returns>Client width in pixels</returns>
	unsigned int GetClientWidth() const
	{
		return m_Width;
	}

	/// <summary>
	/// Get client height in pixels
	/// </summary>
	/// <returns>Client height in pixels</returns>
	unsigned int GetClientHeight() const
	{
		return m_Height;
	}
private:
	/// <summary>
	/// Handles
	/// </summary>
	HWND m_HWND;
	HINSTANCE m_HInstance;

	/// <summary>
	/// Window Client Dimensions
	/// </summary>
	unsigned int m_Width;
	unsigned int m_Height;

	/// <summary>
	/// Callbacks
	/// </summary>
	void (*m_UpdateCallback)();
};