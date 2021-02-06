#include "Direct3D9.h"

bool D3D9::Initialize(HWND hWnd, unsigned int uWidth, unsigned int uHeight)
{
	m_D3DInterface = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = uWidth;
	d3dpp.BackBufferHeight = uHeight;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; // Depth is 16 bit
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT hr = m_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice);
	if (FAILED(hr))
		return false;

	return true;
}

void D3D9::Uninitialize()
{
	if (m_D3DInterface)
		m_D3DInterface->Release();

	if (m_D3DDevice)
		m_D3DDevice->Release();
}