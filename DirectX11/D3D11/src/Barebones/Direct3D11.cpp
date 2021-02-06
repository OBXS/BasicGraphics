#include "Direct3D11.h"

bool D3D11::Initialize(HWND hWnd, unsigned int uWidth, unsigned int uHeight)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1
	};
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, 1,
		D3D11_SDK_VERSION, &m_D3DDevice, &featureLevel, &m_ImmediateContext);

	if (FAILED(hr))
		return false;

	IDXGIFactory1 * dxgiFactory = nullptr;
	{
		IDXGIDevice * dxgiDevice = nullptr;
		hr = m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void **>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter * adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void **>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return false;

	IDXGIFactory2 * dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void **>(&dxgiFactory2));
	if (!dxgiFactory2)
		return false;

	hr = m_D3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void **>(&m_D3DDevice1));
	if (SUCCEEDED(hr))
		(void)m_ImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void **>(&m_ImmediateContext1));

	DXGI_SWAP_CHAIN_DESC1 sd = {};
	sd.Width = width;
	sd.Height = height;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	hr = dxgiFactory2->CreateSwapChainForHwnd(m_D3DDevice, hWnd, &sd, nullptr, nullptr, &m_SwapChain1);
	if (SUCCEEDED(hr))
	{
		hr = m_SwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void **>(&m_SwapChain));
	}

	dxgiFactory2->Release();
	dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();

	if (FAILED(hr))
		return false;

	ID3D11Texture2D * pBackBuffer = nullptr;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&pBackBuffer));
	if (FAILED(hr))
		return false;

	hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);

	D3D11_BLEND_DESC blendStateDescription = {};

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = m_D3DDevice->CreateBlendState(&blendStateDescription, &m_BlendAlpha);
	if (FAILED(hr))
		return false;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

	m_ImmediateContext->OMSetBlendState(m_BlendAlpha, blendFactor, sampleMask);

	return true;
}

void D3D11::Uninitialize()
{
	if (m_BlendAlpha) m_BlendAlpha->Release();
	if (m_RenderTargetView) m_RenderTargetView->Release();
	if (m_SwapChain1) m_SwapChain1->Release();
	if (m_SwapChain) m_SwapChain->Release();
	if (m_ImmediateContext1) m_ImmediateContext1->Release();
	if (m_ImmediateContext) m_ImmediateContext->Release();
	if (m_D3DDevice1) m_D3DDevice1->Release();
	if (m_D3DDevice) m_D3DDevice->Release();
}