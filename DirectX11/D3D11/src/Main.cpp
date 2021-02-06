#include "Barebones/Direct3D11.h"
#include "WindowsContainer.h"
#include "WindowsContainer.cpp"

#include <memory>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

struct VertexLayout
{
	FLOAT x, y, z, w;
	FLOAT r, g, b, a;
};

HRESULT CompileShaderFromString(const CHAR * szSource, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut);
const char g_Shader[] = "\
struct VIn\
{\
	float4 vertex : POSITION;\
	float4 color : COLOR;\
};\
struct PIn\
{\
	float4 vertex : SV_POSITION;\
	float4 color : COLOR;\
};\
PIn VS(VIn input)\
{\
	PIn output;\
	output.vertex = input.vertex;\
	output.color = input.color;\
	return output;\
}\
\
float4 PS(PIn input) : SV_Target\
{\
	return input.color;\
}";

class D3D11Application
{
public:
	static void FnUpdate()
	{
		auto d3dDevice = Instance->m_D3d.GetDevice();
		auto d3dContext = Instance->m_D3d.GetContext();

		UINT stride = sizeof(VertexLayout);
		UINT offset = 0;
		d3dContext->IASetVertexBuffers(0, 1, &Instance->m_VertexBuffer, &stride, &offset);
		d3dContext->IASetIndexBuffer(Instance->m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto rtv = Instance->m_D3d.GetRenderTargetView();
		d3dContext->OMSetRenderTargets(1, &rtv, nullptr);

		FLOAT clearColor[] = {0.25f, 0.5f, 1.0f, 1.0f};
		d3dContext->ClearRenderTargetView(rtv, clearColor);

		d3dContext->VSSetShader(Instance->m_VertexShader, nullptr, 0);
		d3dContext->PSSetShader(Instance->m_PixelShader, nullptr, 0);

		d3dContext->DrawIndexed(6, 0, 0);

		Instance->m_D3d.GetSwapchain()->Present(0, 0);
	}

	int Run()
	{
		if (!m_Wc.Initialize(TEXT("D3D11"), 1280, 720))
			return -1;

		auto width = m_Wc.GetClientWidth(), height = m_Wc.GetClientHeight();

		if (!m_D3d.Initialize(m_Wc.GetHWND(), width, height))
		{
			m_D3d.Uninitialize();
			return -2;
		}

		auto d3dDevice = m_D3d.GetDevice();
		auto d3dContext = m_D3d.GetContext();
		// Setup Shader
		{
			ID3DBlob * pVSBlob = nullptr;
			HRESULT hr = CompileShaderFromString(g_Shader, "VS", "vs_5_0", &pVSBlob);
			if (FAILED(hr))
				return -3;

			hr = d3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_VertexShader);
			if (FAILED(hr))
			{
				pVSBlob->Release();
				return -4;
			}

			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			hr = d3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_VertexLayout);

			pVSBlob->Release();
			if (FAILED(hr))
				return -5;

			d3dContext->IASetInputLayout(m_VertexLayout);

			ID3DBlob * pPSBlob = nullptr;
			hr = CompileShaderFromString(g_Shader, "PS", "ps_5_0", &pPSBlob);
			if (FAILED(hr))
				return -6;

			hr = d3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_PixelShader);
			pPSBlob->Release();
			if (FAILED(hr))
				return -7;
		}

		// Setup Vertex, Index Data
		{
			VertexLayout vertices[] =
			{
				{-0.5f,  0.5f,  0.5f,  1.0f,	0, 0, 0, 1},
				{ 0.5f,  0.5f,  0.5f,  1.0f,	1, 0, 0, 1},
				{-0.5f, -0.5f,  0.5f,  1.0f,	0, 1, 0, 1},
				{ 0.5f, -0.5f,  0.5f,  1.0f,	1, 1, 0, 1},
			};

			D3D11_BUFFER_DESC bdv = {};
			bdv.Usage = D3D11_USAGE_DEFAULT;
			bdv.ByteWidth = sizeof(VertexLayout) * 4;
			bdv.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bdv.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitDatav = {};
			InitDatav.pSysMem = vertices;
			HRESULT hr = d3dDevice->CreateBuffer(&bdv, &InitDatav, &m_VertexBuffer);
			if (FAILED(hr))
				return -8;

			UINT indices[] =
			{
				0, 1, 2,
				1, 3, 2
			};

			D3D11_BUFFER_DESC bdi = {};
			bdi.Usage = D3D11_USAGE_DEFAULT;
			bdi.ByteWidth = sizeof(UINT) * 6;
			bdi.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bdi.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitDatai = {};
			InitDatai.pSysMem = indices;
			hr = d3dDevice->CreateBuffer(&bdi, &InitDatai, &m_IndexBuffer);
			if (FAILED(hr))
				return -9;
		}

		m_Wc.SetUpdateCallback(FnUpdate);
		m_Wc.Run();

		m_IndexBuffer->Release();
		m_VertexBuffer->Release();
		m_VertexLayout->Release();
		m_PixelShader->Release();
		m_VertexShader->Release();

		m_D3d.Uninitialize();

		return 0;
	}
public:
	/// <summary>
	/// Get Singleton Instance
	/// </summary>
	/// <returns>Singleton Instance</returns>
	static std::unique_ptr<D3D11Application> & GetInstance()
	{
		return Instance;
	}

private:
	/// <summary>
	/// Helper Class
	/// </summary>
	WindowsContainer m_Wc;
	D3D11 m_D3d;

	/// <summary>
	/// Shader Class
	/// </summary>
	ID3D11VertexShader * m_VertexShader = nullptr;
	ID3D11PixelShader * m_PixelShader = nullptr;

	/// <summary>
	/// Vertex Stream layout Class
	/// </summary>
	ID3D11InputLayout * m_VertexLayout = nullptr;

	/// <summary>
	/// Buffer Class
	/// </summary>
	ID3D11Buffer * m_VertexBuffer = nullptr;
	ID3D11Buffer * m_IndexBuffer = nullptr;

	/// <summary>
	/// Singleton Instance
	/// </summary>
	inline static std::unique_ptr<D3D11Application> Instance = std::make_unique<D3D11Application>();
};

int main()
{
	return D3D11Application::GetInstance()->Run();
}


HRESULT CompileShaderFromString(const CHAR * szSource, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob * pErrorBlob = nullptr;
	hr = D3DCompile(szSource, strlen(szSource), NULL,
		nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char *>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}