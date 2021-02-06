#pragma once
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

class D3D9
{
public:
	/// <summary>
	/// Initialize D3D9 Device
	/// </summary>
	/// <param name="hWnd">Window Handle, obtained from WindowsContainer</param>
	/// <param name="uWidth">Width of client region, in Pixels</param>
	/// <param name="uHeight">Height of client region, in Pixels</param>
	/// <returns>true if Succeeded, false otherwise</returns>
	bool Initialize(HWND hWnd, unsigned int uWidth, unsigned int uHeight);
	
	/// <summary>
	/// Uninitialize D3D9 Device
	/// </summary>
	void Uninitialize();

	/// <summary>
	/// Get D3D9 Device
	/// </summary>
	/// <returns>D3D9 Device Pointer</returns>
	LPDIRECT3DDEVICE9 Device() const
	{
		return m_D3DDevice;
	}
private:
	/// <summary>
	/// D3D9 Interfaces
	/// </summary>
	LPDIRECT3D9 m_D3DInterface = nullptr;
	LPDIRECT3DDEVICE9 m_D3DDevice = nullptr;
};