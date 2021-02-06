#pragma once
#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

class D3D11
{
public:
	/// <summary>
	/// Initialize D3D11 Device
	/// </summary>
	/// <param name="hWnd">Window Handle, obtained from WindowsContainer</param>
	/// <param name="uWidth">Width of client region, in Pixels</param>
	/// <param name="uHeight">Height of client region, in Pixels</param>
	/// <returns>true if Succeeded, false otherwise</returns>
	bool Initialize(HWND hWnd, unsigned int uWidth, unsigned int uHeight);

	/// <summary>
	/// Uninitialize D3D11 Device
	/// </summary>
	void Uninitialize();

	/// <summary>
	/// Get D3D11 Device
	/// </summary>
	/// <returns>D3D11 Device Pointer</returns>
	ID3D11Device * GetDevice() const
	{
		return m_D3DDevice;
	}
	/// <summary>
	/// Get D3D11.1 Device
	/// </summary>
	/// <returns>D3D11.1 Device Pointer</returns>
	ID3D11Device1 * GetDevice1() const
	{
		return m_D3DDevice1;
	}

	/// <summary>
	/// Get the Current D3D11 Immediate Context
	/// </summary>
	/// <returns>Context Pointer</returns>
	ID3D11DeviceContext * GetContext() const
	{
		return m_ImmediateContext;
	}

	/// <summary>
	/// Get the Current D3D11.1 Immediate Context
	/// </summary>
	/// <returns>Context Pointer</returns>
	ID3D11DeviceContext1 * GetContext1() const
	{
		return m_ImmediateContext1;
	}

	/// <summary>
	/// Get D3D11 Swapchain
	/// </summary>
	/// <returns>Swapchain Pointer</returns>
	IDXGISwapChain * GetSwapchain() const
	{
		return m_SwapChain;
	}
	/// <summary>
	/// Get D3D11.1 Swapchain
	/// </summary>
	/// <returns>Swapchain Pointer</returns>
	IDXGISwapChain1 * GetSwapchain1() const
	{
		return m_SwapChain1;
	}

	/// <summary>
	/// Get the RenderTargetView for the back buffer
	/// </summary>
	/// <returns>RenderTargetView Pointer</returns>
	ID3D11RenderTargetView * GetRenderTargetView()
	{
		return m_RenderTargetView;
	}

private:
	/// <summary>
	/// D3D Devices
	/// </summary>
	ID3D11Device * m_D3DDevice = nullptr;
	ID3D11Device1 * m_D3DDevice1 = nullptr;

	/// <summary>
	/// D3D Contexts
	/// </summary>
	ID3D11DeviceContext * m_ImmediateContext = nullptr;
	ID3D11DeviceContext1 * m_ImmediateContext1 = nullptr;

	/// <summary>
	/// D3D Swapchains
	/// </summary>
	IDXGISwapChain * m_SwapChain = nullptr;
	IDXGISwapChain1 * m_SwapChain1 = nullptr;

	/// <summary>
	/// RenderTargetView
	/// </summary>
	/// <remarks>
	/// Does not contain the render texture itself,
	/// texture buffer can be obtained using
	/// 
	/// m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&pBackBuffer));
	/// 
	/// </remarks>
	ID3D11RenderTargetView * m_RenderTargetView = nullptr;

	ID3D11BlendState * m_BlendAlpha = nullptr;
};