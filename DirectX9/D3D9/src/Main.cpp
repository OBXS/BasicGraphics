#include "Barebones/Direct3D9.h"
#include "WindowsContainer.h"
#include "WindowsContainer.cpp"

#include <memory>

struct VertexLayout
{
	FLOAT x, y, z, w;
	D3DCOLOR color;
};

class D3D9Application
{
public:
	static void FnUpdate()
	{
		auto d3dDevice = Instance->m_D3d.Device();

		d3dDevice->BeginScene();
		d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(64, 128, 255, 255), 1.0f, 0);
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		
		d3dDevice->SetIndices(Instance->m_IndexBuffer);
		d3dDevice->SetStreamSource(0, Instance->m_VertexBuffer, 0, sizeof(VertexLayout));
		d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

		d3dDevice->EndScene();

		d3dDevice->Present(NULL, NULL, NULL, NULL);
	}

	int Run()
	{
		if (!m_Wc.Initialize(TEXT("D3D9"), 1280, 720))
			return -1;

		auto width = m_Wc.GetClientWidth(), height = m_Wc.GetClientHeight();

		if (!m_D3d.Initialize(m_Wc.GetHWND(), width, height))
		{
			m_D3d.Uninitialize();
			return -2;
		}

		// Setup Vertex, Index Data
		{
			auto d3dDevice = Instance->m_D3d.Device();
			d3dDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);
			WORD * pIndex;
			m_IndexBuffer->Lock(0, 0, (void **)&pIndex, D3DLOCK_DISCARD);
			pIndex[0] = 0;
			pIndex[1] = 1;
			pIndex[2] = 2;
			pIndex[3] = 1;
			pIndex[4] = 3;
			pIndex[5] = 2;
			m_IndexBuffer->Unlock();
			d3dDevice->CreateVertexBuffer(sizeof(VertexLayout) * 4, D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);
		
			width /= 2;
			height /= 2;

			auto hwidth = width / 2;
			auto hheight = height / 2;

			VertexLayout * pv;
			m_VertexBuffer->Lock(0, 0, (void **)&pv, D3DLOCK_DISCARD);

			pv[0] = { (FLOAT)(width - hwidth), (FLOAT)(height - hheight), 0, 1,	 D3DCOLOR_RGBA(  0,   0,   0, 255) };
			pv[1] = { (FLOAT)(width + hwidth), (FLOAT)(height - hheight), 0, 1,	 D3DCOLOR_RGBA(255,   0,   0, 255) };
			pv[2] = { (FLOAT)(width - hwidth), (FLOAT)(height + hheight), 0, 1,	 D3DCOLOR_RGBA(  0, 255,   0, 255) };
			pv[3] = { (FLOAT)(width + hwidth), (FLOAT)(height + hheight), 0, 1,	 D3DCOLOR_RGBA(255, 255,   0, 255) };

			m_VertexBuffer->Unlock();
		}

		m_Wc.SetUpdateCallback(FnUpdate);
		m_Wc.Run();

		m_D3d.Uninitialize();

		return 0;
	}
public:
	/// <summary>
	/// Get Singleton Instance
	/// </summary>
	/// <returns>Singleton Instance</returns>
	static std::unique_ptr<D3D9Application> & GetInstance()
	{
		return Instance;
	}

private:
	/// <summary>
	/// Helper Class
	/// </summary>
	WindowsContainer m_Wc;
	D3D9 m_D3d;

	/// <summary>
	/// Buffer Class
	/// </summary>
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 m_IndexBuffer = NULL;

	/// <summary>
	/// Singleton Instance
	/// </summary>
	inline static std::unique_ptr<D3D9Application> Instance = std::make_unique<D3D9Application>();
};

int main()
{
	return D3D9Application::GetInstance()->Run();
}